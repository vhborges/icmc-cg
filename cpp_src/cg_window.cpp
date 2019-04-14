#include <cg_window.hpp>

namespace cgicmc {

Window::Window() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Window::~Window() { glfwTerminate(); }

void Window::framebufferCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void Window::createWindow() {
  _window = glfwCreateWindow(700, 700, "CG 2019", NULL, NULL);
  if (_window == NULL) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(_window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    exit(-2);
  }

  glfwSwapInterval(1);

  glfwSetFramebufferSizeCallback(_window, this->framebufferCallback);
}

void Window::processInput() {
  if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(_window, true);
}

void Window::run() {
  while (!glfwWindowShouldClose(_window)) {

    // Comandos de entrada
    processInput();

    // Comandos de renderizacao vao aqui
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // etc...

    // Controla eventos e troca os buffers para renderizacao
    glfwSwapBuffers(_window);
    glfwPollEvents();
  }
}
} // namespace cgicmc