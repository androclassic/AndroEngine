#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include "../../AndroUtils/EventManager/EventManager.h"
#include "../../AndroUtils/EventManager/WindowsInputEvents.h"
#include "AndroUtils/Utils/Trace.h"

#include<string>

    class Window {
    public:
        Window(int width, int height);

        ~Window();

        void Update(afloat dt);

        bool ShouldClose();

        void SetTitle(std::string title);
        private:
            //event callbacks
            static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
            static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void mouse_move_callback(GLFWwindow *window, double xpos, double ypos);
            //---


            void InitWindow();
            static void ErrorCallback(int pError, const char* pDescription);

            //private members
            GLFWwindow* mWindow; //reference to window object; managed by glfw library
            int mWidth;
            int mHeight;

    };
