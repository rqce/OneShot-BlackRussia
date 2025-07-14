#include "cheat.h"
#include "security/obfuscate.h"

Cheat::Cheat()
{
	/*chams = false;
	flycar = false;
	flyped = false;
	carshot = false;
	airbrake_car = false;
	nopClearAnim = false;
	nopVehicleVelocity = false;
	infinite_engine = false;
	espPedLine = false;
	espPedBox = false;
	espPedSkeleton = false;
	nametags = false;
	no_reload = false;
	aimbot = false;
	gmped = false;
	gmcar = false;
	infiniteRun = false;
	longjump = false;
	highjump = false;
	fastrotate = false;
	tuning = false;
	damager = false;
	nobike = false;
	wallhack = false;
	set_player_skin = false;
	set_player_money = false;
	freecam = false;
	walk_underwater = false;
	drive_underwater = false;
	flooder = false;
	fake_chat = false;
	// боты
	botminer = false;
	botfactory = false;
	botfarm = false;
	botcourier = false;
	botbus = false;
	bottruck = false;
	anti_afk = false;
	auto_heal = false;
	game_speed = false;
	collision = false;
	change_car_model = false;
	change_car_model_entered = false;
	anti_remove_trailer = false;
	tuning_menu = false;
	auto_armour = false;
	set_fov = false;
	set_time = false;
	set_weather = false;
	trigger_gevent = false;*/
}

void Cheat::SetLibAddr(uintptr_t addr)
{
	this->libaddr = addr;
}

uintptr_t Cheat::GetLibAddr()
{
	return this->libaddr;
}

