#ifndef __INDEX_BUFFER__
#define __INDEX_BUFFER__

#include "OpenGl_Header.h"

class IndexBuffer {
    public:
    IndexBuffer(const GLuint count, const GLuint *data, const GLint GL_OPTION);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
    GLuint getCount() const;
    GLuint getId() const;

    private:
    GLuint gl_id;
    GLuint count;
};
#endif
