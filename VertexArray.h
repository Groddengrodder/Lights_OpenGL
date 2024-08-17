#include "OpenGl_Header.h"

class VertexArray {
    public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    GLuint getId() const;

    private:
    GLuint gl_id;
};
