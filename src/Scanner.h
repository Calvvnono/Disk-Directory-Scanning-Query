#ifndef SCANNER_H
#define SCANNER_H
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include <queue>
#include <string>
#include <unordered_map>
#include "tick.h"

class Scanner
{
	public:
		// C:\Windows扫描函数，采用队列辅助的非递归层序遍历 
		char* scanDiskC(char *path, int& dirCount, int& fileCount, char* maxStr, int& maxPathLength, int& maxLayer);
	private:
};

#endif
