#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace tictactoe_astar::renderer {

class Nought {
private:
  size_t _vertices_size;
  GLuint _vertex_buffer_object;
  GLuint _vertex_array_object;

  std::vector<float> create_nought_vectices(int size, float line_width);

public:
  Nought(int size, float line_width);
  ~Nought();
  void draw();
};

} // namespace tictactoe_astar::renderer
