#include <iostream>
#include <thread>

void syncPerformTask() {//sync 동기 코드
	std::cout << "작업 수행중 ..." << std::endl;
}

void asyncPerformTask() {
	for (int i = 0; i < 100; i++) {

		std::cout << i << std::endl;
	}
}



int main(void) {

	std::cout << "10" << std::endl;
	std::cout << "시작" << std::endl;
	asyncPerformTask();
	std::cout << "종료" << std::endl;
	return 0;

}