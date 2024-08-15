#include "tictactoe_astar/engine/engine.h"
#include "tictactoe_astar/renderer/renderer.h"
#include <iostream>

int main() {
  tictactoe_astar::renderer::init();
  tictactoe_astar::engine::Engine engine;
  engine.dummy_engine();
  tictactoe_astar::renderer::Renderer renderer(3, 1080, 1080);
  while (!renderer.should_close()) {
    renderer.render();
    std::tuple<int, bool> result = renderer.get_selected_location();
    if (std::get<1>(result)) {
      std::cout << "Clicked at location: " << std::get<0>(result) << std::endl;
    }
  }
  tictactoe_astar::renderer::terminate();
  return 0;
}