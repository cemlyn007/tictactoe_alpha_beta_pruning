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

Renderer::Renderer(int size, int window_width, int window_height,
                   float line_width)
    : _size(size), _window(create_window(window_width, window_height)),
      _shader(read_shader(SHADER_VERTEX_FILE_PATH),
              read_shader(SHADER_FRAGMENT_FILE_PATH)),
      _grid(_size, line_width, _shader), _mouse_click(false),
      _escape_pressed(false) {
  int max_of_each = _size * _size;
  for (int index = 0; index < max_of_each; ++index) {
    _noughts.push_back(Nought(_size, line_width, &_shader));
    _noughts[index].set_location(0);
    _crosses.push_back(Cross(_size, line_width, &_shader));
    _crosses[index].set_location(0);
  }
}

Renderer::~Renderer() { glfwDestroyWindow(_window); };

void Renderer::render(const std::vector<engine::Occupancy> &grid) {
  glfwMakeContextCurrent(_window);
  GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

  _shader.use();

  _grid.draw();

  int free_nought_index = 0;
  int free_cross_index = 0;
  int location = 0;
  for (const engine::Occupancy &occupancy : grid) {
    if (occupancy == engine::Occupancy::EMPTY) {
    } else if (occupancy == engine::Occupancy::NOUGHT) {
      _noughts[free_nought_index].set_location(location);
      _noughts[free_nought_index].draw();
      ++free_nought_index;
    } else if (occupancy == engine::Occupancy::CROSS) {
      _crosses[free_cross_index].set_location(location);
      _crosses[free_cross_index].draw();
      ++free_cross_index;
    } else {
      throw std::runtime_error("Occupancy unknown!");
    }
    ++location;
  }

  GL_CALL(glfwSwapBuffers(_window));
  GL_CALL(glfwPollEvents());
}

bool Renderer::should_close() {
  return glfwWindowShouldClose(_window) || _escape_pressed;
}

std::tuple<int, bool> Renderer::get_selected_location() {
  int width;
  int height;
  glfwGetWindowSize(_window, &width, &height);
  float cell_width_in_pixels = float(width) / _size;
  float cell_height_in_pixels = float(height) / _size;
  // GLFW gives the y position where 0 is at the top of the window.
  double mouse_y_position_in_pixels = height - _mouse_position_in_pixels[1];
  int row = mouse_y_position_in_pixels / cell_height_in_pixels;
  int column = _mouse_position_in_pixels[0] / cell_width_in_pixels;
  int location = row * _size + column;
  bool mouse_clicked = _mouse_click;
  _mouse_click = false;
  return {location, mouse_clicked};
};

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
  glfwSetWindowUserPointer(window, this);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
  glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_position_callback);
  return window;
};

void Renderer::key_callback(GLFWwindow *window, int key, int scancode,
                            int action, int mods) {
  Renderer *renderer =
      static_cast<Renderer *>(glfwGetWindowUserPointer(window));
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    (renderer->_escape_pressed) = true;
}

void Renderer::mouse_button_callback(GLFWwindow *window, int button, int action,
                                     int mods) {
  Renderer *renderer =
      static_cast<Renderer *>(glfwGetWindowUserPointer(window));
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    (renderer->_mouse_click) = true;
}

void Renderer::cursor_position_callback(GLFWwindow *window, double xpos,
                                        double ypos) {
  Renderer *renderer =
      static_cast<Renderer *>(glfwGetWindowUserPointer(window));
  (renderer->_mouse_position_in_pixels[0]) = xpos;
  (renderer->_mouse_position_in_pixels[1]) = ypos;
}

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
