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

	// �о� �޴� ��Ʈ�� �޼ҵ�
	void areaControl();
	// �о� ���� �޴� ��Ʈ�� �޼ҵ�
	void selectedAreaControl();
	// �о� ���� �޴��� ù��° �޴� ��Ʈ�� �޼ҵ�
	void allQuestionsInSeletedArea();
public :
	// ��� ������ �ʱ�ȭ �ϸ鼭 ���θ޴����� ���
	MainController();
	// ������� �Ҵ� ����, manager ��ü�� ����
	~MainController();

	
};