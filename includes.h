
// Include system headers
#include <vector>
#include <string>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <dlfcn.h>
#include <stdint.h>
#include <cmath>

// Debug utils
#include "debug/Logger.h"
#include "debug/BRNotification.h"

// Security utils
#include "security/obfuscate.h"

// Memory utils
#include "memory/Utils.h"
#include "memory/Dobby/dobby.h"
#include "memory/Substrate/CydiaSubstrate.h"
#include "memory/memory.h"

// Interface
#include "interface/res/icon_image.h"
#include "interface/res/alpha_icon_image.h"
#include "interface/res/font.h"
#include "interface/res/icon_font.h"
#define STB_IMAGE_IMPLEMENTATION
#include "interface/stb/stb_image.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "interface/ImGui/imgui.h"
#include "interface/ImGui/imgui_internal.h"
#include "interface/ImGui/backends/imgui_impl_opengl3.h"

// Network
#include "network/raknet/BitStream.h"
#include "network/raknet/NetworkTypes.h"

// Cheat Implementations
#include "gameLibAddr.h"
#include "game/peds/ped.h"
#include "cheat/cheat_hooks.h"
#include "cheat/cheat_draw.h"

// math
#include "game/math/math.h"

using namespace std;

extern "C"
{
	JNIEXPORT void JNICALL Java_com_blackhub_bronline_game_core_JNILib_multiTouchEvent(JNIEnv* env, jclass cls, int, int, int, int, int, int, int, int);
	JNIEXPORT void JNICALL Java_wrapper_Wrapper_next(JNIEnv* env, jobject obj, jobject mActivity);
	JNIEXPORT void JNICALL Java_wrapper_Wrapper_initbaby(JNIEnv* env, jclass cls, jobject context);
}

void Notification(JNIEnv* env, int type, string text, int duration, string text2 = "null");
void startService(JNIEnv *env, jobject ctx);
void Draw3dBox(int object, ImVec4, float tickness);
bool CollisionProcessed(int ped, int object);
