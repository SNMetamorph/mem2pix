#pragma once

// Universal macros
#define EXCEPT(message) (throw CException(message, __FUNCTION__, __FILE__, __LINE__))

class CException
{
public:
	CException(const char *_desc, const char *_funcName, const char *_filePath, int _line);
    ~CException() {};

	const char *getFormattedMessage();
	const char *getDescription() const;
	const char *getFunctionName() const;
	const char *getFileName() const;
	int			getLineNumber() const;

	char m_szMessageBuffer[256];

private:
	int m_iLine;
	const char *m_szDescription;
	const char *m_szFuncName;
	const char *m_szFilePath;
};

