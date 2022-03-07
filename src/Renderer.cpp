#include "Renderer.h"

#include <GL/glew.h>                    // glGetError

namespace snbl {

void clearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool logCall(const char *file, int line, const char *function) {
    GLenum error;
    while (error = glGetError()) {
        std::printf("OpenGL error: in file %s, on line %d, in function %s: %x\n",
            file, line, function, error
        );
        return false;
    }
    return true;
}

} // namespace snbl