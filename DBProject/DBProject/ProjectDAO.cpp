#include <cstdio>
#include <iostream>
#include <list>
#include <cstring>
#include "ProjectDAO.h"

using namespace std;

#pragma region DBConnect, 생성자 관련 메소드

bool ProjectDAO::DBConnect()
{
	SQLRETURN Ret;

	// Allocate the Environment Handle (hEnv)
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
	{
		return false;
	}

	// Set Attributes of the Environment Handle (hEnv)
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		return false;
	}

	// Allocate the Connection Handle(hDbc)
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
	{
		return false;
	}

	//Connect to the Server with ODBC
	Ret = SQLConnect(hDbc, (SQLCHAR *)"DB01_Server", SQL_NTS, (SQLCHAR *)"DB01", SQL_NTS, (SQLCHAR *)"1234", SQL_NTS);
	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO))
	{
		return false;
	}

	return true;
}

void ProjectDAO::DBDisconnect()
{
	SQLDisconnect(hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

void ProjectDAO::InitializeAreaCount()
{
	list<CheckArea*> * checkAreaList = new list<CheckArea*>();
	list<CheckArea*>::iterator iter;
	SQLHSTMT hStmt;
	CheckArea tempArea;

	// question 개수 알아낸 후, 다시 update
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char *)query, "SELECT QUE_BIG_CLASS, QUE_SUB_CLASS, COUNT(QUE_NUM) FROM QUESTION GROUP BY QUE_BIG_CLASS, QUE_SUB_CLASS");
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, &tempArea.bigClass, LENGTH_BIGCLASS, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, &tempArea.subClass, LENGTH_SUBCLASS, NULL);
		SQLBindCol(hStmt, 3, SQL_C_CHAR, &tempArea.countQue, LENGTH_COUNTQUESTION, NULL);

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			CheckArea * areaData = new CheckArea();
			strcpy((char*)areaData->bigClass, (char*)tempArea.bigClass);
			strcpy((char*)areaData->subClass, (char*)tempArea.subClass);
			strcpy((char*)areaData->countQue, (char*)tempArea.countQue);
			checkAreaList->push_back(areaData);
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	iter = checkAreaList->begin();
	while (iter != checkAreaList->end())
	{
		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
		{
			sprintf((char *)query, "UPDATE AREA SET COUNT_QUERY = %s WHERE AREA_BIG_CLASS = '%s' AND AREA_SUB_CLASS = '%s'", (*iter)->countQue, (*iter)->bigClass, (*iter)->subClass);
			SQLExecDirect(hStmt, query, SQL_NTS);

			delete (*iter);
			iter++;

			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		}
	}

	delete checkAreaList;
}

ProjectDAO::ProjectDAO()
{
	bisConnected = DBConnect();
	// DB 연결 성공
	if (bisConnected)
	{
		InitializeAreaCount();
		printf("Connection Success!!\n");
	} // DB 연결 실패
	else
	{
		printf("Connection Fail!!\n");
	}
}

ProjectDAO::~ProjectDAO()
{
	if (bisConnected)
	{
		DBDisconnect();
		printf("Disconnecting...\n");
	}
}

#pragma endregion

bool ProjectDAO::GetbIsConnected()
{
	return bisConnected;
}

#pragma region AREA 테이블 관련 작업

