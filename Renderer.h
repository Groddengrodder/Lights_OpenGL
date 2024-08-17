#ifndef __RENDERER__
#define __RENDERER__
#include "IndexBuffer.h"
#include "OpenGl_Header.h"
#include "Shader.h"
#include "VertexBuffer.h"

void draw(const VertexBuffer &vertexbuffer, const IndexBuffer &indexbuffer, const Shader &shader);

void drawRect(const GLfloat position[], const GLfloat color[], const GLuint width,
              const GLuint height);

void drawRect(const GLfloat position[], const GLuint width, const GLuint height);

void drawRect(const GLfloat x, const GLfloat y, const GLuint width, const GLuint height);
#endif
