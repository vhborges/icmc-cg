#ifndef __CG_WINDOW_HPP__
#define __CG_WINDOW_HPP__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <catavento.hpp>

namespace cgicmc {

class Window {
public:
  ///
  /// Instantiates glfw environment
  Window();

  ///
  /// Finalizes glfw environment
  ~Window();

  ///
  /// Create single window of 800x600 size
  void createWindow();

  ///
  /// Run the application in a loop.
  void run(Catavento& c);

  void prepare();
  unsigned int loadVertexShader();
  unsigned int loadFragmentShader();
  void loadShaders();

  static void framebufferCallback(GLFWwindow *window, int width, int height);

protected:
  void processInput(Catavento& c);

  int _viewPortSize;
  int shaderProgramId;
  unsigned int _VBO, _VAO, _EBO;
  GLFWwindow *_window;
  const char *_vertexShader;
};
} // namespace cgicmc

#endif