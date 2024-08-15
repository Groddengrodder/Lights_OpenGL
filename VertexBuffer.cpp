#include "VertexBuffer.h"

size_t getSize_bytes(const GLenum type) {
    switch (type) {
    case GL_FLOAT:
        return sizeof(GLfloat);
    case GL_INT:
        return sizeof(GLint);
    case GL_UNSIGNED_INT:
        return sizeof(GLuint);
    case GL_BOOL:
        return sizeof(GLboolean);
    default:
        return 0;
    }
}

size_t getTotalSize_bytes(const Attribute *layout, const GLuint count) {
    size_t size = 0;

    for (uint i = 0; i < count; i++) {
        size += layout[i].count * getSize_bytes(layout[i].type);
    }

    return size;
}

VertexBuffer::VertexBuffer(const GLuint size_bytes, const void *data, const GLint GL_OPTION) {
    layout = NULL;
    layout_count = 0;
    stride = 0;

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
                                     stride, layout[i].pointer));
    }
}

void VertexBuffer::unbind() const { glCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

void VertexBuffer::addAttribute(const Attribute attrib) {
    layout_count += 1;

    if (layout == NULL) {
        layout = (Attribute *)malloc(sizeof(Attribute));
    } else {
        layout = (Attribute *)realloc(layout, layout_count * sizeof(Attribute));
    }

    layout[layout_count - 1] = attrib;

    stride = getTotalSize_bytes(layout, layout_count);
    layout[layout_count - 1].pointer = (void *)getTotalSize_bytes(layout, layout_count - 1);
}

void VertexBuffer::addAttribute(const GLint count, const GLenum type, const GLboolean normalized) {
    layout_count += 1;

    if (layout == NULL) {
        layout = (Attribute *)malloc(sizeof(Attribute));
    } else {
        layout = (Attribute *)realloc(layout, layout_count * sizeof(Attribute));
    }

    layout[layout_count - 1].count = count;
    layout[layout_count - 1].type = type;
    layout[layout_count - 1].normalized = normalized;

    stride = getTotalSize_bytes(layout, layout_count);
    layout[layout_count - 1].pointer = (void *)getTotalSize_bytes(layout, layout_count - 1);
}

void VertexBuffer::setLayout(const Attribute *input_layout, const GLuint count) {
    if (layout != NULL) {
        layout = (Attribute *)realloc(layout, count * sizeof(Attribute));
    } else {
        layout = (Attribute *)malloc(count * sizeof(Attribute));
    }

    layout_count = count;
    stride = getTotalSize_bytes(input_layout, count);

    for (uint i = 0; i < layout_count; i++) {
        layout[i] = input_layout[i];
        layout[i].pointer = (void *)getTotalSize_bytes(input_layout, i);
    }
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
        layout[i].pointer = (void *)getTotalSize_bytes(layout, i);
    }

    layout_count -= 1;

    layout = (Attribute *)realloc(layout, layout_count * sizeof(Attribute));

    stride = getTotalSize_bytes(layout, layout_count);
}

GLuint VertexBuffer::getCount() const { return layout_count; }
GLuint VertexBuffer::getId() const { return gl_id; }
GLuint VertexBuffer::getStride() const { return stride; }
Attribute VertexBuffer::getAttribute(const uint index) const {
    if (index < 0 || index >= layout_count) {
        printf("Error: Invalid Index\n");
        exit(1);
    }

    return layout[index];
}
