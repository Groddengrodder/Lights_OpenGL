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

static GLuint CompileShader(const char *source, const GLuint type) {
    clearError();
    const GLuint id = glCreateShader(type);
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
    const GLuint program = glCreateProgram();
    if (checkError()) {
        printf("Error while creating Program\n");
        exit(1);
    }

    const GLuint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    const GLuint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glCall(glAttachShader(program, vs));
    glCall(glAttachShader(program, fs));
    glCall(glLinkProgram(program));
    glCall(glValidateProgram(program));

    glCall(glDeleteShader(vs));
    glCall(glDeleteShader(fs));

    return program;
}

Shader::Shader(const char VertexPath[], const char FragmentPath[]) {
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

void Shader::setUniform(const char *UniformName, const GLfloat f1, const GLfloat f2,
                        const GLfloat f3, const GLfloat f4) const {
    clearError();
    const GLint location = glGetUniformLocation(gl_id, UniformName);
    if (checkError()) {
        printf("Error while getting Uniform Location\n");
        exit(1);
    }

    if (location != -1) {
        glCall(glUniform4f(location, f1, f2, f3, f4));
    }
}

GLuint Shader::getId() const { return gl_id; }
