#include "tictactoe_ai/engine/engine.h"
#include "tictactoe_ai/renderer/renderer.h"
#include <iostream>

static const tictactoe_ai::engine::Player AI_PLAYER =
    tictactoe_ai::engine::Player::NOUGHT;

int main(int argc, char *argv[]) {
  int size = 3;
  int win_length = 3;
  if (argc == 2 || argc == 3) {
    size = std::stoi(argv[1]);
  }
  if (argc == 3) {
    win_length = std::stoi(argv[2]);
  }
  tictactoe_ai::renderer::init();
  tictactoe_ai::engine::Engine engine(size, win_length);

  tictactoe_ai::engine::Player player = engine.get_player();
  auto grid = engine.get_grid();
  grid = std::ref(engine.get_grid());

  int location;
  bool selected;
  tictactoe_ai::engine::GameOutcome game_outcome =
      tictactoe_ai::engine::GameOutcome::ONGOING;
  tictactoe_ai::renderer::Renderer renderer(size, 1080, 1080, 0.025);
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