bool Cheat::InitTouchScreen(std::string filename = "")
{
	std::ifstream file(filename);
	
	if(!file.is_open()) {
		SetLastErrorMessage("Не удалось открыть файл touchscreen-custom.json");
		return false;
	}
	
	std::string file_content((std::istreambuf_iterator<char>(file)),
							std::istreambuf_iterator<char>());
	
	file.close();
	
	if(file_content.size() <= 0) {
		SetLastErrorMessage("Не удалось считать текст из touchscreen-custom.json");
		return false;
	}
	
	try {
        // Распарсить JSON
        nlohmann::json js = nlohmann::json::parse(file_content);

        // Получить конфигурацию
        nlohmann::json config = js["config"];
		
        // Проход по каждому элементу в конфигурации
        for (auto& item : config) {
            // Проверить имя элемента
            if (item["name"] == "accelerate") {
                // Получить значение "width"
                this->Accelerate.x = item["x"];
                
				break;
            }
        }
    } catch (const nlohmann::json::parse_error& e) {
		std::string error_message("JSON parsing error: ");
		error_message.append(e.what());
        SetLastErrorMessage(error_message);
		return false;
    }
	
	try {
        // Распарсить JSON
        nlohmann::json js = nlohmann::json::parse(file_content);

        // Получить конфигурацию
        nlohmann::json config = js["config"];
		
        // Проход по каждому элементу в конфигурации
        for (auto& item : config) {
            // Проверить имя элемента
            if (item["name"] == "accelerate") {
                // Получить значение "width"
                this->Accelerate.y = item["y"];
                
				break;
            }
        }
    } catch (const nlohmann::json::parse_error& e) {
		std::string error_message("JSON parsing error: ");
		error_message.append(e.what());
        SetLastErrorMessage(error_message);
		return false;
    }
	
	try {
        // Распарсить JSON
        nlohmann::json js = nlohmann::json::parse(file_content);

        // Получить конфигурацию
        nlohmann::json config = js["config"];
		
        // Проход по каждому элементу в конфигурации
        for (auto& item : config) {
            // Проверить имя элемента
            if (item["name"] == "accelerate") {
                // Получить значение "width"
                this->Accelerate.width = item["width"];
                
				break;
            }
        }
    } catch (const nlohmann::json::parse_error& e) {
		std::string error_message("JSON parsing error: ");
		error_message.append(e.what());
        SetLastErrorMessage(error_message);
		return false;
    }
	
	try {
        // Распарсить JSON
        nlohmann::json js = nlohmann::json::parse(file_content);

        // Получить конфигурацию
        nlohmann::json config = js["config"];
		
        // Проход по каждому элементу в конфигурации
        for (auto& item : config) {
            // Проверить имя элемента
            if (item["name"] == "accelerate") {
                // Получить значение "width"
                this->Accelerate.height = item["height"];
                
				break;
            }
        }
    } catch (const nlohmann::json::parse_error& e) {
		std::string error_message("JSON parsing error: ");
		error_message.append(e.what());
        SetLastErrorMessage(error_message);
		return false;
    }
	
	try {
        // Распарсить JSON
        nlohmann::json js = nlohmann::json::parse(file_content);

        // Получить конфигурацию
        nlohmann::json config = js["config"];
		
        // Проход по каждому элементу в конфигурации
        for (auto& item : config) {
            // Проверить имя элемента
            if (item["name"] == "brake") {
                // Получить значение "width"
                this->Brake.x = item["x"];
                
				break;
            }
        }
    } catch (const nlohmann::json::parse_error& e) {
		std::string error_message("JSON parsing error: ");
		error_message.append(e.what());
        SetLastErrorMessage(error_message);
		return false;
    }
	
	try {
        // Распарсить JSON
        nlohmann::json js = nlohmann::json::parse(file_content);

        // Получить конфигурацию
        nlohmann::json config = js["config"];
		
        // Проход по каждому элементу в конфигурации
        for (auto& item : config) {
            // Проверить имя элемента
            if (item["name"] == "brake") {
                // Получить значение "width"
                this->Brake.y = item["y"];
                
				break;
            }
        }
    } catch (const nlohmann::json::parse_error& e) {
		std::string error_message("JSON parsing error: ");
		error_message.append(e.what());
        SetLastErrorMessage(error_message);
		return false;
    }
	
	try {
        // Распарсить JSON
        nlohmann::json js = nlohmann::json::parse(file_content);

        // Получить конфигурацию
        nlohmann::json config = js["config"];
		
        // Проход по каждому элементу в конфигурации
        for (auto& item : config) {
            // Проверить имя элемента
            if (item["name"] == "brake") {
                // Получить значение "width"
                this->Brake.width = item["width"];
                
				break;
            }
        }
    } catch (const nlohmann::json::parse_error& e) {
		std::string error_message("JSON parsing error: ");
		error_message.append(e.what());
        SetLastErrorMessage(error_message);
		return false;
    }
	
	try {
        // Распарсить JSON
        nlohmann::json js = nlohmann::json::parse(file_content);

        // Получить конфигурацию
        nlohmann::json config = js["config"];
		
        // Проход по каждому элементу в конфигурации
        for (auto& item : config) {
            // Проверить имя элемента
            if (item["name"] == "brake") {
                // Получить значение "width"
                this->Brake.height = item["height"];
                
				break;
            }
        }
    } catch (const nlohmann::json::parse_error& e) {
		std::string error_message("JSON parsing error: ");
		error_message.append(e.what());
        SetLastErrorMessage(error_message);
		return false;
    }
	
	return true;
}

bool Cheat::TouchInJoystickZone(int x, int y)
{
	float y1 = screen_height / 2;
	float width = screen_width / 2;
	
	if(x < width && x > 0 && y > y1) {
		return true;
	}
	
	return false;
}

bool Cheat::TouchInAccelerateZone(int x, int y)
{
	if(x > this->Accelerate.x && x < this->Accelerate.x + this->Accelerate.width)
	{
		if(y > this->Accelerate.y && y < this->Accelerate.y + this->Accelerate.height)
		{
			return true;
		}
	}
	return false;
}

bool Cheat::TouchInBrakeZone(int x, int y)
{
	if(x > this->Brake.x && x < this->Brake.x + this->Brake.width)
	{
		if(y > this->Brake.y && y < this->Brake.y + this->Brake.height)
		{
			return true;
		}
	}
	return false;
}

