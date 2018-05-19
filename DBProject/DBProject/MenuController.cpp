#include <iostream>
#include "MenuController.h"

using namespace std;

// 프로그램 시작 부분..
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
	cout << "지정된 범위를 벗어난 입력입니다..." << endl;
}