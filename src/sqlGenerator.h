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
		// ���ɿ�ִ�е�sql�ļ�����40000�����ֿ� 
		void writeSQL(char* path);
		// �ļ�/Ŀ¼����
		int TOTAL; 
	private:
		// �ļ���������ͷ 
		std::string istFile = "INSERT INTO files\nVALUES(";
		// Ŀ¼��������ͷ 
		std::string istDir = "INSERT INTO directories\nVALUES(";
};

#endif
