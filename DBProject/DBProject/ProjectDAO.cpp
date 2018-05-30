#include <cstdio>
#include <iostream>
#include <list>
#include <hash_map>
#include <cstring>
#include "ProjectDAO.h"

using namespace std;
using namespace stdext;

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

void ProjectDAO::PrintQuestionsInSeletedArea()
{
	SQLHSTMT hStmt;
	char bigClass[LENGTH_BIGCLASS];
	char subClass[LENGTH_SUBCLASS];
	AreaQuestionJoin data;
	nullAreaQuestionJoin nullData;

	while (true)
	{
		cout << "대분류 >> ";
		cin.getline(bigClass, LENGTH_BIGCLASS);
		cout << "소분류 >> ";
		cin.getline(subClass, LENGTH_SUBCLASS);

		if (bcheckArea(bigClass, subClass))
		{
			break;
		}
		else
		{
			cout << "해당 분야가 존재하지 않습니다." << endl;
		}
	}

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

void ProjectDAO::PrintUsersInSelectedArea()
{
	SQLHSTMT hStmt;
	char bigClass[LENGTH_BIGCLASS];
	char subClass[LENGTH_SUBCLASS];
	USERS userData;
	NULLUSERS nullUserData;

	while (true)
	{
		cout << "대분류 >> ";
		cin.getline(bigClass, LENGTH_BIGCLASS);
		cout << "소분류 >> ";
		cin.getline(subClass, LENGTH_SUBCLASS);

		if (bcheckArea(bigClass, subClass))
		{
			break;
		}
		else
		{
			cout << "해당 분야가 존재하지 않습니다." << endl;
		}
	}

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

void ProjectDAO::PrintAnswersInSelectedArea()
{
	SQLHSTMT hStmt;
	Response resData;
	NULLRESPONSE nullResData;
	char bigClass[LENGTH_BIGCLASS];
	char subClass[LENGTH_SUBCLASS];

	while (true)
	{
		cout << "대분류 >> ";
		cin.getline(bigClass, LENGTH_BIGCLASS);
		cout << "소분류 >> ";
		cin.getline(subClass, LENGTH_SUBCLASS);

		if (bcheckArea(bigClass, subClass))
		{
			break;
		}
		else
		{
			cout << "해당하는 분야가 존재하지 않습니다." << endl;
		}
	}

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

void ProjectDAO::PrintResponseUsersInSelectedArea()
{
	SQLHSTMT hStmt;
	USERS user;
	NULLUSERS nullUser;
	char bigClass[LENGTH_BIGCLASS];
	char subClass[LENGTH_SUBCLASS];
	char resNum[LENGTH_QUENUM];

	while (true)
	{
		cout << "대분류 >> ";
		cin.getline(bigClass, LENGTH_BIGCLASS);
		cout << "소분류 >> ";
		cin.getline(subClass, LENGTH_SUBCLASS);

		if (bcheckArea(bigClass, subClass))
		{
			break;
		}
		else
		{
			cout << "해당하는 분야가 존재하지 않습니다." << endl;
		}
	}

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

#pragma region CHECK 메소드

bool ProjectDAO::bcheckResponseNum(int num)
{
	HSTMT hStmt;
	bool isExist;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT RES_NUM FROM RESPONSE WHERE RES_NUM = %d", num);
		SQLExecDirect(hStmt, query, SQL_NTS);

		if (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			isExist = true;
		}
		else
		{
			isExist = false;
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	return isExist;
}

bool ProjectDAO::bcheckQuestionNum(int num)
{
	HSTMT hStmt;
	bool isExist;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT QUE_NUM FROM QUESTION WHERE QUE_NUM = %d", num);
		SQLExecDirect(hStmt, query, SQL_NTS);

		if (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			isExist = true;
		}
		else
		{
			isExist = false;
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	return isExist;
}

bool ProjectDAO::bcheckArea(char * bigClass, char * subClass)
{
	HSTMT hStmt;
	bool bisExist;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT * FROM AREA WHERE AREA_BIG_CLASS = '%s' AND AREA_SUB_CLASS = '%s'", bigClass, subClass);
		SQLExecDirect(hStmt, query, SQL_NTS);

		if (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			bisExist = true;
		}
		else
		{
			bisExist = false;
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	return bisExist;
}

bool ProjectDAO::bcheckUser(char * ID, int domainNum)
{
	HSTMT hStmt;
	bool bisExist;
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT * FROM USERS WHERE USER_DOMAIN_NUM = %d AND USER_ID = '%s'", domainNum, ID);
		SQLExecDirect(hStmt, query, SQL_NTS);

		if (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			bisExist = true;
		}
		else
		{
			bisExist = false;
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	return bisExist;
}

int ProjectDAO::checkQuestionKey()
{
	HSTMT hStmt;
	int maxKey = 0;
	int tempKey = 0;

	// 인덱스 검색
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT Q.QUE_NUM FROM QUESTION AS Q");
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_SLONG, &tempKey, LENGTH_QUENUM, NULL);

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			if (tempKey > maxKey)
			{
				maxKey = tempKey;
			}
		}
		maxKey++;
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	return maxKey;
}

int ProjectDAO::checkDomain()
{
	SQLHSTMT hStmt;
	hash_map<int, char*> * domain = new hash_map<int, char*>();
	char domainName[LENGTH_DOMAIN_NAME];
	int domainNum;

	// 도메인 검색
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT D.DOMAIN_NUM, D.DOMAIN_NAME FROM DOMAIN AS D");
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_SLONG, &domainNum, LENGTH_QUENUM, NULL);
		SQLBindCol(hStmt, 2, SQL_C_CHAR, domainName, LENGTH_DOMAIN_NAME, NULL);

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			char * tempName = new char[LENGTH_DOMAIN_NAME];
			strcpy(tempName, domainName);
			domain->insert(hash_map<int, char*>::value_type(domainNum, tempName));
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	while (true)
	{
		printf("%-10s %-20s\n", "D.NUM", "D.NAME");
		hash_map<int, char*>::iterator domainIter = domain->begin();
		while (domainIter != domain->end())
		{
			printf("%-10d %-20s\n", domainIter->first, domainIter->second);
			domainIter++;
		}

		cout << "등록할 사이트 번호 >> ";
		cin >> domainNum;
		while (cin.get() != '\n');

		domainIter = domain->lower_bound(domainNum);

		if (domainIter == domain->end())
		{
			cout << "등록되지 않은 사이트입니다." << endl;
			continue;
		}
		else
		{
			break;
		}
	}
	hash_map<int, char*>::iterator domainIter = domain->begin();
	while (domainIter != domain->end())
	{
		delete[]domainIter->second;
		domainIter++;
	}
	delete domain;
	return domainNum;
}

int ProjectDAO::checkResponseKey()
{
	HSTMT hStmt;
	int maxKey = 0;
	int tempKey = 0;

	// 인덱스 검색
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT RES_NUM FROM RESPONSE");
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_SLONG, &tempKey, LENGTH_QUENUM, NULL);

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			if (tempKey > maxKey)
			{
				maxKey = tempKey;
			}
		}
		maxKey++;
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	return maxKey;
}

bool ProjectDAO::bcheckDomain(int domainNum)
{
	HSTMT hStmt;
	bool bisExist;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT * FROM DOMAIN AS D WHERE D.DOMAIN_NUM = %d", domainNum);
		SQLExecDirect(hStmt, query, SQL_NTS);

		if (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			bisExist = true;
		}
		else
		{
			bisExist = false;
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	return bisExist;
}

int ProjectDAO::checkDomainKey()
{
	HSTMT hStmt;
	int maxKey = 0;
	int tempKey = 0;

	// 인덱스 검색
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT DOMAIN_NUM FROM DOMAIN");
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_SLONG, &tempKey, LENGTH_QUENUM, NULL);

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			if (tempKey > maxKey)
			{
				maxKey = tempKey;
			}
		}
		maxKey++;
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	return maxKey;
}

bool ProjectDAO::bcheckDate(char * date)
{
	int index = 0;
	int count = 0;
	// 처음 4자리 숫자 검사
	while (date[index] != '-')
	{
		if (date[index] < '0' || date[index] >'9' || count > 3)
		{
			return false;
		}
		count++;
		index++;
	}

	if (count != 3)
	{
		return false;
	}
	count = 0;
	index++;
	while (date[index] != '-')
	{
		if (date[index] < '0' || date[index] >'9' || count > 1)
		{
			return false;
		}
		index++;
	}
	if (count != 1)
	{
		return false;
	}
	count = 0;
	index++;
	while (date[index] != '-')
	{
		if (date[index] < '0' || date[index] >'9' || count > 1)
		{
			return false;
		}
		index++;
	}
	if (count != 1)
	{
		return false;
	}
	count = 0;
	return true;
}

bool ProjectDAO::bcheckEmail(char * email)
{
	int index = 0;
	int count = 0;
	while (email[index] != '@')
	{
		// @문자를 만나지 못하고 문자열이 끝나는 경우
		if (email[index] == '\0')
		{
			return false;
		}
		index++;
		count++;
	}
	// @ 앞의 글자 수가 3개 이하인 경우
	if (count < 4)
	{
		return false;
	}
	count = 0;
	index++;
	while (email[index] != '.')
	{
		if (email[index] == '\0')
		{
			return false;
		}
		index++;
		count++;
	}
	// . 앞의 글자 수가 2개 이하인 경우
	if (count < 2)
	{
		return false;
	}
	return true;
}

bool ProjectDAO::bcheckString(char * string)
{
	int index = 0;
	while (string[index] != '\0')
	{
		// 문자가 3글자 이상인 경우
		if (index > 2)
		{
			return true;
		}
		index++;
	}
	return false;
}

#pragma endregion

#pragma region INSERT 관련 작업

void ProjectDAO::InsertArea()
{
	SQLHSTMT hStmt;
	CheckArea newArea;
	while (true)
	{
		cout << "대분류 >> ";
		cin.getline((char*)newArea.bigClass, LENGTH_BIGCLASS);
		cout << "소분류 >> ";
		cin.getline((char*)newArea.subClass, LENGTH_SUBCLASS);

		if (bcheckArea((char*)newArea.bigClass, (char*)newArea.subClass))
		{
			cout << "해당 분야가 이미 존재합니다!" << endl;
		}
		else
		{
			break;
		}
	}
	cout << "분류 내용 >> ";
	cin.getline((char*)newArea.contents, LENGTH_AREA_CONTENTS);

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "INSERT INTO AREA VALUES('%s', '%s', '%s', 0, NULL, NULL)", newArea.bigClass, newArea.subClass, newArea.contents);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	cout << newArea.bigClass << " - " << newArea.subClass << " 분야가 등록되었습니다." << endl;
}

void ProjectDAO::InsertQuestion()
{
	SQLHSTMT hStmt;
	char bigClass[LENGTH_BIGCLASS];
	char subClass[LENGTH_SUBCLASS];
	int domainNum;
	QUESTION newQuestion;
	int maxKey;
	char select;

	maxKey = checkQuestionKey();

	domainNum = checkDomain();

	while (true)
	{
		cout << "대분야 >> ";
		cin.getline(bigClass, LENGTH_BIGCLASS);
		cout << "소분야 >> ";
		cin.getline(subClass, LENGTH_SUBCLASS);

		if (bcheckArea(bigClass, subClass))
		{
			break;
		}
		else
		{
			cout << "해당 분야가 존재하지 않습니다!" << endl;
		}
	}
	while (true)
	{
		cout << "ID가 비공개입니까?(Y/N) >> ";
		cin >> select;
		while (cin.get() != '\n');

		if (select == 'Y' || select == 'y')
		{
			while (true)
			{
				cout << "작성일(Y-M-D) >> ";
				cin.getline((char*)newQuestion.queDate, LENGTH_DATE);

				if (bcheckDate((char*)newQuestion.queDate))
				{
					break;
				}
				else
				{
					cout << "잘못된 형식의 입력입니다." << endl;
				}
			}
			while (true)
			{
				cout << "제목 >> ";
				cin.getline((char*)newQuestion.queTitle, LENGTH_TITLE);

				if (bcheckString((char*)newQuestion.queTitle))
				{
					break;
				}
				else
				{
					cout << "제목은 최소 3글자 이상 입력하셔야 합니다.";
				}
			}
			while (true)
			{
				cout << "내용 >> ";
				cin.getline((char*)newQuestion.queContents, LENGTH_CONTENTS);

				if (bcheckString((char*)newQuestion.queContents))
				{
					break;
				}
				else
				{
					cout << "내용은 최소 3글자 이상 입력하셔야 합니다.";
				}
			}
			if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
			{
				sprintf((char*)query, "INSERT INTO QUESTION VALUES(%d, '%s', '%s', NULL, %d, '%s', '%s', '%s')", maxKey, bigClass, subClass, domainNum, newQuestion.queDate, newQuestion.queTitle, newQuestion.queContents);
				SQLExecDirect(hStmt, query, SQL_NTS);

				SQLCloseCursor(hStmt);
				SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			}
			break;
		}
		else if (select == 'N' || select == 'n')
		{
			while (true)
			{
				cout << "ID >> ";
				cin.getline((char*)newQuestion.queID, LENGTH_ID);
				if (bcheckUser((char*)newQuestion.queID, domainNum))
				{
					break;
				}
				else
				{
					cout << "입력된 유저가 존재하지 않습니다." << endl;
				}
			}
			while (true)
			{
				cout << "작성일(Y-M-D) >> ";
				cin.getline((char*)newQuestion.queDate, LENGTH_DATE);

				if (bcheckDate((char*)newQuestion.queDate))
				{
					break;
				}
				else
				{
					cout << "잘못된 형식의 입력입니다." << endl;
				}
			}
			while (true)
			{
				cout << "제목 >> ";
				cin.getline((char*)newQuestion.queTitle, LENGTH_TITLE);

				if (bcheckString((char*)newQuestion.queTitle))
				{
					break;
				}
				else
				{
					cout << "제목은 최소 3글자 이상 입력하셔야 합니다.";
				}
			}
			while (true)
			{
				cout << "내용 >> ";
				cin.getline((char*)newQuestion.queContents, LENGTH_CONTENTS);

				if (bcheckString((char*)newQuestion.queContents))
				{
					break;
				}
				else
				{
					cout << "내용은 최소 3글자 이상 입력하셔야 합니다.";
				}
			}
			if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
			{
				sprintf((char*)query, "INSERT INTO QUESTION VALUES(%d, '%s', '%s', '%s', %d, '%s', '%s', '%s')", maxKey, bigClass, subClass, newQuestion.queID, domainNum, newQuestion.queDate, newQuestion.queTitle, newQuestion.queContents);
				SQLExecDirect(hStmt, query, SQL_NTS);

				SQLCloseCursor(hStmt);
				SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			}
			break;
		}
		else
		{
			cout << "잘못된 입력입니다." << endl << endl;
		}
	}


	cout << domainNum << "번 도메인에 " << maxKey << "번의 질문이 입력되었습니다." << endl;
	InitializeAreaCount();
}

void ProjectDAO::InsertResponse()
{
	SQLHSTMT hStmt;
	int domainNum;
	int maxKey = 0;
	int tempKey = 0;
	list<int> * queNum = new list<int>();
	RESPONSE response;
	int isFind = 0;
	char select;
	maxKey = checkResponseKey();

	// 모든 QUE_NUM을 list에 저장
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT Q.QUE_NUM FROM QUESTION AS Q");
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_SLONG, &tempKey, LENGTH_QUENUM, NULL);

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			queNum->push_back(tempKey);
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	// 도메인 번호 입력
	domainNum = checkDomain();

	while (true)
	{
		cout << "답변할 질문 번호 >> ";
		cin >> tempKey;
		while (cin.get() != '\n');
		list<int>::iterator queIter = queNum->begin();

		while (queIter != queNum->end())
		{
			if ((*queIter) == tempKey)
			{
				isFind = 1;
				break;
			}
			queIter++;
		}
		if (isFind == 0)
		{
			cout << "해당하는 질문이 존재하지 않습니다." << endl;
		}
		else
		{
			break;
		}
	}
	while (true)
	{
		cout << "ID가 비공개입니까?(Y/N) >> ";
		cin >> select;
		while (cin.get() != '\n');

		if (select == 'Y' || select == 'y')
		{
			while (true)
			{
				cout << "작성일(Y-M-D) >> ";
				cin.getline((char*)response.resDate, LENGTH_DATE);

				if (bcheckDate((char*)response.resDate))
				{
					break;
				}
				else
				{
					cout << "잘못된 형식의 입력입니다." << endl;
				}
			}
			while (true)
			{
				cout << "내용 >> ";
				cin.getline((char*)response.resContents, LENGTH_CONTENTS);
				
				if (bcheckString((char*)response.resContents))
				{
					break;
				}
				else
				{
					cout << "내용은 최소 3글자 이상이어야 합니다." << endl;
				}
			}
			if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
			{
				sprintf((char*)query, "INSERT INTO RESPONSE VALUES(%d, NULL, %d, '%s', '%s')", maxKey, domainNum, response.resDate, response.resContents);
				SQLExecDirect(hStmt, query, SQL_NTS);

				SQLCloseCursor(hStmt);
				SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			}
			break;
		}
		else if (select == 'N' || select == 'n')
		{
			while (true)
			{
				cout << "ID >> ";
				cin.getline((char*)response.resID, LENGTH_ID);
				if (bcheckUser((char*)response.resID, domainNum))
				{
					break;
				}
				else
				{
					cout << "입력된 유저가 존재하지 않습니다." << endl;
				}
			}

			while (true)
			{
				cout << "작성일(Y-M-D) >> ";
				cin.getline((char*)response.resDate, LENGTH_DATE);

				if (bcheckDate((char*)response.resDate))
				{
					break;
				}
				else
				{
					cout << "잘못된 형식의 입력입니다." << endl;
				}
			}
			while (true)
			{
				cout << "내용 >> ";
				cin.getline((char*)response.resContents, LENGTH_CONTENTS);

				if (bcheckString((char*)response.resContents))
				{
					break;
				}
				else
				{
					cout << "내용은 최소 3글자 이상이어야 합니다." << endl;
				}
			}
			cin.getline((char*)response.resContents, LENGTH_CONTENTS);

			if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
			{
				sprintf((char*)query, "INSERT INTO RESPONSE VALUES(%d, '%s', %d, '%s', '%s')", maxKey, response.resID, domainNum, response.resDate, response.resContents);
				SQLExecDirect(hStmt, query, SQL_NTS);

				SQLCloseCursor(hStmt);
				SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			}
			break;
		}
		else
		{
			cout << "잘못된 입력입니다." << endl << endl;
		}
	}
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "INSERT INTO RESPOND VALUES(%d, %d)", tempKey, maxKey);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	cout << tempKey << "번에 대한 답변 " << maxKey << "번이 등록되었습니다." << endl;

	delete queNum;
}

