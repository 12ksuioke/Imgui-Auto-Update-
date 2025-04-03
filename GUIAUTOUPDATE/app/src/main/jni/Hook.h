#pragma once

#define HOOK(ret, func, ...) \
    ret (*orig##func)(__VA_ARGS__); \
    ret my##func(__VA_ARGS__)

HOOK(void, Input, void *thiz, void *ex_ab, void *ex_ac) { 
    origInput(thiz, ex_ab, ex_ac);
ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    return;
}

int32_t (*orig_ANativeWindow_getWidth)(ANativeWindow* window);
int32_t _ANativeWindow_getWidth(ANativeWindow* window) {
	screenWidth = orig_ANativeWindow_getWidth(window);
	return orig_ANativeWindow_getWidth(window);
}

int32_t (*orig_ANativeWindow_getHeight)(ANativeWindow* window);
int32_t _ANativeWindow_getHeight(ANativeWindow* window) {
	screenHeight = orig_ANativeWindow_getHeight(window);
	return orig_ANativeWindow_getHeight(window);
}

#if defined(__aarch64__) 
    #define RETURN OBFUSCATE("CO 03 5F D6");
    #define TRUE OBFUSCATE("20 00 80 D2 CO 03 5F D6")
    #define FALSE OBFUSCATE("00 00 80 D2 CO 03 5F D6")
    auto androidDataPath = std::string("/storage/emulated/0/Android/data/").append(GetPackageName()).append("/").append(GetPackageName()).append(" [X64]").append(".cs");
#else
    #define RETURN OBFUSCATE("1E FF 2F E1")
    #define TRUE OBFUSCATE("01 00 A0 E3 1E FF 2F E1")
    #define FALSE OBFUSCATE("00 00 A0 E3 1E FF 2F E1")
    auto androidDataPath = std::string("/storage/emulated/0/Android/data/").append(GetPackageName()).append("/").append(GetPackageName()).append(" [X32]").append(".cs");
#endif
