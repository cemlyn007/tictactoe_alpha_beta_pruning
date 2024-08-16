#include "tictactoe_astar/engine/engine.h"
#include "tictactoe_astar/renderer/renderer.h"
#include <iostream>

static const int SIZE = 3;
static const int WIN_LENGTH = 3;

int main() {
  tictactoe_astar::renderer::init();
  tictactoe_astar::engine::Engine engine(SIZE, WIN_LENGTH);

  tictactoe_astar::engine::Player player = engine.get_player();
  auto grid = engine.get_grid();
  grid = std::ref(engine.get_grid());

  int location;
  bool selected;
  tictactoe_astar::engine::GameOutcome game_outcome =
      tictactoe_astar::engine::GameOutcome::ONGOING;
  tictactoe_astar::renderer::Renderer renderer(SIZE, 1080, 1080, 0.025);
  while (game_outcome == tictactoe_astar::engine::GameOutcome::ONGOING &&
         !renderer.should_close()) {
    renderer.render(grid);
    std::tie(location, selected) = renderer.get_selected_location();
    if (selected) {
      std::tie(player, grid, game_outcome) = engine.select(location);
      std::cout << (player == tictactoe_astar::engine::Player::NOUGHT ? "Nought"
                                                                      : "Cross")
                << " clicked at location: " << location << std::endl;
    }
  }
  switch (game_outcome) {
  case tictactoe_astar::engine::GameOutcome::DRAW:
    std::cout << "Draw!" << std::endl;
    break;
  case tictactoe_astar::engine::GameOutcome::ONGOING:
    std::cout << "Game stopped early!" << std::endl;
    break;
  case tictactoe_astar::engine::GameOutcome::NOUGHT:
    std::cout << "Nought won!" << std::endl;
    break;
  case tictactoe_astar::engine::GameOutcome::CROSS:
    std::cout << "Cross won!" << std::endl;
    break;
  }
  tictactoe_astar::renderer::terminate();
  return 0;
}