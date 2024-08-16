#include "tictactoe_astar/engine/engine.h"
#include "tictactoe_astar/renderer/renderer.h"
#include <iostream>

static const int SIZE = 3;

int main() {
  tictactoe_astar::renderer::init();
  tictactoe_astar::engine::Engine engine(SIZE);

  tictactoe_astar::engine::Player player = engine.get_player();
  auto grid = engine.get_grid();
  grid = std::ref(engine.get_grid());

  int location;
  bool selected;
  bool game_over = false;
  tictactoe_astar::renderer::Renderer renderer(SIZE, 1080, 1080, 0.025);
  while (!game_over && !renderer.should_close()) {
    renderer.render(grid);
    std::tie(location, selected) = renderer.get_selected_location();
    if (selected) {
      std::tie(player, grid, game_over) = engine.select(location);
      std::cout << (player == tictactoe_astar::engine::Player::NOUGHT ? "Nought"
                                                                      : "Cross")
                << " clicked at location: " << location << std::endl;
    }
  }
  tictactoe_astar::renderer::terminate();
  return 0;
}