// UpdateDB.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DBHandle.h"
#include "IniParam.h"
#include "UpdateDB.h"
#include "DataHandle.h"


int main()
{
	CDataHandle<TigerData> TigerDataHandler;
	CDataHandle<TigerWorkData> TigerWorkDataHandler;
	TigerWorkDataHandler.setFieldCharSet('\x26');
	TigerWorkDataHandler.setDataCharSet('\x30');
	TigerData tp;
	TigerWorkData twd;
	std::ofstream ofs;

	ofs.open("tmp", std::ofstream::binary | std::ofstream::app);
	if(!ofs.is_open()) 
	{
		//errlog("Fail to open File:%s",file);
		return 1;
	}

	if (TigerDataHandler.writeTo(ofs))
	{
		TigerWorkDataHandler.writeTo(tp["WorkDesc"].val(), tp["WorkDesc"].size());
		tp["CName"] = "huaxin";
		/*
		column_add(USERID, INT, 10, false, "");
		column_add(CNAME, STRING, 50, true, "");
		column_add(SCHOOL, STRING, 1000, true, "");
		column_add(WorkDesc, STRING, 1000, true, "");
		*/
		tp["CNAME"] = "huaxin";
		tp["SCHOOL"] = "beijing";
		tp["USERID"] = "11973";
		
		twd["WORKID"] = "00921";
		twd["COMPANY"] = "51job";
		twd["POSITION"] = "SOFTWAREENGINEER";

		TigerWorkDataHandler.putValue(twd);

		TigerDataHandler.putValue(tp);
	}
	TigerDataHandler.flush();
	ofs.close();
	system("pause");
    return 0;
}

