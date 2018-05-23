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
	cout << "1. Area ���̺��� �̿��Ͽ� ���� Join �۾� ����" << endl;
	cout << "2. Schema ���" << endl;
	cout << "3. SQL�� �Է�" << endl;
	cout << "0. ���α׷� ����" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowAreaMenu()
{
	showLine();
	cout << "<Area> ���̺� ����" << endl;
	cout << " - ���Ͻô� �۾��� �����ϼ���." << endl;
	showLine();
	cout << "1. ��� Area ������ ���" << endl;
	cout << "2. ��з�, �Һз� ���� ��, �ش� ������ ���(2Table Join, 3Table Join, 4Table Join �۾��� �ֽ��ϴ�.)" << endl;
	cout << "0. �ڷ� ����" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowSelectedAreaMenu()
{
	showLine();
	cout << "1. �� �о��� ��� ������ ���(2Table Join)" << endl;
	cout << "2. �� �о��� ������ �ۼ��� ��� �������� ������ ���(3Table Join)" << endl;
	cout << "3. �� �о��� ������ ���� ��� �亯���� ���(3Table Join)" << endl;
	cout << "4. �� �о��� ������ ���� �亯�� �ۼ��� ��� �������� ������ ���(4Table Join)" << endl;
	cout << "0. �ڷ� ����" << endl;
	showLine();
	cout << " >> ";
}

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
