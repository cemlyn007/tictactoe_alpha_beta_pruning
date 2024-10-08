#include "tictactoe_ai/renderer/shader.h"
#include "tictactoe_ai/renderer/gl_error_macro.h"
#include <string>

namespace tictactoe_ai::renderer {

Shader::Shader(std::string vertex_source_code, std::string fragment_source_code)
    : _program(load_program(vertex_source_code, fragment_source_code)) {}

GLuint Shader::load_program(std::string vertex_source_code,
                            std::string fragment_source_code) {
  GLuint vertex_shader = load_vertex_shader(vertex_source_code);
  GLuint fragment_shader = load_fragment_shader(fragment_source_code);
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
    glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
    throw std::runtime_error(std::string("ERROR::SHADER::PROGRAM::FAILED\n") +
                             info_log);
  }
  // else...
  return shader_program;
};

Shader::~Shader() { glDeleteProgram(_program); }

void Shader::use() { GL_CALL(glUseProgram(_program)); }

void Shader::set_uniform(std::string name, float x, float y) {
  GLuint location = glGetUniformLocation(_program, name.c_str());
  GL_CALL(glUniform2f(location, x, y););
}

GLuint Shader::load_vertex_shader(std::string vertex_source_code) {
  const char *vertex_shader_c_str = vertex_source_code.c_str();
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  GL_CALL(glShaderSource(vertex_shader, 1, &vertex_shader_c_str, nullptr));
  GL_CALL(glCompileShader(vertex_shader));
  int success;
  char info_log[512];
  GL_CALL(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success));
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
    throw std::runtime_error(
        std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n") + info_log);
  }
  // else...
  return vertex_shader;
};

GLuint Shader::load_fragment_shader(std::string fragment_source) {
  const char *fragment_shader_source_c_str = fragment_source.c_str();
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  GL_CALL(glShaderSource(fragment_shader, 1, &fragment_shader_source_c_str,
                         nullptr));
  GL_CALL(glCompileShader(fragment_shader));
  int success;
  char info_log[512];
  GL_CALL(glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success));
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
    throw std::runtime_error(
        std::string("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n") +
        info_log);
  }
  // else...
  return fragment_shader;
};

} // namespace tictactoe_ai::renderer