#pragma once
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
//#define LENGTH_BIGCLASS 20
//#define LENGTH_SMALLCLASS 20
//#define LENGTH_CONTENTS 100
//#define LENGTH_COUNTQUESTION 10
//#define LENGTH_EXPERTID 20
//#define LENGTH_EXPERTDOMAIN 10
//
//typedef struct _Area
//{
//	SQLCHAR bigClass[LENGTH_BIGCLASS];
//	SQLCHAR smallClass[LENGTH_SMALLCLASS];
//	SQLCHAR contents[LENGTH_CONTENTS];
//	SQLCHAR countQuestion[LENGTH_COUNTQUESTION];
//	SQLCHAR expertID[LENGTH_EXPERTID];
//	SQLCHAR expertDomainNum[LENGTH_EXPERTDOMAIN];
//} Area;
//typedef struct _nullArea
//{
//	SQLINTEGER bigClass;
//	SQLINTEGER smallClass;
//	SQLINTEGER contents;
//	SQLINTEGER coutQuestion;
//	SQLINTEGER expertID;
//	SQLINTEGER expertDomainNum;
//} nullArea;

class ProjectDAO
{
private:
	bool isConnected = false;
	SQLHENV hEnv = NULL;
	SQLHDBC hDbc = NULL;
	SQLCHAR query[100];
	// DB 연결하는 메소드, 생성자에서 호출
	bool DBConnect();
	// DB 연결 해제하는 메소드, 소멸자에서 호출
	void DBDisconnect();

public:
	ProjectDAO();
	~ProjectDAO();

	void PrintAllArea();
	
};