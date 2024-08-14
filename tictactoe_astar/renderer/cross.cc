
#include "tictactoe_astar/renderer/cross.h"
#include "tictactoe_astar/renderer/gl_error_macro.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

static const float GL_WIDTH = 2.0f;
static const float GL_MIN_COORDINATE = -1.0f;

namespace tictactoe_astar::renderer {

Cross::Cross(int size, float line_width, Shader &shader)
    : _size(size), _shader(shader) {
  GL_CALL(glGenBuffers(1, &_vertex_buffer_object));
  GL_CALL(glGenVertexArrays(1, &_vertex_array_object));

  std::vector<float> vertices = create_cross_vectices(size, line_width);

  _vertices_size = vertices.size();

  GL_CALL(glBindVertexArray(_vertex_array_object));

  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_object));
  GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices_size,
                       vertices.data(), GL_STATIC_DRAW));

  GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                (void *)0));
  GL_CALL(glEnableVertexAttribArray(0));
}

Cross::~Cross() {
  glDeleteBuffers(1, &_vertex_array_object);
  glDeleteBuffers(1, &_vertex_buffer_object);
};

void Cross::set_location(int location) {
  int row = (int)location / _size;
  int column = location % _size;
  float cell_width = GL_WIDTH / _size;
  _offset[0] = column * cell_width;
  _offset[1] = row * cell_width;
}

void Cross::draw() {
  _shader.set_uniform("offset", _offset[0], _offset[1]);
  GL_CALL(glBindVertexArray(_vertex_array_object));
  GL_CALL(glDrawArrays(GL_TRIANGLES, 0, _vertices_size));
}

std::vector<float> Cross::create_cross_vectices(int size, float line_width) {
  float cell_width = GL_WIDTH / size;

  float half_line_width = line_width / 2.0f;

  float top_right_corner = cell_width;

  float x0 = std::cos(M_PI / 4) * half_line_width + GL_MIN_COORDINATE;
  float y0 = -std::sin(M_PI / 4) * half_line_width + GL_MIN_COORDINATE;

  float x1 = -std::cos(M_PI / 4) * half_line_width + GL_MIN_COORDINATE;
  float y1 = std::sin(M_PI / 4) * half_line_width + GL_MIN_COORDINATE;

  float x2 = -std::cos(M_PI / 4) * half_line_width + GL_MIN_COORDINATE +
             top_right_corner;
  float y2 = std::sin(M_PI / 4) * half_line_width + GL_MIN_COORDINATE +
             top_right_corner;

  float x3 = std::cos(M_PI / 4) * half_line_width + GL_MIN_COORDINATE +
             top_right_corner;
  float y3 = -std::sin(M_PI / 4) * half_line_width + GL_MIN_COORDINATE +
             top_right_corner;

  float x4 =
      std::cos(M_PI / 4) * half_line_width + GL_MIN_COORDINATE + cell_width;
  float y4 = std::sin(M_PI / 4) * half_line_width + GL_MIN_COORDINATE;

  float x5 =
      -std::cos(M_PI / 4) * half_line_width + GL_MIN_COORDINATE + cell_width;
  float y5 = -std::sin(M_PI / 4) * half_line_width + GL_MIN_COORDINATE;

  float x6 = -std::cos(M_PI / 4) * half_line_width + GL_MIN_COORDINATE;
  float y6 =
      -std::sin(M_PI / 4) * half_line_width + GL_MIN_COORDINATE + cell_width;

  float x7 = std::cos(M_PI / 4) * half_line_width + GL_MIN_COORDINATE;
  float y7 =
      std::sin(M_PI / 4) * half_line_width + GL_MIN_COORDINATE + cell_width;

  return {
      x0, y0, 0.0f, x1, y1, 0.0f, x2, y2, 0.0f,
      x2, y2, 0.0f, x3, y3, 0.0f, x0, y0, 0.0f,

      x4, y4, 0.0f, x5, y5, 0.0f, x6, y6, 0.0f,
      x6, y6, 0.0f, x7, y7, 0.0f, x4, y4, 0.0f,
  };
};

} // namespace tictactoe_astar::renderer
