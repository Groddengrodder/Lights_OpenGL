#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const GLuint size_bytes, const void *data, const GLint GL_OPTION) {
    glCall(glGenBuffers(1, &gl_id));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, gl_id));
    glCall(glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_OPTION));
}

VertexBuffer::~VertexBuffer() { glCall(glDeleteBuffers(1, &gl_id)); }

void VertexBuffer::bind() const { glCall(glBindBuffer(GL_ARRAY_BUFFER, gl_id)); }

void VertexBuffer::unbind() const { glCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