void ProjectDAO::InsertDomain()
{
	SQLHSTMT hStmt;
	DOMAIN_ domain;
	int maxKey = 0;
	char select;

	maxKey = checkDomainKey();

	while (true)
	{
		cout << "등록하는 사이트가 타 사이트의 하위 사이트 입니까?(Y/N) >> ";
		cin >> select;
		while (cin.get() != '\n');

		if (select == 'Y' || select == 'y' || select == 'N' || select == 'n')
		{
			break;
		}
		else
		{
			cout << "잘못된 입력입니다." << endl << endl;
		}
	}

	if (select == 'Y' || select == 'y')
	{
		while (true)
		{
			cout << "상위 사이트 번호 >> ";
			cin >> select;
			while (cin.get() != '\n');

			if (bcheckDomain(select))
			{
				break;
			}
			else
			{
				cout << "해당하는 도메인이 존재하지 않습니다." << endl;
			}
		}
		cout << "NAME >> ";
		cin.getline((char*)domain.domainName, LENGTH_DOMAIN_NAME);
		cout << "COMPANY >> ";
		cin.getline((char*)domain.domainCompany, LENGTH_DOMAIN_COMPANY);

		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
		{
			sprintf((char*)query, "INSERT INTO DOMAIN VALUES(%d, '%s', %d, '%s')", maxKey, domain.domainName, select, domain.domainCompany);
			SQLExecDirect(hStmt, query, SQL_NTS);

			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		}
	}
	else
	{
		cout << "NAME >> ";
		cin.getline((char*)domain.domainName, LENGTH_DOMAIN_NAME);
		cout << "COMPANY >> ";
		cin.getline((char*)domain.domainCompany, LENGTH_DATE);

		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
		{
			sprintf((char*)query, "INSERT INTO DOMAIN VALUES(%d, '%s', NULL, '%s')", maxKey, domain.domainName, domain.domainCompany);
			SQLExecDirect(hStmt, query, SQL_NTS);

			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		}
	}
}

