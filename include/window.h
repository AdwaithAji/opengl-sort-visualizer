#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>


class Window {
  
  private:
    GLFWwindow* window;
    
  public:
    int windowWidth;
    int windowHeight;
    bool fullScreen = false;

    Window(int width, int height);
   ~Window();
    
    bool init();
    bool shouldClose();
    GLFWwindow* getWindow();
    void clearScreen(float r, float g, float b, float a);
    void setViewPort();
    void swapBuffers();
    void pollEvents();
};
