#include "stdafx.h"
#include "DataHandle.h"

std::ostream & operator<<(std::ostream & os, const DataField & data)
{
	// TODO: �ڴ˴����� return ���

	os << "[" << &data << "][" << typeid(data).name() <<"]" << (data.pvalue == NULL ? "":static_cast<const void*>(data.pvalue) )<< "";
	return os;
}

std::ostream & operator<<(std::ostream & os, const Data & data)
{
	// TODO: �ڴ˴����� return ���
	os << "[" << &data << "][" << typeid(data).name() << "]";
	return os;
}

size_t Data::_size = 0;

size_t TigerData::_size = 0;
DataField  DataField::nullDataField;

Data Data::nullData;

const DataField & DataField::nullSingleton()
{
	static const DataField nullStatic;
	return nullStatic;
}
