#pragma once
#include "ProjectManager.h"

class MenuController
{
private :
	ProjectManager * manager;
	ShowMenu * menu;
	// ������ �Է��� ������ �Ѿ �ԷµǸ� ȣ��Ǵ� �޼ҵ�
	void showOutOfIndex();
public :
	// ��� ������ �ʱ�ȭ �ϸ鼭 ���θ޴����� ���
	MenuController();
	// ������� �Ҵ� ����, manager ��ü�� ����
	~MenuController();
};