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
		while (cin.get() != '\n');
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
		while (cin.get() != '\n');
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
		while (cin.get() != '\n');
		if (select < '0' || select > '4')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			dao->PrintQuestionsInSeletedArea();
			break;
		case '2':
			dao->PrintUsersInSelectedArea();
			break;
		case '3':
			dao->PrintAnswersInSelectedArea();
			break;
		case '4':
			dao->PrintResponseUsersInSelectedArea();
			break;
		case '0':
			return;
		default:
			break;
		}

	}
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
		while (cin.get() != '\n');
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
		while (cin.get() != '\n');
		if (select < '0' || select > '1')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			cout << "CONTENTS >> ";
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
		while (cin.get() != '\n');
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
		while (cin.get() != '\n');
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
		while (cin.get() != '\n');
		if (select < '0' || select > '5')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			dao->InsertArea();
			return;
		case '2':
			dao->InsertQuestion();
			return;
		case '3':
			dao->InsertResponse();
			return;
		case '4':
			dao->InsertDomain();
			return;
		case '5':
			dao->InsertUsers();
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
		menu->ShowDeleteMenu();
		cin >> select;
		while (cin.get() != '\n');
		if (select < '0' || select > '5')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			deleteAreaControl();
			return;
		case '2':
			deleteQuestionControl();
			return;
		case '3':
			deleteResponseControl();
			return;
		case '4':
			deleteDomainControl();
			return;
		case '5':
			deleteUsersControl();
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

void MainController::deleteAreaControl()
{
	char select;
	while (true)
	{
		menu->ShowDeleteAreaMenu();
		cin >> select;
		while (cin.get() != '\n');

		if (select < '0' || select > '1')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			dao->DeleteArea();
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

void MainController::deleteQuestionControl()
{
	char select;
	while (true)
	{
		menu->ShowDeleteQuestionMenu();
		cin >> select;
		while (cin.get() != '\n');

		if (select < '0' || select > '1')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			dao->DeleteQuestion();
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

void MainController::deleteResponseControl()
{
	char select;
	while (true)
	{
		menu->ShowDeleteResponseMenu();
		cin >> select;
		while (cin.get() != '\n');

		if (select < '0' || select > '1')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			dao->DeleteResponse();
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

void MainController::deleteUsersControl()
{
	char select;
	while (true)
	{
		menu->ShowDeleteUsersMenu();
		cin >> select;
		while (cin.get() != '\n');

		if (select < '0' || select > '1')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			dao->DeleteUsers();
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

void MainController::deleteDomainControl()
{
	char select;
	while (true)
	{
		menu->ShowDeleteDomainMenu();
		cin >> select;
		while (cin.get() != '\n');

		if (select < '0' || select > '1')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			dao->DeleteDomain();
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
		while (cin.get() != '\n');
		if (select < '0' || select > '5')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			updateAreaControl();
			return;
		case '2':
			updateQuestionControl();
			return;
		case '3':
			updateResponseControl();
			return;
		case '4':
			updateDomainControl();
			return;
		case '5':
			updateUsersControl();
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

void MainController::updateAreaControl()
{
	char select;

	while (true)
	{
		menu->ShowUpdateAreaMenu();
		cin >> select;
		while (cin.get() != '\n');
		if (select < '0' || select > '1')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			dao->UpdateArea();
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

void MainController::updateQuestionControl()
{
	char select;

	while (true)
	{
		menu->ShowUpdateQuestionMenu();
		cin >> select;
		while (cin.get() != '\n');
		if (select < '0' || select > '3')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			dao->UpdateQuestionID();
			return;
		case '2':
			dao->UpdateQuestionTitle();
			return;
		case '3':
			dao->UpdateQuestionContents();
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

void MainController::updateResponseControl()
{
	char select;

	while (true)
	{
		menu->ShowUpdateResponseMenu();
		cin >> select;
		while (cin.get() != '\n');
		if (select < '0' || select > '2')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			dao->UpdateResponseID();
			return;
		case '2':
			dao->UpdateResponseContents();
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

void MainController::updateDomainControl()
{
	char select;

	while (true)
	{
		menu->ShowUpdateDomainMenu();
		cin >> select;
		while (cin.get() != '\n');
		if (select < '0' || select > '3')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			dao->UpdateDomainName();
			return;
		case '2':
			dao->UpdateDomainCompany();
			return;
		case '3':
			dao->UpdateDomainParentDomain();
			return;
		case '0':
			return;
		default:
			return;
		}
	}
}

void MainController::updateUsersControl()
{
	char select;

	while (true)
	{
		menu->ShowUpdateUsersMenu();
		cin >> select;
		while (cin.get() != '\n');
		if (select < '0' || select > '2')
		{
			showOutOfIndex();
			continue;
		}

		switch (select)
		{
		case '1':
			dao->UpdateUsersEmail();
			return;
		case '2':
			dao->UpdateUsersJob();
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
		while (cin.get() != '\n');
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