#ifndef CHEAT_CLIENT_H
#define CHEAT_CLIENT_H

#include <jni.h>
#include <vector>
#include <cmath>
#include <fstream>
#include "stdint.h"
#include "nlohmann/json.hpp"
#include "vector/vector3.h"
#include "network/raknet/NetworkTypes.h"
#include "game/core/timer.h"
#include "interface/ImGui/imgui.h"

struct ServerPlayer
{
	short id;
	char nickname[25];
};

struct DrawBuffer
{
	ImVec2 start;
	ImVec2 end;
};

class Cheat
{
public:
	Cheat();
	
	// Must have fields
	int screen_width = 0;
	int screen_height = 0;
	std::string response;

	int ServerPlayersCount = 1;
	ServerPlayer ServerPlayers[1500] = {0};
	
	// Game Library Setter/Getter
	void SetLibAddr(uintptr_t);
	uintptr_t GetLibAddr();
	
	// Touch Screen methods
	bool InitTouchScreen(std::string filename);
	bool TouchInJoystickZone(int x, int y);
	bool TouchInAccelerateZone(int x, int y);
	bool TouchInBrakeZone(int x, int y);
	
	// Last error's methods for debugging
	// TODO: delete this functions to debug folder
	std::string GetLastErrorMessage();
	void SetLastErrorMessage(std::string error_message);
	std::string last_error_message;
	
	// Pool's functions
	uintptr_t GetPlayerPool();
	uintptr_t GetVehiclePool();
	uintptr_t GetObjectPool();
	uintptr_t GetAtPlayerPool(int id);
	uintptr_t GetAtVehiclePool(int id);
	uintptr_t GetAtObjectPool(int id);
	
	// Game Network Function's
	void SetNameTagsDrawDistance(float m_fDistance);
	float GetNameTagsDrawDistance();
	
	// Game Function's
	
	// Player Ped Function's
	uintptr_t FindLocalPlayer();
	uintptr_t FindNearestPlayer();
	int GetWeaponId();
	
	// Matrix Function's
	void SetRotate(uintptr_t matrix, float, float, float);
	void SetRotateZOnly(uintptr_t matrix, float z);
	Vec3 GetPos(uintptr_t object);
	void SetPos(uintptr_t object, Vec3 pos);
	void SetPos(uintptr_t object, float x, float y, float z);
	
	// Camera Function's
	Vec3 GetCameraCoors();
	uintptr_t GetCameraMatrix();
	void SetViewRotateX(float x);
	void SetViewRotateY(float y);
	float GetViewRotateX();
	float GetViewRotateY();
	
	// Map marker's Function's
	Vec2 GetMarkerCoors();
	Vec3 GetCheckpointCoors();
	Vec3 GetRaceCheckpointCoors();

	CTimer* timer = nullptr;
	
	// Variables
	struct {
		struct {
			struct {
				float up = 3.5f;
				float down = 3.5f;
			} Slap;
			struct {
				bool state = false;
				float speed = 0.f;
			} FlyPed;
			struct {
				bool health = false;
				bool endurance = false;
				bool walkUnderwater = false;
				bool noBike = false;
			} Invulnerability;
			struct {
				bool high = false;
				bool long_ = false;
				float long__ = 1.5f;
				float high__ = 1.2f;
			} Jumping;
			struct {
				bool fastRotate = false;
				float rate = 2.f;
			} Control;
		} Base;

		struct {
			struct {
				bool state = false;
				int deagleClip = 0;
				int prev_deagleClip = 0;
				int AK47Clip = 0;
				int prev_AK47Clip = 0;
				int m4Clip = 0;
				int prev_m4Clip = 0;
			} NoReload;
			struct {
				bool state = false;
			} AimBot;
			struct {
				bool autoHeal = false;
				bool autoArmour = false;
				// TODO: automatically take gun from family

			} Invulnerability;
			struct {
				bool state = false;
				uint16_t targetID;
				int last_shot = 0;
			} Damager;
		} Rage;
	} PlayerCheat;
	
	struct {
		struct {
			bool state = false;
			float speed = 0.f;
			struct {
				float x = 0.f;
				float y = 0.f;
				float z = 0.f;
			} Angle;
		} FlyCar;
		struct {
			bool state = false;
			float speed = 0.f;
			float accelerate = 0.3f;
		} CarShot;
		struct {
			// TODO: implement cheat
			bool state = false;
		} AirBrake;
		struct {
			bool health = false;
			bool engine = false;
			bool driveUnderwater = false;
		} Invulnerability;
		struct {
			bool state = false;
			uintptr_t trailer = 0;
		} AntiDetachTrailer;
		struct {
			bool menu = false;
			bool entered = false;
			uint16_t modelID;
			std::string numpad;
			uintptr_t prev_vehicle = 0;
			bool removedFromVehicle = false;
			float prev_health = 0;
			int prev_network_id = 0;
			float prev_angle = 0;
		} CarChanger;
	} VehicleCheat;
	
	struct {
		struct {
			bool freeze = false;
			int time = 12;
		} TimeChanger;
		struct {
			bool menu = false;
			bool entered = false;
			std::string numpad;
		} MoneyChanger;
		struct {
			bool menu = false;
			bool entered = false;
			std::string numpad;
			uint16_t value = 0;
		} SkinChanger;
		struct {
			float FOV = 70.f;
			struct {
				bool state = false;
				float speed = 1.f;
				Vec3 pos = {0,0,0};
				Vec3 startPos = {0,0,0};
				Vec3 angles = {0,0,0};
			} FreeCam;
		} Camera;
	} VisualCheat;

