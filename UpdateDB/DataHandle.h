

#pragma once
#include <typeinfo>
#include <io.h>
#include <fstream>
#include <assert.h>
#define MAX_BUFFER_SIZE 1024*1024
#define MAX_DATA_BUFFER_COUNT 100
#define MAX_DATA_SIZE 102400
/*


*/

//const char* dd = { "","" };


#define debug_msg(msg) cout << "[" <<__FUNCTION__ <<"]"<<"[L"<<__LINE__<<"]"<<endl<<"\t--"<< msg << endl;
#define column_add(name,type,size,isnull,defaultvalue) \
	fields.insert(fields.cbegin(),std::make_pair(#name, DataField(#name,#type,size,isnull,defaultvalue)));\
	//Data::_size += (it->second).size();\


class Data;
class DataField {
public:
	static DataField nullDataField;
	static const DataField& nullSingleton();
	friend std::ostream& operator<<(std::ostream& stream, const DataField& data);
	DataField() {
		pvalue = NULL;
		debug_msg("[default]" << *this);
	};
	DataField(string pname,string ptype,uint32_t psize,bool pisnull=true, string pdefaultvalue = "")
		:name(pname),type(ptype),_size(psize),isnull(pisnull),defaultvalue(pdefaultvalue), _realsize(psize)
	{

		pvalue = (char*)malloc(_realsize+1);
		clean();
		debug_msg(*this);
	};
	
	long asLong();
	DataField(const DataField& other)
	{
		copyBasic(other);
		pvalue = (char*)malloc(other._realsize+1);
		clean();
		memcpy(pvalue, other.pvalue,_realsize);
		debug_msg("[(other)]" << *this);

	}

	void iniBasic()
	{
		name = "";
		type = "";
		_size = 0;
		isnull = true;
		defaultvalue = "";
		_realsize = 0;
	}

	void copyBasic(const DataField& other)
	{
		name = other.name;
		type = other.type;
		_size = other._size;
		isnull = other.isnull;
		defaultvalue = other.defaultvalue;
		_realsize = other._realsize;
	}

	DataField& operator=(const DataField& other)
	{

		if (&other!= this)
		{
			DataField tmp(other);
			copyBasic(other);
			char* ptmpb = tmp.pvalue;
			tmp.pvalue = pvalue;
			tmp.pvalue = NULL;
			pvalue = ptmpb;			
		}
		debug_msg("[(=)]" << *this);
		return *this;
	}
	DataField& operator=(const char* value)
	{
		if (this == &nullDataField) return *this;
	
		size_t len = strlen(value);
		if (len > _realsize) len = _realsize;
		clean();
		memcpy(pvalue, value, len);

		return *this;
	}

	//DataField& operator=(Data& data)
	//{

	//	return *this;
	//}
	//static DataField* pt = &test.fields["WorkDesc1"];
	//DataField* operator=(map<string, DataField>)


	~DataField(){
		debug_msg(*this);
		if (pvalue)
		{
			free(pvalue); 
			pvalue = NULL;
		}
	};
	void clean() {
		memset(pvalue, '\0', _realsize+1);
		
	};
	char* val()
	{
		return pvalue;
	}
	uint32_t size() { return _realsize; };
	string	name;
	string	type;
	uint32_t _size;
	uint32_t _realsize;
	bool	isnull;
	string	defaultvalue;
	char*	pvalue;
};


class Data
{
public:

	static Data nullData;
	Data() {
		debug_msg(*this);

		countSize();
		_itCurr = fields.begin();
	};
	virtual ~Data() {};
	friend std::ostream& operator<<(std::ostream& os, const Data& data);
	size_t itemCount() { return fields.size(); };//包含的子列数
	static size_t size() {
		return _size;
	};
	static size_t _size;//所有子列的占的内存大小
	map<string, DataField> fields;
    DataField& operator[](string key);
	//Data& operator=(const char* value);
	virtual bool iniByFmtFile(const char* file) {


		return true;
	};//由固定格式的fmt文件初始化
	bool clean() {
		for (map<string, DataField>::iterator it = fields.begin(); it != fields.end(); it++)
		{
			it->second.clean();
		}
		return true;
	};

	virtual bool countSize() {
		Data::_size = 0;
		for (map<string, DataField>::iterator it = fields.begin(); it != fields.end(); it++)
		{
			Data::_size += (it->second).size();
		}
		return true;
	}
	map<string, DataField>::iterator _itCurr;
	DataField& begin() {
		_itCurr = fields.begin();
		if (end()) return DataField::nullDataField;
		return _itCurr->second;
	};
	bool end() {
		return _itCurr == fields.end();
	};
	DataField& next() {
		++_itCurr;
		if(end()) return  DataField::nullDataField;
		return _itCurr->second;
	};
};



inline  DataField & Data::operator[](string key)
{

	map<string, DataField>::iterator it = fields.lower_bound(key);
	if (it == fields.end()) return DataField::nullDataField;
	return it->second;

	
}




 class TigerData :public Data
 {
	 static size_t _size;
 public:
	 TigerData() {
		 column_add(USERID, INT, 10, false, "");
		 column_add(CNAME, STRING, 50, true, "");
		 column_add(SCHOOL, STRING, 1000, true, "");
		 column_add(WorkDesc, STRING, 1000, true, "");
		 countSize();
	 };
	 ~TigerData() {};
	 virtual bool countSize() {

		 if (TigerData::_size > 0) return true;
		 if (TigerData::_size < 0) TigerData::_size = 0;
		 for (map<string, DataField>::iterator it = fields.begin(); it != fields.end(); it++)
		 {
			 TigerData::_size += (it->second).size();
		 }
		 return true;
	 }

 };

 class TigerWorkData :public Data
 {
 public:
	 TigerWorkData() {
		 column_add(WORKID, INT, 10, false, "");
		 column_add(COMPANY, STRING, 1000, true, "");
		 column_add(POSITION, STRING, 1000, true, "");
		 countSize();
	 };
	 ~TigerWorkData() {};


 };


static TigerData test;
static DataField* pt = &test.fields["WorkDesc1"];


template <typename T>
class CDataHandle
{
public:
	CDataHandle();
	~CDataHandle();
	T*			_data;
	char		_dataCharSet;
	char		_fieldCharSet;
	char*		_pWriteBuffer;
	int			_writeType;
	char*		_pReadBuffer;
	size_t		_buffersize;
	char*		_pCurr;
	char*		_pEnd;
	size_t		_len;
	string		_file;
	size_t		_filesize;
	std::ifstream* _ifs;
	std::ofstream* _ofs;
	
	void setDataCharSet(char datacharset) { _dataCharSet = datacharset; };
	void setFieldCharSet(char fieldcharset) { _fieldCharSet = fieldcharset; }
	uint32_t length() const { return _len; };

	//Read
	bool parse(const char* pbegin, const char* pend, T& data) {

		_filesize = pend - pbegin;
		if (_pReadBuffer)	free(_pReadBuffer), _pReadBuffer = NULL;
		_pReadBuffer = (char*)malloc(_filesize+1);
		memset(_pReadBuffer, '\0', _filesize + 1);
		memcpy(_pReadBuffer, pbegin, _filesize);

		_data = &data;
		_pCurr = _pReadBuffer;
		_pEnd = _pReadBuffer + _filesize;
		return true;
	};
	bool getValue();
	bool end() {
		return _pCurr > _pEnd;
	}
	//write
	bool writeTo(std::ofstream& ofs)
	{
		_ofs = &ofs;
		char *p = (char*)malloc(_buffersize + 1);
		_writeType = 1;
		return setWriteBuffer(p, _buffersize);
	}

	bool writeTo(char* p,size_t size)
	{
		_writeType = 2;
		return setWriteBuffer(p, size);
	}


	bool flush()
	{
		_write();
		//if (ofs.is_open()) ofs.close();
		return true;
	}
	bool setWriteBuffer(char* p,size_t size) {
		assert(p);
		if (!p)
		{
			//errlog("分配空间失败")
			return false;
		}

		if (_pWriteBuffer) free(_pWriteBuffer), _pWriteBuffer = NULL;
		_pWriteBuffer = p;
		_buffersize = size;
		if (_buffersize > MAX_BUFFER_SIZE) _buffersize = MAX_BUFFER_SIZE;
		memset(_pWriteBuffer, '\0', _buffersize + 1);
		_pCurr = _pWriteBuffer;
		_pEnd = _pWriteBuffer + _buffersize;
		return true;
	}
	char* putValue(T& data,bool forced = false){
		assert(_writeType && _pCurr && _pWriteBuffer);
		char* pStart = _pCurr;
		if  (_len + T::size() > _buffersize)
		{
			//缓冲区满，不再追加信息
			//T::size()代表的是传入的data的最大长度，如果没有满，则应该往里面填充，就算超出了，也分为完整截断和尽可能填充两种情况
			//if (_writeType == 2)
			//{
			//	return pStart;
			//}
			//写文件
			_write();
	

		}

		for (DataField* it = &(data.begin()); !data.end(); it=&(data.next()))
		{
			_putValue(&_pCurr, it->pvalue,strlen(it->pvalue));
			_putValue(&_pCurr, &_fieldCharSet,1);
		}
		_putValue(&_pCurr, &_dataCharSet,1);


		if (forced)
			_write();

		return pStart;
	};
	//写文件
	void _write()
	{
		if(_writeType ==1 && _ofs->is_open()) _ofs->write(_pWriteBuffer, _len);
		//复原
		_len = 0;
		_pCurr = _pWriteBuffer;
		memset(_pWriteBuffer, '\0', _buffersize + 1);
	}

	bool _putValue(char** p, const char* pvalue, size_t len)
	{
		//再填充要超出填充区大小
		if (_len + len > _buffersize)
		{
			len = _buffersize - _len;
			return false;
		}
		memcpy(*p, pvalue, len);
		*p += len;
		_len += len;
		return true;
	}

	void ini()
	{
		_buffersize = T::size() * (MAX_DATA_BUFFER_COUNT + 10);
		if (_buffersize > MAX_BUFFER_SIZE) _buffersize = MAX_BUFFER_SIZE;
		_pWriteBuffer = NULL;
		_pReadBuffer = NULL;
		_pCurr = NULL;
		_pEnd = NULL;
		_dataCharSet = '\x16';
		_fieldCharSet = '\x10';
		_data = NULL;
		_len = 0;
		_file = "";
		_filesize = 0;
		_ofs = NULL;
		_ifs = NULL;
		_writeType = 0;
	}
};

template <typename T>
CDataHandle<T>::CDataHandle()
{
	ini();
}

template <typename T>
CDataHandle<T>::~CDataHandle()
{
	//if (_ofs->is_open())
	//{
	//	//TODO:写文件
	//	_ofs->write(_pWriteBuffer, _len);
	//}
	if (_writeType!=2 && _pWriteBuffer)
	{
		free(_pWriteBuffer);
		_pWriteBuffer = NULL;
	}
	if (_pReadBuffer) free(_pReadBuffer), _pReadBuffer = NULL;
}

template<typename T>
inline bool CDataHandle<T>::getValue()
{
	if (_data == NULL) return false;
	_data->clean();
	assert(_pCurr && _pEnd);
	if (_pCurr >= _pEnd) return false;
	char* pos = NULL;
	//for (DataField& it = _data->begin(); ! _data->end(); it = _data->next())
	for (DataField* it = &(_data->begin()); !_data->end(); it = &(_data->next()))
	{
		if (_pCurr >= _pEnd) return false;
		pos = strchr(_pCurr, _fieldCharSet);
		if (!pos) 
		{
			_pCurr = _pEnd;
			return false;
		};
		size_t len = pos - _pCurr;
			assert(len > 0);
			if (len > it->size()) len = it->size();
			memcpy(it->pvalue, _pCurr, len);
			_pCurr = pos + 1;

	}
	pos = strchr(_pCurr, _dataCharSet);
	if (!pos)
	{
		_pCurr = _pEnd;
		return false;//如果是一个不完整的记录，已经保存部分字段，是否算完整记录？
	}
	_pCurr = pos + 1;
	return true;
}

