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

		if (select < '0' || select > '3')
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
				menu->ShowSchema();
				break;
			case '3':
				dao->ExecuteSelectSQL();
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
		if (select == '0')
		{
			return;
		}
		else
		{
			allDataInSeletedArea(select);
		}
	}
}

void MainController::allDataInSeletedArea(char select)
{
	char bigClass[20], smallClass[20];
	cout << "��з� >> ";
	cin >> bigClass;
	cout << "�Һз� >> ";
	cin >> smallClass;
	
	switch (select)
	{
	case '1':
		dao->PrintQuestionsInSeletedArea(bigClass, smallClass);
		return;
	case '2':
		dao->PrintUsersInSelectedArea(bigClass, smallClass);
		return;
	case '3':
		dao->PrintAnswersInSelectedArea(bigClass, smallClass);
		return;
	case '4':
		dao->PrintResponseUsersInSelectedArea(bigClass, smallClass);
		return;
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