#pragma once
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#define LENGTH_QUENUM 10
#define LENGTH_ID 40
#define LENGTH_DOMAIN_NUM 10
#define LENGTH_DATE 15
#define LENGTH_TITLE 100
#define LENGTH_CONTENTS 4000

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

#pragma region DAO ������ ó���� �ʿ��� ���� ����ü ����
typedef struct _AreaQuestionJoin
{
	SQLCHAR queNum[LENGTH_QUENUM];
	SQLCHAR queID[LENGTH_ID];
	SQLCHAR	queDomainNum[LENGTH_DOMAIN_NUM];
	SQLCHAR queDate[LENGTH_DATE];
	SQLCHAR queTitle[LENGTH_TITLE];
	SQLCHAR queContents[LENGTH_CONTENTS];
} AreaQuestionJoin;

typedef struct _nullAreaQuestionJoin
{
	SQLINTEGER nullQueNum;
	SQLINTEGER nullQueID;
	SQLINTEGER nullQueDomainNum;
	SQLINTEGER nullQueDate;
	SQLINTEGER nullQueTitle;
	SQLINTEGER nullQueContents;
} nullAreaQuetionJoin;
#pragma endregion

class ProjectDAO
{
private:
	bool bisConnected = false;
	SQLHENV hEnv = NULL;
	SQLHDBC hDbc = NULL;
	SQLCHAR query[100];
	// DB �����ϴ� �޼ҵ�, �����ڿ��� ȣ��
	bool DBConnect();
	// DB ���� �����ϴ� �޼ҵ�, �Ҹ��ڿ��� ȣ��
	void DBDisconnect();

	// Area ���̺��� ���� ���� �Ӽ��� �ʱ�ȭ �ϴ� �޼ҵ�
	void InitializeAreaCount();
public:
	ProjectDAO();
	~ProjectDAO();
	bool GetbIsConnected();
	void PrintAllArea();
	void PrintQuestionsInSeletedArea(char * bigClass, char * subClass);
};