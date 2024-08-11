#include "tictactoe_astar/engine/engine.h"
#include "tictactoe_astar/renderer/renderer.h"

int main() {
  tictactoe_astar::renderer::init();
  tictactoe_astar::engine::Engine engine;
  engine.dummy_engine();
  tictactoe_astar::renderer::Renderer renderer(3);
  while (!renderer.should_close()) {
    renderer.render();
  }
  tictactoe_astar::renderer::terminate();
  return 0;
}