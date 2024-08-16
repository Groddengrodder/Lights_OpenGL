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
#endif
