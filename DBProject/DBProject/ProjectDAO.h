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

#pragma region DAO 데이터 처리에 필요한 각종 구조체 정의
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
	void PrintQuestionsInSeletedArea(char * bigClass, char * subClass);
};