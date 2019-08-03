/*
	Utils functions export
*/
#pragma once
#include <string>
#include <stdint.h>


bool IsDigitString(std::string& str);
int32_t FindProcessID(std::string& processName, size_t& processCount);
bool IsValidProcessID(int32_t processID);
void ReportError(const char *errorMessage);
void Log(const char *message);