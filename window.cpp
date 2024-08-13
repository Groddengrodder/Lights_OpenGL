#include "IndexBuffer.h"
#include "OpenGl_Header.h"
#include "VertexBuffer.h"

const GLuint window_width = 640;
const GLuint window_height = 480;
const GLchar *window_name = "A new Window";

char *getShader(FILE *file) {
    uint MaxFileSize = 1000;
    char *code = (char *)malloc(MaxFileSize * sizeof(char));

    for (uint i = 0; (code[i] = fgetc(file)) != EOF; i++) {
        if (i >= MaxFileSize) {
            MaxFileSize *= 2;
            code = (char *)realloc(code, MaxFileSize);
        }
    }

    return code;
}

static GLuint CompileShader(const char *source, GLuint type) {
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
        printf("%s\n", message);
        glDeleteShader(id);
        exit(1);
    }

    return id;
}

static GLuint CreateShader(const char *vertexShader, const char *fragmentShader) {
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void init_OpenGL(GLFWwindow **window) {
    if (!glfwInit()) {
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    *window = glfwCreateWindow(window_width, window_height, window_name, NULL, NULL);

    if (*window == NULL) {
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(*window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        printf("hmm\n");
        exit(1);
    }
}

int main(void) {
    GLFWwindow *window;
    init_OpenGL(&window);

    GLfloat positions[15] = {-0.5, -0.5, 0., 0., 1., 0.0, 0.5, 1., 1., 1., 0.5, -0.5, 0., 0., 1.};

    GLuint index[3] = {0, 1, 2};

    GLuint vertex_array_id;
    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);

    VertexBuffer vb_first_object(15 * sizeof(GLfloat), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat) + 3 * sizeof(GLfloat), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat) + 2 * sizeof(GLfloat),
                          (void *)(2 * sizeof(GLfloat)));

    IndexBuffer ib_first_object(3, index, GL_STATIC_DRAW);

    FILE *file_vertex = fopen("shader/vertex_shader.glsl", "r");
    FILE *file_fragment = fopen("shader/fragment_shader.glsl", "r");
    char *vertexShader = getShader(file_vertex);
    char *fragmentShader = getShader(file_fragment);
    fclose(file_vertex);
    fclose(file_fragment);

    GLuint shader = CreateShader(vertexShader, fragmentShader);
    free(vertexShader);
    free(fragmentShader);

    glUseProgram(shader);

    GLint uni_id = glGetUniformLocation(shader, "u_Color");
    glUniform4f(uni_id, 1., 0., 0., 1.);

    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
