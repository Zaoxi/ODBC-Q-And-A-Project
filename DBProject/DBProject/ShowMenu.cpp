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
	cout << "1. Area 테이블을 이용하여 여러 Join 작업 실행" << endl;
	cout << "2. Schema 출력" << endl;
	cout << "3. SQL문 입력" << endl;
	cout << "0. 프로그램 종료" << endl;
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
	cout << "2. 대분류, 소분류 선택 후, 해당 데이터 출력(2Table Join, 3Table Join, 4Table Join 작업이 있습니다.)" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowSelectedAreaMenu()
{
	showLine();
	cout << "1. 이 분야의 모든 질문을 출력(2Table Join)" << endl;
	cout << "2. 이 분야의 질문을 작성한 모든 유저들의 정보를 출력(3Table Join)" << endl;
	cout << "3. 이 분야의 질문에 대한 모든 답변들을 출력(3Table Join)" << endl;
	cout << "4. 이 분야의 질문에 대한 답변을 작성한 모든 유저들의 정보를 출력(4Table Join)" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowSchema()
{
	showLine();
	cout << "Schema 정보" << endl;
	cout << "AREA (AREA_BIG_CLASS, AREA_SUB_CLASS, AREA_CONTENTS, COUNT_QUERY, EXPERT_ID, EXPERT_DOMAIN_NUM)" << endl;
	cout << "QUESTION (QUE_NUM, QUE_BIG_CLASS, QUE_SUB_CLASS, QUE_ID, QUE_DOMAIN_NUM, QUE_DATE, QUE_TITLE, QUE_CONTENTS)" << endl;
	cout << "RESPONSE (RES_NUM, RES_ID, RES_DOMAIN_NUM, RES_DATE, RES_CONTENTS)" << endl;
	cout << "RESPOND (QUE_NUM, RES_NUM)" << endl;
	cout << "DOMAIN (DOMAIN_NUM, DOMAIN_NAME, DOMAIN_PARENT, COMPANY_NAME)" << endl;
	cout << "USERS (USER_DOMAIN_NUM, USER_ID, USER_JOIN_DATE, USER_EMAIL, USER_JOB)" << endl;
	showLine();
}
