#include "DebugData.h"

#include <ranges>

EDebugDataType DebugDataElemBase::GetType()
{
	return EDebugDataType::Null;
}

EDebugDataType DebugDataElemKeyValue::GetType()
{
	return EDebugDataType::KeyValue;
}

EDebugDataType DebugDataElemKeyObject::GetType()
{
	return EDebugDataType::KeyObject;
}

DebugDataElemNested::~DebugDataElemNested()
{
	for (auto elem : values)
	{
		delete elem;
	}
}

EDebugDataType DebugDataElemNested::GetType()
{
	return EDebugDataType::Nested;
}

FDebugDataMain::~FDebugDataMain()
{
	for (auto elem : Elems)
	{
		delete elem;
	}
}

FName DebugDataPropertyHandle::GetName()
{
	if (Name != NAME_None)
	{
		return Name;
	}
	return Type->GetFName();
}
