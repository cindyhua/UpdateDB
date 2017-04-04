#pragma once


class CIniParam
{
public:
	CIniParam() {};
	CIniParam(const string filename);
	~CIniParam();
	static CIniParam nullCIniParam;
	string _filename;
	//bool reSet(const string filename);
	//bool init(const string filename);
	map<string, string> _items;
	bool loadConfig(const string filename);

	//connect.ini(config["DBServer"],config["DBName"],config["DBPwd"]);
	const char* operator[](const char* key) const;
	//--INI["DBServer"] = "51JOB";
	//INI.check("DBServer");
	bool check(const char* key, const char* defaultValue=nullptr) ;
	//INI.write("ISWRITE","1");
	bool write(const char* key, const char* value);

	
	
};

