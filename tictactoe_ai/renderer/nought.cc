
#include "tictactoe_ai/renderer/nought.h"
#include "tictactoe_ai/renderer/gl_error_macro.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

static const float GL_WIDTH = 2.0f;
static const int CIRCLE_ITERATIONS = 36;
static const float GL_MIN_COORDINATE = -1.0f;

namespace tictactoe_ai::renderer {

Nought::Nought(int size, float line_width, Shader *shader)
    : _size(size), _shader(shader) {
  GL_CALL(glGenBuffers(1, &_vertex_buffer_object));
  GL_CALL(glGenVertexArrays(1, &_vertex_array_object));

  std::vector<float> vertices = create_nought_vectices(size, line_width);

  _vertices_size = vertices.size();

  GL_CALL(glBindVertexArray(_vertex_array_object));

  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_object));
  GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices_size,
                       vertices.data(), GL_STATIC_DRAW));
  GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                (void *)0));
  GL_CALL(glEnableVertexAttribArray(0));
}

Nought::Nought(Nought &&other)
    : _size(other._size), _vertices_size(other._vertices_size),
      _vertex_buffer_object(other._vertex_buffer_object),
      _vertex_array_object(other._vertex_array_object), _shader(other._shader) {
  // We set these to 0 so that when the original object destructor is called,
  // the OpenGL objects are not destroyed.
  other._vertex_array_object = 0;
  other._vertex_buffer_object = 0;
}

Nought::~Nought() {
  glDeleteBuffers(1, &_vertex_array_object);
  glDeleteBuffers(1, &_vertex_buffer_object);
};

void Nought::set_location(int location) {
  int row = (int)location / _size;
  int column = location % _size;
  float cell_width = GL_WIDTH / _size;
  _offset[0] = column * cell_width;
  _offset[1] = row * cell_width;
}

void Nought::draw() {
  _shader->set_uniform("offset", _offset[0], _offset[1]);
  GL_CALL(glBindVertexArray(_vertex_array_object));
  GL_CALL(glDrawArrays(GL_TRIANGLES, 0, _vertices_size));
}

std::vector<float> Nought::create_nought_vectices(int size, float line_width) {
  float cell_width = GL_WIDTH / size;

  float offset = GL_MIN_COORDINATE + (cell_width / 2);

  std::vector<float> vertices;
  vertices.reserve(6 * 3 * CIRCLE_ITERATIONS);

  float min_radius = cell_width / 2.0f - line_width;
  float max_radius = cell_width / 2.0f;

  for (int index = 0; index < CIRCLE_ITERATIONS; ++index) {
    double previous_radians = (2 * M_PI * (index - 1)) / CIRCLE_ITERATIONS;
    double radians = (2 * M_PI * index) / CIRCLE_ITERATIONS;
    double next_radians = (2 * M_PI * (index + 1)) / CIRCLE_ITERATIONS;

    float x0 = min_radius * std::cos(radians) + offset;
    float y0 = min_radius * std::sin(radians) + offset;

    float x1 = max_radius * std::cos(next_radians) + offset;
    float y1 = max_radius * std::sin(next_radians) + offset;

    float x2 = max_radius * std::cos(radians) + offset;
    float y2 = max_radius * std::sin(radians) + offset;

    float x3 = min_radius * std::cos(previous_radians) + offset;
    float y3 = min_radius * std::sin(previous_radians) + offset;

    float x4 = min_radius * std::cos(radians) + offset;
    float y4 = min_radius * std::sin(radians) + offset;

    float x5 = max_radius * std::cos(radians) + offset;
    float y5 = max_radius * std::sin(radians) + offset;

    vertices.insert(vertices.end(), {
                                        x0,
                                        y0,
                                        0.0f,
                                        x1,
                                        y1,
                                        0.0f,
                                        x2,
                                        y2,
                                        0.0f,
                                        x3,
                                        y3,
                                        0.0f,
                                        x4,
                                        y4,
                                        0.0f,
                                        x5,
                                        y5,
                                        0.0f,
                                    });
  }
  return vertices;
};

} // namespace tictactoe_ai::renderer
