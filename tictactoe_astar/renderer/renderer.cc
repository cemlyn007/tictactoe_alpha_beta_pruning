#include "tictactoe_astar/renderer/renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#define GL_CALL(cmd)                                                           \
  {                                                                            \
    cmd;                                                                       \
    GLenum err = glGetError();                                                 \
    if (err != GL_NO_ERROR) {                                                  \
      std::ostringstream oss;                                                  \
      oss << "OpenGL error: " << glErrorString(err) << " in file " << __FILE__ \
          << " at line " << __LINE__ << " after calling " #cmd;                \
      throw std::runtime_error(oss.str());                                     \
    }                                                                          \
  }

static const char *glErrorString(GLenum err) {
  switch (err) {
  case GL_NO_ERROR:
    return "No error";
  case GL_INVALID_ENUM:
    return "Invalid enum";
  case GL_INVALID_VALUE:
    return "Invalid value";
  case GL_INVALID_OPERATION:
    return "Invalid operation";
  case GL_STACK_OVERFLOW:
    return "Stack overflow";
  case GL_STACK_UNDERFLOW:
    return "Stack underflow";
  case GL_OUT_OF_MEMORY:
    return "Out of memory";
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    return "Invalid framebuffer operation";
  default:
    return "Unknown error";
  }
}

static void glfwErrorCallback(int error, const char *description) {
  std::cerr << "GLFW Error: " << error << " - " << description << std::endl;
  throw std::runtime_error(std::string("GLFW Error: ") + description);
}

namespace tictactoe_astar::renderer {

void init() {
  glfwSetErrorCallback(glfwErrorCallback);
  if (!glfwInit()) {
    throw std::runtime_error("Could not initialize glfw");
  }
  // else...
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
};

void terminate() {
  glfwSetErrorCallback(nullptr);
  glfwTerminate();
};

Renderer::Renderer(size_t size) : _size(size) {
  _window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!_window) {
    throw std::runtime_error("Could not create window");
  }
  glfwMakeContextCurrent(_window);
  GLenum error = glewInit();
  if (GLEW_OK != error) {
    throw std::runtime_error(
        std::string("Error initializing glew: ") +
        reinterpret_cast<const char *>(glewGetErrorString(error)));
  }
}

Renderer::~Renderer() { glfwDestroyWindow(_window); };

void Renderer::render() {
  glfwMakeContextCurrent(_window);
  GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

  unsigned int VBO;
  GL_CALL(glGenBuffers(1, &VBO));

  unsigned int VAO;
  GL_CALL(glGenVertexArrays(1, &VAO));
  GL_CALL(glBindVertexArray(VAO));

  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
  GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                       GL_STATIC_DRAW));

  GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                (void *)0));
  GL_CALL(glEnableVertexAttribArray(0));

  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  GL_CALL(glShaderSource(vertexShader, 1, &vertexShaderSource, NULL));
  GL_CALL(glCompileShader(vertexShader));
  int success;
  char infoLog[512];
  GL_CALL(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    throw std::runtime_error(
        std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n") + infoLog);
  }
  const char *fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\0";

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  GL_CALL(glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL));
  GL_CALL(glCompileShader(fragmentShader));

  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  GL_CALL(glAttachShader(shaderProgram, vertexShader));
  GL_CALL(glAttachShader(shaderProgram, fragmentShader));
  GL_CALL(glLinkProgram(shaderProgram));
  GL_CALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success));
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::FAILED\n" << infoLog << std::endl;
  }
  GL_CALL(glUseProgram(shaderProgram));

  GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                (void *)0));
  GL_CALL(glEnableVertexAttribArray(0));

  GL_CALL(glUseProgram(shaderProgram));

  GL_CALL(glBindVertexArray(VAO));
  GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));

  GL_CALL(glfwSwapBuffers(_window));
  GL_CALL(glfwPollEvents());

  GL_CALL(glDeleteBuffers(1, &VBO));
  GL_CALL(glDeleteShader(vertexShader));
  GL_CALL(glDeleteShader(fragmentShader));
}

bool Renderer::should_close() { return glfwWindowShouldClose(_window); }

} // namespace tictactoe_astar::renderer
