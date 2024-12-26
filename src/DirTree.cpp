#include "DirTree.h"

DirTree::node* DirTree::buildTree(char* path) {
	DIR* dir;
    struct dirent* entry;
    std::queue<node*> qd;

    struct stat info;
	clock_t start,end;
	start = clock(); 
	int percentage = 0, round = 0;
	
	// 设置根节点 
	node* root = new node();  
    root->pathName = path;
    root->type = TYPEDIR;
    root->bro = NULL;
    qd.push(root);

    while (!qd.empty()) {
    	round = 0;
    	node* now = qd.front(); 
    	node* last = now;
		node* next = (node*)malloc(sizeof(node));
    	qd.pop();

		DIR* dir = opendir(now->pathName);
		struct dirent* entry;
	
		while ((entry = readdir(dir)) != NULL) {
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { continue; }
			
			char* newPath = (char*)malloc(1024);
        	snprintf(newPath, 1024, "%s\\%s", now->pathName, entry->d_name);
	    	stat(newPath,&info); 
	    	
			if (S_ISDIR(info.st_mode)) { 
		        next = setNode(TYPEDIR); 
		        next->pathName = newPath;   
		
				if(round == 0)	last->son = next;
				else			last->bro = next;
				
		        qd.push(next);
		        percentage ++; 
		    } else if (S_ISREG(info.st_mode)) {
		    	next = setNode(TYPEREG);
		    	next->pathName = newPath;
		    	// 设置文件信息 
		    	next->fileTime = info.st_mtime;	
				next->fileSize = info.st_size;
				
		    	if(round == 0)	last->son = next;
				else			last->bro = next;
	    
		        percentage ++;
	    	}
	    	last = next;
	    	round ++;
	    }
	    // 实时进度显示功能 
	    Tick::tick(percentage, TOTAL);
    }
    end = clock();
    std::cout << "\n\n目录树构建已完成，共用时" << end-start << "毫秒" << std::endl;
	//go(root);
    return root;
}

DirTree::node* DirTree::setNode(int type) {
	node* d = (node*)malloc(sizeof(node));
	d->type = type;
	d->son = NULL;
	d->bro = NULL;
	return d;
}

int DirTree::countDepthOfTree(node* root) {
	if (!root) { return 0; }
 
    std::queue<node*> q;
    q.push(root);
    int depth = 0;
   
    while (!q.empty()) {
        int size = q.size();
        // 处理当前层级的所有节点
        for (int i = 0; i < size; ++i) {
            node* current = q.front();
            q.pop();

            // 将当前节点的子、兄节点加入队列
            if (current->son) {
                q.push(current->son);
            }
            if (current->bro) {
                q.push(current->bro);
            }
        }
        // 每处理一层，深度加一
        depth ++;
    }
    return depth;
}

void DirTree::go(node* r) {
	int cnt = 0;
	std::cout << r->pathName << std::endl;
	node* n = r->son;
	while(n!=NULL && cnt < 200){
		cnt ++; 
		if(n->type == TYPEREG)
			std::cout << n->pathName << " " << n->fileTime << " " << n->fileSize << std::endl;
		else if(n->type == TYPEDIR)
			std::cout << n->pathName << std::endl;
		n = n->bro;
	}
	std::cout << cnt << std::endl;
}

