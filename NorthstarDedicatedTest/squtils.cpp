#pragma once
#include "pch.h"
#include "squirrel.h"
#include <string>

std::string toLower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
	return s;
}

void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace, bool doAll, bool caseInsensitive)
{
	size_t pos = 0;
	bool loopedOnce = false;
	std::string searchString = doAll ? toLower(subject) : subject;
	while ((pos = searchString.find(search, pos)) != std::string::npos && !(loopedOnce && doAll))
	{
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
		loopedOnce = true;
	}
}

SQRESULT S_SQReplace(void* sqvm)
{
	std::string sourceString = ServerSq_getstring(sqvm, 1);
	std::string search = ServerSq_getstring(sqvm, 2);
	std::string replacement = ServerSq_getstring(sqvm, 3);
	bool replaceAll = ServerSq_getbool(sqvm, 4);
	bool caseInsensitive = ServerSq_getbool(sqvm, 5);
	ReplaceStringInPlace(sourceString, search, replacement, replaceAll, caseInsensitive);
	ServerSq_pushstring(sqvm, sourceString.c_str(), -1);
	return SQRESULT_NOTNULL;
}

SQRESULT C_SQReplace(void* sqvm)
{
	std::string sourceString = ClientSq_getstring(sqvm, 1);
	std::string search = ClientSq_getstring(sqvm, 2);
	std::string replacement = ClientSq_getstring(sqvm, 3);
	bool replaceAll = ClientSq_getbool(sqvm, 4);
	bool caseInsensitive = ClientSq_getbool(sqvm, 5);
	ReplaceStringInPlace(sourceString, search, replacement, replaceAll, caseInsensitive);
	ServerSq_pushstring(sqvm, sourceString.c_str(), -1);
	return SQRESULT_NOTNULL;
}

void InitialiseSquirrelUtilsServer(HMODULE baseAddress) {
	g_ServerSquirrelManager->AddFuncRegistration("string", "StringReplace", "string source, string search, string replacement, bool replaceAll, bool caseInsensitive", "", S_SQReplace);
}

void InitialiseSquirrelUtilsClient(HMODULE baseAddress)
{
	g_UISquirrelManager->AddFuncRegistration("string", "StringReplace", "string source, string search, string replacement, bool replaceAll, bool caseInsensitive", "", C_SQReplace);
}