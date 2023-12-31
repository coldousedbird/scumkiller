#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "renderer/shaderProgram.hpp"
#include "resources/resourceManager.hpp"

#ifdef __unix__    /* __unix__ is usually defined by compilers targeting Unix systems */
    #define OS_Windows 0
    #include <unistd.h>
    #include <stdlib.h>
    #include <string.h>
#elif defined(_WIN32) || defined(WIN32)    /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */
    #define OS_Windows 1
    #include <windows.h>
    #include <tchar.h>
    // ?????????????
    #define DIV 1048576
    #define WIDTH 7
#endif

// use discrete GPU by default, instead of intel(R)HD Graphics
#if defined(_WIN32) || defined(WIN32)                                           // if Windows
extern "C" {
    int discreteGPU() {
        __declspec(dllexport) DWORD NvOptimusEnablement = 0;                    // NVidia GeForce
        __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0;     // AMD
        return 1;
    }
}
#endif

// !!!!!!!!!!!!!!!!!!!!!WELLL THAT DOESN'T REALLY WORK RIGHT NOW
#if defined(__unix__)                                                           // if Unix (Linux/MacOS)
extern "C" {
    int discreteGPU() {
        //__attribute__((dllexport)) unsigned long NvOptimusEnablement = 0x00000001;           // NVidia Geforce
        //__attribute__((dllexport)) int AmdPowerXpressRequestHighPerformance = 1;              // AMD 
        return 1;
    }
}
#endif

// --------------------------------------- TRIANGLES---------------------------------------
GLfloat point[] = {
    0.0f,  0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f
};

GLfloat color[] = {
    1.0f, 0.0f, 0.0f, 
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

// --------------------------------------- CALLBACKS --------------------------------------- 
int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char ** argv) {
    if (!discreteGPU()) {
        std::cout << "discrete GPU not working\n";
    }

    /* Init GLFW*/
    if (!glfwInit()){
        std::cout << "glfwInit failed\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "SCUMKILLER", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout<<"glfwCreateWindow failed\n";
        glfwTerminate();
        return -1;
    }
    
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    
    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);
    if(!gladLoadGL()){ 
        std::cout << "gladLoadGL failed\n";
        return -1;
    }
    
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    glClearColor(1, 1, 0, 1);

    {
        resourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram) {
            std::cerr << "Can't create shader program: " << "DefaultShader" << "\n";
            return -1;
        }

        resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        GLuint point_vbo = 0;
        glGenBuffers(1, &point_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, point_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

        GLuint color_vbo = 0;
        glGenBuffers(1, &color_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

        // vertecs array object
        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, point_vbo);
        // GL_FALSE responds for normalization, read about it
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}
