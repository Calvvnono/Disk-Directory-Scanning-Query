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
		// 文件/目录总数
		int TOTAL; 		
		// 孩子兄弟树结点 
		typedef struct treeNode{
			char* pathName; // 完整的路径名 
			int type;       // 0表文件，1表目录
			struct treeNode* son;
			struct treeNode* bro;
			// 如果是文件
			time_t fileTime;
			off_t fileSize;
		}node;
		
		// 非递归层序遍历构建目录树 
		node* buildTree(char* path);   
		// 负责将一组数据封装成treeNode结点 
		node* setNode(int type);
		// 计算孩子/兄弟树的深度
		int countDepthOfTree(node* root); 
		// 辅助验证函数 
		void go(node* r);
		// 指定目录文件信息的统计
		void showInfoOfFiles(node* root, char* outcome);
		// 由路径查询目录树中的结点
		node* findNode(node* root, char* path); 
		// 修改指定文件信息
		void modifyFiles(node* root);
		// 验证文件/目录模拟操作的成功性
		void verifyChanges(node* root, char* pathin, char* pathout); 
		// 显示两次文件统计结果的变化
		void showModifiedInfo(char* f1, char* f2, char* fout);
		// 删除指定目录
		void deleteDirs(node* root); 
	 
	private:
		// 标志文件类型 
		const int TYPEREG = 0;
		// 标志目录类型 
		const int TYPEDIR = 1;
};

#endif
