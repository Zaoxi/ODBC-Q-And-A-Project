#include <iostream>
#include "ShowMenu.h"

using namespace std;

void ShowMenu::showLine()
{
	cout << "-------------------------------------------------" << endl;
}

void ShowMenu::ShowMainMenu()
{
	showLine();
	cout << "<Question & Answer> Database" << endl;
	cout << " - 본 데이터는 지식인 데이터를 기반으로 합니다." << endl;
	cout << " - 메뉴를 선택해주세요." << endl;
	showLine();
	cout << "1. Area 테이블 참조(2Table Join, 3Table Join, 4Table Join 작업이 있습니다.)" << endl;
	cout << "2. Question 테이블 참조" << endl;
	cout << "3. Domain 테이블 참조" << endl;
	cout << "4. User 테이블 참조" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowAreaMenu()
{
	showLine();
	cout << "<Area> 테이블 참조" << endl;
	cout << " - 원하시는 작업을 선택하세요." << endl;
	showLine();
	cout << "1. 모든 Area 데이터 출력" << endl;
	cout << "2. 선택된 대분류 데이터만 출력(2Table Join, 3Table Join, 4Table Join 작업이 있습니다.)" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowSelectedAreaMenu()
{
	showLine();
	cout << "1. 이 분야의 질문을 출력(2Table Join)" << endl;
	cout << "2. 이 분야의 질문을 작성한 유저들의 정보를 출력(3Table Join)" << endl;
	cout << "3. 이 분야의 질문에 대한 답변들을 출력(3Table Join)" << endl;
	cout << "4. 이 분야의 질문에 대한 답변을 작성한 유저들의 정보를 출력(4Table Join)" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowQuestionMenu()
{
	showLine();
	cout << "<Question> 테이블 참조" << endl;
	cout << " - 원하시는 작업을 선택하세요." << endl;
	showLine();
	cout << "1. 질문 제목 검색" << endl;
	cout << "2. 질문 내용 검색" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowDomainMenu()
{
	showLine();
	cout << "<Domain> 테이블 참조" << endl;
	cout << " - 등록된 모든 도메인 정보를 출력합니다." << endl;
	showLine();
}

void ShowMenu::ShowUserMenu()
{
	showLine();
	cout << "<User> 테이블 참조" << endl;
	cout << " - 등록된 모든 유저 정보를 출력합니다." << endl;
	showLine();
}