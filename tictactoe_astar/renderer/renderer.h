#ifndef TICTACTOE_ASTAR_RENDERER_H_
#define TICTACTOE_ASTAR_RENDERER_H_
#include "tictactoe_astar/renderer/shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

namespace tictactoe_astar::renderer {

void init();

void terminate();

class Renderer {
private:
  const size_t _size;
  size_t _vertices_size;
  GLFWwindow *_window;
  GLuint _vertex_buffer_object;
  GLuint _vertex_array_object;
  Shader _shader;

public:
  Renderer(size_t size);
  ~Renderer();
  void render();
  bool should_close();

private:
  GLFWwindow *create_window();
  std::vector<float> create_grid_vectices();
  std::string read_shader(const std::string &file_path);
};

} // namespace tictactoe_astar::renderer

#endif
