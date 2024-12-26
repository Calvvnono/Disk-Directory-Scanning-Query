#ifndef FORMATTER_H
#define FORMATTER_H
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <time.h> 

class Formatter
{
	public:
		// ��time_t����ʱ���ʽ��Ϊ�ַ���  
		static std::string timeFormatter(time_t t);
		// ��off_t�����ļ���С��ʽ��Ϊ�����ַ��� 
		static std::string sizeFormatter(off_t s);
		// ��ȡȫ·���е��ļ���
		static std::string toFilename(char* path); 
		// Ŀ¼���ļ���Ϣͳ���������
		static char* makefileInfo(time_t early_time, off_t early_size, std::string early_name, time_t late_time, off_t late_size, std::string late_name, long fileNum, long fileSize); 
	protected:
};

#endif
