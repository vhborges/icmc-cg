#include <cg_window.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Catavento : public cgicmc::Window {
public:
  Catavento() : angle(0), increment(0.01), pause(false),
                release(true), cgicmc::Window() {}

  void run() override {
    while (!glfwWindowShouldClose(_window)) {

      // Comandos de entrada
      processInput();
      if (!pause)
        angle -= increment;

      // Comandos de renderizacao vao aqui
      glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      // etc...

      glm::mat4 transform = glm::mat4(1.0f);
      transform = glm::translate(transform, glm::vec3(x, y, 0.0f));
      transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));

      glUseProgram(shaderProgramId);
      unsigned int transformLoc =
          glGetUniformLocation(shaderProgramId, "transform");
      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

      glBindVertexArray(_VAO);
      glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
      // Controla eventos e troca os buffers para renderizacao
      glfwSwapBuffers(_window);
      glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
  }

  void processInput() override
  {
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(_window, true);
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
      y += 0.01;
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
      y -= 0.01;
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
      x -= 0.01;
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
      x += 0.01;
    if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
    {
      if (!pause)
        increment += 0.001;
    }
    if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
    {
      if (!pause)
        increment -= 0.001;
    }
    if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
      if (release)
      {
        if (pause)
          pause = false;
        else
          pause = true;
      }
      release = false;
    }
    if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_RELEASE)
      release = true;
  }

  void prepare() {
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
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
  }

  unsigned int loadVertexShader() {
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

  unsigned int loadFragmentShader() {
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

  void loadShaders() {
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

protected:
  const char *_vertexShader;
  int shaderProgramId;
  unsigned int _VBO, _VAO, _EBO;
  float angle, increment, x = 0, y = 0;
  bool pause, release;
};

int main(int argc, char const *argv[]) {
  Catavento window;
  window.createWindow();
  window.loadShaders();
  window.prepare();
  window.run();
}