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

#define LENGTH_DOMAIN_NAME 40
#define LENGTH_DOMAIN_COMPANY 40
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
} nullAreaQuestionJoin;

typedef struct _users
{
	SQLCHAR domainName[LENGTH_DOMAIN_NAME];
	SQLCHAR userID[LENGTH_ID];
	SQLCHAR userJoinDate[LENGTH_DATE];
	SQLCHAR userEmail[LENGTH_EMAIL];
	SQLCHAR userJob[LENGTH_JOB];
} USERS;

typedef struct _nullusers
{
	SQLINTEGER joinDate;
	SQLINTEGER userEmail;
	SQLINTEGER userJob;
} NULLUSERS;

typedef struct _question
{
	SQLCHAR queNum[LENGTH_QUENUM];
	SQLCHAR queID[LENGTH_ID];
	SQLCHAR queDomain[LENGTH_DOMAIN_NAME];
	SQLCHAR queDate[LENGTH_DATE];
	SQLCHAR queTitle[LENGTH_TITLE];
	SQLCHAR queContents[LENGTH_CONTENTS];
} QUESTION;

typedef struct _nullQuestion
{
	SQLINTEGER queID;
	SQLINTEGER queDate;
} NULLQUESTION;

typedef struct _response
{
	SQLCHAR resNum[LENGTH_QUENUM];
	SQLCHAR resID[LENGTH_ID];
	SQLCHAR resDomain[LENGTH_DOMAIN_NAME];
	SQLCHAR resDate[LENGTH_DATE];
	SQLCHAR resContents[LENGTH_CONTENTS];
} RESPONSE;

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
	SQLINTEGER resID;
	SQLINTEGER resDate;
} NULLRESPONSE;

typedef struct _domain
{
	SQLCHAR domainNum[LENGTH_DOMAIN_NUM];
	SQLCHAR domainName[LENGTH_DOMAIN_NAME];
	SQLCHAR domainCompany[LENGTH_DOMAIN_COMPANY];
} DOMAIN_;

typedef struct _nullDomain
{
	SQLINTEGER domainCompany;
} NULLDOMAIN;

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
	// 선택된 분야의 답변 정보 출력 , 3Table Join
	void PrintAnswersInSelectedArea(char * bigClass, char * subClass);
	// 선택된 분야에 답변한 유저 정보 출력, 4 Table Join
	void PrintResponseUsersInSelectedArea(char * bigClass, char * subClass);

	// SQL을 입력받아서 출력
	void ExecuteSelectSQL();

	// TITLE을 검색하여 원하는 질문과 답변 출력
	void PrintQuestionUsingTitle(char * title);
	// CONTENTS를 검색하여 원하는 질문과 답변 출력
	void PrintQuestionUsingContents(char * contents);

	// 내용을 검색하여 원하는 답변 출력
	void PrintResponseUsingContents(char * contents);

	// DOMAIN 이름으로 DOMAIN 검색
	void PrintDomainUsingName(char * name);
	// DOMAIN 회사이름으로 DOMAIN 검색
	void PrintDomainUsingCompany(char * name);
	// 검색된 DOMAIN에 등록된 모든 유저정보 검색
	void PrintUsersAccordingDomain(char * domain);

	// USERS 아이디 검색
	void PrintUsersUsingID(char * id);
	// USERS 가입일 검색
	void PrintUsersUsingDate(char * date);
	// USERS 직업 검색
	void PrintUsersUsingJob(char * job);

};