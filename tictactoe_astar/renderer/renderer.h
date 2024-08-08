#ifndef TICTACTOE_ASTAR_RENDERER_H_
#define TICTACTOE_ASTAR_RENDERER_H_
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

namespace tictactoe_astar::renderer {

void init();

void terminate();

class Renderer {
private:
  const size_t _size;
  GLFWwindow *_window;

public:
  Renderer(size_t size);
  ~Renderer();
  void render();
  bool should_close();
};

} // namespace tictactoe_astar::renderer

#endif
