#ifndef __SHADER__
#define __SHADER__
#include "OpenGl_Header.h"

class Shader {
    public:
    Shader(const char VertexPath[], const char FragmentPath[]);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform(const char *UniformName, const GLfloat f1, const GLfloat f2, const GLfloat f3,
                    const GLfloat f4) const;

    GLuint getId() const;

    private:
    GLuint gl_id;
};
#endif
