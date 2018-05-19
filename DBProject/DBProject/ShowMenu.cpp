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
	cout << "1. Area ���̺� ����(2Table Join, 3Table Join, 4Table Join �۾��� �ֽ��ϴ�.)" << endl;
	cout << "2. Question ���̺� ����" << endl;
	cout << "3. Domain ���̺� ����" << endl;
	cout << "4. User ���̺� ����" << endl;
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
	cout << "2. ���õ� ��з� �����͸� ���(2Table Join, 3Table Join, 4Table Join �۾��� �ֽ��ϴ�.)" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowSelectedAreaMenu()
{
	showLine();
	cout << "1. �� �о��� ������ ���(2Table Join)" << endl;
	cout << "2. �� �о��� ������ �ۼ��� �������� ������ ���(3Table Join)" << endl;
	cout << "3. �� �о��� ������ ���� �亯���� ���(3Table Join)" << endl;
	cout << "4. �� �о��� ������ ���� �亯�� �ۼ��� �������� ������ ���(4Table Join)" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowQuestionMenu()
{
	showLine();
	cout << "<Question> ���̺� ����" << endl;
	cout << " - ���Ͻô� �۾��� �����ϼ���." << endl;
	showLine();
	cout << "1. ���� ���� �˻�" << endl;
	cout << "2. ���� ���� �˻�" << endl;
	showLine();
	cout << " >> ";
}

void ShowMenu::ShowDomainMenu()
{
	showLine();
	cout << "<Domain> ���̺� ����" << endl;
	cout << " - ��ϵ� ��� ������ ������ ����մϴ�." << endl;
	showLine();
}

void ShowMenu::ShowUserMenu()
{
	showLine();
	cout << "<User> ���̺� ����" << endl;
	cout << " - ��ϵ� ��� ���� ������ ����մϴ�." << endl;
	showLine();
}