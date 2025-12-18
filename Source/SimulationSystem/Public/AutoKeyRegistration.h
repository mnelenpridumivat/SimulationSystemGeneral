#pragma once
#include "SimulationSystemSettings.h"

// System to automatically register keys for USimulationSystemSettings::EntitiesTableKeys set
// Use SIMULATION_DATATABLE_KEY_REGISTER(KeyName) to define new key KeyName from C++
// Use SIMULATION_DATATABLE_KEY(KeyName) to access key KeyName in C++.
// This helps to find invalid key names (mistakes) in compile time

class SIMULATIONSYSTEM_API FSimulationDataTableKeyRegistry
{
	TSet<FName> RegisteredKeys;
public:
	static FSimulationDataTableKeyRegistry& Instance()
	{
		static FSimulationDataTableKeyRegistry Instance;
		return Instance;
	}
	
	void AddKey(FName Key) { RegisteredKeys.Add(Key); }
	const TSet<FName>& GetKeys() const { return RegisteredKeys; }
};

template <size_t N>
struct TStrLiteral
{
	TCHAR Value[N];
 
	constexpr TStrLiteral(const TCHAR (&InStr)[N])
	{
		for (size_t i = 0; i < N; ++i)
			Value[i] = InStr[i];
	}
};

template<TStrLiteral Str>
class SimulationDataTableKeyRegistrator
{
public:
	static constexpr const TCHAR* Key = Str.Value;
	
	SimulationDataTableKeyRegistrator()
	{
		FName InKey = Str.Value;
		if (!InKey.IsNone())
		{
			FSimulationDataTableKeyRegistry::Instance().AddKey(InKey);
		}
	}
};

#define SIMULATION_DATATABLE_KEY_REGISTER(x) constexpr TStrLiteral constexpr_str_##x{TEXT(#x)}; \
inline const SimulationDataTableKeyRegistrator<constexpr_str_##x> SimulationDataTableKeyRegistrator_##x;

#define SIMULATION_DATATABLE_KEY(x) SimulationDataTableKeyRegistrator_##x.Key

