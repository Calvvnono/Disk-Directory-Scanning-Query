#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <iostream>
#include "Scanner.h"
#include "sqlGenerator.h"
#include "DirTree.h"

class Controller
{
	public:
		// �û������������ƺ��� 
		static void start();
		// ʵ��ɨ��C�̹��� 
		static void scenarioOne();
		// ʵ�����ɿ�ִ�е�sql�ļ� 
		static void scenarioTwo();
		// ʵ��Ŀ¼�����ع����������ֵ����� 
		static void scenarioThree(); 
		// ʵ�ֶ�mystat.txt��Ŀ¼�ļ���Ϣ��ͳ�� 
		static void scenarioFour(); 
		// ʵ�ֶ�myfile.txt/mydir.txt�е��ļ�/Ŀ¼�޸� 
		static void scenarioFive(); 
	private:
};

#endif
