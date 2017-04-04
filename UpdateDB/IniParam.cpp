#include "stdafx.h"
#include "IniParam.h"



CIniParam CIniParam::nullCIniParam;



CIniParam::CIniParam(const string filename)
{
}

CIniParam::~CIniParam()
{
}


bool CIniParam::loadConfig(const string filename)
{

	return false;
}
//INI["USERNAME"],INI["PWD"]
//INI["USERNAME"]="51JOB";
const char* CIniParam::operator[](const char* key) const
{
	map<string, string>::const_iterator it = _items.lower_bound(key);
	if (it != _items.cend())
	{
		return (it->second).c_str();
	}
	return nullptr;
}

bool CIniParam::check(const char* key, const char* defaultValue) 
{
	if (_items.find(key)!=_items.end())
	{
		if (_items.find(key)->second.length()==0 )
		{
			//errlog("配置项【" + key + "】为空";
			if (defaultValue != nullptr)
			{
				//_items[key] = defaultValue;
				_items.insert(std::make_pair(key, defaultValue));
				return true;
			}
			return false;
		}
		return true;
	}
	else
	{
		if (defaultValue != nullptr)
		{
			_items.insert(std::make_pair(key, defaultValue));
			return true;
		}
		//errlog("不存在配置项【" + key + "】";
		return false;
	}
}

bool CIniParam::write(const char* key, const char* value)
{
	return false;
}








