#include "tictactoe_astar/renderer/shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace tictactoe_astar::renderer {

class Nought {
private:
  int _size;
  size_t _vertices_size;
  GLuint _vertex_buffer_object;
  GLuint _vertex_array_object;
  Shader &_shader;
  float _offset[2];

  std::vector<float> create_nought_vectices(int size, float line_width);

public:
  Nought(int size, float line_width, Shader &shader);
  ~Nought();
  void set_location(int location);
  void draw();
};

} // namespace tictactoe_astar::renderer
