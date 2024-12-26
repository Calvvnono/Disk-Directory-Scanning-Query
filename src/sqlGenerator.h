#ifndef SQLGENERATOR_H
#define SQLGENERATOR_H
#include <iostream>
#include <dirent.h>
#include <cstring>
#include <queue>
#include <string>
#include "Tick.h"
#include "Formatter.h"

class sqlGenerator
{
	public:
		// 生成可执行的sql文件，按40000条语句分块 
		void writeSQL(char* path);
		// 文件/目录总数
		int TOTAL; 
	private:
		// 文件表插入语句头 
		std::string istFile = "INSERT INTO files\nVALUES(";
		// 目录表插入语句头 
		std::string istDir = "INSERT INTO directories\nVALUES(";
};

#endif
