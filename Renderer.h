#ifndef __RENDERER__
#define __RENDERER__
#include "IndexBuffer.h"
#include "OpenGl_Header.h"
#include "Shader.h"
#include "VertexBuffer.h"

void draw(const VertexBuffer &vertexbuffer, const IndexBuffer &indexbuffer, const Shader &shader);
#endif