void ProjectDAO::InsertUsers()
{
	SQLHSTMT hStmt;
	int domainNum;
	USERS newUser;

	domainNum = checkDomain();
	while (true)
	{
		cout << "ID >> ";
		cin.getline((char*)newUser.userID, LENGTH_ID);

		if (bcheckUser((char*)newUser.userID, domainNum))
		{
			cout << "해당 ID를 가진 유저가 이미 존재합니다." << endl;
		}
		else
		{
			break;
		}
	}

	while (true)
	{
		cout << "가입일(Y-M-D) >> ";
		cin >> newUser.userJoinDate;
		cin.get();
		if (bcheckDate((char*)newUser.userJoinDate))
		{
			break;
		}
		else
		{
			cout << "잘못된 형식의 입력입니다." << endl;
		}
	}
	while (true)
	{
		cout << "EMAIL >> ";
		cin >> newUser.userEmail;
		cin.get();

		if (bcheckEmail((char*)newUser.userEmail))
		{
			break;
		}
		else
		{
			cout << "잘못된 형식의 입력입니다." << endl;
		}
	}
	cout << "직업 >> ";
	cin >> newUser.userJob;
	cin.get();

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "INSERT INTO USERS VALUES(%d, '%s', '%s', '%s', '%s')", domainNum, newUser.userID, newUser.userJoinDate, newUser.userEmail, newUser.userJob);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	cout << newUser.userID << "이(가) 등록되었습니다." << endl;
}

