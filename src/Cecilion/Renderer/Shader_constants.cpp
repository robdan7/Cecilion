#include "Shader_constants.h"

namespace Cecilion {
    CL_System_params* params;

    void init_shader_constants() {
        params = new CL_System_params();
    }

    CL_System_params* get_system_params() {
        return params;
    }
}