void DirTree::showInfoOfFiles(node* root, char* outcome) {
	char s[301];
	int percentage = 0, trash = 0;
	bool flag = false;
	FILE *fpin, *fpout;
	if((fpin = fopen("mystat.txt","r"))==NULL || (fpout = fopen(outcome,"w")) == NULL){
		printf("文件读取失败！");
		exit(0);
	}
	
	char* head = "以下是指定目录中的文件信息，格式为：最早时间的文件名(大小 时间)---最晚时间的文件名(大小 时间)---文件总数---总的文件大小\n\n";
	fwrite(head, 1, strlen(head), fpout);
	
	time_t early_time = LONG_MAX, late_time = 0;
	off_t early_size, late_size;
	std::string early_name, late_name;
	char* final;
	long fileNum = 0, fileSize = 0;
	
	clock_t start,end;
	start = clock(); 
    while(fgets(s, 300, fpin)!=NULL){
    	if(strcmp(s, "stat dirs\n") == 0 || strcmp(s, "end of dirs\n") == 0)	continue; 
    
   		fwrite(s, 1, strlen(s), fpout);
   		node* Node = findNode(root, s);
   		if(Node == NULL) {
   			final = "\t未找到该目录或已被删除！\n";
   			trash ++;
		} else if(Node->son == NULL) {
   			final = "\t该目录为空！\n";
		} else {
			node* n = Node->son;
	   		while(n != NULL) {
	   			if(n->type == TYPEDIR) {
	   				n = n->bro;
					continue;	
				}	
	   			if(n->fileTime < early_time) {
	   				early_time = n->fileTime;
					early_size = n->fileSize;
					early_name = Formatter::toFilename(n->pathName);
				}
				if(n->fileTime > late_time) {
					late_time = n->fileTime;
					late_size = n->fileSize;
					late_name = Formatter::toFilename(n->pathName);
				}
				fileNum ++;
				fileSize += n->fileSize;
				n = n->bro;
			}
			if(fileNum != 0)	final = Formatter::makefileInfo(early_time, early_size, early_name, late_time, late_size, late_name, fileNum, fileSize);
			else 				final = "\t该目录下不包含任何文件！\n"; 
		}
		fwrite(final, 1, strlen(final), fpout);
		// 实时进度显示功能 
		percentage ++;
	    Tick::tick(percentage, -1);
    }
    end = clock();
    std::cout << "\n文件信息统计已完成，共用时" << end-start << "毫秒" << std::endl;
    printf("完成了%d个目录的信息统计，其中%d个是不存在的无效目录\n", percentage, trash);
    std::cout << "查询的目录下文件信息已写入" << outcome << "，" << "请查看！\n\n";  
    fclose(fpin);	fclose(fpout);
}

DirTree::node* DirTree::findNode(node* root, char* path) {
	std::string wholePath = std::string(path);
	std::string finalPath = wholePath.substr(0, wholePath.length()-2);  // 一行目录名最后还带有'\'和'\n'
	
	// 找到全路径中每一个'\'的位置，以分层进行前缀匹配
	int layers[16];
	int pos = 10, idx = 0;
    while ((pos = wholePath.find('\\', pos)) != -1) {
        layers[idx] = pos;
        // 继续搜索下一个位置
        pos ++;		idx ++;
    }
	
	idx = 0;
	while(root != NULL) {
		char* target = root->pathName;
		std::string prefix = wholePath.substr(0, layers[idx]);
		if(strcasecmp(target, finalPath.data()) == 0) {
			return root;
		} else if(strcasecmp(target, prefix.data()) == 0) {
			idx ++;
			root = root->son;
		} else {
			root = root->bro;
		}	
	}	
	return NULL;
}