#pragma endregion

#pragma region DELETE 관련 작업

void ProjectDAO::DeleteArea()
{
	HSTMT hStmt;
	char bigClass[LENGTH_BIGCLASS];
	char subClass[LENGTH_SUBCLASS];

	while (true)
	{
		cout << "대분류 >> ";
		cin.getline(bigClass, LENGTH_BIGCLASS);
		cout << "소분류 >> ";
		cin.getline(subClass, LENGTH_SUBCLASS);

		if (bcheckArea(bigClass, subClass))
		{
			break;
		}
		else
		{
			cout << "해당 분야는 존재하지 않습니다." << endl;
		}
	}

	// 분야 데이터를 가지고 RESPOND TABLE을 삭제
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "DELETE FROM AREA WHERE AREA_BIG_CLASS = '%s' AND AREA_SUB_CLASS = '%s'", bigClass, subClass);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::DeleteQuestion()
{
	HSTMT hStmt;
	int queNum;
	int tempNum;
	list<int> resNumList;

	while (true)
	{
		cout << "질문 번호 >> ";
		cin >> queNum;
		while (cin.get() != '\n');

		if (bcheckQuestionNum(queNum))
		{
			break;
		}
		else
		{
			cout << "해당 질문이 존재하지 않습니다." << endl;
		}
	}

	// 해당 질문의 답변 데이터 삭제
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT * RES_NUM FROM RESPOND WHERE QUE_NUM = %d", queNum);
		SQLExecDirect(hStmt, query, SQL_NTS);

		SQLBindCol(hStmt, 1, SQL_C_SLONG, &tempNum, LENGTH_QUENUM, NULL);

		while (SQLFetch(hStmt) != SQL_NO_DATA)
		{
			resNumList.push_back(tempNum);
		}

		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	list<int>::iterator resIter = resNumList.begin();
	while (resIter != resNumList.end())
	{
		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
		{
			sprintf((char*)query, "DELETE FROM RESPONSE WHERE RES_NUM = %d", *resIter);
			SQLExecDirect(hStmt, query, SQL_NTS);
			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		}
		resIter++;
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "DELETE FROM QUESTION WHERE QUE_NUM = %d", queNum);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	InitializeAreaCount();
}

