#ifndef DIRTREE_H
#define DIRTREE_H
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#include <queue>
#include <string>
#include <unordered_map>
#include <limits.h>
#include "Tick.h"
#include "Formatter.h"

class DirTree
{
	public:
		// �ļ�/Ŀ¼����
		int TOTAL; 		
		// �����ֵ������ 
		typedef struct treeNode{
			char* pathName; // ������·���� 
			int type;       // 0���ļ���1��Ŀ¼
			struct treeNode* son;
			struct treeNode* bro;
			// ������ļ�
			time_t fileTime;
			off_t fileSize;
		}node;
		
		// �ǵݹ�����������Ŀ¼�� 
		node* buildTree(char* path);   
		// ����һ�����ݷ�װ��treeNode��� 
		node* setNode(int type);
		// ���㺢��/�ֵ��������
		int countDepthOfTree(node* root); 
		// ������֤���� 
		void go(node* r);
		// ָ��Ŀ¼�ļ���Ϣ��ͳ��
		void showInfoOfFiles(node* root, char* outcome);
		// ��·����ѯĿ¼���еĽ��
		node* findNode(node* root, char* path); 
		// �޸�ָ���ļ���Ϣ
		void modifyFiles(node* root);
		// ��֤�ļ�/Ŀ¼ģ������ĳɹ���
		void verifyChanges(node* root, char* pathin, char* pathout); 
		// ��ʾ�����ļ�ͳ�ƽ���ı仯
		void showModifiedInfo(char* f1, char* f2, char* fout);
		// ɾ��ָ��Ŀ¼
		void deleteDirs(node* root); 
	 
	private:
		// ��־�ļ����� 
		const int TYPEREG = 0;
		// ��־Ŀ¼���� 
		const int TYPEDIR = 1;
};

#endif
