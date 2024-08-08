#include "tictactoe_astar/renderer/renderer.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace tictactoe_astar::renderer {

void init() {
  if (!glfwInit()) {
    throw std::runtime_error("Could not initialize");
  }
};

void terminate() { glfwTerminate(); };

Renderer::Renderer(size_t size) : _size(size) {
  _window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!_window) {
    throw std::runtime_error("Could not create window");
  }
}

Renderer::~Renderer() { glfwDestroyWindow(_window); };

void Renderer::render() {
  glfwMakeContextCurrent(_window);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(_window);
  glfwPollEvents();
}

bool Renderer::should_close() { return glfwWindowShouldClose(_window); }

} // namespace tictactoe_astar::renderer
