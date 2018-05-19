#include <iostream>
#include "MainController.h"

using namespace std;

// ���α׷� ���� �κ�.. ���� �޴� ���
MainController::MainController()
{
	manager = ProjectManager::GetInstance();
	menu = manager->GetShowMenu();
	dao = manager->GetProjectDAO();
	// DB�� ������� �ʾҴٸ�
	if (!(dao->GetbIsConnected()))
	{
		cout << "DB ���� ���¸� Ȯ���ϼ���..." << endl;
		delete this;
		return;
	}

	char select;
	
	while (true)
	{
		menu->ShowMainMenu();
		cin >> select;

		if (select < '0' || select > '4')
		{
			showOutOfIndex();
		}
		else
		{
			switch (select)
			{
			case '1':
				areaControl();
				break;
			case '2':
				break;
			case '3':
				break;
			case '4':
				break;
			case '0':
				delete this;
				return;
			default:
				break;
			}
		}
	}
}

// Area �޴� ���
void MainController::areaControl()
{
	char select;

	while (true)
	{
		menu->ShowAreaMenu();
		cin >> select;

		if (select < '0' || select > '2')
		{
			showOutOfIndex();
		}
		else
		{
			switch (select)
			{
			case '1':
				dao->PrintAllArea();
				return;
			case '2':
				selectedAreaControl();
				return;
			case '0':
				return;
			default :
				break;
			}
		}
	}
}

// ���õ� Area �޴� ���
void MainController::selectedAreaControl()
{
	char select;

	while (true)
	{
		menu->ShowSelectedAreaMenu();
		cin >> select;

		if (select < '0' || select > '4')
		{
			showOutOfIndex();
		}
		else
		{
			switch (select)
			{
			case '1':
				allQuestionsInSeletedArea();
				return;
			case '2':
				return;
			case '3':
				return;
			case '4':
				return;
			case '0':
				return;
			default:
				break;
			}
		}
	}
}

void MainController::allQuestionsInSeletedArea()
{
	char bigClass[20], smallClass[20];
	cout << "��з� >> ";
	cin >> bigClass;
	cout << "�Һз� >> ";
	cin >> smallClass;
	
	dao->PrintQuestionsInSeletedArea(bigClass, smallClass);
}

MainController::~MainController()
{
	delete manager;
}

void MainController::showOutOfIndex()
{
	cout << "������ ������ ��� �Է��Դϴ�..." << endl;
}