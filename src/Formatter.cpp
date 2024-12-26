#include "Formatter.h"

std::string Formatter::timeFormatter(time_t t) {
	std::string res;
    struct tm *local_time = localtime(&t);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
    res = std::string(buffer);
	return res;
}

std::string Formatter::sizeFormatter(off_t s) {
	std::string res;
	unsigned int isi = (unsigned int)s; 
	char* S = (char*)malloc(255);
	itoa(isi, S, 10);
	res = std::string(S);
	return res;
}

std::string Formatter::toFilename(char* path) {
	std::string fullPath = std::string(path);
	// �ҵ����һ�� '\' ��λ��
    size_t lastBackslash = fullPath.find_last_of('\\');
    // ʹ�� substr ��ȡ�����һ�� '\' λ��֮������ַ���
    std::string fileName = fullPath.substr(lastBackslash + 1);
	return fileName;
}

// ����ʱ����ļ���(��С ʱ��)---����ʱ����ļ���(��С ʱ��)---�ļ�����---�ܵ��ļ���С\n
char* Formatter::makefileInfo(time_t early_time, off_t early_size, std::string early_name, time_t late_time, off_t late_size, std::string late_name, long fileNum, long fileSize) {
	char* final;
	std::string FINAL;
	
	FINAL = "\t" + early_name + "(" + sizeFormatter(early_size) + " " + timeFormatter(early_time) + ")" + "---" +
				   late_name  + "(" + sizeFormatter(late_size)  + " " + timeFormatter(late_time)  + ")" + "---" +
				   sizeFormatter((off_t)fileNum) + "---" + sizeFormatter((off_t)fileSize) + "\n";
	
	final = const_cast<char*>(FINAL.data());
	return final;
}
