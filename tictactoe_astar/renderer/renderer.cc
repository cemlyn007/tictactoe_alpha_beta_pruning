#include "tictactoe_astar/renderer/renderer.h"
#include "tictactoe_astar/renderer/gl_error_macro.h"
#include "tictactoe_astar/renderer/nought.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

static const char *SHADER_VERTEX_FILE_PATH =
    "tictactoe_astar/renderer/shader.vert";
static const char *SHADER_FRAGMENT_FILE_PATH =
    "tictactoe_astar/renderer/shader.frag";
static const float GL_WIDTH = 2.0f;
static const float GL_MIN_COORDINATE = -1.0f;
static const float GL_MAX_COORDINATE = 1.0f;
static const float LINE_WIDTH = 0.025;

static void glfwErrorCallback(int error, const char *description) {
  std::cerr << "GLFW Error: " << error << " - " << description << std::endl;
  throw std::runtime_error(std::string("GLFW Error: ") + description);
}

namespace tictactoe_astar::renderer {

void init() {
  glfwSetErrorCallback(glfwErrorCallback);
  if (!glfwInit()) {
    throw std::runtime_error("Could not initialize glfw");
  }
  // else...
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
};

void terminate() {
  glfwSetErrorCallback(nullptr);
  glfwTerminate();
};

Renderer::Renderer(size_t size) : _size(size) {
  _window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
  if (!_window) {
    throw std::runtime_error("Could not create window");
  }
  // else...
  glfwMakeContextCurrent(_window);
  GLenum error = glewInit();
  if (GLEW_OK != error) {
    throw std::runtime_error(
        std::string("Error initializing glew: ") +
        reinterpret_cast<const char *>(glewGetErrorString(error)));
  }
  // else...
  GL_CALL(glGenBuffers(1, &_vertex_buffer_object));
  GL_CALL(glGenVertexArrays(1, &_vertex_array_object));

  std::vector<float> vertices = create_grid_vectices();
  std::vector<float> cross_vertices = create_cross_vectices();

  vertices.insert(vertices.end(), cross_vertices.begin(), cross_vertices.end());

  _vertices_size = vertices.size();

  GL_CALL(glBindVertexArray(_vertex_array_object));

  GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_object));
  GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices_size,
                       vertices.data(), GL_STATIC_DRAW));

  GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                                (void *)0));
  GL_CALL(glEnableVertexAttribArray(0));

  _shader_program = load_shader_program();
}

Renderer::~Renderer() {
  glDeleteProgram(_shader_program);
  glDeleteBuffers(1, &_vertex_array_object);
  glDeleteBuffers(1, &_vertex_buffer_object);
  glfwDestroyWindow(_window);
};

void Renderer::render() {
  glfwMakeContextCurrent(_window);
  GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

  GL_CALL(glUseProgram(_shader_program));

  GL_CALL(glBindVertexArray(_vertex_array_object));
  GL_CALL(glDrawArrays(GL_TRIANGLES, 0, _vertices_size));

  Nought(_size, LINE_WIDTH).draw();

  GL_CALL(glfwSwapBuffers(_window));
  GL_CALL(glfwPollEvents());
}

bool Renderer::should_close() { return glfwWindowShouldClose(_window); }

std::vector<float> Renderer::create_grid_vectices() {
  std::vector<float> vertices;
  vertices.reserve((_size + 1) * (_size + 1) * 3 * 2);
  float cell_width = GL_WIDTH / _size;
  for (int index = 0; index <= _size; ++index) {
    float x0 = GL_MIN_COORDINATE + index * cell_width - LINE_WIDTH / 2.0f;
    float x1 = GL_MIN_COORDINATE + index * cell_width + LINE_WIDTH / 2.0f;
    float x2 = GL_MIN_COORDINATE + index * cell_width - LINE_WIDTH / 2.0f;
    float x3 = GL_MIN_COORDINATE + index * cell_width + LINE_WIDTH / 2.0f;

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

std::vector<float> Renderer::create_cross_vectices() {
  float cell_width = GL_WIDTH / _size;

  float half_line_width = LINE_WIDTH / 2.0f;

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

GLuint Renderer::load_shader_program() {
  GLuint vertex_shader = load_vertex_shader();
  GLuint fragment_shader = load_fragment_shader();
  GLuint shader_program = glCreateProgram();
  GL_CALL(glAttachShader(shader_program, vertex_shader));
  GL_CALL(glAttachShader(shader_program, fragment_shader));
  GL_CALL(glLinkProgram(shader_program));
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  int success;
  char info_log[512];
  GL_CALL(glGetProgramiv(shader_program, GL_LINK_STATUS, &success));
  if (!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, info_log);
    throw std::runtime_error(std::string("ERROR::SHADER::PROGRAM::FAILED\n") +
                             info_log);
  }
  // else...
  return shader_program;
};

GLuint Renderer::load_vertex_shader() {
  std::string vertex_shader_source = read_shader(SHADER_VERTEX_FILE_PATH);
  const char *vertex_shader_c_str = vertex_shader_source.c_str();
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  GL_CALL(glShaderSource(vertex_shader, 1, &vertex_shader_c_str, NULL));
  GL_CALL(glCompileShader(vertex_shader));
  int success;
  char info_log[512];
  GL_CALL(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success));
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
    throw std::runtime_error(
        std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n") + info_log);
  }
  // else...
  return vertex_shader;
};

GLuint Renderer::load_fragment_shader() {
  std::string fragment_shader_source = read_shader(SHADER_FRAGMENT_FILE_PATH);
  const char *fragment_shader_source_c_str = fragment_shader_source.c_str();
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  GL_CALL(
      glShaderSource(fragment_shader, 1, &fragment_shader_source_c_str, NULL));
  GL_CALL(glCompileShader(fragment_shader));
  int success;
  char info_log[512];
  GL_CALL(glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success));
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
    throw std::runtime_error(
        std::string("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n") +
        info_log);
  }
  // else...
  return fragment_shader;
};

std::string Renderer::read_shader(const std::string &file_path) {
  std::ifstream file(file_path);
  if (!file) {
    throw std::runtime_error("Could not open file: " + file_path);
  }
  // else...
  std::stringstream buffer;
  buffer << file.rdbuf(); // Read the entire file into the stringstream buffer
  return buffer.str();    // Return the string from the buffer
}
} // namespace tictactoe_astar::renderer
