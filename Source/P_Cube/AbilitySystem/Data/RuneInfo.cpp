// Fill out your copyright notice in the Description page of Project Settings.


#include "RuneInfo.h"

#include "P_Cube/CubeLogChannels.h"

FCubeRuneInfo URuneInfo::FindRuneInfoForTag(const FGameplayTag& RuneTag, bool bLogNotFound) const
{
	for ( const FCubeRuneInfo& Info : RuneInformation )
	{
		if ( Info.RuneTag == RuneTag )
		{
			return Info;
		}
	}

	if ( bLogNotFound )
	{
		UE_LOG(LogCube, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *RuneTag.ToString(), *GetNameSafe(this));
	}

	return FCubeRuneInfo();
}
