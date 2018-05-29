#include <iostream>
#include "ProjectManager.h"

using namespace std;

ProjectManager * ProjectManager::instance = nullptr;

ProjectManager::ProjectManager()
{
	dao = new ProjectDAO();
	menu = new ShowMenu();
}

ProjectManager * ProjectManager::GetInstance()
{
	if (instance == nullptr) instance = new ProjectManager();
	return instance;
}

ProjectManager::~ProjectManager()
{
	//delete dao;
	delete menu;

	instance = nullptr;
}

ProjectDAO * ProjectManager::GetProjectDAO()
{
	return dao;
}

ShowMenu * ProjectManager::GetShowMenu()
{
	return menu;
}

