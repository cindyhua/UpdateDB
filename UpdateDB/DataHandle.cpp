#include "stdafx.h"
#include "DataHandle.h"

std::ostream & operator<<(std::ostream & os, const DataField & data)
{
	// TODO: 在此处插入 return 语句

	os << "[" << &data << "][" << typeid(data).name() <<"]" << (data.pvalue == NULL ? "":static_cast<const void*>(data.pvalue) )<< "";
	return os;
}

std::ostream & operator<<(std::ostream & os, const Data & data)
{
	// TODO: 在此处插入 return 语句
	os << "[" << &data << "][" << typeid(data).name() << "]";
	return os;
}

uint32_t Data::_size = 0;
DataField  DataField::nullDataField;

Data Data::nullData;

const DataField & DataField::nullSingleton()
{
	static const DataField nullStatic;
	return nullStatic;
}
