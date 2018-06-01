#pragma once
#include "ProjectManager.h"

class MainController
{
private :
	ProjectManager * manager;
	ShowMenu * menu;
	ProjectDAO * dao;
	// 지정된 입력의 범위를 넘어서 입력되면 호출되는 메소드
	void showOutOfIndex();

	void selectMenuControl();
	void insertMenuControl();
	void deleteMenuControl();
	void updateMenuControl();
	// 구현필요
	void printMenuControl();

	void selectAreaControl();
	void selectQuestionControl();
	void selectResponseControl();
	void selectUsersControl();
	void selectDomainControl();

	void deleteAreaControl();
	void deleteQuestionControl();
	void deleteResponseControl();
	void deleteUsersControl();
	void deleteDomainControl();

	void updateAreaControl();
	void updateQuestionControl();
	void updateResponseControl();
	void updateDomainControl();
	void updateUsersControl();
	

public :
	// 멤버 변수를 초기화 하면서 메인메뉴선택 출력
	MainController();
	// 멤버변수 할당 해제, manager 객체는 제외
	~MainController();

	
};