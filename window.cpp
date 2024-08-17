#include "IndexBuffer.h"
#include "OpenGl_Header.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

const GLuint window_width = 640;
const GLuint window_height = 480;
const GLchar *window_name = "A new Window";

int main(void) {
    GLFWwindow *window;
    init_OpenGL(&window, window_width, window_height, window_name);

    VertexArray GlobalVertexArray;
    GlobalVertexArray.bind();

    GLuint bg_index[6] = {0, 1, 2, 0, 2, 3};

    IndexBuffer ib_first_object(6, bg_index, GL_STATIC_DRAW);

    GLfloat rechteck_positions[20] = {
        -.5, -.5, 1., 0., 1., -.5, +.5, 1., 0., 1., +.5, +.5, 0., 0., 1., +.5, -.5, 0., 0., 1.,
    };

    GLfloat bg_position[2];
    bg_position[0] = -1;
    bg_position[1] = -1;

    GLfloat bg_color[3];
    bg_color[0] = 0;
    bg_color[1] = 0.5;
    bg_color[2] = 1;

    VertexBuffer rechteck(20 * sizeof(GLfloat), rechteck_positions, GL_STATIC_DRAW);
    rechteck.addAttribute(2, GL_FLOAT, GL_FALSE);
    rechteck.addAttribute(3, GL_FLOAT, GL_FALSE);

    char VertexSource[] = "shader/vertex_shader.glsl";
    char FragmentSource[] = "shader/fragment_shader.glsl";
    Shader shader(VertexSource, FragmentSource);

    shader.bind();
    ib_first_object.bind();

    char UniformName[] = "u_Color";
    shader.setUniform(UniformName, 1., 0., 0., 1.);

    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        drawRect(bg_position, bg_color, window_width, window_height);
        draw(rechteck, ib_first_object, shader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
