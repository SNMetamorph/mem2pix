/*
	Utils functions export
*/
#pragma once
#include <string>
#include <stdint.h>

namespace Utils
{
	bool IsDigitString(std::string &str);
	int32_t FindProcessID(const std::string &processName, size_t &processCount);
	bool GetProcessName(int32_t processID, std::string &processName);
	bool IsValidProcessID(int32_t processID);
	void Snprintf(std::string &result, const char *format, ...);
	void ReportError(const char *errorMessage);
	void Log(const char *message);
};
