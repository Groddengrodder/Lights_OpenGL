#include "Renderer.h"

void draw(const VertexBuffer &vertexbuffer, const IndexBuffer &indexbuffer, const Shader &shader) {
    shader.bind();
    vertexbuffer.bind();
    indexbuffer.bind();

    glCall(glDrawElements(GL_TRIANGLES, indexbuffer.getCount(), GL_UNSIGNED_INT, nullptr));
}
