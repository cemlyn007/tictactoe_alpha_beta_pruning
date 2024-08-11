#ifndef TICTACTOE_ASTAR_RENDERER_H_
#define TICTACTOE_ASTAR_RENDERER_H_
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
  GLuint _shader_program;

public:
  Renderer(size_t size);
  ~Renderer();
  void render();
  bool should_close();

private:
  std::vector<float> create_grid_vectices();
  GLuint load_shader_program();
  GLuint load_vertex_shader();
  GLuint load_fragment_shader();
  std::string read_shader(const std::string &file_path);
};

} // namespace tictactoe_astar::renderer

#endif
