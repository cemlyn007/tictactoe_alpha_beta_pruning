#include "tictactoe_ai/engine/engine.h"
#include <algorithm>
#include <ctime>
#include <limits.h>
#include <stdexcept>

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

GameOutcome Engine::get_game_outcome() { return get_game_outcome(_grid); }

Player Engine::get_player() { return _player_turn; }
const std::vector<Occupancy> &Engine::get_grid() { return _grid; }

std::tuple<int, int> Engine::get_best_location() {
  return get_best_location(_player_turn, _grid, 0);
};

std::tuple<int, int> Engine::get_best_location(Player player,
                                               std::vector<Occupancy> &grid,
                                               int depth) {
  GameOutcome game_outcome = get_game_outcome(grid);
  if (game_outcome != GameOutcome::ONGOING) {
    int score;
    if (game_outcome == GameOutcome::NOUGHT) {
      score = INT_MAX - depth;
    } else if (game_outcome == GameOutcome::CROSS) {
      score = INT_MIN + depth;
    } else {
      score = 0;
    }
    return {-1, score};
  }
  // else...
  Player next_player;
  int best_location = -1;
  int best_score;
  if (player == Player::NOUGHT) {
    best_score = INT_MIN;
    next_player = Player::CROSS;
  } else if (player == Player::CROSS) {
    best_score = INT_MAX;
    next_player = Player::NOUGHT;
  } else {
    throw std::runtime_error("Unknown player");
  }
  for (int location = 0; location < grid.size(); ++location) {
    if (grid[location] != Occupancy::EMPTY) {
      continue;
    }
    // else...
    if (player == Player::NOUGHT) {
      grid[location] = Occupancy::NOUGHT;
    } else if (player == Player::CROSS) {
      grid[location] = Occupancy::CROSS;
    } else {
      throw std::runtime_error("Unknown player");
    }
    auto result = get_best_location(next_player, grid, depth + 1);
    grid[location] = Occupancy::EMPTY;
    int score = std::get<1>(result);
    if (player == Player::NOUGHT) {
      best_score = std::max(best_score, score);
      if (best_score == score) {
        best_location = location;
      }
    } else if (player == Player::CROSS) {
      best_score = std::min(best_score, score);
      if (best_score == score) {
        best_location = location;
      }
    } else {
      throw std::runtime_error("Should not happen");
    }
  }
  if ((player == Player::NOUGHT && best_score == INT_MIN) ||
      (player == Player::CROSS && best_score == INT_MAX)) {
    throw std::runtime_error(
        "Should have reached a terminal state but returning ongoing");
  }
  return {best_location, best_score};
};

GameOutcome Engine::get_game_outcome(const std::vector<Occupancy> &grid) {
  int count = 0;
  Player count_player = Player::NOUGHT;
  for (int row = 0; row < _size; ++row) {
    count = 0;
    for (int column = 0; column < _size; ++column) {
      int location = row * _size + column;
      if (grid[location] == Occupancy::EMPTY) {
        count = 0;
      } else if (grid[location] == Occupancy::NOUGHT) {
        count = count_player == Player::NOUGHT ? count + 1 : 1;
        count_player = Player::NOUGHT;
      } else if (grid[location] == Occupancy::CROSS) {
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
      if (grid[location] == Occupancy::EMPTY) {
        count = 0;
      } else if (grid[location] == Occupancy::NOUGHT) {
        count = count_player == Player::NOUGHT ? count + 1 : 1;
        count_player = Player::NOUGHT;
      } else if (grid[location] == Occupancy::CROSS) {
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
      } else if (grid[location] == Occupancy::EMPTY) {
        count = 0;
      } else if (grid[location] == Occupancy::NOUGHT) {
        count = count_player == Player::NOUGHT ? count + 1 : 1;
        count_player = Player::NOUGHT;
      } else if (grid[location] == Occupancy::CROSS) {
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
      } else if (grid[location] == Occupancy::EMPTY) {
        count = 0;
      } else if (grid[location] == Occupancy::NOUGHT) {
        count = count_player == Player::NOUGHT ? count + 1 : 1;
        count_player = Player::NOUGHT;
      } else if (grid[location] == Occupancy::CROSS) {
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
  for (const Occupancy &occupancy : grid) {
    if (occupancy == Occupancy::EMPTY) {
      return GameOutcome::ONGOING;
    }
  }
  return GameOutcome::DRAW;
};

} // namespace tictactoe_ai::engine
