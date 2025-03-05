// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInfo.h"

#include "P_Cube/CubeLogChannels.h"

FWeaponInformation UWeaponInfo::FindWeaponInfoForTag(const FGameplayTag& WeaponTag, bool bLogNotFound) const
{
	for ( const FWeaponInformation& Info : WeaponInformation )
	{
		if ( Info.WeaponTag == WeaponTag )
		{
			return Info;
		}
	}

	if ( bLogNotFound )
	{
		UE_LOG(LogCube, Error, TEXT("Can't find info for WeaponTag [%s] on WeaponInfo [%s]"), *WeaponTag.ToString(), *GetNameSafe(this));
	}

	return FWeaponInformation();
}
