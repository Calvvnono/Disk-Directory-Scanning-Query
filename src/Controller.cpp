#include "Controller.h"
int totalNum = 0;
DirTree::node* root;

void Controller::start() {
	std::cout << "欢迎进入磁盘扫描模拟系统!\n\n";
	std::cout << "请按照需求选择您要执行的功能：\n\n"; 
	std::cout << "0.退出系统   1.扫描c:\\windows中的子目录和文件 2.生成插入数据库的SQL文件\n3.重构目录树 4.统计指定的文件信息             5.模拟子目录/文件操作\n\n";
	while(1) {
		std::cout << "------------------------------------------------------------------------\n";
		char ins;
		std::cin >> ins;
		switch(ins) {
			case '0':
				std::cout << "感谢您本次使用!\n";
				return;
			case '1':
				Controller::scenarioOne();
				break;
			case '2':
				Controller::scenarioTwo();
				break;
			case '3':
				Controller::scenarioThree();
				break;
			case '4':
				Controller::scenarioFour();
				break;
			case '5':
				Controller::scenarioFive();
				return;
			default:
				std::cout << "无效指令！\n";
		}
		std::cout << "------------------------------------------------------------------------\n\n";
	}
}

void Controller::scenarioOne() {
	Scanner sc = Scanner();
	char* path = "C:\\Windows";
	char* maxStr;
	int dirCount = 0, fileCount = 0, maxPathLength = 0, maxLayer = 1;
	
	maxStr = sc.scanDiskC(path, dirCount, fileCount, maxStr, maxPathLength, maxLayer);
	totalNum = dirCount + fileCount;
	
	std::cout << "\n···C:\\Windows统计结果如下···" << std::endl;
	std::cout << "···子目录数量···" << dirCount << std::endl;
	std::cout << "···文件总数量···" << fileCount << std::endl;
	std::cout << "···目录层数···" << maxLayer << std::endl;
	std::cout << "···最长的带全路径的文件名··· " << maxStr << std::endl;
	std::cout << "···最长的带全路径的文件名及长度··· " << maxPathLength << std::endl;
}

void Controller::scenarioTwo() {
	sqlGenerator sg = sqlGenerator();
	sg.TOTAL = totalNum;
	char* path = "C:\\Windows";
	
	sg.writeSQL(path);
	std::cout << "可执行sql文件已经全部生成，请查看并在数据库中执行！\n";
}

void Controller::scenarioThree() {
	DirTree tree = DirTree();
	tree.TOTAL = totalNum;
	char* path = "C:\\Windows";
	int maxLayer;
	
	root = tree.buildTree(path);
	maxLayer = tree.countDepthOfTree(root);
	std::cout << "目录树深度为：" << maxLayer << std::endl;
	std::cout << "目录树已在内存构建，请进行下一步操作！\n";
}

void Controller::scenarioFour() {
	DirTree tree = DirTree();
	tree.showInfoOfFiles(root, "outcome1.txt");
}

void Controller::scenarioFive() {
	std::cout << "本系统主要功能已运行完毕，请选择您要继续进行的操作：\n"; 
	std::cout << "F.模拟文件操作   D.模拟目录操作\n";
	bool flag = false;
	DirTree tree = DirTree();
	tree.TOTAL = totalNum;
	while(1) {
		char ins;
		std::cin >> ins;
		switch(ins) {
			case 'F':
				flag = true;
				tree.modifyFiles(root);
				std::cout << "\n重新扫描mystat中的目录，并分析发生的变化?(y/n)\n";
				char tmp1;	std::cin >> tmp1;
				if(tmp1 == 'y') {
					tree.showInfoOfFiles(root, "outcome2.txt");
					tree.showModifiedInfo("outcome1.txt", "outcome2.txt", "differ1_2.txt"); 
					std::cout << "mystat中目录文件信息变化统计已写入differ1_2.txt，请查看！\n\n";	
				} else if(tmp1 == 'n') {
					std::cout << "感谢您本次使用！";
					return;
				}
				std::cout << "------------------------------------------------------------------------\n";
				break;
			case 'D':
				if(flag) {
					std::cout << "正在将目录树还原为初始状态，请稍候……\n";
					root = tree.buildTree("C:\\Windows");
				}
				tree.deleteDirs(root);
				std::cout << "\n重新扫描mystat中的目录，并分析发生的变化?(y/n)\n";
				char tmp2;	std::cin >> tmp2;
				if(tmp2 == 'y') {
					tree.showInfoOfFiles(root, "outcome3.txt");
					tree.showModifiedInfo("outcome1.txt", "outcome3.txt", "differ1_3.txt"); 
					std::cout << "mystat中目录文件信息变化统计已写入differ1_3.txt，请查看！\n\n";	
				} else if(tmp2 == 'n') {
					std::cout << "感谢您本次使用！";
					return;
				}
				break;
			default:
				std::cout << "感谢您本次使用！";
				return;
		}
	}
}
