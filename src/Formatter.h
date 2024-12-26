#ifndef FORMATTER_H
#define FORMATTER_H
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <time.h> 

class Formatter
{
	public:
		// 将time_t类型时间格式化为字符串  
		static std::string timeFormatter(time_t t);
		// 将off_t类型文件大小格式化为数字字符串 
		static std::string sizeFormatter(off_t s);
		// 提取全路径中的文件名
		static std::string toFilename(char* path); 
		// 目录中文件信息统计语句生成
		static char* makefileInfo(time_t early_time, off_t early_size, std::string early_name, time_t late_time, off_t late_size, std::string late_name, long fileNum, long fileSize); 
	protected:
};

#endif
