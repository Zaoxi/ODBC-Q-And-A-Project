#pragma once
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>

class ProjectDAO
{
private:
	bool isConnected = false;
	SQLHENV hEnv = NULL;
	SQLHDBC hDbc = NULL;
	// DB �����ϴ� �޼ҵ�, �����ڿ��� ȣ��
	bool DBConnect();
	// DB ���� �����ϴ� �޼ҵ�, �Ҹ��ڿ��� ȣ��
	void DBDisconnect();

public:
	ProjectDAO();
	~ProjectDAO();
};