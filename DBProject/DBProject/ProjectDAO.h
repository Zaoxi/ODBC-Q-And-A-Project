#pragma once
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>

#pragma region DAO ���
#define LENGTH_QUENUM 10
#define LENGTH_ID 40
#define LENGTH_DOMAIN_NUM 10
#define LENGTH_DATE 15
#define LENGTH_TITLE 100
#define LENGTH_CONTENTS 4000

#define LENGTH_BIGCLASS 20
#define LENGTH_SUBCLASS 20
#define LENGTH_COUNTQUESTION 10

#define LENGTH_EMAIL 30
#define LENGTH_JOB 40
#pragma endregion

#pragma region DAO ������ ó���� �ʿ��� ���� ����ü ����
// Area ���̺��� QUERY_COUNT �Ӽ��� �ʱ�ȭ�ϴ� ����ü
typedef struct _checkArea
{
	SQLCHAR bigClass[LENGTH_BIGCLASS];
	SQLCHAR subClass[LENGTH_SUBCLASS];
	SQLCHAR countQue[LENGTH_COUNTQUESTION];
} CheckArea;

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

typedef struct _users
{
	SQLCHAR domainNum[LENGTH_DOMAIN_NUM];
	SQLCHAR userID[LENGTH_ID];
	SQLCHAR userJoinDate[LENGTH_DATE];
	SQLCHAR userEmail[LENGTH_EMAIL];
	SQLCHAR userJob[LENGTH_JOB];
} UserData;

typedef struct _nullusers
{
	SQLINTEGER nullJoinDate;
	SQLINTEGER userEmail;
	SQLINTEGER userJob;
} nullUserData;

typedef struct _Response
{
	SQLCHAR queNum[LENGTH_QUENUM];
	SQLCHAR domainNum[LENGTH_DOMAIN_NUM];
	SQLCHAR ID[LENGTH_ID];
	SQLCHAR date[LENGTH_DATE];
	SQLCHAR contents[LENGTH_CONTENTS];
} Response;

typedef struct _nullResponse
{
	SQLINTEGER ID;
	SQLINTEGER data;
} nullResponse;
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
	// ���õ� �о��� ������ ���, 2 Table Join
	void PrintQuestionsInSeletedArea(char * bigClass, char * subClass);
	// ���õ� �о��� ���� ������ ���, 3 Table Join
	void PrintUsersInSelectedArea(char * bigClass, char * subClass);

	void PrintAnswersInSelectedArea(char * bigClass, char * subClass);
};