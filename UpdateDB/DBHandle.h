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

typedef map<Param, string> ParamValue;
typedef map<string, Param> ParamList;


template <typename T>
class CDBHandle
{
public:
	CDBHandle();
	virtual ~CDBHandle();


	T* _data;
	Reader<T>* _rdData;
	bool bindReaderData(Reader<T>& rdData) {
		_rdData = &rdData;
		return true;
	}

	bool iniParam()
	{
		return true;
	}

	bool putParamValue(T& data)
	{
		for (DataField* it = &(data.begin()); !data.end(); it = &(data.next()))
		{
			//it->pvalue;
		}
	};



};

