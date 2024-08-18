#include "tictactoe_ai/engine/engine.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <limits.h>
#include <stdexcept>

namespace tictactoe_ai::engine {

Engine::Engine(int size, int win_length)
    : _size(size), _win_length(win_length), _player_turn(Player::NOUGHT),
      _remaining_moves(_size * _size), _previous_location(0) {
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
    _previous_location = location;
    --_remaining_moves;
  }
  return {_player_turn, _grid, get_game_outcome()};
}

GameOutcome Engine::get_game_outcome() { return get_game_outcome(_grid); }

Player Engine::get_player() { return _player_turn; }
const std::vector<Occupancy> &Engine::get_grid() { return _grid; }

std::tuple<int, int> Engine::get_best_location() {
  return get_best_location(_player_turn, _grid, 0, INT_MIN, INT_MAX,
                           _previous_location, _remaining_moves);
};

std::tuple<int, int> Engine::get_best_location(Player player,
                                               std::vector<Occupancy> &grid,
                                               int depth, int alpha, int beta,
                                               int previous_location,
                                               int remaining_moves) {
  Player previous_player;
  switch (player) {
  case Player::NOUGHT:
    previous_player = Player::CROSS;
    break;
  case Player::CROSS:
    previous_player = Player::NOUGHT;
    break;
  default:
    throw std::runtime_error("Should not happen");
  }
  GameOutcome game_outcome = get_played_move_outcome(
      grid, previous_player, previous_location, remaining_moves);
  if (game_outcome != GameOutcome::ONGOING) {
    int score;
    if (game_outcome == GameOutcome::NOUGHT) {
      score = grid.size() - depth;
    } else if (game_outcome == GameOutcome::CROSS) {
      score = -grid.size() + depth;
    } else {
      score = 0;
    }
    return {-1, score};
  }
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
    auto result = get_best_location(next_player, grid, depth + 1, alpha, beta,
                                    location, remaining_moves - 1);
    grid[location] = Occupancy::EMPTY;
    int score = std::get<1>(result);
    if (player == Player::NOUGHT) {
      if (score > best_score) {
        best_score = score;
        best_location = location;
      }
      alpha = std::max(alpha, best_score);
    } else if (player == Player::CROSS) {
      if (score < best_score) {
        best_score = score;
        best_location = location;
      }
      beta = std::min(beta, best_score);
    }
    if (beta <= alpha) {
      break;
    }
  }

  if (best_location == -1) {
    throw std::runtime_error("Unexpected failure to find the best move");
  }
  return {best_location, best_score};
}

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

GameOutcome Engine::get_played_move_outcome(const std::vector<Occupancy> &grid,
                                            Player player, int played_location,
                                            int moves_remaining) {
  int played_row = played_location / _size;
  int played_column = played_location % _size;
  Occupancy player_occupancy;
  GameOutcome win_outcome;
  switch (player) {
  case Player::NOUGHT:
    player_occupancy = Occupancy::NOUGHT;
    win_outcome = GameOutcome::NOUGHT;
    break;
  case Player::CROSS:
    player_occupancy = Occupancy::CROSS;
    win_outcome = GameOutcome::CROSS;
    break;
  default:
    throw std::runtime_error("Unknown player");
  }
  int count = 0;
  int start_column = std::max(played_column - _win_length, 0);
  int end_column = std::min(played_column + _win_length, _size);
  for (int column = start_column; column < end_column; ++column) {
    int location = played_row * _size + column;
    count = (grid[location] == player_occupancy) ? count + 1 : 0;
    if (count == _win_length) {
      return win_outcome;
    }
  }
  count = 0;
  int start_index = std::max(played_row - _win_length, 0);
  int end_index = std::min(played_row + _win_length, _size);
  for (int row = start_index; row < end_index; ++row) {
    int location = row * _size + played_column;
    count = (grid[location] == player_occupancy) ? count + 1 : 0;
    if (count == _win_length) {
      return win_outcome;
    }
  }
  count = 0;
  for (int index = -_win_length; index < _win_length; ++index) {
    int row = played_row + index;
    int column = played_column + index;
    if (row >= 0 && column >= 0 && row < _size && column < _size) {
      int location = row * _size + column;
      count = (grid[location] == player_occupancy) ? count + 1 : 0;
      if (count == _win_length) {
        return win_outcome;
      }
    }
  }
  count = 0;
  for (int index = -_win_length; index < _win_length; ++index) {
    int row = played_row + index;
    int column = played_column - index;
    if (row >= 0 && column >= 0 && row < _size && column < _size) {
      int location = row * _size + column;
      count = (grid[location] == player_occupancy) ? count + 1 : 0;
      if (count == _win_length) {
        return win_outcome;
      }
    }
  }
  if (moves_remaining == 0) {
    return GameOutcome::DRAW;
  }
  return GameOutcome::ONGOING;
};

void Engine::print_board(const std::vector<Occupancy> &grid) {
  for (int row = _size - 1; row >= 0; --row) {
    for (int column = 0; column < _size; ++column) {
      int location = (row * _size) + column;
      char cell;
      switch (grid[location]) {
      case Occupancy::EMPTY:
        cell = ' ';
        break;
      case Occupancy::NOUGHT:
        cell = 'O';
        break;
      case Occupancy::CROSS:
        cell = 'X';
        break;
      default:
        throw std::runtime_error("Should not fail");
      }
      std::cout << cell;
    }
    std::cout << std::endl;
  }
}

} // namespace tictactoe_ai::engine
