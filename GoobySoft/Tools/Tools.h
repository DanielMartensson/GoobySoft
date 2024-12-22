#ifndef Tools
#define Tools
#include <string>
#include <vector>
#include <filesystem>
#include "Communications/Communications.h"
#include "Hardware/Hardware.h"
#include "Gui/Gui.h"
#include "Storage/Storage.h"
#include "Software/Software.h"

#include <SDL3/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#endif // !Tools
