#include <cg_window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace cgicmc {

Window::Window() {
  glfwSetErrorCallback(error_callback);
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

void Window::processInput(Catavento& catavento) {
  if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(_window, true);

  if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
    catavento.verticalTranslation(0.01f);

  if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
    catavento.verticalTranslation(-0.01f);

  if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
    catavento.horizontalTranslation(-0.01f);

  if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
    catavento.horizontalTranslation(0.01f);

  if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
    catavento.changeRotationSpeed(0.001f);

  if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
    catavento.changeRotationSpeed(-0.001f);

  if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    catavento.pauseRotation();

  if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    catavento.buttonRelease();
}

void Window::prepare() {
  float positions [] =
  {
      -0.3f, 0.3f, 0.0f,
      -0.3f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.0f, 0.3f, 0.0f,
      0.3f, 0.3f, 0.0f,
      0.3f, 0.0f, 0.0f,
      0.3f, -0.3f, 0.0f,
      0.0f, -0.3f, 0.0f,
      -0.3f, -0.3f, 0.0f
  };
  
  unsigned int indices [] = 
  {
      0, 1, 2, //primeiro triangulo
      3, 4, 2, //segundo triangulo
      5, 6, 2, //terceiro triangulo
      7, 8, 2  //quarto triangulo
  };
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  glGenBuffers(1, &_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
  
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int Window::loadVertexShader() {
  _vertexShader = "#version 330 core\n"
                  "layout(location = 0) in vec3 aPos;\n"
                  "uniform mat4 transform;\n"
                  "void main() {\n"
                  "gl_Position = transform * vec4(aPos, 1.0f);\n"
                  "}\0";
  unsigned int vertexShaderId;
  vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShaderId, 1, &_vertexShader, NULL);
  glCompileShader(vertexShaderId);
  int succes;
  glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &succes);
  if (!succes) {
    char infoLog[512];
    glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
    std::cerr << "Vertex shader error: " << infoLog << std::endl;
  }
  return vertexShaderId;
}

unsigned int Window::loadFragmentShader() {
  const char *shaderSource = "#version 330 core\n"
                             "out vec4 FragColor;\n"
                             "void main()\n"
                             "{\n"
                             "   FragColor = vec4(0.6f, 0.8f, 0.8f, 1.0f);\n"
                             "}\n\0";
  unsigned int fragmentShaderId;
  fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderId, 1, &shaderSource, NULL);
  glCompileShader(fragmentShaderId);
  int success = 0;
  glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
    std::cout << "Shader source: " << shaderSource << std::endl;
    std::cerr << "Fragment shader error: " << infoLog << std::endl;
  }
  return fragmentShaderId;
}

void Window::loadShaders() {
  unsigned int vshader, fshader;
  vshader = loadVertexShader();
  fshader = loadFragmentShader();
  shaderProgramId = glCreateProgram();
  glAttachShader(shaderProgramId, fshader);
  glAttachShader(shaderProgramId, vshader);
  glLinkProgram(shaderProgramId);
  int success;
  glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
    std::cerr << "Program shader error: " << infoLog << std::endl;
  }
  glDeleteShader(vshader);
  glDeleteShader(fshader);
}

void Window::run(Catavento& catavento) {
  while (!glfwWindowShouldClose(_window)) {

      // Comandos de entrada
      processInput(catavento);
      catavento.rotate();

      // Comandos de renderizacao vao aqui
      glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glm::mat4 transform = glm::mat4(1.0f);
      transform = glm::translate(transform, catavento.getVec3Position());
      transform = glm::rotate(transform, catavento.getAngle(), glm::vec3(0.0f, 0.0f, 1.0f));

      glUseProgram(shaderProgramId);
      unsigned int transformLoc =
          glGetUniformLocation(shaderProgramId, "transform");
      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

      glBindVertexArray(_VAO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
      glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
      // Controla eventos e troca os buffers para renderizacao
      glfwSwapBuffers(_window);
      glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
}

void Window::error_callback(int error, const char* description)
{
  std::cerr << "GLFW error code: " << error << ". Description: " << description << std::endl;
}

} // namespace cgicmc