#include "Tick.h"

void Tick::tick(int per, int TOTAL) {
	if(TOTAL == -1) {
		std::cout << " <---- ������ļ�/Ŀ¼����";
		printf("\r");
		std::cout << per;
	}
	else {
		std::cout << " <---- ��ǰ����";
		printf("\r");
		std::cout << std::setw(5) << std::fixed << std::setprecision(2) << 100*(float)per / TOTAL << "%";
	}
}
