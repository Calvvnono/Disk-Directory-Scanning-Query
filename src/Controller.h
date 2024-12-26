#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <iostream>
#include "Scanner.h"
#include "sqlGenerator.h"
#include "DirTree.h"

class Controller
{
	public:
		// 用户交互的主控制函数 
		static void start();
		// 实现扫描C盘功能 
		static void scenarioOne();
		// 实现生成可执行的sql文件 
		static void scenarioTwo();
		// 实现目录树的重构建（孩子兄弟树） 
		static void scenarioThree(); 
		// 实现对mystat.txt中目录文件信息的统计 
		static void scenarioFour(); 
		// 实现对myfile.txt/mydir.txt中的文件/目录修改 
		static void scenarioFive(); 
	private:
};

#endif
