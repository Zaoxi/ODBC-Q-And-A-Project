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
	// DB 연결하는 메소드, 생성자에서 호출
	bool DBConnect();
	// DB 연결 해제하는 메소드, 소멸자에서 호출
	void DBDisconnect();

public:
	ProjectDAO();
	~ProjectDAO();
};