std::string Cheat::GetLastErrorMessage()
{
	if(last_error_message.size() > 0) {
		return last_error_message;
	} else {
		std::string ret("Ошибки не обнаружены");
		return ret;
	}
}

void Cheat::SetLastErrorMessage(std::string error_message)
{
	last_error_message = error_message;
}

uintptr_t Cheat::GetPlayerPool()
{
	if((int *)(libaddr + 0x4B624EC))
	{
		return *(int *)(libaddr + 0x4B624EC);
	}
	else
	{
		return 0;
	}
}

uintptr_t Cheat::GetVehiclePool()
{
	if((int *)(libaddr + 0x4B624F0))
	{
		return *(int *)(libaddr + 0x4B624F0);
	}
	else
	{
		return 0;
	}
}

uintptr_t Cheat::GetObjectPool()
{
	if((int *)(libaddr + 0x4B62508))
	{
		return *(int *)(libaddr + 0x4B62508);
	}
	else
	{
		return 0;
	}
}

uintptr_t Cheat::GetAtPlayerPool(int id)
{
	uintptr_t player_pool = GetPlayerPool();
	if(player_pool) {
		return ((int (*)(int,int))(libaddr + 0x368702 + 1))(player_pool, id);
	} else {
		return 0;
	}
}

uintptr_t Cheat::GetAtVehiclePool(int id)
{
	uintptr_t vehicle_pool = GetVehiclePool();
	if(vehicle_pool) {
		return ((int (*)(int,int))(libaddr + 0x374378 + 1))(vehicle_pool, id);
	} else {
		return 0;
	}
}

uintptr_t Cheat::GetAtObjectPool(int id)
{
	uintptr_t object_pool = GetObjectPool();
	if(object_pool) {
		return ((int (*)(int,int))(libaddr + 0x3681BA + 1))(object_pool, id);
	} else {
		return 0;
	}
}

uintptr_t Cheat::FindLocalPlayer()
{
	if(libaddr)
	{
		if((int *)(libaddr + 0x6F9410) != NULL)
		{
			return *(int *)(libaddr + 0x6F9410);
		}
	}
	
	return 0;
}

uintptr_t Cheat::FindNearestPlayer()
{
	uintptr_t player_pool = GetPlayerPool();
	if(player_pool) {
		int nearest_player = -1;
		float nearest_distance = std::numeric_limits<float>::max();
		for(int i = 0; i < 1501; i++)
		{
			uintptr_t player = GetAtPlayerPool(i);
			if(player) {
				player = *(int *)(player + 4);
				if(player) {
					uintptr_t localped = FindLocalPlayer();
					if(localped) {
						Vec3 local_coors = { *(float *)(localped + 0x34) , *(float *)(localped + 0x38) , *(float *)(localped + 0x3C) };
						Vec3 coors = { *(float *)(player + 0x34) , *(float *)(player + 0x38) , *(float *)(player + 0x3C) };
						float distance = sqrt(pow(local_coors.x - coors.x, 2.f) + pow(local_coors.y - coors.y, 2.f) + pow(local_coors.z - coors.z, 2.f));
						if(distance < nearest_distance) {
							nearest_distance = distance;
							nearest_player = i;
						}
					}
				}
			}
		}
		return nearest_player;
	} else {
		return -1;
	}
}

int Cheat::GetWeaponId()
{
	return *(int *)(libaddr + 0x48E6CC4);
}

