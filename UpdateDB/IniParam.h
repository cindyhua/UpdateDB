#pragma once


class CIniParam
{
public:
	CIniParam();
	~CIniParam();

	string _filename;
	bool reSet(const string filename);
	bool init(const string filename);
	map<string, string> _items;
	bool loadConfig();

	//connect.ini(config["DBServer"],config["DBName"],config["DBPwd"]);
	const char* operator[](const char* key) const;
	bool check(const char* key, const char* defaultValue=nullptr) ;
	bool write(const char* key, const char* value);

	
	
};

