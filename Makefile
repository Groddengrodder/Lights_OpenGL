CC = g++
CFLAGS = -std=c++11 -O3
LIBS = -lm -lglfw -lGLEW -lGL

VertexArray.o: VertexArray.cpp
	$(CC) $(CFLAGS) -c -o VertexArray.o VertexArray.cpp

Renderer.o: Renderer.cpp
	$(CC) $(CFLAGS) -c -o Renderer.o Renderer.cpp

VertexBuffer.o: VertexBuffer.cpp
	$(CC) $(CFLAGS) -c -o VertexBuffer.o VertexBuffer.cpp

IndexBuffer.o: IndexBuffer.cpp
	$(CC) $(CFLAGS) -c -o IndexBuffer.o IndexBuffer.cpp

Shader.o: Shader.cpp
	$(CC) $(CFLAGS) -c -o Shader.o Shader.cpp

window.o: window.cpp
	$(CC) $(CFLAGS) -c -o window.o window.cpp

window: window.o VertexBuffer.o IndexBuffer.o Shader.o Renderer.o VertexArray.o
	$(CC) $(CFLAGS) -o window window.o VertexBuffer.o IndexBuffer.o Shader.o Renderer.o VertexArray.o $(LIBS)

rm_window:
	rm -f window *.o
