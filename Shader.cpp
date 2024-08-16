#include "Shader.h"

static char *getShader(FILE *file) {
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
    clearError();
    GLuint id = glCreateShader(type);
    if (checkError()) {
        printf("Error while creating shader from type\n");
        exit(1);
    }

    glCall(glShaderSource(id, 1, &source, nullptr));
    glCall(glCompileShader(id));

    GLint result;
    glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char message[length];
        glCall(glGetShaderInfoLog(id, length, &length, message));
        printf("%s\n", message);
        glCall(glDeleteShader(id));
        exit(1);
    }

    return id;
}

static GLuint CreateShader(const char *vertexShader, const char *fragmentShader) {
    clearError();
    GLuint program = glCreateProgram();
    if (checkError()) {
        printf("Error while creating Program\n");
        exit(1);
    }

    GLuint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glCall(glAttachShader(program, vs));
    glCall(glAttachShader(program, fs));
    glCall(glLinkProgram(program));
    glCall(glValidateProgram(program));

    glCall(glDeleteShader(vs));
    glCall(glDeleteShader(fs));

    return program;
}

Shader::Shader(char VertexPath[], char FragmentPath[]) {
    FILE *VertexFile = fopen(VertexPath, "r");
    FILE *FragmentFile = fopen(FragmentPath, "r");

    char *VertexSource = getShader(VertexFile);
    char *FragmentSource = getShader(FragmentFile);

    fclose(VertexFile);
    fclose(FragmentFile);

    gl_id = CreateShader(VertexSource, FragmentSource);

    free(FragmentSource);
    free(VertexSource);
}

Shader::~Shader() { glCall(glDeleteProgram(gl_id)); }

void Shader::bind() const { glCall(glUseProgram(gl_id)); }

void Shader::unbind() const { glCall(glUseProgram(0)); }

GLuint Shader::getId() const { return gl_id; }
