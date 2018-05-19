#pragma once
#include "ProjectDAO.h"
#include "ShowMenu.h"

class ProjectManager
{
private:
	// Singleton Pattern
	static ProjectManager * instance;

	ProjectDAO * dao;
	ShowMenu * menu;

	ProjectManager();
public:
	static ProjectManager * GetInstance();
	~ProjectManager();

	ProjectDAO * GetProjectDAO();
	ShowMenu * GetShowMenu();
};