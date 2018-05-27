#include <iostream>
#include "MainController.h"

using namespace std;

// 프로그램 시작 부분.. 메인 메뉴 출력
MainController::MainController()
{
	manager = ProjectManager::GetInstance();
	menu = manager->GetShowMenu();
	dao = manager->GetProjectDAO();
	// DB가 연결되지 않았다면
	if (!(dao->GetbIsConnected()))
	{
		cout << "DB 연결 상태를 확인하세요..." << endl;
		delete this;
		return;
	}

	char select;
	
	while (true)
	{
		menu->ShowMainMenu();
		cin >> select;

		if (select < '0' || select > '6')
		{
			showOutOfIndex();
		}
		else
		{
			switch (select)
			{
			case '1':
				dao->ExecuteSelectSQL();
				break;
			case '2':
				selectMenuControl();
				break;
			case '3':
				insertMenuControl();
				break;
			case '4':
				deleteMenuControl();
				break;
			case '5':
				updateMenuControl();
				break;
			case '6':
				printMenuControl();
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

#pragma region SELECT 메뉴에 대한 컨트롤 메소드

void MainController::selectMenuControl()
{
	char select;

	while (true)
	{
		menu->ShowSelectMenu();
		cin >> select;

		if (select < '0' || select > '5')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			selectAreaControl();
			return;
		case '2':
			selectQuestionControl();
			return;
		case '3':
			selectResponseControl();
			return;
		case '4':
			selectDomainControl();
			return;
		case '5':
			selectUsersControl();
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

#pragma region SELECT AREA에 대한 메소드

void MainController::selectAreaControl()
{
	char select;

	while (true)
	{
		menu->ShowSelectAreaMenu();
		cin >> select;

		if (select < '0' || select > '4')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			selectQuestionInArea();
			break;
		case '2':
			selectQueUsersInArea();
			break;
		case '3':
			selectResponseInArea();
			break;
		case '4':
			selectResUsersInArea();
			break;
		case '0':
			return;
		default:
			break;
		}

	}
}

void MainController::selectQuestionInArea()
{
	char bigClass[20], smallClass[20];
	cout << "대분류 >> ";
	cin >> bigClass;
	cout << "소분류 >> ";
	cin >> smallClass;

	dao->PrintQuestionsInSeletedArea(bigClass, smallClass);
}

void MainController::selectQueUsersInArea()
{
	char bigClass[20], smallClass[20];
	cout << "대분류 >> ";
	cin >> bigClass;
	cout << "소분류 >> ";
	cin >> smallClass;

	dao->PrintUsersInSelectedArea(bigClass, smallClass);
}

void MainController::selectResponseInArea()
{
	char bigClass[20], smallClass[20];
	cout << "대분류 >> ";
	cin >> bigClass;
	cout << "소분류 >> ";
	cin >> smallClass;

	dao->PrintAnswersInSelectedArea(bigClass, smallClass);
}

void ::MainController::selectResUsersInArea()
{
	char bigClass[20], smallClass[20];
	cout << "대분류 >> ";
	cin >> bigClass;
	cout << "소분류 >> ";
	cin >> smallClass;

	dao->PrintResponseUsersInSelectedArea(bigClass, smallClass);
}

#pragma endregion

#pragma region SELECT QUESTION에 대한 메소드

void MainController::selectQuestionControl()
{
	char select;
	char searchData[100];

	while (true)
	{
		menu->ShowSelectQuestionMenu();
		cin >> select;
		cin.get();
		if (select < '0' || select > '2')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			cout << "TITLE >> ";
			cin.getline(searchData, 100);
			dao->PrintQuestionUsingTitle(searchData);
			break;
		case '2':
			cout << "CONTENTS >> ";
			cin.getline(searchData, 100);
			dao->PrintQuestionUsingContents(searchData);
			break;
		case '0':
			return;
		default:
			break;
		}
	}
}

#pragma endregion

#pragma region SELECT RESPONSE에 대한 메소드

void MainController::selectResponseControl()
{
	char select;
	char searchData[100];

	while (true)
	{
		menu->ShowSelectResponseMenu();
		cin >> select;
		cin.get();
		if (select < '0' || select > '1')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			cin.getline(searchData, 100);
			dao->PrintResponseUsingContents(searchData);
			break;
		case '0':
			return;
		default:
			break;
		}
	}
}

#pragma endregion

#pragma region SELECT USERS에 대한 메소드

void MainController::selectUsersControl()
{
	char select;
	char search[100];

	while (true)
	{
		menu->ShowSelectUsersMenu();
		cin >> select;
		cin.get();
		if (select < '0' || select > '3')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			cout << "ID >> ";
			cin.getline(search, 100);
			dao->PrintUsersUsingID(search);
			break;
		case '2':
			cout << "DATE(Y-M-D) >> ";
			cin.getline(search, 100);
			dao->PrintUsersUsingDate(search);
			break;
		case '3':
			cout << "JOB >> ";
			cin.getline(search, 100);
			dao->PrintUsersUsingJob(search);
			break;
		case '0':
			return;
		default:
			break;
		}
	}
}

#pragma endregion

#pragma region SELECT DOMAIN에 대한 메소드

void MainController::selectDomainControl()
{
	char select;
	char search[100];
	while (true)
	{
		menu->ShowSelectDomainMenu();
		cin >> select;
		cin.get();
		if (select < '0' || select > '3')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			cout << "NAME >> ";
			cin.getline(search, 100);
			dao->PrintDomainUsingName(search);
			break;
		case '2':
			cout << "COMPANY >> ";
			cin.getline(search, 100);
			dao->PrintDomainUsingCompany(search);
			break;
		case '3':
			cout << "NAME >> ";
			cin.getline(search, 100);
			dao->PrintUsersAccordingDomain(search);
			break;
		case '0':
			return;
		default:
			break;
		}
	}
}

#pragma endregion

#pragma endregion

#pragma region INSERT 메뉴에 대한 컨트롤 메소드

void MainController::insertMenuControl()
{
	char select;

	while (true)
	{
		menu->ShowInsertMenu();
		cin >> select;

		if (select < '0' || select > '5')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			return;
		case '2':
			return;
		case '3':
			return;
		case '4':
			return;
		case '5':
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

#pragma endregion

#pragma region DELETE 메뉴에 대한 컨트롤 메소드

void MainController::deleteMenuControl()
{
	char select;

	while (true)
	{
		menu->ShowInsertMenu();
		cin >> select;

		if (select < '0' || select > '5')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			return;
		case '2':
			return;
		case '3':
			return;
		case '4':
			return;
		case '5':
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

#pragma endregion

#pragma region UPDATE 메뉴에 대한 컨트롤 메소드

void MainController::updateMenuControl()
{
	char select;

	while (true)
	{
		menu->ShowUpdateMenu();
		cin >> select;

		if (select < '0' || select > '5')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			return;
		case '2':
			return;
		case '3':
			return;
		case '4':
			return;
		case '5':
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

#pragma endregion

#pragma region PRINT 메뉴에 대한 컨트롤 메소드

void MainController::printMenuControl()
{
	char select;

	while (true)
	{
		menu->ShowPrintMenu();
		cin >> select;

		if (select < '0' || select > '5')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			return;
		case '2':
			return;
		case '3':
			return;
		case '4':
			return;
		case '5':
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

#pragma endregion

MainController::~MainController()
{
	delete manager;
}

void MainController::showOutOfIndex()
{
	cout << "지정된 범위를 벗어난 입력입니다..." << endl;
}