#include <GL/glew.h>
#include "Window.h"


    Window::Window(int width, int height)
    {
         mHeight = height;
         mWidth = width;
         InitWindow();
    }


    bool Window::ShouldClose() {
        //convert to bool before returning to avoid warning
        return glfwWindowShouldClose(mWindow) != 0;
    }


    void Window::ErrorCallback(int pError, const char *pDescription) {
    	TRACE(L"GLFW Error[%d]: %s", pError, pDescription);
    }


    void Window::InitWindow()
    {
        //-------------------------------

        //init glfw
		glfwSetErrorCallback(Window::ErrorCallback);

        if (!glfwInit()) {
			TRACE(L"glfwInit Failed!");
        }

        glfwWindowHint( GLFW_RESIZABLE , GL_FALSE );

        glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef ENABLE_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
        //create window
        mWindow = glfwCreateWindow(mWidth, mHeight, "AndroEngine", NULL, NULL);


        if (!mWindow) {
			TRACE(L"glfwCreateWindow Failed!");
        }

        glfwSetKeyCallback(mWindow,Window::key_callback);
        glfwSetMouseButtonCallback(mWindow,Window::mouse_button_callback);
        glfwSetCursorPosCallback(mWindow,Window::mouse_move_callback);

        //use the context created
        glfwMakeContextCurrent(mWindow);
        //VSync: 0->off, 1->on
        glfwSwapInterval(0);


    }

    Window::~Window()
    {
        glfwDestroyWindow(mWindow);
        glfwTerminate();
    }

    void Window::Update(afloat dt)
    {

        //get the events
        glfwPollEvents();
        // Present
        glfwSwapBuffers(mWindow);

    }


    void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
		if (action != GLFW_RELEASE)
        {
            PostEvent(new KeyPressedEvent(key));
        }
        else 
        {
            PostEvent(new KeyUpEvent(key));
        }
    }

    void Window::mouse_move_callback(GLFWwindow *window, double xpos, double ypos)
    {
        PostEvent(new MouseMove(xpos,ypos));
    }

    void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT )
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            if(action == GLFW_PRESS)
                PostEvent(new MouseLButtonPressed( xpos, ypos));
            else if(action == GLFW_RELEASE)
                PostEvent(new MouseLButtonUp( xpos, ypos));

        }
    }

    void Window::SetTitle(std::string title)
    {
        glfwSetWindowTitle (mWindow, title.c_str());
    }