void ProjectDAO::PrintAllArea()
{
	SQLHSTMT hStmt;

	SQLSMALLINT colCount = -1;
	SQLCHAR areaData[30][100];
	SQLINTEGER nullData[30];

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char *)query, "SELECT * FROM AREA");
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLNumResultCols(hStmt, &colCount);

		for (int i = 0; i < colCount; i++)
		{
			// AREA CONTENTS 따로 처리
			if (i < 2)
			{
				SQLBindCol(hStmt, i + 1, SQL_C_CHAR, areaData[i], 100, &nullData[i]);
			}
			else if (i > 2)
			{
				SQLBindCol(hStmt, i + 1, SQL_C_CHAR, areaData[i - 1], 100, &nullData[i - 1]);
			}

		}
		SQLBindCol(hStmt, 3, SQL_C_CHAR, areaData[colCount - 1], 100, &nullData[colCount - 1]);

		printf("%-10s %-10s %-10s %-10s %-10s \n", "대분류", "소분류", "질문 수", "전문가ID", "도메인번호");
		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			for (int i = 0; i < colCount - 1; i++)
			{
				if (nullData[i] == SQL_NULL_DATA)
				{
					printf("%-10s ", "NULL");
				}
				else
				{
					printf("%-10s ", areaData[i]);
				}
			}

			printf("\n");

			if (nullData[colCount - 1] == SQL_NULL_DATA)
			{
				printf("%s ", "NULL");
			}
			else
			{
				printf("%s ", areaData[colCount - 1]);
			}

			printf("\n\n");
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::PrintQuestionsInSeletedArea(char * bigClass, char * subClass)
{
	SQLHSTMT hStmt;

	AreaQuestionJoin data;
	nullAreaQuestionJoin nullData;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char *)query, "SELECT Q.QUE_NUM, Q.QUE_DOMAIN_NUM, Q.QUE_ID, Q.QUE_DATE, Q.QUE_TITLE, Q.QUE_CONTENTS FROM AREA AS A, QUESTION AS Q WHERE (Q.QUE_BIG_CLASS = A.AREA_BIG_CLASS) AND (Q.QUE_SUB_CLASS = A.AREA_SUB_CLASS) AND (A.AREA_BIG_CLASS = '%s' AND A.AREA_SUB_CLASS = '%s')", bigClass, subClass);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, data.queNum, LENGTH_QUENUM, &nullData.nullQueNum);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, data.queDomainNum, LENGTH_DOMAIN_NUM, &nullData.nullQueDomainNum);
		SQLBindCol(hStmt, 3, SQL_C_CHAR, data.queID, LENGTH_ID, &nullData.nullQueID);
		SQLBindCol(hStmt, 4, SQL_C_CHAR, data.queDate, LENGTH_DATE, &nullData.nullQueDate);
		SQLBindCol(hStmt, 5, SQL_C_CHAR, data.queTitle, LENGTH_TITLE, &nullData.nullQueTitle);
		SQLBindCol(hStmt, 6, SQL_C_CHAR, data.queContents, LENGTH_CONTENTS, &nullData.nullQueContents);

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-10s %-10s %-15s %-10s\n", "질문번호", "도메인번호", "아이디", "작성일");
			printf("%-10s %-10s ", data.queNum, data.queDomainNum);
			if (nullData.nullQueID == SQL_NULL_DATA)
			{
				printf("%-15s ", "NULL");
			}
			else
			{
				printf("%-15s ", data.queID);
			}
			printf("%-10s \n", data.queDate);
			printf("Title : %s \n\n", data.queTitle);
			printf("%s\n\n", data.queContents);
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::PrintUsersInSelectedArea(char * bigClass, char * subClass)
{
	SQLHSTMT hStmt;
	USERS userData;
	NULLUSERS nullUserData;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char *)query, "SELECT D.DOMAIN_NAME, U.USER_ID, U.USER_JOIN_DATE, U.USER_EMAIL, U.USER_JOB FROM USERS AS U, AREA AS A, QUESTION AS Q, DOMAIN AS D WHERE A.AREA_BIG_CLASS = '%s' AND A.AREA_SUB_CLASS = '%s' AND Q.QUE_BIG_CLASS = A.AREA_BIG_CLASS AND Q.QUE_SUB_CLASS = A.AREA_SUB_CLASS AND U.USER_DOMAIN_NUM = Q.QUE_DOMAIN_NUM AND U.USER_ID = Q.QUE_ID AND U.USER_DOMAIN_NUM = D.DOMAIN_NUM", bigClass, subClass);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, &(userData.domainName), LENGTH_DOMAIN_NAME, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, &(userData.userID), LENGTH_ID, NULL);
		SQLBindCol(hStmt, 3, SQL_C_CHAR, &(userData.userJoinDate), LENGTH_DATE, &(nullUserData.joinDate));
		SQLBindCol(hStmt, 4, SQL_C_CHAR, &(userData.userEmail), LENGTH_EMAIL, &(nullUserData.userEmail));
		SQLBindCol(hStmt, 5, SQL_C_CHAR, &(userData.userJob), LENGTH_JOB, &(nullUserData.userJob));

		printf("%-20s %-15s %-10s %-25s %-10s\n", "도메인이름", "아이디", "가입일", "이메일", "직업");
		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-20s %-15s %-10s %-25s ", userData.domainName, userData.userID, userData.userJoinDate, userData.userEmail);
			if (nullUserData.userJob == SQL_NULL_DATA)
			{
				printf("NULL ");
			}
			else
			{
				printf("%-10s ", userData.userJob);
			}
			printf("\n");
		}
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::PrintAnswersInSelectedArea(char * bigClass, char * subClass)
{
	SQLHSTMT hStmt;
	Response resData;
	NULLRESPONSE nullResData;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char *)query, "SELECT Q.QUE_NUM, R.RES_DOMAIN_NUM, R.RES_ID, R.RES_DATE, R.RES_CONTENTS FROM AREA AS A, QUESTION AS Q, RESPOND AS QR, RESPONSE AS R WHERE A.AREA_BIG_CLASS = '%s' AND A.AREA_SUB_CLASS = '%s' AND Q.QUE_BIG_CLASS = A.AREA_BIG_CLASS AND Q.QUE_SUB_CLASS = A.AREA_SUB_CLASS AND QR.QUE_NUM = Q.QUE_NUM AND R.RES_NUM = QR.RES_NUM", bigClass, subClass);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, &(resData.queNum), LENGTH_QUENUM, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, &(resData.domainNum), LENGTH_DOMAIN_NUM, NULL);
		SQLBindCol(hStmt, 3, SQL_C_CHAR, &(resData.ID), LENGTH_ID, &(nullResData.resID));
		SQLBindCol(hStmt, 4, SQL_C_CHAR, &(resData.date), LENGTH_DATE, &(nullResData.resDate));
		SQLBindCol(hStmt, 5, SQL_C_CHAR, &(resData.contents), LENGTH_CONTENTS, NULL);


		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-10s %-10s %-15s %-10s\n", "질문번호", "도메인번호", "ID", "작성일");
			printf("%-10s %-10s ", resData.queNum, resData.domainNum);
			if (nullResData.resID == SQL_NULL_DATA)
			{
				printf("NULL ");
			}
			else
			{
				printf("%-15s ", resData.ID);
			}
			printf("%-10s \n %-10s \n", resData.date, resData.contents);
		}
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::PrintResponseUsersInSelectedArea(char * bigClass, char * subClass)
{
	SQLHSTMT hStmt;
	USERS user;
	NULLUSERS nullUser;
	char resNum[LENGTH_QUENUM];

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char *)query, "SELECT R.RES_NUM, D.DOMAIN_NAME, U.USER_ID, U.USER_JOIN_DATE, U.USER_EMAIL, U.USER_JOB FROM AREA AS A, QUESTION AS Q, RESPOND AS QR, RESPONSE AS R, USERS AS U WHERE A.AREA_BIG_CLASS = '%s' AND A.AREA_SUB_CLASS = '%s' AND Q.QUE_BIG_CLASS = A.AREA_BIG_CLASS AND Q.QUE_SUB_CLASS = A.AREA_SUB_CLASS AND QR.QUE_NUM = Q.QUE_NUM AND R.RES_NUM = QR.RES_NUM AND U.USER_DOMAIN_NUM = R.RES_DOMAIN_NUM AND U.USER_ID = R.RES_ID AND U.USER_DOMAIN_NUM = D.DOMAIN_NUM", bigClass, subClass);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, &resNum, LENGTH_DOMAIN_NUM, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, &(user.domainName), LENGTH_DOMAIN_NAME, NULL);
		SQLBindCol(hStmt, 3, SQL_C_CHAR, &(user.userID), LENGTH_ID, NULL);
		SQLBindCol(hStmt, 4, SQL_C_CHAR, &(user.userJoinDate), LENGTH_DATE, &(nullUser.joinDate));
		SQLBindCol(hStmt, 5, SQL_C_CHAR, &(user.userEmail), LENGTH_EMAIL, &(nullUser.userEmail));
		SQLBindCol(hStmt, 6, SQL_C_CHAR, &(user.userJob), LENGTH_JOB, &(nullUser.userJob));

		printf("%-10s %-10s %-15s %-10s %-25s %-10s\n", "답변번호", "도메인이름", "아이디", "가입일", "이메일", "직업");
		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-10s %-10s %-15s %-10s %-25s ", resNum, user.domainName, user.userID, user.userJoinDate, user.userEmail);
			if (nullUser.joinDate == SQL_NULL_DATA)
			{
				printf("NULL ");
			}
			else
			{
				printf("%-10s ", user.userJob);
			}
			printf("\n");
		}
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

