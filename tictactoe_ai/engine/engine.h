#ifndef TICTACTOE_AI_ENGINE_H_
#define TICTACTOE_AI_ENGINE_H_
#include <tuple>
#include <vector>

namespace tictactoe_ai::engine {

enum class Player { NOUGHT = 0, CROSS = 1 };

enum class Occupancy { EMPTY = 0, NOUGHT = 1, CROSS = 2 };

enum class GameOutcome { DRAW = 0, ONGOING = 1, NOUGHT = 2, CROSS = 3 };

class Engine {
private:
  int _size;
  int _win_length;
  std::vector<Occupancy> _grid;
  Player _player_turn;

public:
  Engine(int size, int win_length);
  std::tuple<Player, const std::vector<Occupancy> &, GameOutcome>
  select(int location);
  Player get_player();
  const std::vector<Occupancy> &get_grid();
  GameOutcome get_game_outcome();
  std::tuple<int, GameOutcome> get_best_location();

private:
  std::tuple<int, GameOutcome> get_best_location(Player player,
                                                 std::vector<Occupancy> &grid);
  GameOutcome get_game_outcome(const std::vector<Occupancy> &grid);
};
} // namespace tictactoe_ai::engine

#endif
