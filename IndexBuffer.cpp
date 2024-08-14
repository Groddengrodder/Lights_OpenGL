#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const GLuint _count, const GLuint *data, const GLint GL_OPTION) {
    count = _count;

    glCall(glGenBuffers(1, &gl_id));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_id));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_OPTION));
}

IndexBuffer::~IndexBuffer() { glCall(glDeleteBuffers(1, &gl_id)); }

void IndexBuffer::bind() const { glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_id)); }

void IndexBuffer::unbind() const { glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

GLuint IndexBuffer::getCount() const { return count; }
GLuint IndexBuffer::getId() const { return gl_id; }
