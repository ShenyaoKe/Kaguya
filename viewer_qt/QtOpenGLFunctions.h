#if 1
#include <QtGui/QOpenGLFunctions_4_1_Core>
#define OGL_MAJOR_VER 4
#define OGL_MINOR_VER 1
using QtGLFunctions = QOpenGLFunctions_4_1_Core;
#else
#include <QtGui/QOpenGLFunctions_3_3_Core>
#define OGL_MAJOR_VER 3
#define OGL_MINOR_VER 3
using QtGLFunctions = QOpenGLFunctions_3_3_Core;
#endif
