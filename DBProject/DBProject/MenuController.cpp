#include <iostream>
#include "MenuController.h"

using namespace std;

// ���α׷� ���� �κ�..
MenuController::MenuController()
{
	manager = ProjectManager::GetInstance();
	menu = manager->GetShowMenu();
	char select;
	
	while (true)
	{
		menu->ShowMainMenu();
		cin >> select;

		if (select < '1' || select > '4')
		{
			break;
		}
		else
		{
			showOutOfIndex();
		}
	}

	switch (select)
	{
	case '1':
		break;
	case '2':
		break;
	case '3':
		break;
	case '4':
		break;
	default:
		break;
	}
}

MenuController::~MenuController()
{
	delete menu;
}

void MenuController::showOutOfIndex()
{
	cout << "������ ������ ��� �Է��Դϴ�..." << endl;
}