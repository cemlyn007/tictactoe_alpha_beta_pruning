#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace tictactoe_astar::renderer {
void render_nought(int size, float line_width);
std::vector<float> create_nought_vectices(int size, float line_width);
} // namespace tictactoe_astar::renderer
