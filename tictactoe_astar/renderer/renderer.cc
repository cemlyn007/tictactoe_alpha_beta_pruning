#include "tictactoe_astar/renderer/renderer.h"
#include <iostream>
#include <string>

namespace tictactoe_astar::renderer {
Renderer::Renderer(size_t size){};

void Renderer::render(const std::string &content) {
  std::cout << content << std::endl;
}
} // namespace tictactoe_astar::renderer