void ProjectDAO::DeleteResponse()
{
	HSTMT hStmt;
	int resNum;

	while (true)
	{
		cout << "답변 번호 >> ";
		cin >> resNum;
		while (cin.get() != '\n');

		if (bcheckResponseNum(resNum))
		{
			break;
		}
		else
		{
			cout << "해당 답변이 존재하지 않습니다." << endl;
		}
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "DELETE FROM RESPONSE WHERE RES_NUM = %d", resNum);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::DeleteDomain()
{
	HSTMT hStmt;
	int domainNum;

	while (true)
	{
		cout << "도메인 번호 >> ";
		cin >> domainNum;
		while (cin.get() != '\n');

		if (bcheckDomain(domainNum))
		{
			break;
		}
		else
		{
			cout << "해당 도메인이 존재하지 않습니다." << endl;
		}
	}
	// 도메인에 존재하는 모든 유저정보 삭제
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "DELETE FROM USERS WHERE USER_DOMAIN_NUM = %d", domainNum);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	// 도메인 삭제
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "DELETE FROM DOMAIN WHERE DOMAIN_NUM = %d", domainNum);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::DeleteUsers()
{
	HSTMT hStmt;
	int domainNum;
	char ID[LENGTH_ID];

	while (true)
	{
		cout << "도메인 번호 >> ";
		cin >> domainNum;
		while (cin.get() != '\n');

		if (bcheckDomain(domainNum))
		{
			break;
		}
		else
		{
			cout << "해당 도메인이 존재하지 않습니다." << endl;
		}
	}

	while (true)
	{
		cout << "ID >> ";
		cin.getline(ID, LENGTH_ID);

		if (bcheckUser(ID, domainNum))
		{
			break;
		}
		else
		{
			cout << "해당 유저가 존재하지 않습니다." << endl;
		}
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "DELETE FROM USERS WHERE USER_DOMAIN_NUM = %d AND USER_ID = '%s'", domainNum, ID);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

#pragma endregion

#pragma region UPDATE 관련 작업

void ProjectDAO::UpdateArea()
{
	HSTMT hStmt;
	char srcBigClass[LENGTH_BIGCLASS];
	char srcSubClass[LENGTH_SUBCLASS];
	char dstBigClass[LENGTH_BIGCLASS];
	char dstSubClass[LENGTH_SUBCLASS];
	char dstAreaContents[LENGTH_AREA_CONTENTS];

	while (true)
	{
		cout << "대분류 >> ";
		cin.getline(srcBigClass, LENGTH_BIGCLASS);
		cout << "소분류 >> ";
		cin.getline(srcSubClass, LENGTH_SUBCLASS);
		
		if (bcheckArea(srcBigClass, srcSubClass))
		{
			break;
		}
		else
		{
			cout << "해당 분야가 존재하지 않습니다." << endl;
		}
	}
	cout << srcBigClass << " - " << srcSubClass << "을(를) 다음으로 변경" << endl;
	cout << "대분류 >> ";
	cin.getline(dstBigClass, LENGTH_BIGCLASS);
	cout << "소분류 >> ";
	cin.getline(dstSubClass, LENGTH_SUBCLASS);
	cout << "변경할 분류 내용 >> ";
	cin.getline(dstAreaContents, LENGTH_AREA_CONTENTS);

	// 새 AREA 삽입
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "INSERT INTO AREA VALUES('%s', '%s', '%s', 0, NULL, NULL)", dstBigClass, dstSubClass, dstAreaContents);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	// QUESTION AREA변경
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "UPDATE QUESTION SET QUE_BIG_CLASS = '%s', QUE_SUB_CLASS = '%s' WHERE QUE_BIG_CLASS = '%s' AND QUE_SUB_CLASS = '%s'", dstBigClass, dstSubClass, srcBigClass, srcSubClass);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	// 기존 AREA 삭제
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "DELETE FROM AREA WHERE AREA_BIG_CLASS = '%s' AND AREA_SUB_CLASS = '%s'", srcBigClass, srcSubClass);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	InitializeAreaCount();
}

