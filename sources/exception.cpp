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

#include <cstdio>
#include <cstring>
#include "exception.h"

CException::CException(
    const char *description, 
    const char *funcName, 
    const char *sourceFilePath, 
    int lineNumber
)
{
	m_szFuncName = funcName;
	m_szFilePath = sourceFilePath;
	m_szDescription = description;
	m_iLineNumber = lineNumber;
    m_szMessageBuffer[0] = '\0';
}

const char *CException::GetFormattedMessage()
{
	snprintf(
		m_szMessageBuffer, 
		sizeof(m_szMessageBuffer), 
		"%s() [%s:%d]: %s\n", 
		m_szFuncName, 
		GetFileName(), 
		m_iLineNumber, 
		m_szDescription
	);
	return m_szMessageBuffer;
}

const char *CException::GetDescription() const
{
	return m_szDescription;
}

const char *CException::GetFunctionName() const
{
	return m_szFuncName;
}

const char *CException::GetFileName() const
{
	return strrchr(m_szFilePath, '\\') + 1;
}

int CException::GetLineNumber() const
{
	return m_iLineNumber;
}
