#include "pch.h"
#include "scriptmonarchbrowser.h"
#include "squirrel.h"
#include "masterserver.h"
#include "gameutils.h"
#include "serverauthentication.h"
#include "dedicated.h"

// functions for viewing server browser

// void function NSRequestMonarchServerList()
SQRESULT SQ_RequestMonarchServerList(void* sqvm)
{
	g_MasterServerManager->RequestMonarchInstanceList();
	return SQRESULT_NULL;
}

// bool function NSIsRequestingMonarchServerList()
SQRESULT SQ_IsRequestingMonarchServerList(void* sqvm)
{
	ClientSq_pushbool(sqvm, g_MasterServerManager->m_scriptRequestingMonarchInstanceList);
	return SQRESULT_NOTNULL;
}

// int function NSGetMonarchServerCount()
SQRESULT SQ_GetMonarchServerCount(void* sqvm)
{
	ClientSq_pushinteger(sqvm, g_MasterServerManager->m_remoteMonarchInstances.size());
	return SQRESULT_NOTNULL;
}

// string function NSGetInstanceName( int serverIndex )
SQRESULT SQ_GetInstanceName(void* sqvm)
{
	SQInteger serverIndex = ClientSq_getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_remoteMonarchInstances.size())
	{
		ClientSq_pusherror(sqvm, fmt::format("Tried to get name of server index {} when only {} servers are available", serverIndex, g_MasterServerManager->m_remoteMonarchInstances.size()).c_str());
		return SQRESULT_ERROR;
	}

	ClientSq_pushstring(sqvm, g_MasterServerManager->m_remoteMonarchInstances[serverIndex].name, -1);
	return SQRESULT_NOTNULL;
}

// string function NSGetInstanceName( int serverIndex )
SQRESULT SQ_GetInstanceHostname(void* sqvm)
{
	SQInteger serverIndex = ClientSq_getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_remoteMonarchInstances.size())
	{
		ClientSq_pusherror(sqvm, fmt::format("Tried to get name of server index {} when only {} servers are available", serverIndex, g_MasterServerManager->m_remoteMonarchInstances.size()).c_str());
		return SQRESULT_ERROR;
	}

	ClientSq_pushstring(sqvm, g_MasterServerManager->m_remoteMonarchInstances[serverIndex].hostname, -1);
	return SQRESULT_NOTNULL;
}

// int function NSGetInstancePort( int serverIndex )
SQRESULT SQ_GetInstancePort(void* sqvm)
{
	SQInteger serverIndex = ClientSq_getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_remoteMonarchInstances.size())
	{
		ClientSq_pusherror(sqvm, fmt::format("Tried to get port of server index {} when only {} servers are available", serverIndex, g_MasterServerManager->m_remoteMonarchInstances.size()).c_str());
		return SQRESULT_ERROR;
	}

	ClientSq_pushstring(sqvm, g_MasterServerManager->m_remoteMonarchInstances[serverIndex].port, -1);
	return SQRESULT_NOTNULL;
}

// string function NSGetÌnstanceID( int serverIndex )
SQRESULT SQ_GetInstanceId(void* sqvm)
{
	SQInteger serverIndex = ClientSq_getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_remoteMonarchInstances.size())
	{
		ClientSq_pusherror(sqvm, fmt::format("Tried to get id of server index {} when only {} servers are available", serverIndex, g_MasterServerManager->m_remoteMonarchInstances.size()).c_str());
		return SQRESULT_ERROR;
	}

	ClientSq_pushstring(sqvm, g_MasterServerManager->m_remoteMonarchInstances[serverIndex].id, -1);
	return SQRESULT_NOTNULL;
}

// bool function NSGetInstanceSecure( int serverIndex )
SQRESULT SQ_GetInstanceSecure(void* sqvm)
{
	SQInteger serverIndex = ClientSq_getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_remoteMonarchInstances.size())
	{
		ClientSq_pusherror(sqvm, fmt::format("Tried to get id of server index {} when only {} servers are available", serverIndex, g_MasterServerManager->m_remoteMonarchInstances.size()).c_str());
		return SQRESULT_ERROR;
	}

	ClientSq_pushbool(sqvm, g_MasterServerManager->m_remoteMonarchInstances[serverIndex].secure);
	return SQRESULT_NOTNULL;
}

// void function NSClearRecievedServerList()
SQRESULT SQ_ClearRecievedInstanceList(void* sqvm)
{
	g_MasterServerManager->ClearInstanceList();
	return SQRESULT_NULL;
}

void InitialiseScriptMonarchBrowser(HMODULE baseAddress)
{
	if (IsDedicated())
		return;

	g_UISquirrelManager->AddFuncRegistration("void", "NSRequestMonarchServerList", "", "", SQ_RequestMonarchServerList);
	g_UISquirrelManager->AddFuncRegistration("bool", "NSIsRequestingMonarchServerList", "", "", SQ_IsRequestingMonarchServerList);
	g_UISquirrelManager->AddFuncRegistration("int", "NSGetMonarchServerCount", "", "", SQ_GetMonarchServerCount);

	g_UISquirrelManager->AddFuncRegistration("void", "NSClearRecievedInstanceList", "", "", SQ_ClearRecievedInstanceList);

	g_UISquirrelManager->AddFuncRegistration("string", "NSGetInstanceName", "int serverIndex", "", SQ_GetInstanceName);
	g_UISquirrelManager->AddFuncRegistration("string", "NSGetInstanceHostname", "int serverIndex", "", SQ_GetInstanceHostname);
	g_UISquirrelManager->AddFuncRegistration("string", "NSGetInstancePort", "int serverIndex", "", SQ_GetInstancePort);
	g_UISquirrelManager->AddFuncRegistration("bool", "NSGetInstanceSecure", "int serverIndex", "", SQ_GetInstanceSecure);
	g_UISquirrelManager->AddFuncRegistration("string", "NSGetInstanceId", "int serverIndex", "", SQ_GetInstanceId);
}