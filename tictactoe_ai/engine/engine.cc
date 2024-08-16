#include "tictactoe_ai/engine/engine.h"
#include <ctime>

namespace tictactoe_ai::engine {

Engine::Engine(int size, int win_length)
    : _size(size), _win_length(win_length), _player_turn(Player::NOUGHT) {
  _grid.resize(_size * _size);
  for (int index = 0; index < _grid.size(); ++index) {
    _grid[index] = Occupancy::EMPTY;
  }
};

std::tuple<Player, const std::vector<Occupancy> &, GameOutcome>
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
  return {_player_turn, _grid, get_game_outcome()};
}

GameOutcome Engine::get_game_outcome() {
  int count = 0;
  Player count_player = Player::NOUGHT;
  for (int row = 0; row < _size; ++row) {
    count = 0;
    for (int column = 0; column < _size; ++column) {
      int location = row * _size + column;
      if (_grid[location] == Occupancy::EMPTY) {
        count = 0;
      } else if (_grid[location] == Occupancy::NOUGHT) {
        count = count_player == Player::NOUGHT ? count + 1 : 1;
        count_player = Player::NOUGHT;
      } else if (_grid[location] == Occupancy::CROSS) {
        count = count_player == Player::CROSS ? count + 1 : 1;
        count_player = Player::CROSS;
      } else {
        throw std::runtime_error("Unknown occupancy");
      }
      if (count == _win_length) {
        if (count_player == Player::NOUGHT) {
          return GameOutcome::NOUGHT;
        } else if (count_player == Player::CROSS) {
          return GameOutcome::CROSS;
        } else {
          throw std::runtime_error("Unknown player");
        }
      }
    }
  }
  for (int column = 0; column < _size; ++column) {
    count = 0;
    for (int row = 0; row < _size; ++row) {
      int location = row * _size + column;
      if (_grid[location] == Occupancy::EMPTY) {
        count = 0;
      } else if (_grid[location] == Occupancy::NOUGHT) {
        count = count_player == Player::NOUGHT ? count + 1 : 1;
        count_player = Player::NOUGHT;
      } else if (_grid[location] == Occupancy::CROSS) {
        count = count_player == Player::CROSS ? count + 1 : 1;
        count_player = Player::CROSS;
      } else {
        throw std::runtime_error("Unknown occupancy");
      }
      if (count == _win_length) {
        if (count_player == Player::NOUGHT) {
          return GameOutcome::NOUGHT;
        } else if (count_player == Player::CROSS) {
          return GameOutcome::CROSS;
        } else {
          throw std::runtime_error("Unknown player");
        }
      }
    }
  }
  for (int starting_row = -_size; starting_row < _size; ++starting_row) {
    count = 0;
    for (int index = 0; index < _size; ++index) {
      int location = (starting_row + index) * _size + index;
      if (location < 0 || location >= (_size * _size)) {
        count = 0;
      } else if (_grid[location] == Occupancy::EMPTY) {
        count = 0;
      } else if (_grid[location] == Occupancy::NOUGHT) {
        count = count_player == Player::NOUGHT ? count + 1 : 1;
        count_player = Player::NOUGHT;
      } else if (_grid[location] == Occupancy::CROSS) {
        count = count_player == Player::CROSS ? count + 1 : 1;
        count_player = Player::CROSS;
      } else {
        throw std::runtime_error("Unknown occupancy");
      }
      if (count == _win_length) {
        if (count_player == Player::NOUGHT) {
          return GameOutcome::NOUGHT;
        } else if (count_player == Player::CROSS) {
          return GameOutcome::CROSS;
        } else {
          throw std::runtime_error("Unknown player");
        }
      }
    }
  }
  for (int starting_row = 2 * _size; starting_row > 0; --starting_row) {
    count = 0;
    for (int index = 0; index < _size; ++index) {
      int location = (starting_row - index) * _size + index;
      if (location < 0 || location >= (_size * _size)) {
        count = 0;
      } else if (_grid[location] == Occupancy::EMPTY) {
        count = 0;
      } else if (_grid[location] == Occupancy::NOUGHT) {
        count = count_player == Player::NOUGHT ? count + 1 : 1;
        count_player = Player::NOUGHT;
      } else if (_grid[location] == Occupancy::CROSS) {
        count = count_player == Player::CROSS ? count + 1 : 1;
        count_player = Player::CROSS;
      } else {
        throw std::runtime_error("Unknown occupancy");
      }
      if (count == _win_length) {
        if (count_player == Player::NOUGHT) {
          return GameOutcome::NOUGHT;
        } else if (count_player == Player::CROSS) {
          return GameOutcome::CROSS;
        } else {
          throw std::runtime_error("Unknown player");
        }
      }
    }
  }
  for (const Occupancy &occupancy : _grid) {
    if (occupancy == Occupancy::EMPTY) {
      return GameOutcome::ONGOING;
    }
  }
  return GameOutcome::DRAW;
};

Player Engine::get_player() { return _player_turn; }
const std::vector<Occupancy> &Engine::get_grid() { return _grid; }

} // namespace tictactoe_ai::engine