void ProjectDAO::UpdateQuestionID()
{
	HSTMT hStmt;
	int queNum;
	int domainNum;
	char ID[LENGTH_ID];
	char select;

	while (true)
	{
		cout << "질문 번호 >> ";
		cin >> queNum;
		while (cin.get() != '\n');

		if (bcheckQuestionNum(queNum))
		{
			break;
		}
		else
		{
			cout << "해당 질문이 존재하지 않습니다." << endl;
		}
	}
	// 해당 질문의 도메인번호를 가져옴
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT QUE_DOMAIN_NUM FROM QUESTION WHERE QUE_NUM = %d", queNum);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLBindCol(hStmt, 1, SQL_C_SLONG, &domainNum, LENGTH_DOMAIN_NUM, NULL);
		SQLFetch(hStmt);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	while (true)
	{
		cout << "ID를 비공개로 바꾸시겠습니까?(Y/N) >> ";
		cin >> select;
		while (cin.get() != '\n');

		if (select == 'Y' || select == 'y')
		{
			if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
			{
				sprintf((char*)query, "UPDATE QUESTION SET QUE_ID = NULL WHERE QUE_NUM = %d", queNum);
				SQLExecDirect(hStmt, query, SQL_NTS);
				SQLCloseCursor(hStmt);
				SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			}
			return;
		}
		else if (select == 'N' || select == 'n')
		{
			while (true)
			{
				cout << "ID >> ";
				cin.getline(ID, LENGTH_ID);
				if (bcheckUser(ID, domainNum))
				{
					break;
				}
				else
				{
					cout << "입력된 유저가 존재하지 않습니다." << endl;
				}
			}

			if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
			{
				sprintf((char*)query, "UPDATE QUESTION SET QUE_ID = '%s' WHERE QUE_NUM = %d", queNum);
				SQLExecDirect(hStmt, query, SQL_NTS);
				SQLCloseCursor(hStmt);
				SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			}
			return;
		}
		else
		{
			cout << "잘못된 입력입니다." << endl;
		}
	}
}

