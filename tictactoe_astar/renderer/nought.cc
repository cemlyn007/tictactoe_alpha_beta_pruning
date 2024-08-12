
#include "tictactoe_astar/renderer/nought.h"
#include "tictactoe_astar/renderer/gl_error_macro.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

static const float GL_WIDTH = 2.0f;
static const float LINE_WIDTH = 0.025;
static const int CIRCLE_ITERATIONS = 36;

namespace tictactoe_astar::renderer {

void render_nought(int size) {
  auto vertices = create_nought_vectices(size);
  auto vertices_size = vertices.size();

  GLuint vertex_buffer_object;
  GLuint vertex_array_object;
  GL_CALL(glGenBuffers(1, &vertex_buffer_object));
  GL_CALL(glGenVertexArrays(1, &vertex_array_object));

  vertices_size = vertices.size();

  GL_CALL(glBindVertexArray(vertex_array_object));

  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object));
  GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_size,
                       vertices.data(), GL_STATIC_DRAW));

  GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                (void *)0));
  GL_CALL(glEnableVertexAttribArray(0));

  GL_CALL(glBindVertexArray(vertex_array_object));
  GL_CALL(glDrawArrays(GL_TRIANGLES, 0, vertices_size));

  glDeleteBuffers(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);
}

std::vector<float> create_nought_vectices(int size) {
  float cell_width = GL_WIDTH / size;

  std::vector<float> vertices;
  vertices.reserve(6 * 3 * CIRCLE_ITERATIONS);

  float min_radius = cell_width / 2.0f - LINE_WIDTH;
  float max_radius = cell_width / 2.0f;

  for (int index = 0; index < CIRCLE_ITERATIONS; ++index) {
    double previous_radians = (2 * M_PI * (index - 1)) / CIRCLE_ITERATIONS;
    double radians = (2 * M_PI * index) / CIRCLE_ITERATIONS;
    double next_radians = (2 * M_PI * (index + 1)) / CIRCLE_ITERATIONS;

    float x0 = min_radius * std::cos(radians);
    float y0 = min_radius * std::sin(radians);

    float x1 = max_radius * std::cos(next_radians);
    float y1 = max_radius * std::sin(next_radians);

    float x2 = max_radius * std::cos(radians);
    float y2 = max_radius * std::sin(radians);

    float x3 = min_radius * std::cos(previous_radians);
    float y3 = min_radius * std::sin(previous_radians);

    float x4 = min_radius * std::cos(radians);
    float y4 = min_radius * std::sin(radians);

    float x5 = max_radius * std::cos(radians);
    float y5 = max_radius * std::sin(radians);

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

} // namespace tictactoe_astar::renderer
