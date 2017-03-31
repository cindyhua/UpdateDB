#include "stdafx.h"
#include "IniParam.h"





CIniParam::CIniParam()
{

}

CIniParam::~CIniParam()
{
}

bool CIniParam::reSet(const string filename)
{
	return false;
}

bool CIniParam::init(const string filename)
{
	return false;
}



bool CIniParam::loadConfig()
{

	return false;
}

const char * CIniParam::operator[](const char* key) const
{

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






