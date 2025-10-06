// Fill out your copyright notice in the Description page of Project Settings.


#include "CommunityRelationTable.h"

void UCommunityRelationTable::Validate()
{
	auto Arr = Communities.Array();
	for (int i = 0; i < Arr.Num(); i++)
	{
		for (int j = i+1; j < Arr.Num(); j++)
		{
			ensureMsgf(
				CheckRelations(Arr[i], Arr[j]) == CheckRelations(Arr[j], Arr[i]),
				TEXT("Communities [%s] and [%s] have asymmetrical relations!"),
				*Arr[i].ToString(),
				*Arr[j].ToString());
		}
	}
}

int UCommunityRelationTable::CheckRelations(FName A, FName B)
{
	auto ItA = Relations.Find(A);
	if (!ensureMsgf(
		!ItA,
		TEXT("Unable to find relation data for community [%s]"),
		*(A.ToString())))
	{
		return 0;
	}
	auto ItB = ItA->Relations.Find(B);
	if (!ensureMsgf(
		!ItB,
		TEXT("In community [%s] unable to find relation to community [%s]"),
		*(A.ToString()),
		*(B.ToString())))
	{
		return 0;
	}
	return *ItB;
}

#if WITH_EDITOR
void UCommunityRelationTable::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UCommunityRelationTable, Communities))
	{
		TSet<FName> RelationsKeys;
		Relations.GetKeys(RelationsKeys);
		switch (PropertyChangedEvent.ChangeType)
		{
		case EPropertyChangeType::ArrayAdd:
			{
				FName NewValue = NAME_None;
				for (auto& elem : Communities)
				{
					if (!RelationsKeys.Contains(elem))
					{
						NewValue = elem;
						break;
					}
				}
				//if (NewValue != NAME_None)
				//{
					for (auto& elem : Relations)
					{
						elem.Value.Relations.Add(NewValue) = 0;
					}
					auto& NewCommunity = Relations.Add(NewValue);
					for (auto& elem : Communities)
					{
						if (elem == NewValue)
						{
							continue;
						}
						NewCommunity.Relations.Add(elem) = 0;
					}
				//}
				break;
			}
		case EPropertyChangeType::ArrayRemove:
			{
				FName RemovedValue = NAME_None;
				{
					TSet<FName> CommunitiesSet = Communities;
					auto Diff = RelationsKeys.Difference(CommunitiesSet);
					if (ensure(Diff.Num() > 0))
					{
						ensure(Diff.Num() == 1);
						RemovedValue = *Diff.begin();
					}
				}
				//if (RemovedValue != NAME_None)
				//{
					for (auto& elem : Relations)
					{
						if (elem.Key == RemovedValue)
						{
							continue;
						}
						elem.Value.Relations.Remove(RemovedValue);
					}
					Relations.Remove(RemovedValue);
				//}
				break;
			}
		case EPropertyChangeType::ArrayClear:
			{
				Relations.Empty();
				break;
			}
		case EPropertyChangeType::ValueSet:
			{
				FName OldValue, NewValue;
				{
					TSet<FName> CommunitiesSet = Communities;
					auto Diff = RelationsKeys.Difference(CommunitiesSet);
					Diff = Diff.Union(CommunitiesSet.Difference(RelationsKeys));
					if (ensure(Diff.Num() >= 2))
					{
						ensure(Diff.Num() == 2);
						TArray<FName> DiffArr = Diff.Array();
						OldValue = RelationsKeys.Contains(DiffArr[0]) ? DiffArr[0] : DiffArr[1];
						NewValue = OldValue == DiffArr[0] ? DiffArr[1] : DiffArr[0];
					}
				}
				Relations.Add(NewValue) = Relations[OldValue];
				Relations.Remove(OldValue);
				for (auto& elem : Relations)
				{
					if (elem.Key == NewValue)
					{
						continue;
					}
					elem.Value.Relations.Add(NewValue) = elem.Value.Relations[OldValue];
					elem.Value.Relations.Remove(OldValue);
				}
				break;
			}
			default:
			{
				ensureMsgf(false, TEXT("Unhandled Communities property change type"));
			}
		}
	}
}
#endif
