#pragma once
class ShowMenu
{
private :
	// 메뉴 간 경계를 그리는 메소드
	void showLine();
public :
	// 메뉴 출력 관련 메소드
	void ShowMainMenu();
	void ShowSchema();
	// 메인메뉴에 관한 메소드
	void ShowSelectMenu();
	void ShowInsertMenu();
	void ShowDeleteMenu();
	void ShowUpdateMenu();
	void ShowPrintMenu();

	// SELECT TABLE에 관한 메소드
	void ShowSelectAreaMenu();
	void ShowSelectQuestionMenu();
	void ShowSelectResponseMenu();
	void ShowSelectDomainMenu();
	void ShowSelectUsersMenu();

	// INSERT TABLE에 관한 메소드
	
};

