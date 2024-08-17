#ifndef __HEADER_FILE__
#define __HEADER_FILE__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#define glCall(x)                                                                                  \
    clearError();                                                                                  \
    (x);                                                                                           \
    if (checkError()) {                                                                            \
        printf("%s, %s, %d\n", #x, __FILE__, __LINE__);                                            \
        exit(1);                                                                                   \
    }

static void clearError() {
    while (glGetError() != GL_NO_ERROR) {
    }
}

static bool checkError() {
    GLenum error;
    bool found = false;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("Error: %u\n", error);
        found = true;
    }
    return found;
}

static void init_OpenGL(GLFWwindow **window, const GLuint width, const GLuint height,
                        const GLchar *name) {
    if (!glfwInit()) {
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    *window = glfwCreateWindow(width, height, name, NULL, NULL);

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
#endif
