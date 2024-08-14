
#include "tictactoe_astar/renderer/grid.h"
#include "tictactoe_astar/renderer/gl_error_macro.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const float GL_WIDTH = 2.0f;
static const float GL_MIN_COORDINATE = -1.0f;
static const float GL_MAX_COORDINATE = 1.0f;

namespace tictactoe_astar::renderer {

Grid::Grid(int size, float line_width) {
  GL_CALL(glGenBuffers(1, &_vertex_buffer_object));
  GL_CALL(glGenVertexArrays(1, &_vertex_array_object));

  std::vector<float> vertices = create_grid_vectices(size, line_width);

  _vertices_size = vertices.size();

  GL_CALL(glBindVertexArray(_vertex_array_object));

  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_object));
  GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices_size,
                       vertices.data(), GL_STATIC_DRAW));

  GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                (void *)0));
  GL_CALL(glEnableVertexAttribArray(0));
}

Grid::~Grid() {
  glDeleteBuffers(1, &_vertex_array_object);
  glDeleteBuffers(1, &_vertex_buffer_object);
};

void Grid::draw() {
  GL_CALL(glBindVertexArray(_vertex_array_object));
  GL_CALL(glDrawArrays(GL_TRIANGLES, 0, _vertices_size));
}

std::vector<float> Grid::create_grid_vectices(int size, float line_width) {
  std::vector<float> vertices;
  vertices.reserve((size + 1) * (size + 1) * 3 * 2);
  float cell_width = GL_WIDTH / size;
  for (int index = 0; index <= size; ++index) {
    float x0 = GL_MIN_COORDINATE + index * cell_width - line_width / 2.0f;
    float x1 = GL_MIN_COORDINATE + index * cell_width + line_width / 2.0f;
    float x2 = GL_MIN_COORDINATE + index * cell_width - line_width / 2.0f;
    float x3 = GL_MIN_COORDINATE + index * cell_width + line_width / 2.0f;

    float y0 = GL_MAX_COORDINATE;
    float y1 = GL_MIN_COORDINATE;
    float y2 = GL_MIN_COORDINATE;
    float y3 = GL_MAX_COORDINATE;

    vertices.insert(vertices.end(), {// Row
                                     y0, x0, 0.0f, y1, x1, 0.0f, y2, x2, 0.0f,
                                     y0, x0, 0.0f, y3, x3, 0.0f, y1, x1, 0.0f,
                                     // Column
                                     x0, y0, 0.0f, x1, y1, 0.0f, x2, y2, 0.0f,
                                     x0, y0, 0.0f, x3, y3, 0.0f, x1, y1, 0.0f});
  }
  return vertices;
};

} // namespace tictactoe_astar::renderer
