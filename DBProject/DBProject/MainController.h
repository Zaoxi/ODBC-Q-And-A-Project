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


	void AreaControl();
public :
	// ��� ������ �ʱ�ȭ �ϸ鼭 ���θ޴����� ���
	MainController();
	// ������� �Ҵ� ����, manager ��ü�� ����
	~MainController();

	
};