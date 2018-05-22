#pragma once
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>

#pragma region DAO 상수
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

#pragma region DAO 데이터 처리에 필요한 각종 구조체 정의
// Area 테이블의 QUERY_COUNT 속성을 초기화하는 구조체
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
	// DB 연결하는 메소드, 생성자에서 호출
	bool DBConnect();
	// DB 연결 해제하는 메소드, 소멸자에서 호출
	void DBDisconnect();

	// Area 테이블의 질문 개수 속성을 초기화 하는 메소드
	void InitializeAreaCount();
public:
	ProjectDAO();
	~ProjectDAO();
	bool GetbIsConnected();
	void PrintAllArea();
	// 선택된 분야의 질문을 출력, 2 Table Join
	void PrintQuestionsInSeletedArea(char * bigClass, char * subClass);
	// 선택된 분야의 유저 정보를 출력, 3 Table Join
	void PrintUsersInSelectedArea(char * bigClass, char * subClass);

	void PrintAnswersInSelectedArea(char * bigClass, char * subClass);
};