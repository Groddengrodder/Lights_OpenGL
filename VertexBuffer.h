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
    void addAttribute(GLint count, GLenum type, GLboolean normalized);
    void setLayout(Attribute *input_layout, GLuint count);
    void removeAttribute(const uint index);

    GLuint getCount() const;
    GLuint getId() const;
    GLuint getStride() const;
    Attribute getAttribute(uint index) const;

    private:
    Attribute *layout;
    GLuint gl_id;
    GLuint layout_count;
    GLsizei stride;
};
#endif
