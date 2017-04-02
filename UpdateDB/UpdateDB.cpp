// UpdateDB.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DBHandle.h"
#include "IniParam.h"
#include "UpdateDB.h"
#include "DataHandle.h"

bool Test_basic();

int main()
{
	Test_basic();
	system("pause");
	return 0;
}

//int main()
//{
//	CDataHandle<TigerData> TigerDataHandler;
//	CDataHandle<TigerWorkData> TigerWorkDataHandler;
//	TigerWorkDataHandler.setFieldCharSet('\x12');
//	TigerWorkDataHandler.setDataCharSet('\x14');
//	TigerData tp;
//	TigerWorkData twd;
//	string file = "tmp";
//	//write
//	//std::ofstream ofs;
//	//ofs.open(file, std::ofstream::binary | std::ofstream::app);
//	//if(!ofs.is_open()) 
//	//{
//	//	//errlog("Fail to open File:%s",file);
//	//	return 1;
//	//}
//
//	//if (TigerDataHandler.writeTo(ofs))
//	//{
//	//	TigerWorkDataHandler.writeTo(tp["WorkDesc"].val(), tp["WorkDesc"].size());
//	//	tp["CName"] = "huaxin";
//	//	/*
//	//	column_add(USERID, INT, 10, false, "");
//	//	column_add(CNAME, STRING, 50, true, "");
//	//	column_add(SCHOOL, STRING, 1000, true, "");
//	//	column_add(WorkDesc, STRING, 1000, true, "");
//	//	*/
//	//	tp["CNAME"] = "huaxin";
//	//	tp["SCHOOL"] = "beijing";
//	//	tp["USERID"] = "11973";
//	//	
//	//	twd["WORKID"] = "00921";
//	//	twd["COMPANY"] = "51job";
//	//	twd["POSITION"] = "SOFTWAREENGINEER";
//	//	TigerWorkDataHandler.putValue(twd);
//	//	TigerDataHandler.putValue(tp);
//	//}
//	//TigerDataHandler.flush();
//	//ofs.close();
//
//	size_t s1 = TigerData::size();
//	size_t s2 = TigerWorkData::size();
//
//	std::ifstream ifs;
//	//read
//	if (ifs.is_open())	ifs.close();
//
//	ifs.open(file, std::ifstream::binary);
//	if (!ifs.is_open())
//	{
//		//errlog("Fail to open File:%s",file);
//		return false;
//	}
//
//	 //get size of file
//	ifs.seekg(0, ifs.end);
//	size_t _filesize = ifs.tellg();
//	ifs.seekg(0);
//	// allocate memory for file content
//	char* buffer = new char[_filesize];
//	// read data as a block:
//	ifs.read(buffer, _filesize);
//	//buffer[_filesize + 1] = '\0';
//	ifs.close();
//
//	TigerDataHandler.parse(buffer, buffer + _filesize, tp);
//	while (TigerDataHandler.getValue())
//	{
//		string name = tp["CNAME"].val();
//		cout << name << endl;
//	}
//
//
//	system("pause");
//	delete[] buffer;
//    return 0;
//}

bool Test_basic()
{
	Writer<TigerData> wTigerData;
	Writer<TigerWorkData> wTigerWorkData;

	wTigerWorkData.setFieldCharSet('\x12');
	wTigerWorkData.setDataCharSet('\x14');


	TigerData tp;
	TigerWorkData twd;
	string file = "tmp";


	//write
	//std::ofstream ofs;
	//ofs.open(file, std::ofstream::binary | std::ofstream::app);
	//if(!ofs.is_open()) 
	//{
	//	//errlog("Fail to open File:%s",file);
	//	return 1;
	//}

	//if (wTigerData.writeTo(ofs))
	//{
	//	wTigerWorkData.writeTo(tp["WorkDesc"].val(), tp["WorkDesc"].size());
	//	tp["CName"] = "huaxinC";

	//	tp["CNAME"] = "huaxin";
	//	tp["SCHOOL"] = "beijing";
	//	tp["USERID"] = "11973";
	//	
	//	twd["WORKID"] = "00921";
	//	twd["COMPANY"] = "51job";
	//	twd["POSITION"] = "SOFTWAREENGINEER";
	//	wTigerWorkData.putValue(twd);
	//	wTigerData.putValue(tp);
	//}
	//wTigerData.flush();
	//ofs.close();


	Reader<TigerData> rTigerData;
	Reader<TigerWorkData> rTigerWorkData;

	std::ifstream ifs;
	//read
	if (ifs.is_open())	ifs.close();

	ifs.open(file, std::ifstream::binary);
	if (!ifs.is_open())
	{
		//errlog("Fail to open File:%s",file);
		return false;
	}

	//get size of file
	ifs.seekg(0, ifs.end);
	size_t _filesize = ifs.tellg();
	ifs.seekg(0);
	// allocate memory for file content
	char* buffer = new char[_filesize];
	// read data as a block:
	ifs.read(buffer, _filesize);
	//buffer[_filesize + 1] = '\0';
	ifs.close();

	rTigerData.parse(buffer, buffer + _filesize, tp);
	while (rTigerData.getValue())
	{
		string name = tp["CNAME"].val();
		cout << name << endl;
	}

	return true;
}
