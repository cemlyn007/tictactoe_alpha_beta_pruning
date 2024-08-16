#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sstream>

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

const char *glErrorString(GLenum err);

} // namespace tictactoe_ai::renderer