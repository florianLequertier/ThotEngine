#ifndef GLCONFIG_HPP
#define GLCONFIG_HPP

namespace te{

namespace gl{

/*enumeration to make a link between shaders parameters and program parameters*/
enum opengl_option {GLINDEX_POSITION = 0, GLINDEX_NORMAL = 1, GLINDEX_TEXCOORDS = 2, GLINDEX_COLOR = 3};

static const int NB_POINT_LIGHT = 5;
static const int NB_DIRECTIONAL_LIGHT = 5;

}

}

#endif // GLCONFIG_HPP
