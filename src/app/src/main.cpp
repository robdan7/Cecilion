//#define GLEW_STATIC
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <iostream>
#include <demo/demo.h>
//#include "../../glUtils/Test2.h"

int main(void)
{
        std::cout << "Application says hello" << std::endl;
        try_me();
//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//    /* Create a windowed mode window and its OpenGL context */
//    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
//    if (!window)
//    {
//        glfwTerminate();
//        return -1;
//    }
//
//    /* Make the window's context current. GLEW needs this before we can use it. */
//    glfwMakeContextCurrent(window);
//
//
//    GLenum err = glewInit();
//    if (GLEW_OK != err)
//    {
//        /* Problem: glewInit failed, something is seriously wrong. */
//        return -1;
//    }
//
//    /* Loop until the user closes the window */
//    while (!glfwWindowShouldClose(window))
//    {
//        /* Render here */
//        glClear(GL_COLOR_BUFFER_BIT);
//
//
//        glBegin(GL_TRIANGLES);
//        glVertex2f(-0.5f,-0.5f);
//        glVertex2f(0,0.5f);
//        glVertex2f(0.5f,-0.5f);
//        glEnd();
//
//        /* Swap front and back buffers */
//        glfwSwapBuffers(window);
//
//        /* Poll for and process events */
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//    return 0;
}