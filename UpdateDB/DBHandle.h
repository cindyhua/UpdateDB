#pragma once

#include "DataHandle.h"


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

class SAConnection;
template <typename T>
class CDBHandle
{
public:
	CDBHandle() :_data(NULL), _conn(NULL){};
	~CDBHandle();


	T* _data;
	SAConnection* _conn;
	string _tablename;
	uint32_t _commitcount;
	/*
	db.bind(td,conn)
	db.iniSQL("tiger",20);;
	*/
	bool bind(T& data,SAConnection& conn) {
		_data = &data;
		return true;
	}
	
	
	bool iniSQL(const char* tablename,uint32_t muticount=1) {
		bindParam(muticount);


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
	bool release()
	{
		_data = NULL;
		_conn = NULL;
		_tablename = "";
		return true;
	}

	string _updateSQL;
	string _insertSQL;

	/*
	批量更新语句(从文本）:
	1.bulk(sqlserver)
	2.load(mysql)
	*/
	string _bulkSQL;
	//通过一次多条参数绑定，实现批量，形如:insert table(a,b,c) values(?,?,?),(?,?,?)....
	bool bindParam(uint32_t coumuticountnt=1)
	{
		//clear at first


		//then bind
		for (uint32_t i = 0; i++; i < muticount)
		{
			for (DataField* it = &(_data->begin()); !_data->end(); it = &(_data->next()))
			{
				//it->pvalue;
			}
		}
		return true;
	}

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

