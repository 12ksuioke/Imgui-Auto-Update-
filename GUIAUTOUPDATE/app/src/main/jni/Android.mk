LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE            := libdobby
LOCAL_SRC_FILES         := BunRiew/Tools/Dobby/libraries/$(TARGET_ARCH_ABI)/libdobby.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/BunRiew/Tools/Dobby/
include $(PREBUILT_STATIC_LIBRARY)
# ============================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE           := main

LOCAL_CFLAGS           := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w
LOCAL_CFLAGS           += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS         := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -Werror -s -std=c++17
LOCAL_CPPFLAGS         += -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fpermissive
LOCAL_LDFLAGS          += -Wl,--gc-sections,--strip-all, -llog
LOCAL_ARM_MODE         := arm
LOCAL_LDLIBS           := -llog -landroid -lEGL -lGLESv3 -lGLESv2 -lGLESv1_CM -lz

LOCAL_C_INCLUDES       += $(LOCAL_PATH)
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/ImGui
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/ImGui/backends
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/BunRiew
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/BunRiew/Tools/curl/openssl-android-$(TARGET_ARCH_ABI)/include

LOCAL_SRC_FILES := Main.cpp \
    ImGui/imgui.cpp \
    ImGui/imgui_draw.cpp \
    ImGui/imgui_tables.cpp \
    ImGui/imgui_widgets.cpp \
    ImGui/imgui_stdlib.cpp \
    ImGui/backends/imgui_impl_opengl3.cpp \
    ImGui/backends/imgui_impl_android.cpp \
	Substrate/hde64.c \
	Substrate/SubstrateDebug.cpp \
	Substrate/SubstrateHook.cpp \
	Substrate/SubstratePosixMemory.cpp \
	Substrate/SymbolFinder.cpp \
    KittyMemory/KittyMemory.cpp \
    KittyMemory/MemoryPatch.cpp \
    KittyMemory/MemoryBackup.cpp \
    KittyMemory/KittyUtils.cpp \
    KittyMemory/KittyScanner.cpp \
    KittyMemory/KittyArm64.cpp \
    And64InlineHook/And64InlineHook.cpp \
    BunRiew/IL2CppSDKGenerator/Il2Cpp.cpp \
    BunRiew/Tools/MonoString.cpp \
    BunRiew/Tools/Tools.cpp \
    BunRiew/Dump/Il2Cpp/il2cpp_dump.cpp \
     
LOCAL_STATIC_LIBRARIES := libdobby
LOCAL_CPP_FEATURES     := exceptions

include $(BUILD_SHARED_LIBRARY)
# ============================================================================

