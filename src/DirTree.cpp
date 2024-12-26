#include "DirTree.h"

DirTree::node* DirTree::buildTree(char* path) {
	DIR* dir;
    struct dirent* entry;
    std::queue<node*> qd;

    struct stat info;
	clock_t start,end;
	start = clock(); 
	int percentage = 0, round = 0;
	
	// ���ø��ڵ� 
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
		    	// �����ļ���Ϣ 
		    	next->fileTime = info.st_mtime;	
				next->fileSize = info.st_size;
				
		    	if(round == 0)	last->son = next;
				else			last->bro = next;
	    
		        percentage ++;
	    	}
	    	last = next;
	    	round ++;
	    }
	    // ʵʱ������ʾ���� 
	    Tick::tick(percentage, TOTAL);
    }
    end = clock();
    std::cout << "\n\nĿ¼����������ɣ�����ʱ" << end-start << "����" << std::endl;
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
        // ����ǰ�㼶�����нڵ�
        for (int i = 0; i < size; ++i) {
            node* current = q.front();
            q.pop();

            // ����ǰ�ڵ���ӡ��ֽڵ�������
            if (current->son) {
                q.push(current->son);
            }
            if (current->bro) {
                q.push(current->bro);
            }
        }
        // ÿ����һ�㣬��ȼ�һ
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
		printf("�ļ���ȡʧ�ܣ�");
		exit(0);
	}
	
	char* head = "������ָ��Ŀ¼�е��ļ���Ϣ����ʽΪ������ʱ����ļ���(��С ʱ��)---����ʱ����ļ���(��С ʱ��)---�ļ�����---�ܵ��ļ���С\n\n";
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
   			final = "\tδ�ҵ���Ŀ¼���ѱ�ɾ����\n";
   			trash ++;
		} else if(Node->son == NULL) {
   			final = "\t��Ŀ¼Ϊ�գ�\n";
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
			else 				final = "\t��Ŀ¼�²������κ��ļ���\n"; 
		}
		fwrite(final, 1, strlen(final), fpout);
		// ʵʱ������ʾ���� 
		percentage ++;
	    Tick::tick(percentage, -1);
    }
    end = clock();
    std::cout << "\n�ļ���Ϣͳ������ɣ�����ʱ" << end-start << "����" << std::endl;
    printf("�����%d��Ŀ¼����Ϣͳ�ƣ�����%d���ǲ����ڵ���ЧĿ¼\n", percentage, trash);
    std::cout << "��ѯ��Ŀ¼���ļ���Ϣ��д��" << outcome << "��" << "��鿴��\n\n";  
    fclose(fpin);	fclose(fpout);
}

