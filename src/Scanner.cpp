#include "Scanner.h"

char* Scanner::scanDiskC(char* path, int& dirCount, int& fileCount, char* maxStr, int& maxPathLength, int& maxLayer) {
    DIR* dir;
    struct dirent* entry;
    std::queue<char*> qd;
    std::unordered_map<char*,int> layerOfDirectory; // 目录~层数 

    struct stat info;
	clock_t start,end;
	start = clock(); 
	int percentage = 0;

    qd.push(path);
    layerOfDirectory.insert({path,1}); 

    while (!qd.empty()) {
    	char* tmp = qd.front();
    	qd.pop();

		DIR* dir = opendir(tmp);
		struct dirent* entry;
		
		while ((entry = readdir(dir)) != NULL) {
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { continue; }
			
			char* newPath = (char*)malloc(1024);
        	snprintf(newPath, 1024, "%s\\%s", tmp, entry->d_name);
	    	stat(newPath,&info);
	    	
			if (S_ISDIR(info.st_mode)) {
		        // 是目录
		        layerOfDirectory[newPath] = layerOfDirectory[tmp] + 1;
		        maxLayer = std::max(layerOfDirectory[newPath], maxLayer);
		        dirCount ++;
		        percentage ++;
		        qd.push(newPath);
		    } else if (S_ISREG(info.st_mode)) {
		        // 是文件
		        fileCount ++;
		        percentage ++;
		        if (strlen(newPath) > static_cast<size_t>(maxPathLength)) {
		        	maxStr = newPath;
		            maxPathLength = static_cast<int>(strlen(newPath));
		        }
	    	}
	    }
	    // 实时进度显示功能 
	    Tick::tick(percentage, -1);
    }
    end = clock();
    std::cout << "\n\nC:\\Windows扫描已全部完成，共用时" << end-start << "毫秒" << std::endl;
    return maxStr;
}
