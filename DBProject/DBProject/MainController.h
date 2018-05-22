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
	// ���õ� �о߿� ���õ� ���� ��� ���
	void allDataInSeletedArea(char select);

public :
	// ��� ������ �ʱ�ȭ �ϸ鼭 ���θ޴����� ���
	MainController();
	// ������� �Ҵ� ����, manager ��ü�� ����
	~MainController();

	
};