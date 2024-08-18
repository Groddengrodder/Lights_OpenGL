#ifndef __SOLVER__
#define __SOLVER__

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool *Transformation = NULL; // Transformations-Matrix von Inputs zu Änderung des Feldes

////Verknüpfungen für den Körper F_2
#define add(a, b) ((a) ^ (b))
#define multi(a, b) ((a) && (b))

// Initialisierung der arrays
void *init(int N, long int size) {
    void *arr = malloc(N * size);
    memset(arr, 0, N * size);
    if (arr == NULL) {
        printf("ups\n");
        exit(1);
    }
    return arr;
}

void print_matrix(bool *matrix, int dimension) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            printf("%d  ", matrix[i * dimension + j]);
        }
        printf("\n");
    }
}

void print_puzzle(bool *puzzle, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%d  ", puzzle[i * width + j]);
        }
        printf("\n");
    }
}

// Erstellt für die angegebene Feldgröße eine passende Transformations-Matrix
void init_Transformation(int dimension, int global_width) {
    Transformation = (bool *)malloc(dimension * dimension * sizeof(bool));

    int row_puzzle_i = 0;
    int row_puzzle_j = 0;

    for (int i = 0; i < dimension; i++) {
        row_puzzle_i = i / global_width;

        for (int j = 0; j < dimension; j++) {
            row_puzzle_j = j / global_width;

            if (i == j || i == j + global_width || i == j - global_width ||
                (row_puzzle_i == row_puzzle_j && (i == j + 1 || i == j - 1))) {
                Transformation[dimension * i + j] = 1;
            } else {
                Transformation[dimension * i + j] = 0;
            }
        }
    }
}

// Erstellt ein zufälliges lösbares Rätsel
bool *rand_rat(const int dimension) {
    bool solution[dimension];
    bool *puzzle = (bool *)init(dimension, sizeof(bool));

    for (int i = 0; i < dimension; i++) {
        solution[i] = rand() % 2;
    }

    for (int i = 0; i < dimension; i++) {
        puzzle[i] = 0;
        for (int j = 0; j < dimension; j++) {
            puzzle[i] = add(multi(Transformation[dimension * i + j], solution[j]), puzzle[i]);
        }
    }

    return puzzle;
}

// Vergleicht Transformation der Lösung mit dem Rätsel, um richtiges Ergebnis sicherzustellen
int test(bool *solution, bool *puzzle, const int dimension) {
    bool check = 0;
    int solved = 0;

    for (int i = 0; i < dimension; i++) {
        check = 0;
        for (int j = 0; j < dimension; j++) {
            check = add(multi(Transformation[dimension * i + j], solution[j]), check);
        }
        check = add(check, puzzle[i]);
        solved += check;
    }

    return solved;
}

// Löst das gegebene Gleichungssystem über den Gauß-Algorithmus
// Die Lösung wird in Solution geschrieben
int Gauss(bool *solution, bool *puzzle, bool *solution_trafo, const int dimension) {
    bool *solved_trafo = NULL;
    bool puzzle_copy[dimension];

    if (solution_trafo == NULL) {
        solved_trafo = (bool *)init(dimension, sizeof(bool));
    } else {
        solved_trafo = solution_trafo;
    }

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            solved_trafo[dimension * i + j] = Transformation[dimension * i + j];
        }
    }
    for (int i = 0; i < dimension; i++) {
        puzzle_copy[i] = puzzle[i];
        solution[i] = 0;
    }

    for (int i = 0; i < dimension; i++) {
        int temp = -1;
        for (int j = 0; j < dimension; j++) {
            for (int k = 0; k < dimension; k++) {
                if (solved_trafo[dimension * j + k] == 1 && k != i) {
                    break;
                }
                if (solved_trafo[dimension * j + k] == 1 && k == i) {
                    temp = j;
                    break;
                }
            }
            if (temp != -1) {
                break;
            }
        }
        if (temp != -1) {
            for (int j = 0; j < dimension; j++) {
                if (temp != j) {
                    if (solved_trafo[dimension * j + i] == 1) {
                        for (int k = 0; k < dimension; k++) {
                            solved_trafo[dimension * j + k] =
                                add(solved_trafo[dimension * j + k],
                                    solved_trafo[dimension * temp + k]);
                        }
                        puzzle_copy[j] = add(puzzle_copy[j], puzzle_copy[temp]);
                    }
                }
            }
        }
    }

    for (int i = 0; i < dimension; i++) {
        int temp = -2;
        for (int j = 0; j < dimension; j++) {
            if (solved_trafo[dimension * i + j] == 1) {
                temp = j;
                break;
            }
        }
        if (temp != -2) {
            solution[temp] = puzzle_copy[i];
        } else if (temp == -2 && puzzle_copy[i] == 1) {
            return 0;
        }
    }
    return 1;
}

