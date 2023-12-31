// Copyright Luka Otović


#include "AbilitySystem/Data/AbilityInfo.h"
#include "FantasyLogChannels.h"

FFantasyAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FFantasyAbilityInfo Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
			return Info;
	}

	if (bLogNotFound)
	{
		UE_LOG(LogFantasy, Error, TEXT("Can't find info for ability tag [%s] on ability info [%s]"), * AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FFantasyAbilityInfo();
}
