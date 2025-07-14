#ifndef NOTIFICATION_BLACKRUSSIA
#define NOTIFICATION_BLACKRUSSIA

#include <vector>
#include <string>
#include <jni.h>

using namespace std;

enum eBrNotificationType
{
	TYPE_MONEY_RED = 0,
	TYPE_MONEY_GREEN = 1,
	TYPE_TEXT_RED = 2,
	TYPE_TEXT_GREEN = 3,
	TYPE_BUTTON_VECTOR_ORANGE = 4,
	TYPE_BUTTON_TEXT_ORANGE = 5,
	TYPE_NEW_GUI_INTERACTIVE = 6
};

struct s_BrNotification
{
	eBrNotificationType type;
	string message;
	int duration;
};

void BrNotificationUpdate(JNIEnv* env);
void ShowBrNotification(JNIEnv* env, eBrNotificationType type, std::string msg, int duration, std::string text2 = "");
void BrNotification(eBrNotificationType type, std::string msg, int duration);

#endif
