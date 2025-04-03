#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <KittyMemory/KittyMemory.h>
#include <KittyMemory/MemoryPatch.h>
#include <KittyMemory/KittyScanner.h>
#include <KittyMemory/KittyUtils.h>
#include "Includes/obfuscate.h"
#include "Includes/Logger.h"
#include "Includes/Macros.h"
#include "Includes/Utils.h"
#include "BunRiew/Call_Me.h"
#include "Hook.h"
#define RealLibToLoad "libBunRiew.so"

bool setup = false;

EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean _eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
	
	eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
	eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
    
	if (glWidth <= 0 || glHeight <= 0) {
		return eglSwapBuffers(dpy, surface);
	}

    if (!setup){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsLight();
    io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 25.f, NULL, io.Fonts->GetGlyphRangesVietnamese());
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImGui::GetStyle().ScaleAllSizes(3.0f);
    setup = true;
    }
    
    auto il2cpp_handle = dlopen("libil2cpp.so", 4);
    ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame(); 
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Once);
    if (ImGui::Begin("BunRiew ", nullptr)) {
    if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
        
    if (ImGui::BeginTabItem("Visual")) {
    ImGui::EndTabItem(); }
    
    if (ImGui::BeginTabItem("Other")) {
        if(ImGui::Button("Il2Cpp Dump"))   
        ImGui::OpenPopup("##DUMP");
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));           
        if (ImGui::BeginPopupModal("##DUMP", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
        {
            ImGui::Text("Are you sure you want to dump this game?");
            ImGui::Text("File path : \n%s", androidDataPath.c_str());            
            ImGui::Separator();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ImGui::PopStyleVar();
            if (ImGui::Button("OK", ImVec2(120, 0))) {il2cpp_dump(il2cpp_handle), ImGui::CloseCurrentPopup(); }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
            }ImGui::EndTabItem(); } ImGui::EndTabBar(); }
    
           
    ImGui::End(); }
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return orig_eglSwapBuffers(dpy, surface);
}

uintptr_t il2cppMap;
ProcMap anogsMap, il2cppMap2;

void *Init_Thread(void *) {
	
	while (!il2cppMap) {
		il2cppMap2 = KittyMemory::getLibraryBaseMap("libil2cpp.so");
	    anogsMap = KittyMemory::getLibraryBaseMap("libanogs.so");
        il2cppMap = Tools::GetBaseAddress("libil2cpp.so");
		sleep(4);
	}
	
    IL2Cpp::Il2CppAttach();
    
	return nullptr;
}

void *pLibRealUnity = 0;

typedef jint(JNICALL *CallJNI_OnLoad_t)(JavaVM *vm, void *reserved);

typedef void(JNICALL *CallJNI_OnUnload_t)(JavaVM *vm, void *reserved);

CallJNI_OnLoad_t RealJNIOnLoad = 0;
CallJNI_OnUnload_t RealJNIOnUnload = 0;

JNIEXPORT jint JNICALL CallJNIOL(
        JavaVM *vm, void *reserved) {
    LOGI("Exec %s", RealLibToLoad);
    if (!pLibRealUnity)
        pLibRealUnity = dlopen(RealLibToLoad, RTLD_NOW);
    if (!RealJNIOnLoad)
        RealJNIOnLoad = reinterpret_cast<CallJNI_OnLoad_t>(dlsym(pLibRealUnity, "JNI_OnLoad"));
    return RealJNIOnLoad(vm, reserved);
}

JNIEXPORT void JNICALL CallJNIUL(
        JavaVM *vm, void *reserved) {
    if (!pLibRealUnity)
        pLibRealUnity = dlopen(RealLibToLoad, RTLD_NOW);
    if (!RealJNIOnUnload)
        RealJNIOnUnload = reinterpret_cast<CallJNI_OnUnload_t>(dlsym(pLibRealUnity,
                                                                     "JNI_OnUnload"));
    RealJNIOnUnload(vm, reserved);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("Initialize JNI");

    return CallJNIOL(vm, reserved);
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
    LOGI("Unload JNI");

    CallJNIUL(vm, reserved);
}

__attribute__((constructor))
void lib_main()
{
	Tools::Hook((void *) DobbySymbolResolver(OBFUSCATE("/system/lib/libandroid.so"), OBFUSCATE("ANativeWindow_getWidth")), (void *) _ANativeWindow_getWidth, (void **) &orig_ANativeWindow_getWidth);
	Tools::Hook((void *) DobbySymbolResolver(OBFUSCATE("/system/lib/libandroid.so"), OBFUSCATE("ANativeWindow_getHeight")), (void *) _ANativeWindow_getHeight, (void **) &orig_ANativeWindow_getHeight);
    Tools::Hook((void *) DobbySymbolResolver(OBFUSCATE("/system/lib/libinput.so"), OBFUSCATE("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE")), (void *) myInput, (void **) &origInput);
    Tools::Hook((void *) DobbySymbolResolver(OBFUSCATE("/system/lib/libEGL.so"), OBFUSCATE("eglSwapBuffers")), (void *) _eglSwapBuffers, (void **) &orig_eglSwapBuffers);
	pthread_t myThread;
	pthread_create(&myThread, NULL, Init_Thread, NULL);
}
