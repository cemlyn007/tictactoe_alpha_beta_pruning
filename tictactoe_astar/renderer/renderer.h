#ifndef TICTACTOE_ASTAR_RENDERER_H_
#define TICTACTOE_ASTAR_RENDERER_H_
#include "tictactoe_astar/renderer/cross.h"
#include "tictactoe_astar/renderer/grid.h"
#include "tictactoe_astar/renderer/nought.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string>
#include <vector>

namespace tictactoe_astar::renderer {

void init();

void terminate();

class Renderer {
private:
  const int _size;
  GLFWwindow *_window;
  Shader _shader;
  Grid _grid;
  std::vector<Nought> _noughts;
  std::vector<Cross> _crosses;

public:
  Renderer(int size, int window_width, int window_height);
  ~Renderer();

  void render();
  bool should_close();

private:
  GLFWwindow *create_window(int width, int height);
  std::string read_shader(const std::string &file_path);
};

} // namespace tictactoe_astar::renderer

#endif
