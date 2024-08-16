#include "tictactoe_astar/engine/engine.h"
#include <ctime>

namespace tictactoe_astar::engine {

Engine::Engine(int size) : _size(size), _player_turn(Player::NOUGHT) {
  _grid.resize(_size * _size);
  for (int index = 0; index < _grid.size(); ++index) {
    _grid[index] = Occupancy::EMPTY;
  }
};

std::tuple<Player, const std::vector<Occupancy> &>
Engine::select(int location) {
  if (_grid[location] == Occupancy::EMPTY) {
    if (_player_turn == Player::NOUGHT) {
      _grid[location] = Occupancy::NOUGHT;
      _player_turn = Player::CROSS;
    } else if (_player_turn == Player::CROSS) {
      _grid[location] = Occupancy::CROSS;
      _player_turn = Player::NOUGHT;
    } else {
      throw std::runtime_error("Unknown player type");
    }
  }
  return {_player_turn, _grid};
}

Player Engine::get_player() { return _player_turn; }
const std::vector<Occupancy> &Engine::get_grid() { return _grid; }

} // namespace tictactoe_astar::engine
