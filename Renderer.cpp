#include "Renderer.h"

void draw(const VertexBuffer &vertexbuffer, const IndexBuffer &indexbuffer, const Shader &shader) {
    shader.bind();
    vertexbuffer.bind();
    indexbuffer.bind();

    glCall(glDrawElements(GL_TRIANGLES, indexbuffer.getCount(), GL_UNSIGNED_INT, nullptr));
}

void drawRect(const GLfloat position[], const GLfloat color[], const GLuint width,
              const GLuint height) {
    GLfloat data[20];
    data[0] = position[0];
    data[1] = position[1];

    data[5] = position[0] + width;
    data[6] = position[1];

    data[10] = position[0] + width;
    data[11] = position[1] + height;

    data[15] = position[0];
    data[16] = position[1] + height;

    for (uint i = 0; i < 4; i++) {
        for (uint j = 0; j < 3; j++) {
            data[i * 5 + 2 + j] = color[j];
        }
    }

    GLuint index[6] = {0, 2, 3, 0, 1, 2};

    VertexBuffer rect_vb(sizeof(GLfloat) * 20, data, GL_STATIC_DRAW);
    IndexBuffer rect_ib(6, index, GL_STATIC_DRAW);

    rect_vb.addAttribute(2, GL_FLOAT, GL_FALSE);
    rect_vb.addAttribute(3, GL_FLOAT, GL_FALSE);

    rect_vb.bind();
    rect_ib.bind();

    glCall(glDrawElements(GL_TRIANGLES, rect_ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void drawRect(const GLfloat position[], const GLuint width, const GLuint height) {
    GLfloat data[8];
    data[0] = position[0];
    data[1] = position[1];

    data[2] = position[0] + width;
    data[3] = position[1];

    data[4] = position[0] + width;
    data[5] = position[1] + height;

    data[6] = position[0];
    data[7] = position[1] + height;

    GLuint index[6] = {0, 2, 3, 0, 1, 2};

    VertexBuffer rect_vb(sizeof(GLfloat) * 8, data, GL_STATIC_DRAW);
    IndexBuffer rect_ib(6, index, GL_STATIC_DRAW);

    rect_vb.addAttribute(2, GL_FLOAT, GL_FALSE);

    rect_vb.bind();
    rect_ib.bind();

    glCall(glDrawElements(GL_TRIANGLES, rect_ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void drawRect(const GLfloat x, const GLfloat y, GLuint width, GLuint height) {
    GLfloat data[8];
    data[0] = x;
    data[1] = y;

    data[2] = x + width;
    data[3] = y;

    data[4] = x + width;
    data[5] = y + height;

    data[6] = x;
    data[7] = y + height;

    GLuint index[6] = {0, 2, 3, 0, 1, 2};

    VertexBuffer rect_vb(sizeof(GLfloat) * 8, data, GL_STATIC_DRAW);
    IndexBuffer rect_ib(6, index, GL_STATIC_DRAW);

    rect_vb.addAttribute(2, GL_FLOAT, GL_FALSE);

    rect_vb.bind();
    rect_ib.bind();

    glCall(glDrawElements(GL_TRIANGLES, rect_ib.getCount(), GL_UNSIGNED_INT, nullptr));
}