// Bestimmt die Anzahl der Nullzeilen in der umgeformten Transformations-Matrix
int nullen(bool *solved_trafo, int dimension) {
    int anzahl = 0;
    int temp;
    for (int i = 0; i < dimension; i++) {
        temp = 1;
        for (int j = 0; j < dimension; j++) {
            if (solved_trafo[dimension * i + j] == 1) {
                temp = 0;
                break;
            }
        }
        if (temp == 1) {
            anzahl += 1;
        }
    }
    return anzahl;
}

// Berechnet die Stillen Muster
bool *quiet(int *nullzeilen, bool *solved_trafo, int dimension) {
    const int max_nullzeilen = 15;
    int lul = nullen(solved_trafo, dimension);

    if (lul > max_nullzeilen || lul == 0) {
        return NULL;
    }

    int temp, temp2;
    bool *free_parameter = (bool *)init(lul, sizeof(bool));
    bool *quiet_pattern = (bool *)init(pow(2, lul) - 1, sizeof(bool) * dimension);

    for (int i = 0; i < pow(2, lul) - 1; i++) {
        temp = free_parameter[0];
        free_parameter[0] = add(free_parameter[0], 1);
        for (int j = 0; j < lul - 1; j++) {
            if (free_parameter[j] != temp && free_parameter[j] == 0) {
                temp = free_parameter[j + 1];
                free_parameter[j + 1] = add(free_parameter[j + 1], 1);
            }
        }
        for (int y = 0; y < dimension; y++) {
            temp2 = -1;
            int s = 0;
            for (int x = 0; x < dimension; x++) {
                if (solved_trafo[dimension * y + x] == 1 && temp2 == -1) {
                    temp2 = x;
                } else if (solved_trafo[dimension * y + x] == 1) {
                    s = add(s, free_parameter[lul - (dimension - x)]);
                }
            }
            if (temp2 != -1) {
                quiet_pattern[dimension * i + temp2] = s;
            }
            if ((dimension - y) <= lul) {
                quiet_pattern[dimension * i + y] = free_parameter[lul - (dimension - y)];
            }
        }
    }
    *nullzeilen = lul;
    return quiet_pattern;
}

// Ermittelt mithilfe der Stillen Muster die optimale Lösung
int opt(bool *quiet_pattern, bool *solution, const int dimension, int lul) {
    int max = dimension;
    int pos = 0;
    int einsen;
    bool *solution_copy = (bool *)init(dimension, sizeof(bool));

    for (int i = 0; i < dimension; i++) {
        solution_copy[i] = solution[i];
    }
    for (int i = 0; i < pow(2, lul); i++) {
        einsen = 0;
        if (i > 0) {
            for (int j = 0; j < dimension; j++) {
                solution_copy[j] = add(solution[j], quiet_pattern[dimension * (i - 1) + j]);
            }
        }
        for (int j = 0; j < dimension; j++) {
            if (solution_copy[j] == 1) {
                einsen += 1;
            }
        }
        if (einsen < max) {
            max = einsen;
            pos = i;
        }
    }
    if (pos != 0) {
        for (int i = 0; i < dimension; i++) {
            solution_copy[i] = add(solution[i], quiet_pattern[dimension * (pos - 1) + i]);
        }
    } else {
        for (int i = 0; i < dimension; i++) {
            solution_copy[i] = solution[i];
        }
    }

    for (int i = 0; i < dimension; i++) {
        solution[i] = solution_copy[i];
    }

    return max;
}

bool *solve_puzzle(bool *puzzle, int global_width, int global_height) {
    int dimension = global_width * global_height;
    init_Transformation(dimension, global_width);
    bool *solved_trafo = (bool *)init(dimension * dimension, sizeof(bool));
    bool *solution = (bool *)init(dimension, sizeof(bool));
    bool *quiet_pattern = NULL;
    int nullzeilen = 0;

    if (Gauss(solution, puzzle, solved_trafo, dimension) == 0) {
        bool *temp = Transformation;
        Transformation = NULL;
        free(temp);
        free(solved_trafo);
        free(solution);

        return NULL;
    }

    quiet_pattern = quiet(&nullzeilen, solved_trafo, dimension);
    if (quiet_pattern != NULL) {
        opt(quiet_pattern, solution, dimension, nullzeilen);
    }

    if (test(solution, puzzle, dimension) != 0) {
        printf("That didnt work\n");
    }

    free(solved_trafo);
    if (quiet_pattern != NULL) {
        free(quiet_pattern);
    }
    bool *temp = Transformation;
    Transformation = NULL;
    free(temp);

    return solution;
}
#endif
