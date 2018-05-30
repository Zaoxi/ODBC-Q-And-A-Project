#include <iostream>
#include "ShowMenu.h"

using namespace std;

void ShowMenu::showLine()
{
	cout << "-------------------------------------------------" << endl;
}

#pragma region 기본 기능에 관한 메뉴

void ShowMenu::ShowMainMenu()
{
	showLine();
	cout << "<Question & Answer> Database" << endl;
	cout << " - 본 데이터는 지식인 데이터를 기반으로 합니다." << endl;
	cout << " - 메뉴를 선택해주세요." << endl;
	showLine();
	cout << "1. SQL query" << endl;
	cout << "2. SELECT" << endl;
	cout << "3. INSERT" << endl;
	cout << "4. DELETE" << endl;
	cout << "5. UPDATE" << endl;
	cout << "6. PRINT" << endl;
	cout << "0. EXIT" << endl;
	showLine();
	cout << " >> ";
}

// 2
void ShowMenu::ShowSelectMenu()
{
	showLine();
	cout << "<SELECT 메뉴>" << endl;
	cout << "1. AREA 테이블 참조" << endl;
	cout << "2. QUESTION 테이블 참조" << endl;
	cout << "3. RESPONSE 테이블 참조" << endl;
	cout << "4. DOMAIN 테이블 참조" << endl;
	cout << "5. USERS 테이블 참조" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}


