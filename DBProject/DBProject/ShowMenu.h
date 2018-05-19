#pragma once
class ShowMenu
{
private :
	// 메뉴 간 경계를 그리는 메소드
	void showLine();
public :
	void ShowMainMenu();
	void ShowAreaMenu();
	void ShowSelectedAreaMenu();
	void ShowQuestionMenu();
	void ShowDomainMenu();
	void ShowUserMenu();
};

