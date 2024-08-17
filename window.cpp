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

    GLfloat bg_positions[20] = {
        -1., -1., 0., 0.5, 1., -1., +1., 0., 0.5, 1., +1., +1., 0., 0.5, 1., +1., -1., 0., 0.5, 1.,
    };

    GLuint bg_index[6] = {0, 1, 2, 0, 2, 3};

    VertexBuffer vb_first_object(20 * sizeof(GLfloat), bg_positions, GL_STATIC_DRAW);
    vb_first_object.addAttribute(2, GL_FLOAT, GL_FALSE);
    vb_first_object.addAttribute(3, GL_FLOAT, GL_FALSE);

    IndexBuffer ib_first_object(6, bg_index, GL_STATIC_DRAW);

    GLfloat rechteck_positions[20] = {
        -.5, -.5, 1., 0., 1., -.5, +.5, 1., 0., 1., +.5, +.5, 0., 0., 1., +.5, -.5, 0., 0., 1.,
    };

    VertexBuffer rechteck(20 * sizeof(GLfloat), rechteck_positions, GL_STATIC_DRAW);
    rechteck.addAttribute(vb_first_object.getAttribute(0));
    rechteck.addAttribute(vb_first_object.getAttribute(1));

    char VertexSource[] = "shader/vertex_shader.glsl";
    char FragmentSource[] = "shader/fragment_shader.glsl";
    Shader shader(VertexSource, FragmentSource);

    shader.bind();
    vb_first_object.bind();
    ib_first_object.bind();

    char UniformName[] = "u_Color";
    shader.setUniform(UniformName, 1., 0., 0., 1.);

    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        draw(vb_first_object, ib_first_object, shader);
        draw(rechteck, ib_first_object, shader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
