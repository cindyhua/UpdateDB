#pragma once

class ThreadParam
{
public:
	ThreadParam() {};
	ThreadParam(const ThreadParam& other) {};
	~ThreadParam() {};

};

class CThreadHandle
{
public:
	ThreadParam mParam;
	CThreadHandle();
	~CThreadHandle();
	bool Process();

};

