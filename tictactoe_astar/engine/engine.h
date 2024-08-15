#ifndef TICTACTOE_ASTAR_ENGINE_H_
#define TICTACTOE_ASTAR_ENGINE_H_

namespace tictactoe_astar::engine {

enum Occupancy { EMPTY, NOUGHT, CROSS };

class Engine {
public:
  Engine();
  void dummy_engine();
};
} // namespace tictactoe_astar::engine

#endif
