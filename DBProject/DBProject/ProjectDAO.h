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
#define LENGTH_AREA_CONTENTS 200

#define LENGTH_BIGCLASS 20
#define LENGTH_SUBCLASS 20
#define LENGTH_COUNTQUESTION 10

#define LENGTH_EMAIL 30
#define LENGTH_JOB 40

#define LENGTH_DOMAIN_NAME 40
#define LENGTH_DOMAIN_COMPANY 40
#pragma endregion

#pragma region DAO ������ ó���� �ʿ��� ���� ����ü ����
// Area ���̺��� QUERY_COUNT �Ӽ��� �ʱ�ȭ�ϴ� ����ü
typedef struct _checkArea
{
	SQLCHAR bigClass[LENGTH_BIGCLASS];
	SQLCHAR subClass[LENGTH_SUBCLASS];
	SQLCHAR countQue[LENGTH_COUNTQUESTION];
	SQLCHAR contents[LENGTH_AREA_CONTENTS];
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
	// DB �����ϴ� �޼ҵ�, �����ڿ��� ȣ��
	bool DBConnect();
	// DB ���� �����ϴ� �޼ҵ�, �Ҹ��ڿ��� ȣ��
	void DBDisconnect();

	// Area ���̺��� ���� ���� �Ӽ��� �ʱ�ȭ �ϴ� �޼ҵ�
	void InitializeAreaCount();
	// �ش� �о߰� �����ϴ��� ��ȯ
	bool bcheckArea(char * bigClass, char * subClass);
	// �ش� ID�� ���� ������ �����ϴ��� ��ȯ
	bool bcheckUser(char * ID, int domainNum);
	// DOMAIN ���̺��� ��ȸ�Ͽ� ��ȣ�� �Է¹޾Ƽ� ��ȯ
	int checkDomain();
	// QUESTION ���̺��� ���ο� QUE_NUM ��ȣ�� ��ȯ
	int checkQuestionKey();
	// RESPONSE ���̺��� ���ο� RES_NUM ��ȣ�� ��ȯ
	int checkResponseKey();
	// DOMAIN ���̺��� ���ο� DOMAIN_NUM ��ȣ�� ��ȯ
	int checkDomainKey();
	// �ش� DOMAIN_NUM�� ���� �������� �����ϴ��� ��ȯ
	bool bcheckDomain(int domainNum);
	// ��¥ ������ �´��� üũ�ϴ� �޼ҵ�
	bool bcheckDate(char * date);
	// �̸��� ������ �´��� üũ�ϴ� �޼ҵ�
	bool bcheckEmail(char * email);
	// ���ڿ� ���̰� �ּ� 3���� �̻����� �˻�
	bool bcheckString(char * string);
	// �ش� ������ȣ�� �����ϴ��� ��ȯ
	bool bcheckQuestionNum(int num);
	// �ش� �亯��ȣ�� �����ϴ��� ��ȯ
	bool bcheckResponseNum(int num);
public:
	ProjectDAO();
	~ProjectDAO();
	bool GetbIsConnected();
	void PrintAllArea();
	// ���õ� �о��� ������ ���, 2 Table Join
	void PrintQuestionsInSelectedArea();
	// ���õ� �о��� ���� ������ ���, 3 Table Join
	void PrintUsersInSelectedArea();
	// ���õ� �о��� �亯 ���� ��� , 3Table Join
	void PrintAnswersInSelectedArea();
	// ���õ� �о߿� �亯�� ���� ���� ���, 4 Table Join
	void PrintResponseUsersInSelectedArea();

	// SQL�� �Է¹޾Ƽ� ���
	void ExecuteSelectSQL();

	// TITLE�� �˻��Ͽ� ���ϴ� ������ �亯 ���
	void PrintQuestionUsingTitle(char * title);
	// CONTENTS�� �˻��Ͽ� ���ϴ� ������ �亯 ���
	void PrintQuestionUsingContents(char * contents);

	// ������ �˻��Ͽ� ���ϴ� �亯 ���
	void PrintResponseUsingContents(char * contents);

	// DOMAIN �̸����� DOMAIN �˻�
	void PrintDomainUsingName(char * name);
	// DOMAIN ȸ���̸����� DOMAIN �˻�
	void PrintDomainUsingCompany(char * name);
	// �˻��� DOMAIN�� ��ϵ� ��� �������� �˻�
	void PrintUsersAccordingDomain(char * domain);

	// USERS ���̵� �˻�
	void PrintUsersUsingID(char * id);
	// USERS ������ �˻�
	void PrintUsersUsingDate(char * date);
	// USERS ���� �˻�
	void PrintUsersUsingJob(char * job);

	// INSERT �޼ҵ�
	void InsertArea();
	void InsertQuestion();
	void InsertUsers();
	void InsertResponse();
	void InsertDomain();

	// DELETE �޼ҵ�
	void DeleteArea();
	void DeleteQuestion();
	void DeleteUsers();
	void DeleteResponse();
	void DeleteDomain();

	// UPDATE �޼ҵ�
	void UpdateArea();
	void UpdateQuestionID();
	void UpdateQuestionTitle();
	void UpdateQuestionContents();
	void UpdateResponseID();
	void UpdateResponseContents();
	void UpdateDomainName();
	void UpdateDomainCompany();
	void UpdateDomainParentDomain();
	void UpdateUsersEmail();
	void UpdateUsersJob();

};

/* �⺻���� ����
HSTMT hStmt;

if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
{
	sprintf((char*)query, "");
	SQLExecDirect(hStmt, query, SQL_NTS);
	SQLCloseCursor(hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}
*/