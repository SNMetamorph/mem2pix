#include <cstdio>
#include <cstring>
#include "exception.h"

CException::CException(const char *_desc, const char *_funcName, const char *_fileName, int _line)
{
	m_szFuncName = _funcName;
	m_szFilePath = _fileName;
	m_szDescription = _desc;
	m_iLine = _line;
    m_szMessageBuffer[0] = '\0';
}

const char *CException::getFormattedMessage()
{
	snprintf(
		m_szMessageBuffer, 
		sizeof(m_szMessageBuffer), 
		"%s() [%s:%d]: %s\n", 
		m_szFuncName, 
		getFileName(), 
		m_iLine, 
		m_szDescription
	);
	return m_szMessageBuffer;
}

const char *CException::getDescription()
{
	return m_szDescription;
}

const char *CException::getFunctionName()
{
	return m_szFuncName;
}

const char *CException::getFileName()
{
	return strrchr(m_szFilePath, '\\') + 1;
}

int CException::getLineNumber()
{
	return m_iLine;
}
