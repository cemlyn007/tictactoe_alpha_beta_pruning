#include "tictactoe_astar/engine/engine.h"
#include "tictactoe_astar/renderer/renderer.h"
#include <iostream>
#include <string>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

int main() {
  tictactoe_astar::engine::dummy_engine();
  tictactoe_astar::renderer::dummy_render("Hello there!");

  if (!glfwInit())
    return -1;

  auto window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}