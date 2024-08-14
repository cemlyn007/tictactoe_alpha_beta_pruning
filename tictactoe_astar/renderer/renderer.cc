#include "tictactoe_astar/renderer/renderer.h"
#include "tictactoe_astar/renderer/cross.h"
#include "tictactoe_astar/renderer/gl_error_macro.h"
#include "tictactoe_astar/renderer/grid.h"
#include "tictactoe_astar/renderer/nought.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

static const char *SHADER_VERTEX_FILE_PATH =
    "tictactoe_astar/renderer/shaders/shader.vert";
static const char *SHADER_FRAGMENT_FILE_PATH =
    "tictactoe_astar/renderer/shaders/shader.frag";
static const float LINE_WIDTH = 0.025;

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

Renderer::Renderer(int size, int window_width, int window_height)
    : _size(size), _window(create_window(window_width, window_height)),
      _shader(read_shader(SHADER_VERTEX_FILE_PATH),
              read_shader(SHADER_FRAGMENT_FILE_PATH)),
      _grid(_size, LINE_WIDTH, _shader) {
  int max_of_each = ((_size + 1) / 2) * ((_size + 1) / 2) + 1;
  for (int index = 0; index < max_of_each; ++index) {
    _noughts.push_back(Nought(_size, LINE_WIDTH, &_shader));
    _noughts[index].set_location(0);
    _crosses.push_back(Cross(_size, LINE_WIDTH, &_shader));
    _crosses[index].set_location(0);
  }
}

Renderer::~Renderer() { glfwDestroyWindow(_window); };

void Renderer::render() {
  glfwMakeContextCurrent(_window);
  GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

  _shader.use();

  _grid.draw();
  for (auto &nought : _noughts) {
    nought.set_location(std::rand() % (_size * _size));
    nought.draw();
  }
  for (auto &cross : _crosses) {
    cross.set_location(std::rand() % (_size * _size));
    cross.draw();
  }

  GL_CALL(glfwSwapBuffers(_window));
  GL_CALL(glfwPollEvents());
}

bool Renderer::should_close() { return glfwWindowShouldClose(_window); }

GLFWwindow *Renderer::create_window(int width, int height) {
  GLFWwindow *window =
      glfwCreateWindow(width, height, "Tic Tac Toe", NULL, NULL);
  if (!window) {
    throw std::runtime_error("Could not create window");
  }
  // else...
  glfwMakeContextCurrent(window);
  GLenum error = glewInit();
  if (GLEW_OK != error) {
    glfwDestroyWindow(window);
    throw std::runtime_error(
        std::string("Error initializing glew: ") +
        reinterpret_cast<const char *>(glewGetErrorString(error)));
  }
  return window;
};

std::string Renderer::read_shader(const std::string &file_path) {
  std::ifstream file(file_path);
  if (!file) {
    throw std::runtime_error("Could not open file: " + file_path);
  }
  // else...
  std::stringstream buffer;
  buffer << file.rdbuf(); // Read the entire file into the stringstream buffer
  return buffer.str();    // Return the string from the buffer
}
} // namespace tictactoe_astar::renderer