void ProjectDAO::UpdateQuestionTitle()
{
	HSTMT hStmt;
	int queNum;
	char title[LENGTH_TITLE];

	while (true)
	{
		cout << "질문 번호 >> ";
		cin >> queNum;
		while (cin.get() != '\n');

		if (bcheckQuestionNum(queNum))
		{
			break;
		}
		else
		{
			cout << "해당 질문이 존재하지 않습니다." << endl;
		}
	}

	while (true)
	{
		cout << "TITLE >> ";
		cin.getline(title, LENGTH_TITLE);

		if (bcheckString(title))
		{
			break;
		}
		else
		{
			cout << "제목은 최소 3글자 이상이어야 합니다." << endl;
		}
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "UPDATE QUESTION SET QUE_TITLE = '%s' WHERE QUE_NUM = %d", title, queNum);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::UpdateQuestionContents()
{
	HSTMT hStmt;
	int queNum;
	char contents[LENGTH_CONTENTS];

	while (true)
	{
		cout << "질문 번호 >> ";
		cin >> queNum;
		while (cin.get() != '\n');

		if (bcheckQuestionNum(queNum))
		{
			break;
		}
		else
		{
			cout << "해당 질문이 존재하지 않습니다." << endl;
		}
	}

	while (true)
	{
		cout << "CONTENTS >> ";
		cin.getline(contents, LENGTH_CONTENTS);

		if (bcheckString(contents))
		{
			break;
		}
		else
		{
			cout << "내용은 최소 3글자 이상이어야 합니다." << endl;
		}
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "UPDATE QUESTION SET QUE_CONTENTS = '%s' WHERE QUE_NUM = %d", contents, queNum);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::UpdateResponseID()
{
	HSTMT hStmt;
	int resNum;
	int domainNum;
	char ID[LENGTH_ID];
	char select;

	while (true)
	{
		cout << "답변 번호 >> ";
		cin >> resNum;
		while (cin.get() != '\n');

		if (bcheckQuestionNum(resNum))
		{
			break;
		}
		else
		{
			cout << "해당 답변이 존재하지 않습니다." << endl;
		}
	}
	// 해당 답변의 도메인번호를 가져옴
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "SELECT RES_DOMAIN_NUM FROM RESPONSE WHERE RES_NUM = %d", resNum);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLBindCol(hStmt, 1, SQL_C_SLONG, &domainNum, LENGTH_DOMAIN_NUM, NULL);
		SQLFetch(hStmt);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}

	while (true)
	{
		cout << "ID를 비공개로 바꾸시겠습니까?(Y/N) >> ";
		cin >> select;
		while (cin.get() != '\n');

		if (select == 'Y' || select == 'y')
		{
			if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
			{
				sprintf((char*)query, "UPDATE RESPONSE SET RES_ID = NULL WHERE RES_NUM = %d", resNum);
				SQLExecDirect(hStmt, query, SQL_NTS);
				SQLCloseCursor(hStmt);
				SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			}
			return;
		}
		else if (select == 'N' || select == 'n')
		{
			while (true)
			{
				cout << "ID >> ";
				cin.getline(ID, LENGTH_ID);
				if (bcheckUser(ID, domainNum))
				{
					break;
				}
				else
				{
					cout << "입력된 유저가 존재하지 않습니다." << endl;
				}
			}

			if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
			{
				sprintf((char*)query, "UPDATE RESPONSE SET RES_ID = '%s' WHERE RES_NUM = %d", resNum);
				SQLExecDirect(hStmt, query, SQL_NTS);
				SQLCloseCursor(hStmt);
				SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			}
			return;
		}
		else
		{
			cout << "잘못된 입력입니다." << endl;
		}
	}
}

