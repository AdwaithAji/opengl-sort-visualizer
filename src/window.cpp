#include "window.h"

Window::Window(int width, int height) {
  window = nullptr;
  windowWidth = width;
  windowHeight = height;
}

Window::~Window() {
  if(window) {
    glfwDestroyWindow(window);
  }
  glfwTerminate();
}

bool Window::init() {

  if(!glfwInit()) {
    std::cout<<"Error Initializing glfw\n";
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  window = glfwCreateWindow(windowWidth, windowHeight, "Test", NULL, NULL);
  
  if(!window) {
    std::cout<<"Error Creating window\n";
    return false;
  }

  glfwMakeContextCurrent(window);
  
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout<<"Error loading glad gl\n"; 
    return false;
  }

  return true; 
}

bool Window::shouldClose() {
  if(glfwWindowShouldClose(window)) {
    return true;
  }
  return false;
}

GLFWwindow* Window::getWindow(){
  return window;
}

void Window::clearScreen(float r, float g, float b, float a) {

  glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
  glViewport(0, 0, windowWidth, windowHeight);

  glClearColor(r,g,b,a);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Window::setViewPort() {
  glViewport(0,0, windowWidth, windowHeight);
  if(windowHeight > 600) {
    fullScreen = true;
  }else {
    fullScreen = false;
  }
}

void Window::swapBuffers() {
  glfwSwapBuffers(window);
}

void Window::pollEvents() {
  glfwPollEvents();
  glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
}