#pragma endregion

void ProjectDAO::ExecuteSelectSQL()
{
	SQLHSTMT hStmt;
	SQLSMALLINT colCount = -1;
	SQLCHAR data[30][100];
	SQLINTEGER nullData[30];
	printf("SQL >> ");
	getchar();
	fgets((char*)query, 100, stdin);

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLNumResultCols(hStmt, &colCount);

		for (int i = 0; i < colCount; i++)
		{
			SQLBindCol(hStmt, i + 1, SQL_C_CHAR, data[i], 100, &nullData[i]);
		}
		SQLRETURN re = SQLFetch(hStmt);
		while (re != SQL_NO_DATA && re != SQL_NULL_DATA)
		{
			for (int i = 0; i < colCount; i++)
			{
				if (nullData[i] == SQL_NULL_DATA)
				{
					printf("%-10s ", "NULL");
				}
				else
				{
					printf("%-10s ", data[i]);
				}
			}

			printf("\n");
			re = SQLFetch(hStmt);
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

#pragma region QUESTION 테이블 관련 작업

void ProjectDAO::PrintQuestionUsingTitle(char * title)
{
	SQLHSTMT hStmt;
	list<QUESTION*> * question = new list<QUESTION*>();
	RESPONSE * response = new RESPONSE();
	QUESTION * tempQue = new QUESTION();
	list<NULLQUESTION*> * nullQue = new list<NULLQUESTION*>();
	NULLQUESTION * nullTempQue;
	NULLRESPONSE * nullRes = new NULLRESPONSE();

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT Q.QUE_NUM, Q.QUE_ID, D.DOMAIN_NAME, Q.QUE_DATE, Q.QUE_TITLE, Q.QUE_CONTENTS FROM QUESTION AS Q, DOMAIN AS D WHERE Q.QUE_TITLE LIKE '%%%s%%' AND Q.QUE_DOMAIN_NUM = D.DOMAIN_NUM", title);
		SQLExecDirect(hStmt, query, SQL_NTS);

		while (true)
		{
			tempQue = new QUESTION();
			nullTempQue = new NULLQUESTION();

			SQLBindCol(hStmt, 1, SQL_C_CHAR, (tempQue->queNum), LENGTH_QUENUM, NULL);
			SQLBindCol(hStmt, 2, SQL_C_CHAR, (tempQue->queID), LENGTH_ID, &(nullTempQue->queID));
			SQLBindCol(hStmt, 3, SQL_C_CHAR, (tempQue->queDomain), LENGTH_DOMAIN_NUM, NULL);
			SQLBindCol(hStmt, 4, SQL_C_CHAR, (tempQue->queDate), LENGTH_DATE, &(nullTempQue->queDate));
			SQLBindCol(hStmt, 5, SQL_C_CHAR, (tempQue->queTitle), LENGTH_TITLE, NULL);
			SQLBindCol(hStmt, 6, SQL_C_CHAR, (tempQue->queContents), LENGTH_CONTENTS, NULL);

			if (SQLFetch(hStmt) == SQL_NO_DATA)
			{
				delete tempQue;
				delete nullTempQue;
				break;
			}

			question->push_back(tempQue);
			nullQue->push_back(nullTempQue);
		}
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	list<QUESTION*>::iterator queIter = question->begin();
	list<NULLQUESTION*>::iterator nullQueIter = nullQue->begin();

	while (queIter != question->end())
	{
		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
		{
			sprintf((char*)query, "SELECT R.RES_NUM, R.RES_ID, D.DOMAIN_NAME, R.RES_DATE, R.RES_CONTENTS FROM RESPONSE AS R, RESPOND AS QR, DOMAIN AS D WHERE QR.QUE_NUM = %s AND R.RES_NUM = QR.RES_NUM AND R.RES_DOMAIN_NUM = D.DOMAIN_NUM", (*queIter)->queNum);
			SQLExecDirect(hStmt, query, SQL_NTS);

			SQLBindCol(hStmt, 1, SQL_C_CHAR, (response->resNum), LENGTH_QUENUM, NULL);
			SQLBindCol(hStmt, 2, SQL_C_CHAR, (response->resID), LENGTH_ID, &(nullRes->resID));
			SQLBindCol(hStmt, 3, SQL_C_CHAR, (response->resDomain), LENGTH_DOMAIN_NAME, NULL);
			SQLBindCol(hStmt, 4, SQL_C_CHAR, (response->resDate), LENGTH_DATE, &(nullRes->resDate));
			SQLBindCol(hStmt, 5, SQL_C_CHAR, (response->resContents), LENGTH_CONTENTS, NULL);
			printf("<질문>\n\n");
			printf("%-5s %-20s %-20s %-15s\n", "Q.NUM", "Q.ID", "Q.DOMAIN", "Q.DATE");
			printf("%-5s ", (*queIter)->queNum);
			if ((*nullQueIter)->queID == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", (*queIter)->queID);
			}
			printf("%-20s ", (*queIter)->queDomain);
			if ((*nullQueIter)->queDate == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", (*queIter)->queDate);
			}
			printf("\nTITLE : %s\n\n", (*queIter)->queTitle);
			printf("%s\n\n\n", (*queIter)->queContents);


			while (SQLFetch(hStmt) != SQL_NO_DATA)
			{
				printf("<답변>\n\n");
				printf("%-5s %-20s %-20s %-15s\n", "R.NUM", "R.ID", "R.DOMAIN", "R.DATE");

				printf("%-5s ", response->resNum);

				if (nullRes->resID == SQL_NULL_DATA)
				{
					printf("%-20s ", "NULL");
				}
				else
				{
					printf("%-20s ", response->resID);
				}
				printf("%-20s ", response->resDomain);

				if (nullRes->resDate == SQL_NULL_DATA)
				{
					printf("%-15s\n", "NULL");
				}
				else
				{
					printf("%-15s\n", response->resDate);
				}
				printf("\n%s\n\n", response->resContents);
			}

			queIter++;
			nullQueIter++;
			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		}
	}

	queIter = question->begin();
	nullQueIter = nullQue->begin();

	while (queIter != question->end())
	{
		delete (*queIter);
		delete (*nullQueIter);

		queIter++;
		nullQueIter++;
	}

	delete question;
	delete nullQue;
}

void ProjectDAO::PrintQuestionUsingContents(char * contents)
{
	SQLHSTMT hStmt;
	list<QUESTION*> * question = new list<QUESTION*>();
	RESPONSE response;
	QUESTION * tempQue;
	list<NULLQUESTION*> * nullQue = new list<NULLQUESTION*>();
	NULLQUESTION * nullTempQue;
	NULLRESPONSE nullRes;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT Q.QUE_NUM, Q.QUE_ID, D.DOMAIN_NAME, Q.QUE_DATE, Q.QUE_TITLE, Q.QUE_CONTENTS FROM QUESTION AS Q, DOMAIN AS D WHERE Q.QUE_CONTENTS LIKE '%%%s%%' AND Q.QUE_DOMAIN_NUM = D.DOMAIN_NUM", contents);
		SQLExecDirect(hStmt, query, SQL_NTS);

		while (true)
		{
			tempQue = new QUESTION();
			nullTempQue = new NULLQUESTION();

			SQLBindCol(hStmt, 1, SQL_C_CHAR, (tempQue->queNum), LENGTH_QUENUM, NULL);
			SQLBindCol(hStmt, 2, SQL_C_CHAR, (tempQue->queID), LENGTH_ID, &(nullTempQue->queID));
			SQLBindCol(hStmt, 3, SQL_C_CHAR, (tempQue->queDomain), LENGTH_DOMAIN_NUM, NULL);
			SQLBindCol(hStmt, 4, SQL_C_CHAR, (tempQue->queDate), LENGTH_DATE, &(nullTempQue->queDate));
			SQLBindCol(hStmt, 5, SQL_C_CHAR, (tempQue->queTitle), LENGTH_TITLE, NULL);
			SQLBindCol(hStmt, 6, SQL_C_CHAR, (tempQue->queContents), LENGTH_CONTENTS, NULL);

			if (SQLFetch(hStmt) == SQL_NO_DATA)
			{
				delete tempQue;
				delete nullTempQue;
				break;
			}

			question->push_back(tempQue);
			nullQue->push_back(nullTempQue);
		}
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	list<QUESTION*>::iterator queIter = question->begin();
	list<NULLQUESTION*>::iterator nullQueIter = nullQue->begin();

	while (queIter != question->end())
	{
		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
		{
			sprintf((char*)query, "SELECT R.RES_NUM, R.RES_ID, D.DOMAIN_NAME, R.RES_DATE, R.RES_CONTENTS FROM RESPONSE AS R, RESPOND AS QR, DOMAIN AS D WHERE QR.QUE_NUM = %s AND QR.RES_NUM = R.RES_NUM AND R.RES_DOMAIN_NUM = D.DOMAIN_NUM", (*queIter)->queNum);
			SQLExecDirect(hStmt, query, SQL_NTS);

			SQLBindCol(hStmt, 1, SQL_C_CHAR, response.resNum, LENGTH_QUENUM, NULL);
			SQLBindCol(hStmt, 2, SQL_C_CHAR, response.resID, LENGTH_ID, &(nullRes.resID));
			SQLBindCol(hStmt, 3, SQL_C_CHAR, response.resDomain, LENGTH_DOMAIN_NAME, NULL);
			SQLBindCol(hStmt, 4, SQL_C_CHAR, response.resDate, LENGTH_DATE, &(nullRes.resDate));
			SQLBindCol(hStmt, 5, SQL_C_CHAR, response.resContents, LENGTH_CONTENTS, NULL);
			printf("<질문>\n\n");
			printf("%-5s %-20s %-20s %-15s\n", "Q.NUM", "Q.ID", "Q.DOMAIN", "Q.DATE");
			printf("%-5s ", (*queIter)->queNum);
			if ((*nullQueIter)->queID == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", (*queIter)->queID);
			}
			printf("%-20s ", (*queIter)->queDomain);
			if ((*nullQueIter)->queDate == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", (*queIter)->queDate);
			}
			printf("\nTITLE : %s\n\n", (*queIter)->queTitle);
			printf("%s\n\n\n", (*queIter)->queContents);

			while (SQLFetch(hStmt) != SQL_NO_DATA)
			{
				printf("<답변>\n\n");
				printf("%-5s %-20s %-20s %-15s\n", "R.NUM", "R.ID", "R.DOMAIN", "R.DATE");

				printf("%-5s ", response.resNum);

				if (nullRes.resID == SQL_NULL_DATA)
				{
					printf("%-20s ", "NULL");
				}
				else
				{
					printf("%-20s ", response.resID);
				}
				printf("%-20s ", response.resDomain);

				if (nullRes.resDate == SQL_NULL_DATA)
				{
					printf("%-15s\n", "NULL");
				}
				else
				{
					printf("%-15s\n", response.resDate);
				}
				printf("\n%s\n\n", response.resContents);
			}

			queIter++;
			nullQueIter++;
			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		}
	}

	queIter = question->begin();
	nullQueIter = nullQue->begin();

	while (queIter != question->end())
	{
		delete (*queIter);
		delete (*nullQueIter);

		queIter++;
		nullQueIter++;
	}

	delete question;
	delete nullQue;
}

#pragma endregion

#pragma region RESPONSE 테이블 관련 작업

void ProjectDAO::PrintResponseUsingContents(char * contents)
{
	SQLHSTMT hStmt;
	RESPONSE response;
	NULLRESPONSE nullRes;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT R.RES_NUM, R.RES_ID, D.DOMAIN_NAME, R.RES_DATE, R.RES_CONTENTS FROM RESPONSE AS R, DOMAIN AS D WHERE R.RES_CONTENTS LIKE '%%%s%%' AND R.RES_DOMAIN_NUM = D.DOMAIN_NUM", contents);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, response.resNum, LENGTH_QUENUM, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, response.resID, LENGTH_ID, &(nullRes.resID));
		SQLBindCol(hStmt, 3, SQL_C_CHAR, response.resDomain, LENGTH_DOMAIN_NUM, NULL);
		SQLBindCol(hStmt, 4, SQL_C_CHAR, response.resDate, LENGTH_DATE, &(nullRes.resDate));
		SQLBindCol(hStmt, 5, SQL_C_CHAR, response.resContents, LENGTH_CONTENTS, NULL);

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-5s %-20s %-20s %-15s\n", "R.NUM", "R.ID", "R.DOMAIN", "R.DATE");

			printf("%-5s ", response.resNum);

			if (nullRes.resID == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", response.resID);
			}
			printf("%-20s ", response.resDomain);

			if (nullRes.resDate == SQL_NULL_DATA)
			{
				printf("%-15s\n", "NULL");
			}
			else
			{
				printf("%-15s\n", response.resDate);
			}
			printf("\n%s\n\n", response.resContents);
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

#pragma endregion

#pragma region DOMAIN 테이블 관련 작업

void ProjectDAO::PrintDomainUsingName(char * name)
{
	SQLHSTMT hStmt;
	DOMAIN_ domain;
	NULLDOMAIN nullDomain;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT D.DOMAIN_NUM, D.DOMAIN_NAME, D.COMPANY_NAME FROM DOMAIN AS D WHERE D.DOMAIN_NAME LIKE '%%%s%%'", name);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, domain.domainNum, LENGTH_QUENUM, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, domain.domainName, LENGTH_DOMAIN_NAME, NULL);
		SQLBindCol(hStmt, 3, SQL_C_CHAR, domain.domainCompany, LENGTH_DOMAIN_COMPANY, &(nullDomain.domainCompany));

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-5s %-20s %-20s\n", "D.NUM", "D.NAME", "D.COMPANY");

			printf("%-5s %-20s ", domain.domainNum, domain.domainName);

			if (nullDomain.domainCompany == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", domain.domainCompany);
			}
			printf("\n");
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::PrintDomainUsingCompany(char * name)
{
	SQLHSTMT hStmt;
	DOMAIN_ domain;
	NULLDOMAIN nullDomain;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT D.DOMAIN_NUM, D.DOMAIN_NAME, D.COMPANY_NAME FROM DOMAIN AS D WHERE D.COMPANY_NAME LIKE '%%%s%%'", name);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, domain.domainNum, LENGTH_QUENUM, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, domain.domainName, LENGTH_DOMAIN_NAME, NULL);
		SQLBindCol(hStmt, 3, SQL_C_CHAR, domain.domainCompany, LENGTH_DOMAIN_COMPANY, &(nullDomain.domainCompany));

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-5s %-20s %-20s\n", "D.NUM", "D.NAME", "D.COMPANY");

			printf("%-5s %-20s ", domain.domainNum, domain.domainName);

			if (nullDomain.domainCompany == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", domain.domainCompany);
			}
			printf("\n");
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::PrintUsersAccordingDomain(char * domain)
{
	SQLHSTMT hStmt;
	USERS users;
	NULLUSERS nullUsers;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT U.USER_ID, U.USER_JOIN_DATE, U.USER_EMAIL, U.USER_JOB FROM USERS AS U, DOMAIN AS D WHERE D.DOMAIN_NAME LIKE '%%%s%%' AND D.DOMAIN_NUM = U.USER_DOMAIN_NUM", domain);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, users.userID, LENGTH_ID, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, users.userJoinDate, LENGTH_DATE, &(nullUsers.joinDate));
		SQLBindCol(hStmt, 3, SQL_C_CHAR, users.userEmail, LENGTH_EMAIL, &(nullUsers.userEmail));
		SQLBindCol(hStmt, 4, SQL_C_CHAR, users.userJob, LENGTH_JOB, &(nullUsers.userJob));
		printf("%-20s %-15s %-20s\n", "U.ID", "U.JOIN", "U.EMAIL");
		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-20s ", users.userID);

			if (nullUsers.joinDate == SQL_NULL_DATA)
			{
				printf("%-15s ", "NULL");
			}
			else
			{
				printf("%-15s ", users.userJoinDate);
			}
			
			if (nullUsers.userEmail == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", users.userEmail);
			}

			if (nullUsers.userJob == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", users.userJob);
			}

			printf("\n");
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

#pragma endregion


#pragma region USERS 테이블에 관한 작업

void ProjectDAO::PrintUsersUsingID(char * id)
{
	SQLHSTMT hStmt;
	USERS users;
	NULLUSERS nullUsers;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT D.DOMAIN_NAME, U.USER_ID, U.USER_JOIN_DATE, U.USER_EMAIL, U.USER_JOB FROM USERS AS U, DOMAIN AS D WHERE U.USER_ID LIKE '%%%s%%' AND U.USER_DOMAIN_NUM = D.DOMAIN_NUM", id);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, users.domainName, LENGTH_DOMAIN_NAME, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, users.userID, LENGTH_ID, NULL);
		SQLBindCol(hStmt, 3, SQL_C_CHAR, users.userJoinDate, LENGTH_DATE, &(nullUsers.joinDate));
		SQLBindCol(hStmt, 4, SQL_C_CHAR, users.userEmail, LENGTH_EMAIL, &(nullUsers.userEmail));
		SQLBindCol(hStmt, 5, SQL_C_CHAR, users.userJob, LENGTH_JOB, &(nullUsers.userJob));
		printf("%-20s %-20s %-15s %-20s\n", "D.NAME", "U.ID", "U.JOIN", "U.EMAIL");
		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-20s %-20s ", users.domainName, users.userID);

			if (nullUsers.joinDate == SQL_NULL_DATA)
			{
				printf("%-15s ", "NULL");
			}
			else
			{
				printf("%-15s ", users.userJoinDate);
			}

			if (nullUsers.userEmail == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", users.userEmail);
			}

			if (nullUsers.userJob == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", users.userJob);
			}

			printf("\n");
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::PrintUsersUsingDate(char * date)
{
	SQLHSTMT hStmt;
	USERS users;
	NULLUSERS nullUsers;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT D.DOMAIN_NAME, U.USER_ID, U.USER_JOIN_DATE, U.USER_EMAIL, U.USER_JOB FROM USERS AS U, DOMAIN AS D WHERE U.USER_JOIN_DATE LIKE '%%%s%%' AND U.USER_DOMAIN_NUM = D.DOMAIN_NUM", date);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, users.domainName, LENGTH_DOMAIN_NAME, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, users.userID, LENGTH_ID, NULL);
		SQLBindCol(hStmt, 3, SQL_C_CHAR, users.userJoinDate, LENGTH_DATE, &(nullUsers.joinDate));
		SQLBindCol(hStmt, 4, SQL_C_CHAR, users.userEmail, LENGTH_EMAIL, &(nullUsers.userEmail));
		SQLBindCol(hStmt, 5, SQL_C_CHAR, users.userJob, LENGTH_JOB, &(nullUsers.userJob));


		printf("%-20s %-20s %-15s %-20s\n", "D.NAME", "U.ID", "U.JOIN", "U.EMAIL");
		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-20s %-20s ", users.domainName, users.userID);

			if (nullUsers.joinDate == SQL_NULL_DATA)
			{
				printf("%-15s ", "NULL");
			}
			else
			{
				printf("%-15s ", users.userJoinDate);
			}

			if (nullUsers.userEmail == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", users.userEmail);
			}

			if (nullUsers.userJob == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", users.userJob);
			}
			printf("\n");
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::PrintUsersUsingJob(char * job)
{
	SQLHSTMT hStmt;
	USERS users;
	NULLUSERS nullUsers;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT D.DOMAIN_NAME, U.USER_ID, U.USER_JOIN_DATE, U.USER_EMAIL, U.USER_JOB FROM USERS AS U, DOMAIN AS D WHERE U.USER_JOB LIKE '%%%s%%' AND U.USER_DOMAIN_NUM = D.DOMAIN_NUM", job);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, users.domainName, LENGTH_DOMAIN_NAME, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, users.userID, LENGTH_ID, NULL);
		SQLBindCol(hStmt, 3, SQL_C_CHAR, users.userJoinDate, LENGTH_DATE, &(nullUsers.joinDate));
		SQLBindCol(hStmt, 4, SQL_C_CHAR, users.userEmail, LENGTH_EMAIL, &(nullUsers.userEmail));
		SQLBindCol(hStmt, 5, SQL_C_CHAR, users.userJob, LENGTH_JOB, &(nullUsers.userJob));

		printf("%-20s %-20s %-15s %-20s\n", "D.NAME", "U.ID", "U.JOIN", "U.EMAIL");

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-20s %-20s ", users.domainName, users.userID);

			if (nullUsers.joinDate == SQL_NULL_DATA)
			{
				printf("%-15s ", "NULL");
			}
			else
			{
				printf("%-15s ", users.userJoinDate);
			}

			if (nullUsers.userEmail == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", users.userEmail);
			}

			if (nullUsers.userJob == SQL_NULL_DATA)
			{
				printf("%-20s ", "NULL");
			}
			else
			{
				printf("%-20s ", users.userJob);
			}
			printf("\n");
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

#pragma endregion