DirTree::node* DirTree::findNode(node* root, char* path) {
	std::string wholePath = std::string(path);
	std::string finalPath = wholePath.substr(0, wholePath.length()-2);  // һ��Ŀ¼����󻹴���'\'��'\n'
	
	// �ҵ�ȫ·����ÿһ��'\'��λ�ã��Էֲ����ǰ׺ƥ��
	int layers[16];
	int pos = 10, idx = 0;
    while ((pos = wholePath.find('\\', pos)) != -1) {
        layers[idx] = pos;
        // ����������һ��λ��
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
		printf("�ļ���ȡʧ�ܣ�");
		exit(0);
	}
	verifyChanges(root, "myfile.txt", "beforeMyFile.txt");
	std::cout << "\n���޸�ǰ���ļ���Ϣ��ѯ���������beforeMyFile.txt��"; 
	clock_t start,end;
	start = clock();
	
	time_t time;	off_t size;
	std::string Mode, Time, Size;
    while(fgets(str, 300, fpin)!=NULL){
    	if(strcmp(str, "selected files\n") == 0 || strcmp(str, "end of files\n") == 0)	continue; 
    	// ��ȫ·���������룬ʱ�䣬��С�ָ���
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
		
		// Dģʽ��Time��Size������ 
		if(Mode != "D") {
			time = stol(Time);
			size = stol(Size);
		}
		
    	if(Mode == "M") {
    		// ֱ����Ŀ���ļ� 
    		std::string S = s + "\\" + "\n";
    		node* Node = findNode(root,  const_cast<char*>(S.data()));
    		if(Node == NULL)	continue;
    		Node->fileTime = time;
    		Node->fileSize = size;
		} else if(Mode == "A" || Mode == "D") {
			// ��Ŀ¼�����ļ����ֿ�
			int idx = s.find_last_of('\\');
			std::string dirPath = s.substr(0, idx+1);
			dirPath += "\n";
			char* dp = const_cast<char*>(dirPath.data());
			// �ҵ�����Ŀ���ļ���Ŀ¼ 
			node* Node = findNode(root, dp);
			if(Node == NULL)	continue;
			if (Mode == "A") {
			    // �����µ��ļ��ڵ�
			    node* newFileNode = new node;
			    newFileNode->pathName = new char[s.length() + 1];
			    std::strcpy(newFileNode->pathName, s.c_str());
			    newFileNode->type = 0;  
			    newFileNode->fileTime = time;
			    newFileNode->fileSize = size;
			    newFileNode->bro = NULL;
			    newFileNode->son = NULL;
			
			    // �����ļ��ڵ���뵽Ŀ¼�ڵ��β��
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
				// ȷ���ļ�λ�ò�ɾ�� 
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
    std::cout << "\n�ļ��޸�����ɣ�����ʱ" << end-start << "����" << std::endl;
    printf("���޸�%d���ļ�������%d�������ڵ���Чָ��\n", percentage, whole-percentage);
    verifyChanges(root, "myfile.txt", "afterMyFile.txt");
    std::cout << "���޸ĺ���ļ���Ϣ��ѯ���������afterMyFile.txt��\n"; 
    fclose(fpin);
}

void DirTree::verifyChanges(node* root, char* pathin, char* pathout) {
	char s[301];
	bool flag = false;
	FILE *fpin, *fpout;
	if((fpin = fopen(pathin,"r"))==NULL || (fpout = fopen(pathout,"w")) == NULL){
		printf("�ļ���ȡʧ�ܣ�");
		exit(0);
	}
	
	char* head = "�����ǲ�ѯ���ļ�/Ŀ¼��Ϣ\n\n";
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
   			final = "δ�ҵ���Ŀ¼/�ļ����ѱ�ɾ����\n\n";
		} else if(Node->type == TYPEREG) {
			std::string FINAL = Formatter::timeFormatter(Node->fileTime) + " " + Formatter::sizeFormatter(Node->fileSize) + "\n\n";
			final = const_cast<char*>(FINAL.data());
		} else {
			final = "���ڸ�Ŀ¼\n\n";
		}
		fwrite(final, 1, strlen(final), fpout);
    }
    fclose(fpin);	fclose(fpout);
}

void DirTree::showModifiedInfo(char* f1, char* f2, char* fout) {
	FILE *fi1, *fi2, *fo;
	if((fi1 = fopen(f1,"r"))==NULL || (fi2 = fopen(f2,"r"))==NULL || (fo = fopen(fout,"w")) == NULL){
		printf("�ļ���ȡʧ�ܣ�");
		exit(0);
	}
	
	char dirPath[301], s1[301], s2[301];
	int percentage = 0, total = 0, cnt = 0;
	
	char* head = "������mystat�з����ı������ͳ�ƣ�����ʱ����ļ���(��С ʱ��)---����ʱ����ļ���(��С ʱ��)---�ļ�����---�ܵ��ļ���С\n\n";
	fwrite(head, 1, strlen(head), fo);
	
    while(fgets(s1, 300, fi1)!=NULL && fgets(s2, 300, fi2)!=NULL) {
    	// �����ļ�ͷ�Ϳ��� 
    	if(strcmp(s1, "������ָ��Ŀ¼�е��ļ���Ϣ����ʽΪ������ʱ����ļ���(��С ʱ��)---����ʱ����ļ���(��С ʱ��)---�ļ�����---�ܵ��ļ���С\n") == 0)
    		continue;
    	if(strcmp(s1, "\n") == 0)
    		continue;
    		
    	// dirPath��Ŀ¼����s1/s2�ֱ�������ͳ�ƽ���е���Ϣ 
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
	std::cout << "\nģ�������mystat�Ա�ɨ������ɣ�����" << cnt << "��Ŀ¼��Ϣ�����仯\n"; 
}

void DirTree::deleteDirs(node* root) {
	char str[301];
	int percentage = 0, whole = 0;
	bool flag = false;
	FILE *fpin;
	if((fpin = fopen("mydir.txt","r")) == NULL) {
		printf("�ļ���ȡʧ�ܣ�");
		exit(0);
	}
	verifyChanges(root, "mydir.txt", "beforeMyDir.txt");
	std::cout << "\n���޸�ǰ��Ŀ¼��ѯ���������beforeMyDir.txt��"; 
	clock_t start,end;
	start = clock();
	
    while(fgets(str, 300, fpin)!=NULL){
    	if(strcmp(str, "selected dirs\n") == 0 || strcmp(str, "end of dirs\n") == 0)	continue; 
    	// ��ȫ·���������룬ʱ�䣬��С�ָ���
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
		
		// �ҵ�����Ŀ���ļ���Ŀ¼ 
		node* Node = findNode(root, dp);
		if(Node == NULL)	continue;
		// ȷ��Ŀ¼λ�ò�ɾ�� 
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
    std::cout << "\nĿ¼ɾ������ɣ�����ʱ" << end-start << "����" << std::endl;
    printf("��ɾ��%d��Ŀ¼������%d�������ڵ���Чָ��\n", percentage, whole-percentage);
    verifyChanges(root, "mydir.txt", "afterMyDir.txt");
    std::cout << "���޸ĺ��Ŀ¼��ѯ���������afterMyDir.txt��\n"; 
    fclose(fpin);
}























