#ifndef TICTACTOE_ASTAR_RENDERER_CROSS_H_
#define TICTACTOE_ASTAR_RENDERER_CROSS_H_
#include "tictactoe_astar/renderer/shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace tictactoe_astar::renderer {

class Cross {
private:
  int _size;
  int _vertices_size;
  GLuint _vertex_buffer_object;
  GLuint _vertex_array_object;
  Shader *_shader;
  float _offset[2];

public:
  Cross(int size, float line_width, Shader *shader);
  Cross(Cross &&other);
  ~Cross();
  void set_location(int location);
  void draw();

private:
  std::vector<float> create_cross_vectices(int size, float line_width);
};

} // namespace tictactoe_astar::renderer

#endif
