#include "pch.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>//c++11 부터 시간측정용 하지만 현재 year_month_day, zoned_time,hh_mm_ss time 등 몇몇 클래스는 c++20부터 사용 가능하기때문에 현재 c++ 20기준으로 작성되어있는 코드
#include <iomanip>
#include <format>
#include <sstream>
#include<time.h>
#include<Windows.h>

using namespace std;
using namespace std::chrono;
int main(void) {
	ios::sync_with_stdio;
	cin.tie(NULL);
	cout.tie(NULL);


	using namespace std::chrono;

	// Local Time 기반 현재 시간 구하기
	const local_time<system_clock::duration> now = zoned_time{ current_zone(), system_clock::now() }.get_local_time();
	// year_month_day를 위해 day까지 절삭
	const time_point<std::chrono::local_t, std::chrono::days> dp = floor<std::chrono::days>(now);

	// 날짜를 얻기 위한 year_month_day
	std::chrono::year_month_day ymd{ dp };//중괄호 초기화 dp 변수 생성 년 월만 잘라둔것
	// 시간을 얻기 위한 hh_mm_ss
	std::chrono::hh_mm_ss time{ std::chrono::floor< std::chrono::seconds>(now - dp) };//시간 자른것 여기서 seconds를 nanoseconds,microseconds,milliseconds,seconds 로 교체 가능 해당 단위로 출력가능

	std::cout << "Local now : " << now << std::endl;
	std::cout << "Local year : " << ymd.year() << std::endl;
	std::cout << "Local month : " << (unsigned int)ymd.month() << std::endl;
	std::cout << "Local day : " << ymd.day() << std::endl;
	std::cout << "Local hours : " << time.hours().count() << std::endl;
	std::cout << "Local minutes : " << time.minutes().count() << std::endl;
	std::cout << "Local seconds : " << time.seconds().count() << std::endl;
	std::cout << "Local milliseconds : " << time.subseconds().count() << std::endl;


	//std::string s = std::format("{:%Y년 %m월 %d일}", dp);//%y는 뒤에 두자리만 %Y는 4자리 모두 출력
	std::string s = std::format("{:%Y년 %m월 %d일}", dp);
	std::string sec = std::format("{:%H:%M:%S}", time);
	//std::string sec = std::format("{:%T}", secs);
	cout << sec << endl;
	cout << time.seconds()<<endl;

	cout << dp << endl;//dp가 year month day만 출력
	cout << "초단위" << time << endl;//time이 시 초 출력

	//cout << format("{%X초}", time)<<endl;

	cout << format("{:%Y년 %m월 %d일 %H:%M %S}\n", now);//second 정보 출력필요
	//string d = format("{%H%M}", std::chrono::floor< std::chrono::milliseconds>(now - dp));

	Sleep(1000);

	const local_time<system_clock::duration> now2 = zoned_time{ current_zone(), system_clock::now() }.get_local_time();


	cout << format("{:%Y년 %m월 %d일 %H:%M %S}\n", now2);//second 정보 출력필요
	std::time_t a_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	cout << s;


}
