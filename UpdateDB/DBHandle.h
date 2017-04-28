//<PROJECT>_<PATH>_<FILE>_H_
#ifndef  DBHANDLE_H_
#define  DBHANDLE_H_
#pragma once

#include "DataHandle.h"

#include <SQLAPI.h>

class Param
{
public:
	Param() {};
	~Param() {};

private:
	string	name;
	string	type;
	size_t	size;
	size_t	realsize;
	bool	isnull;
	string	defaultvalue;
};

typedef map<Param, string> mapParamValue;
typedef map<string, mapParamValue> mapParamList;


template <typename T>
class CDBHandle
{
public:
	CDBHandle() :_data(NULL), _conn(NULL){
	
	};
	~CDBHandle() {};
	//Connect( const SAString &sDBString , const SAString &sUserID , const SAString &sPassword , SAClient_t eSAClient = SA_Client_NotSpecified, saConnectionHandler_t fHandler = NULL );  throw (SAException);
	//sqlapi relevent
	SAConnection* _conn;
	SACommand _cmd;
	SAClient_t _client_t;

	T* _data;

	string _tablename;
	uint32_t _commitcount;
	/*
	
	db.bind(td,"tiger",conn);
	while(data.getValue())
	{
		db.excute();
	}
	*/
	bool bind(T& data, const char* tablename,SAConnection& conn) {
		_data = &data;
		_conn = &conn;
		_client_t = conn.Client();
		_tablename = tablename;
		_cmd.setConnection(_conn);

		iniSQL();
		return true;
	}
	
	
	bool iniSQL() {
		bindParams();
		return true;
	}
	bool setSQL(const char* sql)
	{
		_updateSQL = sql;
		return true;
	}
	bool setCommitCount(uint32_t commitcount)
	{
		_commitcount = commitcount;
		return true;
	}

	bool excute()
	{
		if (!_cmd.isOpened()) return false;
		_cmd.setCommandText(_updateSQL);
		_cmd.Execute();
		long affected = _cmd.RowsAffected();
		if (affected == 0 && _insertSQL.length()>0)
		{
			_cmd.setCommandText(_insertSQL);
			_cmd.Execute();
			affected = _cmd.RowsAffected();
		}
		return affected >= 1;
	}


	bool release()
	{
		_data = NULL;
		_cmd..Close();
		_tablename = "";
		return true;
	}

	string _updateSQL;
	string _insertSQL;

	/*
	批量更新语句(从文本）:
	1.bulk(sqlserver) "bulk insert %s FROM '%s' WITH ( FORMATFILE = '%s') " 
	2.load(mysql) load data LOCAL infile '%s' replace into table %s fields terminated by '%c' lines terminated by '%c' (ID,Cname,Status)
	如有则更新，没有则插入：
	1.mysql:Insert into Table Values(?,?,?...),(?,?,?...)(?,?,?...) on duplicate key update coreid=values(coreid), coredivid=values(coredivid)... mysql only?
	2.sqlserver:
	3.oracle:
	*/
	string _iniFromFileSQL;

