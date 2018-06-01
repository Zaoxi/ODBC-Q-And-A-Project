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
	// �����ʿ�
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
	// ��� ������ �ʱ�ȭ �ϸ鼭 ���θ޴����� ���
	MainController();
	// ������� �Ҵ� ����, manager ��ü�� ����
	~MainController();

	
};