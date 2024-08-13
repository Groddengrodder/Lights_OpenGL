CC = g++
CFLAGS = -std=c++11 -O3
LIBS = -lm -lglfw -lGLEW -lGL

VertexBuffer.o: VertexBuffer.cpp
	$(CC) $(CFLAGS) -c -o VertexBuffer.o VertexBuffer.cpp

IndexBuffer.o: IndexBuffer.cpp
	$(CC) $(CFLAGS) -c -o IndexBuffer.o IndexBuffer.cpp

window.o:
	$(CC) $(CFLAGS) -c -o window.o window.cpp

window: window.o VertexBuffer.o IndexBuffer.o
	$(CC) $(CFLAGS) -o window window.o VertexBuffer.o IndexBuffer.o $(LIBS)

rm_window:
	rm -f window.o window IndexBuffer.o VertexBuffer.o
