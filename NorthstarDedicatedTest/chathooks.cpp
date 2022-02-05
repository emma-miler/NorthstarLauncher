#include "pch.h"
#include "squirrel.h"
#include "gameutils.h"
#include "serverauthentication.h"
#include "dedicated.h"
#include "chathooks.h"
#include <Windows.h>
#include <stack>
#include "serverauthentication.h"

bool ShouldBlockMessage = false;

const char* currentMessage;
int currentPlayerId;
int currentChannelId;
void* currentSelf;

bool isProcessed;

void setProcessed(bool value) { isProcessed = value; }
bool getProcessed() { return isProcessed; }

void registerMessage(void* self, const char* message, int playerId, int channelId) 
{ 
	currentMessage = message;
	currentPlayerId = playerId;
	currentChannelId = channelId;
}

const char* getMessage() { return currentMessage; }

SQRESULT ShouldProcessClient(void* sqvm)
{
	ClientSq_pushbool(sqvm, !isProcessed);
	return SQRESULT_NOTNULL;
}

SQRESULT ShouldProcessServer(void* sqvm)
{
	ClientSq_pushbool(sqvm, !isProcessed);
	return SQRESULT_NOTNULL;
}

 SQRESULT getMessageClient(void* sqvm) { 
	 ClientSq_pushstring(sqvm, currentMessage, -1);
	 return SQRESULT_NOTNULL;
 }

 SQRESULT getMessageServer(void* sqvm)
 {
	 ServerSq_pushstring(sqvm, currentMessage, -1);
	 return SQRESULT_NOTNULL;
 }

  SQRESULT getPlayerClient(void* sqvm)
 {
	 ClientSq_pushinteger(sqvm, currentPlayerId);
	 return SQRESULT_NOTNULL;
 }

 SQRESULT getPlayerServer(void* sqvm)
 {
	 ServerSq_pushinteger(sqvm, currentPlayerId);
	 return SQRESULT_NOTNULL;
 }

  SQRESULT getChannelClient(void* sqvm)
 {
	  ClientSq_pushinteger(sqvm, currentChannelId);
	 return SQRESULT_NOTNULL;
 }

 SQRESULT getChannelServer(void* sqvm)
 {
	 ServerSq_pushinteger(sqvm, currentChannelId);
	 return SQRESULT_NOTNULL;
 }

SQRESULT sendMessageClient(void* sqvm) {
	currentMessage = ServerSq_getstring(sqvm, 1);
	currentPlayerId = ServerSq_getinteger(sqvm, 2);
	currentChannelId = ServerSq_getinteger(sqvm, 3);
	isProcessed = true;
	return SQRESULT_NOTNULL;
}

SQRESULT sendMessageServer(void* sqvm)
{
	currentMessage = ServerSq_getstring(sqvm, 1);
	currentPlayerId = ServerSq_getinteger(sqvm, 2);
	currentChannelId = ServerSq_getinteger(sqvm, 3);
	isProcessed = true;
	return SQRESULT_NOTNULL;
}


void InitialiseChathookCommandClient(HMODULE baseAddress)
{
	g_ClientSquirrelManager->AddFuncRegistration("bool", "NSShouldProcess", "", "", ShouldProcessClient);

	g_ClientSquirrelManager->AddFuncRegistration("string", "NSChatGetCurrentMessage", "", "", getMessageClient);
	g_ClientSquirrelManager->AddFuncRegistration("int", "NSChatGetCurrentPlayer", "", "", getPlayerClient);
	g_ClientSquirrelManager->AddFuncRegistration("int", "NSChatGetCurrentChannel", "", "", getChannelClient);
	
	g_ClientSquirrelManager->AddFuncRegistration("void", "NSSendMessage", "string message, int playerId, int channelId", "", sendMessageClient);

	g_UISquirrelManager->AddFuncRegistration("bool", "NSShouldProcess", "", "", ShouldProcessClient);

	g_UISquirrelManager->AddFuncRegistration("string", "NSChatGetCurrentMessage", "", "", getMessageClient);
	g_UISquirrelManager->AddFuncRegistration("int", "NSChatGetCurrentPlayer", "", "", getPlayerClient);
	g_UISquirrelManager->AddFuncRegistration("int", "NSChatGetCurrentChannel", "", "", getChannelClient);

	g_UISquirrelManager->AddFuncRegistration("void", "NSSendMessage", "string message, int playerId, int channelId", "", sendMessageClient);
}

void InitialiseChathookCommandServer(HMODULE baseAddress) {
	g_ServerSquirrelManager->AddFuncRegistration("bool", "NSShouldProcess", "", "", ShouldProcessServer);

	g_ServerSquirrelManager->AddFuncRegistration("string", "NSChatGetCurrentMessage", "", "", getMessageServer);
	g_ServerSquirrelManager->AddFuncRegistration("int", "NSChatGetCurrentPlayer", "", "", getPlayerServer);
	g_ServerSquirrelManager->AddFuncRegistration("int", "NSChatGetCurrentChannel", "", "", getChannelServer);

	g_ServerSquirrelManager->AddFuncRegistration("void", "NSSendMessage", "string message, int playerId, int channelId", "", sendMessageServer);
}