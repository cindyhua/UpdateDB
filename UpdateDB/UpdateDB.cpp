// UpdateDB.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "IniParam.h"
#include "UpdateDB.h"
#include "DataHandle.h"
#include "DBHandle.h"


bool Test_basic();
int Test_sqlapi();

int main()
{
	printf("start\r\n");
	//Test_basic();
	Test_sqlapi();
	system("pause");
	return 0;
}


bool loadConf(const char* file);
bool checkConf();
bool openDBConn();
bool ini();
bool beginThread();
bool getData();
bool insertLog();
bool process();
bool updateLog();



bool closeDBConn();
bool releasAll();
char* readfile(const char* file, size_t& file_size);

int Test_sqlapi()
{
	SAConnection con; // connection object
	SACommand cmd;    // command object

	try
	{
		// connect to database (Oracle in our example)
		//con.Connect("test", "tester", "tester", SA_Oracle_Client);
		con.Connect("10.100.2.71@huaxin", "resume", "resume",SA_SQLServer_Client );
		con.setAutoCommit(SA_AutoCommitOff);
		// associate a command with connection
		cmd.setConnection(&con);

		// Insert 2 rows
		cmd.setCommandText(
			"Insert into Student(Cname,Age) values(:1, :2)");

		// use first method of binding - param assignment
		cmd.Param(1).setAsString() = "huaxin";
		cmd.Param(2).setAsLong() = 12;
		// Insert first row
		cmd.Execute();

		// use second method of binding - stream binding
		//cmd << (long)3 << "Some string (3)";

		// Insert second row
		//cmd.Execute();

		// commit changes on success
		con.Commit();

		printf("Input parameters bound, rows inserted!\n");
	}
	catch (SAException &x)
	{
		// SAConnection::Rollback()
		// can also throw an exception
		// (if a network error for example),
		// we will be ready
		try
		{
			// on error rollback changes
			con.Rollback();
		}
		catch (SAException &)
		{
		}
		// print error message
		printf("%s\n", (const char*)x.ErrText());
	}

	return 0;
}


bool Test_DB()
{
	CDBHandle<TigerData> db;
	SAConnection conn;
	//sDBString
	//oracle：tns.hostname
	//mysql: One of the following formats:
	    //  "" or "@" - empty string or '@' character, connects to a local server.
		//<database name> or @ <database name> -connects to a database with the specified name on local server.
		//<server name>@ - connects to the specified server.
		//<server name>@<database name> -connects to a database with the specified name on the specified server.
	//sql server(odbc): like "[[<server_name>@][<database_name][;<driver_connection_option_list>]"
		//	<server_name> -connects to a specified server.If it's ommitted SQLAPI++ tries to connect to default local server instance.
		//	<database_name> -connects to a database with the specified name.If it's ommitted SQLAPI++ tries to connect to default database.
		//	<driver_connection_option_list>  -SQL Server Native ODBC driver specific option list.
	try {
		conn.Connect("10.100.2.71@huaxin", "resume", "resume");
		conn.setAutoCommit(SA_AutoCommitOff);
	}
	catch (SAException &x)
	{
		// SAConnection::Rollback()
		// can also throw an exception
		// (if a network error for example),
		// we will be ready
		try
		{
			// on error rollback changes
			conn.Rollback();
		}
		catch (SAException &)
		{
		}
		// print error message
		printf("%s\n", (const char*)x.ErrText());
	}

	if (!conn.isConnected())
	{
		//errlog
		cout << "连接失败" << endl;
	}
	Reader<SyncData> rSyncData;
	TigerData tp;
	SyncData sd;
	string file = "tmp";

	size_t filesize = 0;
	char* buffer = readfile(file.c_str(), filesize);
	rSyncData.parse(buffer, buffer + filesize, sd);
	
	db.bind(tp, "tiger", conn);
	while (rSyncData.getValue())
	{
		tp.fields["CName"] = sd.fields["CName"];
	}
	delete[] buffer;
	return true;


	return true;
}


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

	size_t filesize = 0;
	char* buffer = readfile(file.c_str(), filesize);
	rTigerData.parse(buffer, buffer + filesize, tp);
	while (rTigerData.getValue())
	{
		string name = tp["CNAME"].val();
		cout << name << endl;
	}
	delete[] buffer;
	return true;
}

char* readfile(const char* file,size_t& file_size)
{
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
	file_size = ifs.tellg();
	ifs.seekg(0);
	// allocate memory for file content
	char* buffer = new char[file_size];
	// read data as a block:
	ifs.read(buffer, file_size);
	//buffer[_filesize + 1] = '\0';
	ifs.close();
}