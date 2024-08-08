#include "tictactoe_astar/engine/engine.h"
#include "tictactoe_astar/renderer/renderer.h"
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  tictactoe_astar::engine::dummy_engine();
  tictactoe_astar::renderer::dummy_render("Hello there!");
  return 0;
}
