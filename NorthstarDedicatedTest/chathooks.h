#pragma once
#include <minwindef.h>

void setProcessed(bool value);
bool getProcessed();

const char* getMessage();

void registerMessage(void* self, const char* message, int playerId, int channelId);
void InitialiseChathookCommandClient(HMODULE baseAddress);
void InitialiseChathookCommandServer(HMODULE baseAddress);