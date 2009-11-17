#pragma warning(disable:4996)
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "dprintf.h"

void PrintToDbg(const TCHAR *format,...)
{
	TCHAR statement_buffer[1024]={0,};

	va_list args;
	va_start(args,format);
	_vsntprintf(statement_buffer,sizeof(statement_buffer)/sizeof(TCHAR),format,args);
	va_end(args);
	OutputDebugString(statement_buffer);
}

void PrintToStdOutWithTime(const TCHAR *format,...)
{
	TCHAR statement_buffer[1024]={0,};

	va_list args;
	va_start(args,format);
	_vsntprintf(statement_buffer,sizeof(statement_buffer)/sizeof(TCHAR),format,args);
	va_end(args);

	SYSTEMTIME lt;
	GetLocalTime(&lt);

	printf("[%02d:%02d:%02d] %s",lt.wHour,lt.wMinute,lt.wSecond,statement_buffer);
}

HANDLE OpenLogFile(char *szTempName)
{
#define BUFSIZE 512
	
	DWORD dwBufSize=BUFSIZE;

	HANDLE hFile;
	// Create the new file to write the upper-case version to.
	hFile=CreateFile((LPTSTR)szTempName,// file name 
			GENERIC_READ | GENERIC_WRITE,// open r-w 
			FILE_SHARE_READ,
			NULL,				// default security 
			OPEN_ALWAYS,		// overwrite existing
			FILE_ATTRIBUTE_NORMAL,// normal file 
			NULL);				// no template 
	if(hFile==INVALID_HANDLE_VALUE)
	{ 
		printf("CreateFile failed with error %u.\n",GetLastError());
	}
	SetFilePointer(hFile,0,0,FILE_END);
	return hFile;
}

void WriteToLogFile(HANDLE hFile,const char *format,...)
{
	va_list args;
	va_start(args,format);
	char Contents[1024]={0,};
	_vsnprintf(Contents,sizeof(Contents)/sizeof(char),format,args);
	va_end(args);

	if(hFile!=INVALID_HANDLE_VALUE) 
	{

		DWORD dwBytesWritten;
		BOOL fSuccess=WriteFile(hFile,
			Contents,
			strlen(Contents),
			&dwBytesWritten,
			NULL); 
		if(!fSuccess) 
		{
			printf("WriteFile failed with error %u.\n",GetLastError());
		}
	}else
	{
#ifdef IDA_PLUGIN
		msg("%s",Contents);
#else
		OutputDebugString(Contents);
#endif
	}
}

void CloseLogFile(HANDLE hFile)
{
	CloseHandle(hFile);
}