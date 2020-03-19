//#include "../../glUtils/Test2.h"
#include "demo.h"
#include "Log.h"
//int main(void) {
//    try_me();
//}

int try_me(void)
{
    //Cecilion::Log::Init();
    CORE_LOG_INFO("Demo says hello! ");
    //std::cout << "The Demo says hello" << std::endl;
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World!", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current. GLEW needs this before we can use it. */
    glfwMakeContextCurrent(window);


//    GLenum err = glewInit();
//    if (GLEW_OK != err)
//    {
//        /* Problem: glewInit failed, something is seriously wrong. */
//        return -1;
//    }

//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        //std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }


//    std::shared_ptr<spdlog::logger> s_CoreLogger;
//    spdlog::pattern_formatter("%^[%T] %n: %v%$");
//    s_CoreLogger = spdlog::stdout_color_mt("Cecilion");
//    s_CoreLogger->set_level(spdlog::level::trace);
//    s_CoreLogger->trace("YASS IT IS WORKING!1!!");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);


        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f,-0.6f);
        glVertex2f(0,0.5f);
        glVertex2f(0.5f,-0.5f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
