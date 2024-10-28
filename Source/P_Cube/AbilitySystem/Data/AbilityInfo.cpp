// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityInfo.h"

#include "P_Cube/CubeLogChannels.h"

FCubeAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FCubeAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogCube, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FCubeAbilityInfo();
}

FGameplayTag UAbilityInfo::FindUniqueTagForBasicTag(const FGameplayTag& BasicAbilityTag, bool bLogNotFound) const
{
	TArray<FString> ParsedArray;
	
	BasicAbilityTag.GetTagName().ToString().ParseIntoArray(ParsedArray, TEXT("."), true);

	FString UniqueTagString;
		

	for ( const FString& Str : ParsedArray )
	{
		if ( Str != "Basic" )
		{
			UniqueTagString.Append(Str);
			UniqueTagString.Append(".");
		}
		else
		{
			UniqueTagString.Append("Unique");
		}
	}

	for ( const FCubeAbilityInfo& Info : AbilityInformation )
	{
		if ( Info.AbilityTag.GetTagName() == UniqueTagString )
		{
			return Info.AbilityTag;
		}
	}

	return FGameplayTag();
}