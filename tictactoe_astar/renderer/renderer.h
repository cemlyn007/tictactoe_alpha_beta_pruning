#ifndef TICTACTOE_ASTAR_RENDERER_H_
#define TICTACTOE_ASTAR_RENDERER_H_

#include <string>

namespace tictactoe_astar::renderer {

class Renderer {
public:
  Renderer(size_t size);
  void render(const std::string &content);
};

} // namespace tictactoe_astar::renderer

#endif