void DirTree::modifyFiles(node* root) {
	char str[301];
	int percentage = 0, whole = 0;
	bool flag = false;
	FILE *fpin;
	if((fpin = fopen("myfile.txt","r")) == NULL) {
		printf("文件读取失败！");
		exit(0);
	}
	verifyChanges(root, "myfile.txt", "beforeMyFile.txt");
	std::cout << "\n对修改前的文件信息查询结果保存在beforeMyFile.txt中"; 
	clock_t start,end;
	start = clock();
	
	time_t time;	off_t size;
	std::string Mode, Time, Size;
    while(fgets(str, 300, fpin)!=NULL){
    	if(strcmp(str, "selected files\n") == 0 || strcmp(str, "end of files\n") == 0)	continue; 
    	// 将全路径，操作码，时间，大小分隔开
    	whole ++;
		std::string s = std::string(str);
		s = s.substr(0, s.length()-1);
		
		int cnt = 0;
		while(cnt < 3) {
			int idx = s.find_last_of(',');
			if(cnt == 0)		Size = s.substr(idx + 1);
			else if(cnt == 1)	Time = s.substr(idx + 1);
			else if(cnt == 2)	Mode = s.substr(idx + 1);
			s = s.substr(0, idx);
			cnt ++;
		}
		
		// D模式下Time，Size无意义 
		if(Mode != "D") {
			time = stol(Time);
			size = stol(Size);
		}
		
    	if(Mode == "M") {
    		// 直接找目标文件 
    		std::string S = s + "\\" + "\n";
    		node* Node = findNode(root,  const_cast<char*>(S.data()));
    		if(Node == NULL)	continue;
    		Node->fileTime = time;
    		Node->fileSize = size;
		} else if(Mode == "A" || Mode == "D") {
			// 将目录名和文件名分开
			int idx = s.find_last_of('\\');
			std::string dirPath = s.substr(0, idx+1);
			dirPath += "\n";
			char* dp = const_cast<char*>(dirPath.data());
			// 找到包含目标文件的目录 
			node* Node = findNode(root, dp);
			if(Node == NULL)	continue;
			if (Mode == "A") {
			    // 创建新的文件节点
			    node* newFileNode = new node;
			    newFileNode->pathName = new char[s.length() + 1];
			    std::strcpy(newFileNode->pathName, s.c_str());
			    newFileNode->type = 0;  
			    newFileNode->fileTime = time;
			    newFileNode->fileSize = size;
			    newFileNode->bro = NULL;
			    newFileNode->son = NULL;
			
			    // 将新文件节点插入到目录节点的尾端
			    if (Node->son) {
			        node* lastNode = Node->son;
			        while (lastNode->bro) {
			            lastNode = lastNode->bro;
			        }
			        lastNode->bro = newFileNode;
			    } else {
			        Node->son = newFileNode;
			    }
			}
			if(Mode == "D") {
				// 确定文件位置并删除 
				if(strcasecmp(Node->son->pathName, s.data()) == 0) {
					Node->son = Node->son->bro;
				} else {
					Node = Node->son;
					while(Node->bro) {
						if(strcasecmp(Node->bro->pathName, s.data()) == 0) {
							Node->bro = Node->bro->bro;
							break;
						}
						Node = Node->bro;
					}
				}
			} 
		}
		percentage ++;
    }
    end = clock();
    std::cout << "\n文件修改已完成，共用时" << end-start << "毫秒" << std::endl;
    printf("共修改%d个文件，另有%d个不存在的无效指令\n", percentage, whole-percentage);
    verifyChanges(root, "myfile.txt", "afterMyFile.txt");
    std::cout << "对修改后的文件信息查询结果保存在afterMyFile.txt中\n"; 
    fclose(fpin);
}

void DirTree::verifyChanges(node* root, char* pathin, char* pathout) {
	char s[301];
	bool flag = false;
	FILE *fpin, *fpout;
	if((fpin = fopen(pathin,"r"))==NULL || (fpout = fopen(pathout,"w")) == NULL){
		printf("文件读取失败！");
		exit(0);
	}
	
	char* head = "以下是查询的文件/目录信息\n\n";
	fwrite(head, 1, strlen(head), fpout);
	
	char* final;
    while(fgets(s, 300, fpin)!=NULL){
    	if(strcmp(s, "selected files\n") == 0 || strcmp(s, "end of files\n") == 0 ||
		   strcmp(s, "selected dirs\n")  == 0 || strcmp(s, "end of dirs\n") == 0) 	 continue; 
		   
		std::string str = std::string(s);
		str = str.substr(0, str.length()-1);
		std::string Size, Time, Mode;
		int cnt = 0;
		while(cnt < 3) {
			int idx = str.find_last_of(',');
			if(cnt == 0)		Size = str.substr(idx + 1);
			else if(cnt == 1)	Time = str.substr(idx + 1);
			else if(cnt == 2)	Mode = str.substr(idx + 1);
			str = str.substr(0, idx);
			cnt ++;
		}
		char* s1 = const_cast<char*>(str.data());
		std::string S2 =  " (" + Mode + " " + Time + " " + Size + ")\n";
		char* s2 = const_cast<char*>(S2.data());
		fwrite(s1, 1, strlen(s1), fpout);
		fwrite(s2, 1, strlen(s2), fpout);
		
    	str = str + "\\" + "\n";
    	s1 = const_cast<char*>(str.data());
   		node* Node = findNode(root, s1);
   		if(Node == NULL) {
   			final = "未找到该目录/文件或已被删除！\n\n";
		} else if(Node->type == TYPEREG) {
			std::string FINAL = Formatter::timeFormatter(Node->fileTime) + " " + Formatter::sizeFormatter(Node->fileSize) + "\n\n";
			final = const_cast<char*>(FINAL.data());
		} else {
			final = "存在该目录\n\n";
		}
		fwrite(final, 1, strlen(final), fpout);
    }
    fclose(fpin);	fclose(fpout);
}

