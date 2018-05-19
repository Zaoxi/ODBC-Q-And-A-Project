#pragma once
#include "ProjectManager.h"

class MenuController
{
private :
	ProjectManager * manager;
	ShowMenu * menu;
	// 지정된 입력의 범위를 넘어서 입력되면 호출되는 메소드
	void showOutOfIndex();
public :
	// 멤버 변수를 초기화 하면서 메인메뉴선택 출력
	MenuController();
	// 멤버변수 할당 해제, manager 객체는 제외
	~MenuController();
};