#include "Tick.h"

void Tick::tick(int per, int TOTAL) {
	if(TOTAL == -1) {
		std::cout << " <---- 已完成文件/目录总数";
		printf("\r");
		std::cout << per;
	}
	else {
		std::cout << " <---- 当前进度";
		printf("\r");
		std::cout << std::setw(5) << std::fixed << std::setprecision(2) << 100*(float)per / TOTAL << "%";
	}
}