void DirTree::showModifiedInfo(char* f1, char* f2, char* fout) {
	FILE *fi1, *fi2, *fo;
	if((fi1 = fopen(f1,"r"))==NULL || (fi2 = fopen(f2,"r"))==NULL || (fo = fopen(fout,"w")) == NULL){
		printf("文件读取失败！");
		exit(0);
	}
	
	char dirPath[301], s1[301], s2[301];
	int percentage = 0, total = 0, cnt = 0;
	
	char* head = "以下是mystat中发生改变的数据统计，最早时间的文件名(大小 时间)---最晚时间的文件名(大小 时间)---文件总数---总的文件大小\n\n";
	fwrite(head, 1, strlen(head), fo);
	
    while(fgets(s1, 300, fi1)!=NULL && fgets(s2, 300, fi2)!=NULL) {
    	// 忽略文件头和空行 
    	if(strcmp(s1, "以下是指定目录中的文件信息，格式为：最早时间的文件名(大小 时间)---最晚时间的文件名(大小 时间)---文件总数---总的文件大小\n") == 0)
    		continue;
    	if(strcmp(s1, "\n") == 0)
    		continue;
    		
    	// dirPath是目录名，s1/s2分别存放两个统计结果中的信息 
    	if(total%2 == 0) {
    		strcpy(dirPath, s1);
    		percentage ++;
		} else if(strcmp(s1, s2) != 0) {
	    	cnt ++;
			std::string dp = std::string(dirPath);
			std::string S1 = std::string(s1);
			std::string S2 = std::string(s2);
			std::string final = dp + "FROM  " + S1 + "TO->  " + S2 + "\n";
			char* f = const_cast<char*>(final.data());
			fwrite(f, 1, strlen(f), fo);
		}
		total ++;
	    Tick::tick(percentage, -1);
	}
	fclose(fi1);	fclose(fi2);	fclose(fo);
	std::cout << "\n模拟操作后mystat对比扫描已完成，共有" << cnt << "个目录信息发生变化\n"; 
}

void DirTree::deleteDirs(node* root) {
	char str[301];
	int percentage = 0, whole = 0;
	bool flag = false;
	FILE *fpin;
	if((fpin = fopen("mydir.txt","r")) == NULL) {
		printf("文件读取失败！");
		exit(0);
	}
	verifyChanges(root, "mydir.txt", "beforeMyDir.txt");
	std::cout << "\n对修改前的目录查询结果保存在beforeMyDir.txt中"; 
	clock_t start,end;
	start = clock();
	
    while(fgets(str, 300, fpin)!=NULL){
    	if(strcmp(str, "selected dirs\n") == 0 || strcmp(str, "end of dirs\n") == 0)	continue; 
    	// 将全路径，操作码，时间，大小分隔开
    	whole ++;
		std::string s = std::string(str);
		int cnt = 0;
		while(cnt < 3) {
			int idx = s.find_last_of(',');
			s = s.substr(0, idx);
			cnt ++;
		}

		int idx = s.find_last_of('\\');
		std::string dirPath = s.substr(0, idx+1);
		dirPath += "\n";
		char* dp = const_cast<char*>(dirPath.data());
		
		// 找到包含目标文件的目录 
		node* Node = findNode(root, dp);
		if(Node == NULL)	continue;
		// 确定目录位置并删除 
		if(strcasecmp(Node->son->pathName, s.data()) == 0) {
			Node->son = Node->son->bro;
		} else {
			Node = Node->son;
			while(Node->bro) {
				if(strcasecmp(Node->bro->pathName, s.data()) == 0) {
					Node->bro = Node->bro->bro;
					break;
				}
				Node = Node->bro;
			}
		} 
		percentage ++;
    }
    end = clock();
    std::cout << "\n目录删除已完成，共用时" << end-start << "毫秒" << std::endl;
    printf("共删除%d个目录，另有%d个不存在的无效指令\n", percentage, whole-percentage);
    verifyChanges(root, "mydir.txt", "afterMyDir.txt");
    std::cout << "对修改后的目录查询结果保存在afterMyDir.txt中\n"; 
    fclose(fpin);
}























