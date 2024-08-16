#ifndef TICTACTOE_ASTAR_ENGINE_H_
#define TICTACTOE_ASTAR_ENGINE_H_
#include <tuple>
#include <vector>

namespace tictactoe_astar::engine {

enum class Player { NOUGHT = 0, CROSS = 1 };

enum class Occupancy { EMPTY = 0, NOUGHT = 1, CROSS = 2 };

class Engine {
private:
  int _size;
  std::vector<Occupancy> _grid;
  Player _player_turn;

public:
  Engine(int size);
  std::tuple<Player, const std::vector<Occupancy> &> select(int location);
  Player get_player();
  const std::vector<Occupancy> &get_grid();
};
} // namespace tictactoe_astar::engine

#endif
