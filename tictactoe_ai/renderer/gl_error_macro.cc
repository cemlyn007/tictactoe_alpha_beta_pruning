#include "tictactoe_ai/renderer/gl_error_macro.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace tictactoe_ai::renderer {
#define GL_CALL(cmd)                                                           \
  {                                                                            \
    cmd;                                                                       \
    GLenum err = glGetError();                                                 \
    if (err != GL_NO_ERROR) {                                                  \
      std::ostringstream oss;                                                  \
      oss << "OpenGL error: " << glErrorString(err) << " in file " << __FILE__ \
          << " at line " << __LINE__ << " after calling " #cmd;                \
      throw std::runtime_error(oss.str());                                     \
    }                                                                          \
  }

const char *glErrorString(GLenum err) {
  switch (err) {
  case GL_NO_ERROR:
    return "No error";
  case GL_INVALID_ENUM:
    return "Invalid enum";
  case GL_INVALID_VALUE:
    return "Invalid value";
  case GL_INVALID_OPERATION:
    return "Invalid operation";
  case GL_STACK_OVERFLOW:
    return "Stack overflow";
  case GL_STACK_UNDERFLOW:
    return "Stack underflow";
  case GL_OUT_OF_MEMORY:
    return "Out of memory";
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    return "Invalid framebuffer operation";
  default:
    return "Unknown error";
  }
}
} // namespace tictactoe_ai::renderer