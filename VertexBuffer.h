#ifndef __VERTEX_BUFFER__
#define __VERTEX_BUFFER__

#include "OpenGl_Header.h"

class VertexBuffer {
    public:
    VertexBuffer(const GLuint size_bytes, const void *data, const GLint GL_OPTION);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    private:
    GLuint gl_id;
};
#endif