void Cheat::SetRotate(uintptr_t matrix, float x, float y, float z)
{
	float cosX;
	float cosY;
	float cosZ;
	float sinX;
	float sinY;
	float sinZ;
	float v14;
	float v15;
	float v16;
		
	cosX = cosf(x);
	sinX = sinf(x);
	cosY = cosf(y);
	sinY = sinf(y);
	cosZ = cosf(z);
	sinZ = sinf(z);
	*((float *)matrix + 6) = sinX;
	//*((int *)matrix + 12) = 0;
	//*((int *)matrix + 13) = 0;
	//*((int *)matrix + 14) = 0;
	v14 = (float)(sinX * cosZ) * sinY;
	v15 = (float)(sinX * cosZ) * cosY;
	v16 = sinX * sinZ;
	*((float *)matrix + 9) = (float)(sinY * sinZ) - v15;
	*((float *)matrix + 8) = (float)(v16 * cosY) + (float)(sinY * cosZ);
	*(float *)matrix = (float)(cosY * cosZ) - (float)(v16 * sinY);
	*((float *)matrix + 1) = v14 + (float)(cosY * sinZ);
	*((float *)matrix + 5) = cosX * cosZ;
	*((float *)matrix + 2) = -(float)(cosX * sinY);
	*((float *)matrix + 4) = -(float)(sinZ * cosX);
	*((float *)matrix + 10) = cosX * cosY;
}
	
void Cheat::SetRotateZOnly(uintptr_t matrix, float z)
{
	float v4;
	float v5;
	float v6;
		
	v4 = sinf(z);
	v5 = -v4;
	*((int *)matrix + 10) = 1065353216;
	*(float *)((char*)matrix + 4) = v4;
	v6 = cosf(z);
	*(float *)matrix = v6;
	*((float *)matrix + 5) = v6;
	*((float *)matrix + 4) = v5;
}

Vec2 Cheat::GetMarkerCoors()
{
	Vec2 coors;
	
	coors.x = *(float *)(libaddr + 0x459C400);
	coors.y = *(float *)(libaddr + 0x459C404);
	
	return coors;
}

Vec3 Cheat::GetCheckpointCoors()
{
	Vec3 coors = Vec3(0,0,0);
	
	coors.x = *(float *)(libaddr + 0x7B7268);
	coors.y = *(float *)(libaddr + 0x7B726C);
	coors.z = *(float *)(libaddr + 0x7B726D);
	
	return coors;
}

Vec3 Cheat::GetCameraCoors()
{
	Vec3 coors = {0,0,0};
	
	uintptr_t cameraMatrix = GetCameraMatrix();
	
	coors.x = *(float *)(cameraMatrix + 0x34);
	coors.y = *(float *)(cameraMatrix + 0x38);
	coors.z = *(float *)(cameraMatrix + 0x3C);
	
	return coors;
}

uintptr_t Cheat::GetCameraMatrix()
{
	return libaddr + 0x825AB8;
}

Vec3 Cheat::GetPos(uintptr_t object)
{
	Vec3 coors = {0,0,0};
	
	if(!object) { return coors; }
	
	coors.x = *(float *)(object + 0x34);
	coors.y = *(float *)(object + 0x38);
	coors.z = *(float *)(object + 0x3C);
	
	return coors;
}

void Cheat::SetPos(uintptr_t object, Vec3 coors)
{
	if(!object) { return; }
	
	*(float *)(object + 0x34) = coors.x;
	*(float *)(object + 0x38) = coors.y;
	*(float *)(object + 0x3C) = coors.z;
}

void Cheat::SetPos(uintptr_t object, float x, float y, float z)
{
	if(!object) { return; }
	
	*(float *)(object + 0x34) = x;
	*(float *)(object + 0x38) = y;
	*(float *)(object + 0x3C) = z;
}

void Cheat::SetNameTagsDrawDistance(float m_fDistance)
{
	*(float *)(libaddr + 0x4B62628) = m_fDistance;
}

float Cheat::GetNameTagsDrawDistance()
{
	return *(float *)(libaddr + 0x4B62628);
}

void Cheat::SetViewRotateX(float x)
{
	*(float *)(libaddr + 0x5EAC80) = x;
}

void Cheat::SetViewRotateY(float y)
{
	*(float *)(libaddr + 0x5EAC70) = y;
}

float Cheat::GetViewRotateX()
{
	return *(float *)(libaddr + 0x5EAC80);
}

float Cheat::GetViewRotateY()
{
	return *(float *)(libaddr + 0x5EAC70);
}

/*int Cheat::GetTimeInMilliseconds()
{
	return *(int *)(libaddr + 0x43BE5CC);
}
*/
