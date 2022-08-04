/*
Copyright (C) 2019 SNMetamorph

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
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