// 3
void ShowMenu::ShowInsertMenu()
{
	showLine();
	cout << "<INSERT 메뉴>" << endl;
	cout << "1. AREA 테이블에 삽입" << endl;
	cout << "2. QUESTION 테이블에 삽입" << endl;
	cout << "3. RESPONSE 테이블에 삽입" << endl;
	cout << "4. DOMAIN 테이블에 삽입" << endl;
	cout << "5. USERS 테이블에 삽입" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

// 4
void ShowMenu::ShowDeleteMenu()
{
	showLine();
	cout << "<DELETE 메뉴>" << endl;
	cout << "1. AREA 테이블의 튜플 삭제" << endl;
	cout << "2. QUESTION 테이블의 튜플 삭제" << endl;
	cout << "3. RESPONSE 테이블의 튜플 삭제" << endl;
	cout << "4. DOMAIN 테이블의 튜플 삭제" << endl;
	cout << "5. USERS 테이블의 튜플 삭제" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}



// 5
void ShowMenu::ShowUpdateMenu()
{
	showLine();
	cout << "<UPDATE 메뉴>" << endl;
	cout << "1. AREA 테이블의 튜플 수정" << endl;
	cout << "2. QUESTION 테이블의 튜플 수정" << endl;
	cout << "3. RESPONSE 테이블의 튜플 수정" << endl;
	cout << "4. DOMAIN 테이블의 튜플 수정" << endl;
	cout << "5. USERS 테이블의 튜플 수정" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

// 6
void ShowMenu::ShowPrintMenu()
{
	showLine();
	cout << "<PRINT 메뉴>" << endl;
	cout << "1. AREA 테이블 출력" << endl;
	cout << "2. QUESTION 테이블 출력" << endl;
	cout << "3. RESPONSE 테이블 출력" << endl;
	cout << "4. DOMAIN 테이블 출력" << endl;
	cout << "5. USERS 테이블 출력" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

#pragma endregion

#pragma region SELECT TABLE에 관한 메뉴
// 2-1
void ShowMenu::ShowSelectAreaMenu()
{
	showLine();
	cout << "<SELECT AREA 메뉴>" << endl;
	cout << "1. AREA 테이블과 관련된 QUESTION 출력" << endl;
	cout << "2. AREA 테이블과 관련된 QUESTION의 유저정보 출력" << endl;
	cout << "3. AREA 테이블과 관련된 RESPONSE 출력" << endl;
	cout << "4. AREA 테이블과 관련된 RESPONSE의 유저정보 출력" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

// 2-2
void ShowMenu::ShowSelectQuestionMenu()
{
	showLine();
	cout << "<SELECT QUESTION 메뉴>" << endl;
	cout << "1. QUESTION 테이블 제목 검색" << endl;
	cout << "2. QUESTION 테이블 내용 검색" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

// 2-3
void ShowMenu::ShowSelectResponseMenu()
{
	showLine();
	cout << "<SELECT RESPONSE 메뉴>" << endl;
	cout << "1. RESPONSE 테이블 내용 검색" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

// 2-4
void ShowMenu::ShowSelectDomainMenu()
{
	showLine();
	cout << "<SELECT DOMAIN 메뉴>" << endl;
	cout << "1. DOMAIN 이름 검색" << endl;
	cout << "2. DOMAIN 회사 검색" << endl;
	cout << "3. 검색된 DOMAIN에 등록된 모든 유저정보 검색" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

// 2-5
void ShowMenu::ShowSelectUsersMenu()
{
	showLine();
	cout << "<SELECT USERS 메뉴>" << endl;
	cout << "1. USERS 아이디 검색" << endl;
	cout << "2. USERS 가입일 검색" << endl;
	cout << "3. USERS 직업 검색" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}
#pragma endregion

#pragma region DELETE TABLE에 관한 메뉴

void ShowMenu::ShowDeleteAreaMenu()
{
	showLine();
	cout << "<DELETE AREA 메뉴>" << endl;
	cout << "- 삭제시 해당 분야에 속한 모든 질문들의 분야는 NULL이 됩니다." << endl;
	showLine();
	cout << "1. 대분류, 소분류를 입력하여 삭제" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowDeleteQuestionMenu()
{
	showLine();
	cout << "<DELETE QUESTION 메뉴>" << endl;
	cout << "- 삭제시 해당 질문에 속한 모든 답변들은 삭제됩니다." << endl;
	showLine();
	cout << "1. 질문 번호를 이용하여 삭제" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowDeleteResponseMenu()
{
	showLine();
	cout << "<DELETE RESPONSE 메뉴>" << endl;
	cout << "1. 답변 번호를 이용하여 삭제" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowDeleteDomainMenu()
{
	showLine();
	cout << "<DELETE DOMAIN 메뉴>" << endl;
	cout << "- 삭제시 해당 도메인에 속한 모든 유저들이 삭제됩니다." << endl;
	cout << "- 삭제되는 유저들이 작성한 모든 질문과 답변 데이터의 유저정보는 NULL이 됩니다." << endl;
	showLine();
	cout << "1. 도메인 번호를 이용하여 삭제" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowDeleteUsersMenu()
{
	showLine();
	cout << "<DELETE USERS 메뉴>" << endl;
	cout << "- 삭제되는 유저들이 작성한 모든 질문과 답변 데이터의 유저정보는 NULL이 됩니다." << endl;
	showLine();
	cout << "1. 도메인 번호와 ID를 이용하여 삭제" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

#pragma endregion

#pragma region UPDATE TABLE에 관한 메뉴

void ShowMenu::ShowUpdateAreaMenu()
{
	showLine();
	cout << "<UPDATE AREA 메뉴>" << endl;
	cout << "- AREA 변경시, 해당 AREA에 속한 모든 질문들의 분야도 변경됩니다." << endl;
	showLine();
	cout << "1. 대분야, 소분야 변경" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowUpdateQuestionMenu()
{
	showLine();
	cout << "<UPDATE QUESTION 메뉴>" << endl;
	cout << "1. 질문자 변경" << endl;
	cout << "2. 제목 변경" << endl;
	cout << "3. 내용 변경" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowUpdateResponseMenu()
{
	showLine();
	cout << "<UPDATE RESPONSE 메뉴>" << endl;
	cout << "1. 답변자 변경" << endl;
	cout << "2. 내용 변경" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowUpdateDomainMenu()
{
	showLine();
	cout << "<UPDATE DOMAIN 메뉴>" << endl;
	cout << "1. 도메인 이름 변경" << endl;
	cout << "2. 도메인 회사 변경" << endl;
	cout << "3. 상위 도메인 변경" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowUpdateUsersMenu()
{
	showLine();
	cout << "<UPDATE USERS 메뉴>" << endl;
	cout << "1. 유저 이메일 변경" << endl;
	cout << "2. 유저 직업 변경" << endl;
	cout << "0. 뒤로 가기" << endl;
	showLine();
	cout << " >> ";
}

#pragma endregion

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
