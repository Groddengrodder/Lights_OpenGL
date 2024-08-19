#include "IndexBuffer.h"
#include "Lights_Out.h"
#include "OpenGl_Header.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include <unistd.h>

typedef struct {
    float width;
    float height;
    bool filled;
    float position[2];
} box;

GLFWwindow *window;
const GLuint window_width = 640;  // 640
const GLuint window_height = 640; // 480
const GLchar *window_name = "A new Window";
bool input_enabled = true;

const char VertexSource[] = "shader/vertex_shader.glsl";
const char FragmentSource[] = "shader/fragment_shader.glsl";
Shader *shader = NULL;

uint puzzle_width = 6;
uint puzzle_height = 6;
uint cell_count = puzzle_width * puzzle_height;
box **cell = NULL;

GLfloat color_on[3] = {1., 1., 0.};
GLfloat color_off[3] = {1., 1., 1.};

GLfloat selection_color[3] = {1., 0., 1.};
uint select_i = -1;

bool show = false;

bool *input_solution = NULL;

const GLfloat bg_color[3] = {0.0, 0.5, 1.0};
const GLfloat bg_position[2] = {-1., -1.};
const GLfloat bg_width = 2;
const GLfloat bg_height = 2;

void box_init(box *input, float width, float height, float pos_x, float pos_y, bool filled) {
    input->width = width;
    input->height = height;
    input->position[0] = pos_x;
    input->position[1] = pos_y;
    input->filled = filled;
}

void cell_statechange(int input_x, int input_y) {
    if (input_x >= 0 && input_x < puzzle_width && input_y >= 0 && input_y < puzzle_height) {
        cell[input_y][input_x].filled = !cell[input_y][input_x].filled;
    }
}

void click_cell(int input_x, int input_y) {
    if (input_x >= 0 && input_x < puzzle_width && input_y >= 0 && input_y < puzzle_height) {
        cell_statechange(input_x, input_y);
        cell_statechange(input_x + 1, input_y);
        cell_statechange(input_x, input_y + 1);
        cell_statechange(input_x - 1, input_y);
        cell_statechange(input_x, input_y - 1);
    }
}

box **init_cells(float cell_width, float cell_height) {
    box *cell_1d = (box *)malloc(puzzle_height * puzzle_width * sizeof(box));
    box **cell = (box **)malloc(puzzle_height * sizeof(box *));

    for (uint i = 0; i < puzzle_height; i++) {
        cell[i] = cell_1d + i * puzzle_width;
    }

    for (uint i = 0; i < puzzle_height; i++) {
        for (uint j = 0; j < puzzle_width; j++) {
            box_init(&cell[i][j], cell_width, cell_height,
                     j * (2 * cell_width) + 0.5 * cell_width - 1,
                     (puzzle_height - 1 - i) * (2 * cell_height) + 0.5 * cell_height - 1, false);
        }
    }

    return cell;
}

void drawCell(box cell, GLfloat *color) {
    GLfloat cell_color[3];

    if (color == NULL) {
        if (cell.filled) {
            cell_color[0] = color_on[0];
            cell_color[1] = color_on[1];
            cell_color[2] = color_on[2];
        } else {
            cell_color[0] = color_off[0];
            cell_color[1] = color_off[1];
            cell_color[2] = color_off[2];
        }
    } else {
        cell_color[0] = color[0];
        cell_color[1] = color[1];
        cell_color[2] = color[2];
    }

    shader->setUniform("rect_info", cell.position[0], cell.position[1], cell.width, cell.height);
    drawRect(cell.position, cell_color, cell.width, cell.height);
}

void cell_randomize() {
    for (int i = 0; i < puzzle_height; i++) {
        for (int j = 0; j < puzzle_width; j++) {
            if (rand() % 2) {
                click_cell(j, i);
            }
        }
    }
}

bool *calculate_solution() {
    bool *puzzle = (bool *)malloc(puzzle_width * puzzle_height * sizeof(bool));

    for (uint i = 0; i < puzzle_height; i++) {
        for (uint j = 0; j < puzzle_width; j++) {
            puzzle[i * puzzle_width + j] = cell[i][j].filled;
        }
    }

    bool *solution = solve_puzzle(puzzle, puzzle_width, puzzle_height);
    free(puzzle);

    return solution;
}

void drawSelection();

