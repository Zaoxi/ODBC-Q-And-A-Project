#include <stdio.h>
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

ProjectDAO::ProjectDAO()
{
	bisConnected = DBConnect();
	// DB 연결 성공
	if (bisConnected)
	{
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
		printf("Disconnection Success!!\n");
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
				SQLBindCol(hStmt, i, SQL_C_CHAR, areaData[i], 100, &nullData[i]);
			}

		}
		SQLBindCol(hStmt, 3, SQL_C_CHAR, areaData[colCount - 1], 100, &nullData[colCount - 1]);


		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			for (int i = 0; i < colCount - 1; i++)
			{
				if (nullData[i] == SQL_NULL_DATA)
				{
					printf("%s", "NULL");
				}
				else
				{
					printf("%s", areaData[i]);
				}
			}

			printf("\n");

			if (nullData[colCount - 1] == SQL_NULL_DATA)
			{
				printf("%s", "NULL");
			}
			else
			{
				printf("%s", areaData[colCount - 1]);
			}

			printf("\n");
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
		SQLBindCol(hStmt, 2, SQL_C_CHAR, data.queDomainNum, LENGTH_QUENUM, &nullData.nullQueDomainNum);
		SQLBindCol(hStmt, 3, SQL_C_CHAR, data.queID, LENGTH_QUENUM, &nullData.nullQueID);
		SQLBindCol(hStmt, 4, SQL_C_CHAR, data.queDate, LENGTH_QUENUM, &nullData.nullQueDate);
		SQLBindCol(hStmt, 5, SQL_C_CHAR, data.queTitle, LENGTH_QUENUM, &nullData.nullQueTitle);
		SQLBindCol(hStmt, 6, SQL_C_CHAR, data.queContents, LENGTH_QUENUM, &nullData.nullQueContents);

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			printf("%-6s %-6s ", data.queNum, data.queDomainNum);
			if (nullData.nullQueID == SQL_NULL_DATA)
			{
				printf("%-6s ", "NULL");
			}
			else
			{
				printf("%-6s ", data.queID);
			}
			printf("%-6s \n", data.queDate);
			printf("Title : %s \n\n", data.queTitle);
			printf("%s\n\n", data.queContents);
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}