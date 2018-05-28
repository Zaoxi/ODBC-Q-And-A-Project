#pragma once
#include "ProjectManager.h"

class MainController
{
private :
	ProjectManager * manager;
	ShowMenu * menu;
	ProjectDAO * dao;
	// ������ �Է��� ������ �Ѿ �ԷµǸ� ȣ��Ǵ� �޼ҵ�
	void showOutOfIndex();

	void selectMenuControl();
	void insertMenuControl();
	void deleteMenuControl();
	void updateMenuControl();
	void printMenuControl();

	void selectAreaControl();
	void selectQuestionControl();
	void selectResponseControl();
	void selectUsersControl();
	void selectDomainControl();

	void selectQuestionInArea();
	void selectQueUsersInArea();
	void selectResponseInArea();
	void selectResUsersInArea();

public :
	// ��� ������ �ʱ�ȭ �ϸ鼭 ���θ޴����� ���
	MainController();
	// ������� �Ҵ� ����, manager ��ü�� ����
	~MainController();

	
};