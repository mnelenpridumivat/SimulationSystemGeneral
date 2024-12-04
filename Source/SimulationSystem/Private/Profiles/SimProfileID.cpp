#include "SimProfileID.h"

#include "SimProfileBase.h"

bool FReplicatedSimProfileID::IsValid() const
{
	return Class!=nullptr
		&& ProfileID!=-1;
}

bool operator==(const FReplicatedSimProfileID& A, const FReplicatedSimProfileID& B)
{
	return A.Class == B.Class && A.ProfileID == B.ProfileID;
}

bool operator!=(const FReplicatedSimProfileID& A, const FReplicatedSimProfileID& B)
{
	return !(A==B);
}

bool FSimProfileID::IsValid() const
{
	return Class!=nullptr
		&& ProfileID!=static_cast<uint32>(-1);
}

bool operator==(const FSimProfileID& A, const FSimProfileID& B)
{
	return A.Class == B.Class && A.ProfileID == B.ProfileID;
}

bool operator!=(const FSimProfileID& A, const FSimProfileID& B)
{
	return !(A==B);
}