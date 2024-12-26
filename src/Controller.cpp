#include "Controller.h"
int totalNum = 0;
DirTree::node* root;

void Controller::start() {
	std::cout << "��ӭ�������ɨ��ģ��ϵͳ!\n\n";
	std::cout << "�밴������ѡ����Ҫִ�еĹ��ܣ�\n\n"; 
	std::cout << "0.�˳�ϵͳ   1.ɨ��c:\\windows�е���Ŀ¼���ļ� 2.���ɲ������ݿ��SQL�ļ�\n3.�ع�Ŀ¼�� 4.ͳ��ָ�����ļ���Ϣ             5.ģ����Ŀ¼/�ļ�����\n\n";
	while(1) {
		std::cout << "------------------------------------------------------------------------\n";
		char ins;
		std::cin >> ins;
		switch(ins) {
			case '0':
				std::cout << "��л������ʹ��!\n";
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
				std::cout << "��Чָ�\n";
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
	
	std::cout << "\n������C:\\Windowsͳ�ƽ�����¡�����" << std::endl;
	std::cout << "��������Ŀ¼����������" << dirCount << std::endl;
	std::cout << "�������ļ�������������" << fileCount << std::endl;
	std::cout << "������Ŀ¼����������" << maxLayer << std::endl;
	std::cout << "��������Ĵ�ȫ·�����ļ��������� " << maxStr << std::endl;
	std::cout << "��������Ĵ�ȫ·�����ļ��������ȡ����� " << maxPathLength << std::endl;
}

void Controller::scenarioTwo() {
	sqlGenerator sg = sqlGenerator();
	sg.TOTAL = totalNum;
	char* path = "C:\\Windows";
	
	sg.writeSQL(path);
	std::cout << "��ִ��sql�ļ��Ѿ�ȫ�����ɣ���鿴�������ݿ���ִ�У�\n";
}

void Controller::scenarioThree() {
	DirTree tree = DirTree();
	tree.TOTAL = totalNum;
	char* path = "C:\\Windows";
	int maxLayer;
	
	root = tree.buildTree(path);
	maxLayer = tree.countDepthOfTree(root);
	std::cout << "Ŀ¼�����Ϊ��" << maxLayer << std::endl;
	std::cout << "Ŀ¼�������ڴ湹�����������һ��������\n";
}

void Controller::scenarioFour() {
	DirTree tree = DirTree();
	tree.showInfoOfFiles(root, "outcome1.txt");
}

void Controller::scenarioFive() {
	std::cout << "��ϵͳ��Ҫ������������ϣ���ѡ����Ҫ�������еĲ�����\n"; 
	std::cout << "F.ģ���ļ�����   D.ģ��Ŀ¼����\n";
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
				std::cout << "\n����ɨ��mystat�е�Ŀ¼�������������ı仯?(y/n)\n";
				char tmp1;	std::cin >> tmp1;
				if(tmp1 == 'y') {
					tree.showInfoOfFiles(root, "outcome2.txt");
					tree.showModifiedInfo("outcome1.txt", "outcome2.txt", "differ1_2.txt"); 
					std::cout << "mystat��Ŀ¼�ļ���Ϣ�仯ͳ����д��differ1_2.txt����鿴��\n\n";	
				} else if(tmp1 == 'n') {
					std::cout << "��л������ʹ�ã�";
					return;
				}
				std::cout << "------------------------------------------------------------------------\n";
				break;
			case 'D':
				if(flag) {
					std::cout << "���ڽ�Ŀ¼����ԭΪ��ʼ״̬�����Ժ򡭡�\n";
					root = tree.buildTree("C:\\Windows");
				}
				tree.deleteDirs(root);
				std::cout << "\n����ɨ��mystat�е�Ŀ¼�������������ı仯?(y/n)\n";
				char tmp2;	std::cin >> tmp2;
				if(tmp2 == 'y') {
					tree.showInfoOfFiles(root, "outcome3.txt");
					tree.showModifiedInfo("outcome1.txt", "outcome3.txt", "differ1_3.txt"); 
					std::cout << "mystat��Ŀ¼�ļ���Ϣ�仯ͳ����д��differ1_3.txt����鿴��\n\n";	
				} else if(tmp2 == 'n') {
					std::cout << "��л������ʹ�ã�";
					return;
				}
				break;
			default:
				std::cout << "��л������ʹ�ã�";
				return;
		}
	}
}
