#include "sqlGenerator.h"

void sqlGenerator::writeSQL(char* path) {
    DIR* dir;
    struct dirent* entry;
    std::queue<char*> qd;

    struct stat info, oriInfo;
	clock_t start,end;
	start = clock(); 
	int count = 0, round = 1, percentage = 0, num_of_files = 0;

    qd.push(path);
    FILE* fp = fopen("sql1.sql", "w");
    while (!qd.empty()) {
    	// sql的限制大小分块(40000条语句) 
    	if(count >= 40000) {
    		count = 0;	round ++;
			fclose(fp);
			char* tmp = (char*)malloc(256);
    		itoa(round, tmp, 10);
    		std::string TMP = std::string(tmp);
    		std::string sqlName = "sql" + TMP;
    		sqlName += ".sql";
    		char tmpName[sqlName.length()]; 
			strcpy(tmpName, sqlName.data()); 
			fopen(tmpName, "w");
		}
		
		num_of_files = 0;
    	char* tmp = qd.front();
    	qd.pop();

		DIR* dir = opendir(tmp);
		struct dirent* entry;
		
		while ((entry = readdir(dir)) != NULL) {
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { continue; }
			char* newPath = (char*)malloc(1024);
        	snprintf(newPath, 1024, "%s\\%s", tmp, entry->d_name);
	    	stat(newPath, &info);
	    	stat(tmp, &oriInfo);
			if (S_ISDIR(info.st_mode)) {
				percentage ++; 
				qd.push(newPath);
		    } else if (S_ISREG(info.st_mode)) {
		    	// 格式化修改时间和文件大小 
		    	std::string time = Formatter::timeFormatter(info.st_mtime);
				std::string size = Formatter::sizeFormatter(info.st_size);
				// 文件的insert语句 (名,修改时间,文件大小)	
		        std::string sql = istFile + "'" + newPath + "'" + "," 
										  + "'" +  time   + "'" + "," + size + ");\n";
		        char tmpSql[sql.length()]; 
				strcpy(tmpSql, sql.data()); 
				
		        fwrite(tmpSql, sizeof(tmpSql), 1, fp);
		        count ++; percentage ++; 
				num_of_files ++;		
	    	}
	    }
	    // 目录的insert语句 (名,修改时间,包含文件数量) 
	    std::string time = Formatter::timeFormatter(oriInfo.st_mtime);
		std::string size = Formatter::sizeFormatter(num_of_files);
		std::string sql = istDir + "'" + tmp  + "'" + "," 
								 + "'" + time + "'" + "," + size + ");\n";
		char tmpSql[sql.length()]; 
		strcpy(tmpSql, sql.data()); 			
		fwrite(tmpSql, sizeof(tmpSql), 1, fp);
		count ++; 
	    
	    Tick::tick(percentage, TOTAL);
    }
    fclose(fp);
    end = clock();
    std::cout << "\n\nSQL语句已全部生成，共用时" << end-start << "毫秒" << std::endl;
    printf("共分%d个文件执行\n",round); 
}

