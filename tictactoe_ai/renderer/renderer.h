#ifndef TICTACTOE_AI_RENDERER_H_
#define TICTACTOE_AI_RENDERER_H_
#include "tictactoe_ai/engine/engine.h"
#include "tictactoe_ai/renderer/cross.h"
#include "tictactoe_ai/renderer/grid.h"
#include "tictactoe_ai/renderer/nought.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string>
#include <vector>

namespace tictactoe_ai::renderer {

void init();

void terminate();

class Renderer {
private:
  const int _size;
  GLFWwindow *_window;
  Shader _shader;
  Grid _grid;
  std::vector<Nought> _noughts;
  std::vector<Cross> _crosses;
  double _mouse_position_in_pixels[2];
  bool _mouse_click;
  bool _escape_pressed;

public:
  Renderer(int size, int window_width, int window_height, float line_width);
  ~Renderer();

  void render(const std::vector<engine::Occupancy> &grid);
  bool should_close();
  std::tuple<int, bool> get_selected_location();

private:
  GLFWwindow *create_window(int width, int height);
  static void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);
  static void mouse_button_callback(GLFWwindow *window, int button, int action,
                                    int mods);
  static void cursor_position_callback(GLFWwindow *window, double xpos,
                                       double ypos);
  std::string read_shader(const std::string &file_path);
};

} // namespace tictactoe_ai::renderer

#endif
