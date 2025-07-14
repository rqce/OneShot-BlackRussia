LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libdobby
LOCAL_SRC_FILES := libraries/$(TARGET_ARCH_ABI)/libdobby.a

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := oneshot

# -std=c++17 is required to support AIDE app with NDK
LOCAL_CFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions
LOCAL_CPPFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -Werror -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fpermissive -Wall -fexceptions -Wno-register
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all,-llog
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2 -lGLESv3 -lGLESv1_CM -lz -fno-asm
LOCAL_ARM_MODE := arm

LOCAL_STATIC_LIBRARIES := libdobby

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/ImGui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/ImGui/backends

# Here you add the cpp file to compile
LOCAL_SRC_FILES := main.cpp \
	memory/Substrate/hde64.c \
	memory/Substrate/SubstrateDebug.cpp \
	memory/Substrate/SubstrateHook.cpp \
	memory/Substrate/SubstratePosixMemory.cpp \
	memory/Substrate/SymbolFinder.cpp \
	memory/And64InlineHook/And64InlineHook.cpp \
	interface/ImGui/imgui.cpp \
	interface/ImGui/imgui_draw.cpp \
	interface/ImGui/imgui_demo.cpp \
	interface/ImGui/imgui_widgets.cpp \
	interface/ImGui/imgui_tables.cpp \
	interface/ImGui/backends/imgui_impl_opengl3.cpp \
	network/raknet/SAMP/samp_auth.cpp \
	network/raknet/SAMP/samp_netencr.cpp \
	network/raknet/SAMP/SAMPRPC.cpp \
	network/raknet/_findfirst.cpp \
	network/raknet/AsynchronousFileIO.cpp \
	network/raknet/BitStream.cpp \
	network/raknet/BitStream_NoTemplate.cpp \
	network/raknet/CheckSum.cpp \
	network/raknet/CommandParserInterface.cpp \
	network/raknet/DataBlockEncryptor.cpp \
	network/raknet/DataCompressor.cpp \
	network/raknet/DS_ByteQueue.cpp \
	network/raknet/DS_HuffmanEncodingTree.cpp \
	network/raknet/DS_Table.cpp \
	network/raknet/EncodeClassName.cpp \
	network/raknet/ExtendedOverlappedPool.cpp \
	network/raknet/FileOperations.cpp \
	network/raknet/GetTime.cpp \
	network/raknet/InternalPacketPool.cpp \
	network/raknet/LinuxStrings.cpp \
	network/raknet/LogCommandParser.cpp \
	network/raknet/NetworkTypes.cpp \
	network/raknet/PluginInterface.cpp \
	network/raknet/RakClient.cpp \
	network/raknet/RakNetCommandParser.cpp \
	network/raknet/RakNetStatistics.cpp \
	network/raknet/RakNetworkFactory.cpp \
	network/raknet/RakPeer.cpp \
	network/raknet/rakserver.cpp \
	network/raknet/RakSleep.cpp \
	network/raknet/Rand.cpp \
	network/raknet/ReliabilityLayer.cpp \
	network/raknet/rijndael.cpp \
	network/raknet/RPCMap.cpp \
	network/raknet/SHA1.cpp \
	network/raknet/SimpleMutex.cpp \
	network/raknet/SocketLayer.cpp \
	network/raknet/StringCompressor.cpp \
	network/raknet/StringTable.cpp \
	network/raknet/SystemAddressList.cpp \
	network/raknet/SystemDatabaseClient.cpp \
	network/raknet/SystemDatabaseServer.cpp \
	network/raknet/TableSerializer.cpp \
	network/raknet/TCPInterface.cpp \
	network/raknet/TelnetTransport.cpp \
	cheat/cheat.cpp \
	memory/memory.cpp \
	debug/BRNotification.cpp \
	game/peds/ped.cpp \
	game/vehicles/vehicle.cpp \
	game/core/timer.cpp \
	game/math/math.cpp \
	gameLibAddr.cpp \
	cheat/cheat_hooks.cpp \
	cheat/cheat_draw.cpp \

include $(BUILD_SHARED_LIBRARY)
