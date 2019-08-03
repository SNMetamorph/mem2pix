#pragma once

// Universal macros
#define EXCEPT(m) (throw CException(m, __FUNCTION__, __FILE__, __LINE__))

class CException
{
public:
	CException(const char *_desc, const char *_funcName, const char *_filePath, int _line);
    ~CException() {};
	const char *getFormattedMessage();
	const char *getDescription();
	const char *getFunctionName();
	const char *getFileName();
	int			getLineNumber();
	char m_szMessageBuffer[256];

private:
	int m_iLine;
	const char *m_szDescription;
	const char *m_szFuncName;
	const char *m_szFilePath;
};

