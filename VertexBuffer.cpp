#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const GLuint size_bytes, const void *data, const GLint GL_OPTION) {
    layout = NULL;
    layout_count = 0;

    glCall(glGenBuffers(1, &gl_id));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, gl_id));
    glCall(glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_OPTION));
}

VertexBuffer::~VertexBuffer() {
    glCall(glDeleteBuffers(1, &gl_id));
    if (layout != NULL) {
        free(layout);
    }
}

void VertexBuffer::bind() const {
    glCall(glBindBuffer(GL_ARRAY_BUFFER, gl_id));

    for (uint i = 0; i < layout_count; i++) {
        glCall(glEnableVertexAttribArray(i));
        glCall(glVertexAttribPointer(i, layout[i].count, layout[i].type, layout[i].normalized,
                                     layout[i].stride, layout[i].pointer));
    }
}

void VertexBuffer::unbind() const { glCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

void VertexBuffer::addAttribute(Attribute attrib) {
    layout_count += 1;

    if (layout == NULL) {
        layout = (Attribute *)malloc(sizeof(Attribute));
    } else {
        layout = (Attribute *)realloc(layout, layout_count * sizeof(Attribute));
    }

    layout[layout_count - 1] = attrib;
}

void VertexBuffer::addAttribute(GLint count, GLenum type, GLboolean normalized, GLsizei stride,
                                GLvoid *pointer) {
    layout_count += 1;

    if (layout == NULL) {
        layout = (Attribute *)malloc(sizeof(Attribute));
    } else {
        layout = (Attribute *)realloc(layout, layout_count * sizeof(Attribute));
    }

    layout[layout_count - 1].count = count;
    layout[layout_count - 1].type = type;
    layout[layout_count - 1].normalized = normalized;
    layout[layout_count - 1].stride = stride;
    layout[layout_count - 1].pointer = pointer;
}

void VertexBuffer::removeAttribute(const uint index) {
    if (layout_count == 0 || index < 0 || index >= layout_count) {
        fprintf(stderr, "Warning in removeAttribute: invalid index\n");
        return;
    }

    if (layout_count - 1 == 0) {
        layout_count = 0;
        Attribute *temp = layout;
        layout = NULL;
        free(temp);
        return;
    }

    for (uint i = index; i < layout_count - 1; i++) {
        layout[i] = layout[i + 1];
    }

    layout_count -= 1;

    layout = (Attribute *)realloc(layout, layout_count * sizeof(Attribute));
}

GLuint VertexBuffer::getCount() const { return layout_count; }
GLuint VertexBuffer::getId() const { return gl_id; }
