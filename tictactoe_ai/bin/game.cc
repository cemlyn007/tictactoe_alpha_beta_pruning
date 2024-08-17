#include "tictactoe_ai/engine/engine.h"
#include "tictactoe_ai/renderer/renderer.h"
#include <iostream>

static const int SIZE = 4;
static const int WIN_LENGTH = 3;
static const tictactoe_ai::engine::Player AI_PLAYER =
    tictactoe_ai::engine::Player::CROSS;

int main() {
  tictactoe_ai::renderer::init();
  tictactoe_ai::engine::Engine engine(SIZE, WIN_LENGTH);

  tictactoe_ai::engine::Player player = engine.get_player();
  auto grid = engine.get_grid();
  grid = std::ref(engine.get_grid());

  int location;
  bool selected;
  tictactoe_ai::engine::GameOutcome game_outcome =
      tictactoe_ai::engine::GameOutcome::ONGOING;
  tictactoe_ai::renderer::Renderer renderer(SIZE, 1080, 1080, 0.025);
  while (game_outcome == tictactoe_ai::engine::GameOutcome::ONGOING &&
         !renderer.should_close()) {
    renderer.render(grid);
    if (player == AI_PLAYER) {
      auto result = engine.get_best_location();
      location = std::get<0>(result);
      selected = true;
    } else {
      std::tie(location, selected) = renderer.get_selected_location();
    }
    if (selected) {
      // Set to false for the bot best location selection case.
      selected = false;
      std::tie(player, grid, game_outcome) = engine.select(location);
      std::cout << (player == tictactoe_ai::engine::Player::NOUGHT ? "Nought"
                                                                   : "Cross")
                << " clicked at location: " << location << std::endl;
    }
  }
  switch (game_outcome) {
  case tictactoe_ai::engine::GameOutcome::DRAW:
    std::cout << "Draw!" << std::endl;
    break;
  case tictactoe_ai::engine::GameOutcome::ONGOING:
    std::cout << "Game stopped early!" << std::endl;
    break;
  case tictactoe_ai::engine::GameOutcome::NOUGHT:
    std::cout << "Nought won!" << std::endl;
    break;
  case tictactoe_ai::engine::GameOutcome::CROSS:
    std::cout << "Cross won!" << std::endl;
    break;
  }
  tictactoe_ai::renderer::terminate();
  return 0;
}