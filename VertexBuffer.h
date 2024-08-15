#ifndef __VERTEX_BUFFER__
#define __VERTEX_BUFFER__

#include "OpenGl_Header.h"

typedef struct {
    GLint count;
    GLenum type;
    GLboolean normalized;
    GLvoid *pointer;
} Attribute;

class VertexBuffer {
    public:
    VertexBuffer(const GLuint size_bytes, const void *data, const GLint GL_OPTION);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
    void addAttribute(const Attribute attrib);
    void addAttribute(const GLint count, const GLenum type, const GLboolean normalized);
    void setLayout(const Attribute *input_layout, const GLuint count);
    void removeAttribute(const uint index);

    GLuint getCount() const;
    GLuint getId() const;
    GLuint getStride() const;
    Attribute getAttribute(const uint index) const;

    private:
    Attribute *layout;
    GLuint gl_id;
    GLuint layout_count;
    GLsizei stride;
};
#endif
