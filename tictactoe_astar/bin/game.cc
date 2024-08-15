#include "tictactoe_astar/engine/engine.h"
#include "tictactoe_astar/renderer/renderer.h"
#include <iostream>

static const int SIZE = 3;

int main() {
  tictactoe_astar::renderer::init();
  tictactoe_astar::engine::Engine engine;
  engine.dummy_engine();
  std::vector<tictactoe_astar::engine::Occupancy> grid;
  grid.resize(SIZE * SIZE);
  for (int index = 0; index < grid.size(); ++index) {
    grid[index] = tictactoe_astar::engine::EMPTY;
  }

  bool noughts_turn = true;
  tictactoe_astar::renderer::Renderer renderer(SIZE, 1080, 1080);
  while (!renderer.should_close()) {
    renderer.render(grid);
    std::tuple<int, bool> result = renderer.get_selected_location();
    int location = std::get<0>(result);
    bool selected = std::get<1>(result);
    if (selected &&
        grid[location] == tictactoe_astar::engine::Occupancy::EMPTY) {
      grid[location] = noughts_turn ? tictactoe_astar::engine::Occupancy::NOUGHT
                                    : tictactoe_astar::engine::Occupancy::CROSS;
      std::cout << (noughts_turn ? "Nought" : "Cross")
                << " clicked at location: " << std::get<0>(result) << std::endl;
      noughts_turn = !noughts_turn;
    }
  }
  tictactoe_astar::renderer::terminate();
  return 0;
}