void ProjectDAO::UpdateResponseContents()
{
	HSTMT hStmt;
	int resNum;
	char contents[LENGTH_CONTENTS];

	while (true)
	{
		cout << "답변 번호 >> ";
		cin >> resNum;
		while (cin.get() != '\n');

		if (bcheckResponseNum(resNum))
		{
			break;
		}
		else
		{
			cout << "해당 답변이 존재하지 않습니다." << endl;
		}
	}

	while (true)
	{
		cout << "CONTENTS >> ";
		cin.getline(contents, LENGTH_CONTENTS);

		if (bcheckString(contents))
		{
			break;
		}
		else
		{
			cout << "내용은 최소 3글자 이상이어야 합니다." << endl;
		}
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "UPDATE RESPONSE SET RES_CONTENTS = '%s' WHERE RES_NUM = %d", contents, resNum);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::UpdateDomainName()
{
	HSTMT hStmt;
	int domainNum;
	char domainName[LENGTH_DOMAIN_NAME];

	domainNum = checkDomain();
	cout << "도메인 이름 >> ";
	cin.getline(domainName, LENGTH_DOMAIN_NAME);

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "UPDATE DOMAIN SET DOMAIN_NAME = '%s' WHERE DOMAIN_NUM = %d", domainName, domainNum);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::UpdateDomainCompany()
{
	HSTMT hStmt;
	int domainNum;
	char domainCompany[LENGTH_DOMAIN_COMPANY];

	domainNum = checkDomain();
	cout << "도메인 회사 >> ";
	cin.getline(domainCompany, LENGTH_DOMAIN_COMPANY);

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "UPDATE DOMAIN SET DOMAIN_COMPANY = '%s' WHERE DOMAIN_NUM = %d", domainCompany, domainNum);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::UpdateDomainParentDomain()
{
	HSTMT hStmt;
	int domainNum;
	char select;

	domainNum = checkDomain();

	while (true)
	{
		cout << "수정하는 사이트가 타 사이트의 하위 사이트 입니까?(Y/N) >> ";
		cin >> select;
		while (cin.get() != '\n');

		if (select == 'Y' || select == 'y' || select == 'N' || select == 'n')
		{
			break;
		}
		else
		{
			cout << "잘못된 입력입니다." << endl << endl;
		}
	}

	if (select == 'Y' || select == 'y')
	{
		while (true)
		{
			cout << "상위 사이트 번호 >> ";
			cin >> select;
			while (cin.get() != '\n');

			if (bcheckDomain(select))
			{
				break;
			}
			else
			{
				cout << "해당하는 도메인이 존재하지 않습니다." << endl;
			}
		}

		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
		{
			sprintf((char*)query, "UPDATE DOMAIN SET DOMAIN_PARENT = %d WHERE DOMAIN_NUM = %d", select, domainNum);
			SQLExecDirect(hStmt, query, SQL_NTS);

			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		}
	}
	else
	{
		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
		{
			sprintf((char*)query, "UPDATE DOMAIN SET DOMAIN_PARENT = NULL WHERE DOMAIN_NUM = %d", domainNum);
			SQLExecDirect(hStmt, query, SQL_NTS);

			SQLCloseCursor(hStmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		}
	}
}

void ProjectDAO::UpdateUsersEmail()
{
	HSTMT hStmt;
	int domainNum;
	char ID[LENGTH_ID];
	char email[LENGTH_EMAIL];

	domainNum = checkDomain();

	while (true)
	{
		cout << "ID >> ";
		cin.getline(ID, LENGTH_ID);

		if (bcheckUser(ID, domainNum))
		{
			break;
		}
		else
		{
			cout << "해당하는 유저가 존재하지 않습니다." << endl;
		}
	}

	while (true)
	{
		cout << "Email >> ";
		cin.getline(email, LENGTH_EMAIL);

		if (bcheckEmail(email))
		{
			break;
		}
		else
		{
			cout << "이메일 형식이 아닙니다." << endl;
		}
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "UPDATE USERS SET USER_EMAIL = '%s' WHERE USER_DOMAIN_NUM = %d AND USER_ID = '%s'", email, domainNum, ID);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

void ProjectDAO::UpdateUsersJob()
{
	HSTMT hStmt;
	int domainNum;
	char ID[LENGTH_ID];
	char job[LENGTH_JOB];

	domainNum = checkDomain();

	while (true)
	{
		cout << "ID >> ";
		cin.getline(ID, LENGTH_ID);

		if (bcheckUser(ID, domainNum))
		{
			break;
		}
		else
		{
			cout << "해당하는 유저가 존재하지 않습니다." << endl;
		}
	}
	cout << "직업 >> ";
	cin.getline(job, LENGTH_JOB);

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
	{
		sprintf((char*)query, "UPDATE USERS SET USER_JOB = '%s' WHERE USER_DOMAIN_NUM = %d AND USER_ID = '%s'", job, domainNum, ID);
		SQLExecDirect(hStmt, query, SQL_NTS);
		SQLCloseCursor(hStmt);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

#pragma endregion