	struct {
		bool menu = false;
		bool working = false;
		bool wait_text_input = false;
		bool wait_delay_input = false;
		int last_sended_message_time = 0;
		int delay_after_message = 3000;
		std::string message;
	} Flooder;

	struct {
		bool menu = false;
		bool state = false;
		std::string nick;
		uint16_t id;
		std::string msg;
		uint8_t type;
		uint8_t fraction;
	} FakeChat;

	struct {
		bool clearAnimation = false;
		bool vehicleVelocity = false;
	} NOPs;

	struct {
		struct {
			bool menu = false;
			bool state = false;
		} AntiAfk;
		struct {
			bool menu = false;
			bool state = false;
			Vec3 prev_point = {0,0,0};
			Vec3 start_point = {549.35f, 568.9f, 999.f};
			Vec3 end_point = {561.65f, 538.9614f, 999.f};
			bool mining = false;
			bool haveStone = false;
			bool goingToPoint = false;
			int takingStoneLastTime = 0;
		} BotMiner;
		struct {
			bool menu = false;
			bool state = false;
		} BotTrucker;
		struct {
			bool menu = false;
			bool state = false;
		} BotBus;
		struct {
			bool menu = false;
			bool state = false;

		} BotCourier;
		struct {
			bool menu = false;
			bool state = false;

		 	struct {

			} BotBeeKeeper;
		} BotFarm;
		struct {
			bool menu = false;
			bool state = false;

		} BotFactory;
	} AutoFarm;

	struct
	{
		bool menu = false;
		bool state = false;
		float speed = 1.f;
	} GameSpeed;

	struct {
		bool menu = false;
		bool state = false;
		float body_color[3] = {1.f,1.f,1.f};
		float wheels_color[3] = {1.f,1.f,1.f};
		float lights_color[3] = {1.f,1.f,1.f};
		float glow_color[3] = {1.f,1.f,1.f};
		float right_glow_color[3] = {1.f,1.f,1.f};
		float left_glow_color[3] = {1.f,1.f,1.f};
		bool stroboscopes = false;
		int stroboscopesType = 0;
		bool longLight = false;
		bool totalClearance = false;
		bool separateClearance = false;
		int total_clearance_val = 0;
		int separate_clearance_val = 0;
		int wheels_radius = 0;
		int departure_front_wheels = 0;
		int departure_back_wheels = 0;
		int collapse_front_wheels = 0;
		int collapse_back_wheels = 0;
		int width_front_wheels = 0;
		int width_back_wheels = 0;
	} Tuning;
	
	struct {
		PlayerID sender;
		RakPeerInterface *recipient;
		RakNet::BitStream *replyToSender;
	} GameServer;

	struct {
		bool toMarker = false;
		bool toCheckpoint = false;
		bool toPeds = false;
		bool toCars = false;
		
		struct {
			float speed = 0.f;
			float speed_max = 50.f;
		} OnFoot;
		
		struct {
			float speed = 0.f;
			float speed_max = 300.f;
		} InCar;
		
	} Teleport;
	
	struct {
		bool menu = false;
		bool ped = false;
		bool vehicle = false;
		bool objects = false;
	} Collision;
	
	struct {
		bool menu = false;
		bool state = false;
		bool entered = false;
		uint16_t modelID;
		std::string numpad;
	} CarSpawn;
	
	struct {
		bool menu = false;
		struct {
			bool lines = false;
			bool box = false;
			bool collision = false;
			bool skeleton = false;
			bool model_id = false;
			bool nametags = false;
			struct {
				struct {
					ImVec4 color = {0,1.f,0,1.f};
					float tickness = 5.f;
				} Lines;
				struct {
					ImVec4 color = {0,1.f,0,1.f};
					float tickness = 5.f;
				} Box;
				struct {
					std::vector<DrawBuffer> buffer;
					bool rendered = false;
					ImVec4 color = {0,1.f,0,1.f};
					float tickness = 5.f;
				} Skeleton;
			} Config;
		} Player;
		struct {
			bool lines = false;
			bool box = false;
			bool collision = false;
			bool model_id = false;
			bool model_name = false;
			struct {
				struct {
					ImVec4 color = {0,1.f,0,1.f};
					float tickness = 5.f;
				} Lines;
			} Config;
		} Vehicle;
		struct {
			bool lines = false;
			bool box = false;
			bool collision = false;
			bool model_id = false;
			bool model_name = false;
			std::vector<unsigned short> filter;
			struct {
				struct {
					ImVec4 color = {0,1.f,0,1.f};
					float tickness = 5.f;
				} Lines;
			} Config;
		} Object;
	} Wallhack;
	
	struct {
		int touch_pointer_id = -1;
		Vec2 Actuator = { 0 , 0 };
		float angle = 0;
		float magnitude = 0;
	} Joystick;
	
	struct {
		int touch_pointer_id = 0;
		bool isPressed = false;
		float width = 0;
		float height = 0;
		float x = 0;
		float y = 0;
	} Accelerate;
	
	struct {
		int touch_pointer_id = 0;
		bool isPressed = false;
		float width = 0;
		float height = 0;
		float x = 0;
		float y = 0;
	} Brake;
	
	struct {
		bool state = false;
		bool show_custom_crosshair = true;
		int target_id = -1;
		int target = 0;
		int target_bone_id = 5;
		int aimXOffset = 70;
		int aimYOffset = -80;
		float fov = 180.f;
		float fov_tickness = 5.f;
		float target_tickness = 10.f;
		float custom_crosshair_size = 5.f;
		float diffX = 0;
		float diffY = 0;
		std::vector<int> players_id;
		std::vector<float> players_distance;
	} Aimbot;
	
private:
	uintptr_t libaddr = 0;
};

#endif 
