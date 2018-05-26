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
	cout << " - �� �����ʹ� ������ �����͸� ������� �մϴ�." << endl;
	cout << " - �޴��� �������ּ���." << endl;
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
	cout << "<SELECT �޴�>" << endl;
	cout << "1. AREA ���̺� ����" << endl;
	cout << "2. QUESTION ���̺� ����" << endl;
	cout << "3. RESPONSE ���̺� ����" << endl;
	cout << "4. DOMAIN ���̺� ����" << endl;
	cout << "5. USERS ���̺� ����" << endl;
	cout << "0. �ڷ� ����" << endl;
	showLine();
	cout << " >> ";
}


// 3
void ShowMenu::ShowInsertMenu()
{
	showLine();
	cout << "<INSERT �޴�>" << endl;
	cout << "1. AREA ���̺� ����" << endl;
	cout << "2. QUESTION ���̺� ����" << endl;
	cout << "3. RESPONSE ���̺� ����" << endl;
	cout << "4. DOMAIN ���̺� ����" << endl;
	cout << "5. USERS ���̺� ����" << endl;
	showLine();
	cout << " >> ";
}

// 4
void ShowMenu::ShowDeleteMenu()
{
	showLine();
	cout << "<DELETE �޴�>" << endl;
	cout << "1. AREA ���̺��� Ʃ�� ����" << endl;
	cout << "2. QUESTION ���̺��� Ʃ�� ����" << endl;
	cout << "3. RESPONSE ���̺��� Ʃ�� ����" << endl;
	cout << "4. DOMAIN ���̺��� Ʃ�� ����" << endl;
	cout << "5. USERS ���̺��� Ʃ�� ����" << endl;
	showLine();
	cout << " >> ";
}

// 5
void ShowMenu::ShowUpdateMenu()
{
	showLine();
	cout << "<UPDATE �޴�>" << endl;
	cout << "1. AREA ���̺��� Ʃ�� ����" << endl;
	cout << "2. QUESTION ���̺��� Ʃ�� ����" << endl;
	cout << "3. RESPONSE ���̺��� Ʃ�� ����" << endl;
	cout << "4. DOMAIN ���̺��� Ʃ�� ����" << endl;
	cout << "5. USERS ���̺��� Ʃ�� ����" << endl;
	showLine();
	cout << " >> ";
}

// 6
void ShowMenu::ShowPrintMenu()
{
	showLine();
	cout << "<PRINT �޴�>" << endl;
	cout << "1. AREA ���̺� ���" << endl;
	cout << "2. QUESTION ���̺� ���" << endl;
	cout << "3. RESPONSE ���̺� ���" << endl;
	cout << "4. DOMAIN ���̺� ���" << endl;
	cout << "5. USERS ���̺� ���" << endl;
	showLine();
	cout << " >> ";
}

#pragma region SELECT TABLE�� ���� �޴�
// 2-1
void ShowMenu::ShowSelectAreaMenu()
{
	showLine();
	cout << "<SELECT AREA �޴�>" << endl;
	cout << "1. AREA ���̺�� ���õ� QUESTION ���" << endl;
	cout << "2. AREA ���̺�� ���õ� QUESTION�� �������� ���" << endl;
	cout << "3. AREA ���̺�� ���õ� RESPONSE ���" << endl;
	cout << "4. AREA ���̺�� ���õ� RESPONSE�� �������� ���" << endl;
	cout << "0. �ڷ� ����" << endl;
	showLine();
	cout << " >> ";
}

// 2-2
void ShowMenu::ShowSelectQuestionMenu()
{
	showLine();
	cout << "<SELECT QUESTION �޴�>" << endl;
	cout << "1. QUESTION ���̺� ���� �˻�" << endl;
	cout << "2. QUESTION ���̺� ���� �˻�" << endl;
	cout << "0. �ڷ� ����" << endl;
	showLine();
	cout << " >> ";
}

// 2-3
void ShowMenu::ShowSelectResponseMenu()
{
	showLine();
	cout << "<SELECT RESPONSE �޴�>" << endl;
	cout << "1. RESPONSE ���̺� ���� �˻�" << endl;
	cout << "0. �ڷ� ����" << endl;
	showLine();
	cout << " >> ";
}

// 2-4
void ShowMenu::ShowSelectDomainMenu()
{
	showLine();
	cout << "<SELECT DOMAIN �޴�>" << endl;
	cout << "1. DOMAIN �̸� �˻�" << endl;
	cout << "2. DOMAIN ȸ�� �˻�" << endl;
	cout << "3. DOMAIN���� �˻��� ����Ʈ�� ��ϵ� ��������" << endl;
	cout << "0. �ڷ� ����" << endl;
	showLine();
	cout << " >> ";
}

// 2-5
void ShowMenu::ShowSelectUsersMenu()
{
	showLine();
	cout << "<SELECT USERS �޴�>" << endl;
	cout << "1. USERS ���̵� �˻�" << endl;
	cout << "2. USERS ������ �˻�" << endl;
	cout << "3. USERS ���� �˻�" << endl;
	cout << "0. �ڷ� ����" << endl;
	showLine();
	cout << " >> ";
}
#pragma endregion

void ShowMenu::ShowSchema()
{
	showLine();
	cout << "Schema ����" << endl;
	cout << "AREA (AREA_BIG_CLASS, AREA_SUB_CLASS, AREA_CONTENTS, COUNT_QUERY, EXPERT_ID, EXPERT_DOMAIN_NUM)" << endl;
	cout << "QUESTION (QUE_NUM, QUE_BIG_CLASS, QUE_SUB_CLASS, QUE_ID, QUE_DOMAIN_NUM, QUE_DATE, QUE_TITLE, QUE_CONTENTS)" << endl;
	cout << "RESPONSE (RES_NUM, RES_ID, RES_DOMAIN_NUM, RES_DATE, RES_CONTENTS)" << endl;
	cout << "RESPOND (QUE_NUM, RES_NUM)" << endl;
	cout << "DOMAIN (DOMAIN_NUM, DOMAIN_NAME, DOMAIN_PARENT, COMPANY_NAME)" << endl;
	cout << "USERS (USER_DOMAIN_NUM, USER_ID, USER_JOIN_DATE, USER_EMAIL, USER_JOB)" << endl;
	showLine();
}
