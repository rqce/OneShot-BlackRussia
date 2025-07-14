#include "BRNotification.h"
#include "security/obfuscate.h"

std::vector<s_BrNotification> BrNotificationPool;

void BrNotificationUpdate(JNIEnv* env)
{
	if(!BrNotificationPool.empty()) {
		s_BrNotification next_notification = BrNotificationPool.front();
		ShowBrNotification(env, next_notification.type, next_notification.message.c_str(), next_notification.duration);
		BrNotificationPool.erase(BrNotificationPool.begin());
	}
}

void ShowBrNotification(JNIEnv* env, eBrNotificationType type, std::string msg, int duration, std::string text2)
{
	string jsonString = "{ \"t\":" + to_string(type) + ", \"i\":\"" + msg + "\", \"d\":" + to_string(duration) + ", \"s\":1, \"b\":1, \"k\":\"" + text2 + "\" }";
    const char* jsonChars = jsonString.c_str();

    // Поиск класса BrNotification
    jclass brNotificationClass = env->FindClass(OBFUSCATE("com/blackhub/bronline/game/gui/notification/BrNotification"));

    // Создание экземпляра BrNotification
    if (brNotificationClass != NULL) {
        jmethodID constructor = env->GetMethodID(brNotificationClass, OBFUSCATE("<init>"), OBFUSCATE("()V"));
        if (constructor) {
            jobject brNotificationObj = env->NewObject(brNotificationClass, constructor);

            jclass jsonObjectClass = env->FindClass(OBFUSCATE("org/json/JSONObject"));
            if (jsonObjectClass != NULL) {
                jmethodID jsonObjectConstructor = env->GetMethodID(jsonObjectClass, OBFUSCATE("<init>"), OBFUSCATE("(Ljava/lang/String;)V"));
                if (jsonObjectConstructor) {
                    jstring jsonStr = env->NewStringUTF(jsonChars);
                    jobject jsonObject = env->NewObject(jsonObjectClass, jsonObjectConstructor, jsonStr);

                    // получите класс guiManager
                    jclass GUIManagerClass = env->FindClass(OBFUSCATE("com/blackhub/bronline/game/GUIManager"));
                    if (GUIManagerClass != NULL) {
                        // получите метод myStaticMethod
                        jmethodID getInstance = env->GetStaticMethodID(GUIManagerClass, OBFUSCATE("getInstance"), OBFUSCATE("()Lcom/blackhub/bronline/game/GUIManager;"));
                        if (getInstance) {
                            // вызовите статический метод и получите результат
                            jobject gUIManagerObj = env->CallStaticObjectMethod(GUIManagerClass, getInstance);

                            // получите класс guiManager
                            jclass JNIActivityClass = env->FindClass(OBFUSCATE("com/blackhub/bronline/game/core/JNIActivity"));
                            if (JNIActivityClass != NULL) {
                                // получите метод myStaticMethod
                                jmethodID getContext = env->GetStaticMethodID(JNIActivityClass, OBFUSCATE("getContext"), OBFUSCATE("()Lcom/blackhub/bronline/game/core/JNIActivity;"));
                                if (getContext) {
                                    // вызовите статический метод и получите результат
                                    jobject instance = env->CallStaticObjectMethod(JNIActivityClass, getContext);

                                    // Вызов метода show
                                    jmethodID showMethod = env->GetMethodID(brNotificationClass, OBFUSCATE("show"), OBFUSCATE("(Lorg/json/JSONObject;Lcom/blackhub/bronline/game/GUIManager;Lcom/blackhub/bronline/game/core/JNIActivity;)V"));
                                    if (showMethod) {
                                        env->CallVoidMethod(brNotificationObj, showMethod, jsonObject, gUIManagerObj, instance);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void BrNotification(eBrNotificationType type, std::string msg, int duration)
{
	s_BrNotification notif;
	notif.type = type;
	notif.message = msg;
	notif.duration = duration;
	BrNotificationPool.push_back(notif);
}
