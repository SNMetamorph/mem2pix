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

// Universal macros
#define EXCEPT(message) (throw CException(message, __FUNCTION__, __FILE__, __LINE__))

class CException
{
public:
	CException(
        const char *description, 
        const char *funcName, 
        const char *sourceFilePath, 
        int lineNumber
    );
    ~CException() {};

	const char *GetFormattedMessage();
	const char *GetDescription() const;
	const char *GetFunctionName() const;
	const char *GetFileName() const;
	int			GetLineNumber() const;

	char m_szMessageBuffer[256];

private:
	int m_iLineNumber;
	const char *m_szDescription;
	const char *m_szFuncName;
	const char *m_szFilePath;
};

