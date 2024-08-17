#include "VertexArray.h"

VertexArray::VertexArray() { glCall(glGenVertexArrays(1, &gl_id)); }

VertexArray::~VertexArray() { glCall(glDeleteVertexArrays(1, &gl_id)); }

void VertexArray::bind() const { glCall(glBindVertexArray(gl_id)); }

void VertexArray::unbind() const { glCall(glBindVertexArray(0)); }
