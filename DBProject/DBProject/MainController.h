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

	// 분야 메뉴 컨트롤 메소드
	void areaControl();
	// 분야 서브 메뉴 컨트롤 메소드
	void selectedAreaControl();
	// 분야 서브 메뉴의 첫번째 메뉴 컨트롤 메소드
	void allQuestionsInSeletedArea();
public :
	// 멤버 변수를 초기화 하면서 메인메뉴선택 출력
	MainController();
	// 멤버변수 할당 해제, manager 객체는 제외
	~MainController();

	
};