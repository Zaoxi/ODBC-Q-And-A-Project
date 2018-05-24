//#include <stdio.h>
//#include <Windows.h>
//#include <sql.h>
//#include <sqlext.h>
//
//
//bool DBConnect();
//void DBDisconnect();
//
//SQLHENV hEnv = NULL;
//SQLHDBC hDbc = NULL;
//
//bool DBConnect()
//{
//	SQLRETURN Ret;
//
//	// Allocate the Environment Handle (hEnv)
//	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
//	{
//		return false;
//	}
//
//	// Set Attributes of the Environment Handle (hEnv)
//	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
//	{
//		return false;
//	}
//
//	// Allocate the Connection Handle(hDbc)
//	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
//	{
//		return false;
//	}
//
//	//Connect to the Server with ODBC
//	Ret = SQLConnect(hDbc, (SQLCHAR *)"DB01_Server", SQL_NTS, (SQLCHAR *)"DB01", SQL_NTS, (SQLCHAR *)"1234", SQL_NTS);
//	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO))
//	{
//		return false;
//	}
//
//	return true;
//}
//
//void DBDisconnect()
//{
//	SQLDisconnect(hDbc);
//	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
//	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
//}
//
//int main(int args, char *argv[])
//{
//	if (DBConnect() == true)
//	{
//		static SQLCHAR query[100];
//		SQLHSTMT hStmt;
//
//		printf("Connection Success!!");
//
//		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) == SQL_SUCCESS)
//		{
//			sprintf((char *)query, "INSERT INTO DEPARTMENT VALUES(6, 'IT', 15)");
//			SQLExecDirect(hStmt, query, SQL_NTS);
//
//			SQLCloseCursor(hStmt);
//			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
//		}
//
//		DBDisconnect();
//	}
//	else
//	{
//		printf("Fail to Connection!!");
//	}
//
//
//	return 0;
//}