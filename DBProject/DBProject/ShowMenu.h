#pragma once
class ShowMenu
{
private :
	// �޴� �� ��踦 �׸��� �޼ҵ�
	void showLine();
public :
	// �޴� ��� ���� �޼ҵ�
	void ShowMainMenu();
	void ShowSchema();
	// ���θ޴��� ���� �޼ҵ�
	void ShowSelectMenu();
	void ShowInsertMenu();
	void ShowDeleteMenu();
	void ShowUpdateMenu();
	void ShowPrintMenu();

	// SELECT TABLE�� ���� �޼ҵ�
	void ShowSelectAreaMenu();
	void ShowSelectQuestionMenu();
	void ShowSelectResponseMenu();
	void ShowSelectDomainMenu();
	void ShowSelectUsersMenu();

	// INSERT TABLE�� ���� �޼ҵ�
	
};

