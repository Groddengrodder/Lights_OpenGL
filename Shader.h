#ifndef __SHADER__
#define __SHADER__
#include "OpenGl_Header.h"

class Shader {
    public:
    Shader(char VertexPath[], char FragmentPath[]);
    ~Shader();

    void bind() const;
    void unbind() const;

    GLuint getId() const;

    private:
    GLuint gl_id;
};
#endif
