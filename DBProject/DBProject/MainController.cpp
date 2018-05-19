#include <iostream>
#include "MainController.h"

using namespace std;

// ���α׷� ���� �κ�..
MainController::MainController()
{
	manager = ProjectManager::GetInstance();
	menu = manager->GetShowMenu();
	dao = manager->GetProjectDAO();
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
				AreaControl();
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

void MainController::AreaControl()
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
				break;
			case '0':
				break;
			default :
				break;
			}
		}
	}
}

MainController::~MainController()
{
	delete manager;
}

void MainController::showOutOfIndex()
{
	cout << "������ ������ ��� �Է��Դϴ�..." << endl;
}