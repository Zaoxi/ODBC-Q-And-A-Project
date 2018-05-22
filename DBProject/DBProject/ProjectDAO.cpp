#include <stdio.h>
#include <iostream>
#include <list>
#include <cstring>
#include "ProjectDAO.h"

using namespace std;

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

bool ProjectDAO::GetbIsConnected()
{
	return bisConnected;
}

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
	nullAreaQuetionJoin nullData;

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
	UserData userData;
	nullUserData nullUserData;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char *)query, "SELECT U.USER_DOMAIN_NUM, U.USER_ID, U.USER_JOIN_DATE, U.USER_EMAIL, U.USER_JOB FROM USERS AS U, AREA AS A, QUESTION AS Q WHERE A.AREA_BIG_CLASS = '%s' AND A.AREA_SUB_CLASS = '%s' AND Q.QUE_BIG_CLASS = A.AREA_BIG_CLASS AND Q.QUE_SUB_CLASS = A.AREA_SUB_CLASS AND U.USER_DOMAIN_NUM = Q.QUE_DOMAIN_NUM AND U.USER_ID = Q.QUE_ID", bigClass, subClass);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, &(userData.domainNum), LENGTH_DOMAIN_NUM, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, &(userData.userID), LENGTH_ID, NULL);
		SQLBindCol(hStmt, 3, SQL_C_CHAR, &(userData.userJoinDate), LENGTH_DATE, &(nullUserData.nullJoinDate));
		SQLBindCol(hStmt, 4, SQL_C_CHAR, &(userData.userEmail), LENGTH_EMAIL, &(nullUserData.userEmail));
		SQLBindCol(hStmt, 5, SQL_C_CHAR, &(userData.userJob), LENGTH_JOB, &(nullUserData.userJob));

		printf("%-10s %-15s %-10s %-20s %-10s\n", "도메인번호", "아이디", "가입일", "이메일", "직업");
		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-10s %-15s %-10s %-20s ", userData.domainNum, userData.userID, userData.userJoinDate, userData.userEmail);
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
	nullResponse nullResData;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char *)query, "SELECT Q.QUE_NUM, R.RES_DOMAIN_NUM, R.RES_ID, R.RES_DATE, R.RES_CONTENTS FROM AREA AS A, QUESTION AS Q, RESPOND AS QR, RESPONSE AS R WHERE A.AREA_BIG_CLASS = '%s' AND A.AREA_SUB_CLASS = '%s' AND Q.QUE_BIG_CLASS = A.AREA_BIG_CLASS AND Q.QUE_SUB_CLASS = A.AREA_SUB_CLASS AND QR.QUE_NUM = Q.QUE_NUM AND R.RES_NUM = QR.RES_NUM", bigClass, subClass);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_CHAR, &(resData.queNum), LENGTH_QUENUM, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, &(resData.domainNum), LENGTH_DOMAIN_NUM, NULL);
		SQLBindCol(hStmt, 3, SQL_C_CHAR, &(resData.ID), LENGTH_ID, &(nullResData.ID));
		SQLBindCol(hStmt, 4, SQL_C_CHAR, &(resData.date), LENGTH_DATE, &(nullResData.data));
		SQLBindCol(hStmt, 5, SQL_C_CHAR, &(resData.contents), LENGTH_CONTENTS, NULL);

		
		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-10s %-10s %-15s %-10s\n", "질문번호", "도메인번호", "ID", "작성일");
			printf("%-10s %-10s ", resData.queNum, resData.domainNum);
			if (nullResData.ID == SQL_NULL_DATA)
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