void solve_sequence() {
    show = false;

    const uint inverse_framerate = 10000000 / (puzzle_width * puzzle_height) > 50000
                                       ? 10000000 / (puzzle_width * puzzle_height)
                                       : 50000; // stable values: 50000

    bool *solution = NULL;
    if (input_solution == NULL) {
        solution = calculate_solution();
    } else {
        solution = input_solution;
    }

    if (solution == NULL) {
        return;
    }

    shader->setUniform("bg_color", bg_color[0], bg_color[1], bg_color[2], 1.);

    input_enabled = false;
    for (uint i = 0; i < puzzle_height && !glfwWindowShouldClose(window); i++) {
        for (uint j = 0; j < puzzle_width && !glfwWindowShouldClose(window); j++) {
            glClear(GL_COLOR_BUFFER_BIT);

            if (solution[puzzle_width * i + j]) {
                click_cell(j, i);
            }

            shader->setUniform("rect_info", bg_position[0], bg_position[1], bg_width, bg_height);
            drawRect(bg_position, bg_color, bg_width, bg_height);
            for (uint i = 0; i < cell_count; i++) {
                drawCell(cell[0][i], NULL);
            }
            drawCell(cell[i][j], selection_color);

            box special = cell[i][j];
            special.width -= 0.3 * cell[i][j].width;
            special.height -= 0.3 * cell[i][j].height;
            special.position[0] += 0.15 * cell[i][j].width;
            special.position[1] += 0.15 * cell[i][j].height;

            shader->setUniform("bg_color", selection_color[0], selection_color[1],
                               selection_color[2], 1.);
            drawCell(special, NULL);
            shader->setUniform("bg_color", bg_color[0], bg_color[1], bg_color[2], 1.);

            glfwSwapBuffers(window);
            glfwPollEvents();
            usleep(inverse_framerate);
        }
    }
    input_enabled = true;

    free(solution);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (select_i == -1 || !input_enabled) {
        return;
    }

    uint i = select_i / puzzle_width;
    uint j = select_i % puzzle_width;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        click_cell(j, i);

        if (show) {
            if (input_solution == NULL) {
                input_solution = calculate_solution();
            } else {
                bool *temp = input_solution;
                input_solution = calculate_solution();
                free(temp);
            }
        }
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    xpos = 2 * xpos / window_width - 1;
    ypos = 2 * (window_height - ypos) / window_height - 1;

    bool found = false;

    for (uint i = 0; i < cell_count; i++) {
        if (xpos >= cell[0][i].position[0] && xpos < cell[0][i].position[0] + cell[0][i].width) {
            if (ypos >= cell[0][i].position[1] &&
                ypos < cell[0][i].position[1] + cell[0][i].height) {
                select_i = i;
                found = true;
            }
        }
    }

    if (!found) {
        select_i = -1;
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_R && action == GLFW_PRESS && input_enabled) {
        cell_randomize();

        if (show) {
            if (input_solution == NULL) {
                input_solution = calculate_solution();
            } else {
                bool *temp = input_solution;
                input_solution = calculate_solution();
                free(temp);
            }
        }
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS && input_enabled) {
        solve_sequence();
    }

    if (key == GLFW_KEY_O && action == GLFW_PRESS && input_enabled) {
        if (show) {
            show = false;
            if (input_solution != NULL) {
                bool *temp = input_solution;
                input_solution = NULL;
                free(temp);
            }
        } else {
            show = true;
            if (input_solution == NULL) {
                input_solution = calculate_solution();
            } else {
                bool *temp = input_solution;
                input_solution = calculate_solution();
                free(temp);
            }
        }
    }

    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(int argc, char *argv[]) {
    printf("width = ");
    scanf("%u", &puzzle_width);

    printf("height = ");
    scanf("%u", &puzzle_height);

    cell_count = puzzle_width * puzzle_height;

    init_OpenGL(&window, window_width, window_height, window_name);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    VertexArray GlobalVertexArray;
    GlobalVertexArray.bind();

    GLuint bg_index[6] = {0, 1, 2, 0, 2, 3};

    IndexBuffer ib_first_object(6, bg_index, GL_STATIC_DRAW);

    const float cell_width = 1. / puzzle_width;
    const float cell_height = 1. / puzzle_height;
    cell = init_cells(cell_width, cell_height);

    Shader newShader(VertexSource, FragmentSource);

    shader = &newShader;

    if (argc == 2) {
        for (uint i = 0; i < puzzle_height; i++) {
            uint j = 0;
            for (j = 0; j < puzzle_width && puzzle_width * i + j < strlen(argv[1]); j++) {
                cell[i][j].filled = (bool)(argv[1][i * puzzle_width + j] - '0');
            }
            if (puzzle_width * i + j >= strlen(argv[1])) {
                break;
            }
        }
    }

    shader->bind();
    ib_first_object.bind();

    char bg_color_uni[] = "bg_color";
    char rect_info[] = "rect_info";
    shader->setUniform(bg_color_uni, bg_color[0], bg_color[1], bg_color[2], 1.);

    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shader->setUniform(rect_info, bg_position[0], bg_position[1], bg_width, bg_height);
        drawRect(bg_position, bg_color, bg_width, bg_height);

        for (uint i = 0; i < cell_count; i++) {
            drawCell(cell[0][i], NULL);
        }

        if (select_i != -1) {
            drawCell(cell[0][select_i], selection_color);

            box special = cell[0][select_i];
            special.width -= 0.3 * cell[0][select_i].width;
            special.height -= 0.3 * cell[0][select_i].height;
            special.position[0] += 0.15 * cell[0][select_i].width;
            special.position[1] += 0.15 * cell[0][select_i].height;

            shader->setUniform("bg_color", selection_color[0], selection_color[1],
                               selection_color[2], 1.);
            drawCell(special, NULL);
            shader->setUniform("bg_color", bg_color[0], bg_color[1], bg_color[2], 1.);
        }

        if (show && input_solution != NULL) {
            for (uint i = 0; i < cell_count; i++) {
                if (input_solution[i] == 1) {
                    if (cell[0][i].filled) {
                        shader->setUniform(bg_color_uni, color_on[0], color_on[1], color_on[2], 1.);
                    } else {
                        shader->setUniform(bg_color_uni, color_off[0], color_off[1], color_off[2],
                                           1.);
                    }

                    box special = cell[0][i];
                    special.width -= 0.6 * cell[0][i].width;
                    special.height -= 0.6 * cell[0][i].height;
                    special.position[0] += 0.3 * cell[0][i].width;
                    special.position[1] += 0.3 * cell[0][i].height;

                    drawCell(special, selection_color);
                    shader->setUniform(bg_color_uni, bg_color[0], bg_color[1], bg_color[2], 1.);
                }
            }
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