	/*
	SACommand cmd(&con, "Select * from test_tbl where fid = :1 or fid = :fidvalue2");
	cmd.Param(1).setAsLong() = 15;
	cmd.Param("fidvalue2").setAsLong() = 32;
	===
	cmd.CreateParam("nInput", SA_dtLong, sizeof(long), SA_ParamInput);
	cmd.CreateParam("nOutput", SA_dtLong, sizeof(long), SA_ParamOutput);
	cmd << SAPos("nInput") << 1L; 
	*/
	//通过一次多条参数绑定，实现批量，形如:Insert into Table(a,b,c) values(?,?,?),(?,?,?),Update Tables set a=?,b=?,c=? 
	bool bindParams()
	{
		//clear at first
		_cmd.DestroyParams();
		//update resume set cname=:cname,userid=:userid
		//insert into resume(cname,userid) values(:cname,:userid);
		string paraminsertsql1 = "";
		string paraminsertsql2 = "";
		string paramupdatesql = "";
		//SAParam& CreateParam( const SAString &sName, SADataType_t eParamType, SAParamDirType_t eDirType = SA_ParamInput );
		//then bind
		for (DataField* it = &(_data->begin()); !_data->end(); it = &(_data->next()))
		{
			SAString saname = it->name.c_str();
			_cmd.CreateParam(saname, getDataType(it->type.c_str()));
			paraminsertsql1 += it->name + ",";
			paraminsertsql2 += ":" + it->name + ",";
			paramupdatesql += it->name + "=:" + it->name + ",";
			
		}
		if (paraminsertsql1.length() == 0 || paraminsertsql2.length() == 0 || paramupdatesql.length() == 0) return false;

		paraminsertsql1[paraminsertsql1.length() - 1] = '\0';
		paraminsertsql2[paraminsertsql2.length() - 1] = '\0';
		paramupdatesql[paramupdatesql.length() - 1] = '\0';

		_updateSQL = "update " + _tablename + " set " + paramupdatesql;
		_insertSQL = "insert into " + _tablename + paraminsertsql2 + " values(" + paraminsertsql2 + ")";


		return true;
	}
	/*
	SA_dtBool    Sets parameter's value as bool data.
	SA_dtShort    Sets parameter's value as short data.
	SA_dtLong    Sets parameter's value as long data.
	SA_dtFloat    Sets parameter's value as float data.
	SA_dtDouble    Sets parameter's value as double data.
	SA_dtNumeric    Data type is SANumeric (used internally).
	SA_dtDateTime    Sets parameter's value as standard C++ datetime structure (struct tm).
	SA_dtString    Sets parameter's value as string data (SAString).
	SA_dtLongBinary    Sets parameter's value as long binary data (SAString).
	SA_dtLongChar    Sets parameter's value as long character data (SAString).
	SA_dtBLob    Sets parameter's value as BLob data (SAString).
	SA_dtCLob    Sets parameter's value as CLob data (SAString).
	SA_dtCursor    Sets parameter's value as Oracle REF CURSOR (SACommand).
	SA_dtSpecificToDBMS    Sets parameter's value as server-specific data type value.  
	*/
	SADataType_t getDataType(const char* type)
	{
		//switch (type)
		//{
		//	case "Bool":
		//		return SA_dtBool;
		//	case "Short":
		//		return SA_dtShort;
		//	case "Long":
		//		return SA_dtLong;
		//	case "Float":
		//		//return SA_dtFloat;
		//	case "Double":
		//		return SA_dtDouble;
		//	case "Numeric":
		//		return SA_dtNumeric;
		//	case "DateTime":
		//		return SA_dtDateTime;
		//	case "LongBinary":
		//		return SA_dtLongBinary;
		//	case "LongChar":
		//		return SA_dtLongChar;
		//	case "BLob":
		//		return SA_dtBLob;
		//	case "CLob":
		//		return SA_dtCLob;
		//	case "Cursor":
		//		return SA_dtCursor;
		//	case "SpecificToDBMS":
		//		return SA_dtSpecificToDBMS;
		//	default:
		//		return SA_dtString;
		//}
		char t = type[0];
		switch (t)
		{
			case 'B':
				if (strcmp(type, "Bool") == 0) return SA_dtBool;
				if (strcmp(type, "BLob") == 0) return SA_dtBLob;
				break;
			case 'C':
				if (strcmp(type, "CLob") == 0) return SA_dtCLob;
				if (strcmp(type, "Cursor") == 0) return SA_dtCursor;
				break;
			case 'D':
				if (strcmp(type, "Double") == 0) return SA_dtDouble;
				if (strcmp(type, "DateTime") == 0) return SA_dtDateTime;
				break;
			case 'F':
				if (strcmp(type, "Float") == 0) return SA_dtDouble;
				break;
			case 'S':
				if (strcmp(type, "Short") == 0) return SA_dtShort;
				if (strcmp(type, "SpecificToDBMS") == 0) return SA_dtSpecificToDBMS;
				break;
			case 'L':
				if (strcmp(type, "Long") == 0) return SA_dtLong;
				if (strcmp(type, "LongBinary") == 0) return SA_dtLongBinary;
				if (strcmp(type, "LongChar") == 0) return SA_dtLongChar;
				break;
			case 'N':
				if (strcmp(type, "Numeric") == 0) return SA_dtNumeric;
				break;
			default:
				return SA_dtString;
		}
		return SA_dtString;
	}

	//两种加载参数值的方式，一种通过之前绑定的T对象引用，一种直接传入一个T对象的引用
	bool putParamValue()
	{
		return putParamValue(*_data);
	};


	bool putParamValue(T& data)
	{
		for (DataField* it = &(data->begin()); !data->end(); it = &(data->next()))
		{
			//it->pvalue;
		}
		return true;
	};


	

};

#endif