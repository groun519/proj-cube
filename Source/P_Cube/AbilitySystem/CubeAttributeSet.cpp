// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "P_Cube/CubeGameplayTags.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemLibrary.h"
#include "P_Cube/Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "P_Cube/Player/CubePlayerController.h"

UCubeAttributeSet::UCubeAttributeSet()
{
	const FCubeGameplayTags& GameplayTags = FCubeGameplayTags::Get();

	TagsToAttributes.Add(GameplayTags.Attributes_Primary_PhysicalPower, GetPhysicalPowerAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_MagicalPower, GetMagicalPowerAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_AttackSpeed, GetAttackSpeedAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorRate, GetArmorRateAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_MagicResistance, GetMagicResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MagicResistanceRate, GetMagicResistanceRateAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_ArmorPenetrationRate, GetArmorPenetrationRateAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_MagicResistancePenetration, GetMagicResistancePenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_MagicResistancePenetrationRate, GetMagicResistancePenetrationRateAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Primary_CriticalChance, GetCriticalChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_CriticalDamage, GetCriticalChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_MovementSpeed, GetMovementSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MovementSpeedIncreaseRate, GetMovementSpeedIncreaseRateAttribute);
	//TagsToAttributes.Add(GameplayTags.Attributes_Primary_CriticalDamage, GetCriticalChanceAttribute); <- �� �߰��Ұ�.
	//�𰨰� / ���۵� �־�߰ڳ� ;; �� �׸��� ����rate�� �׷��� �������Ѵ�.

	// ��׵��� ���߿� ü�� ���� ���� ü�¹� ������ ������ ���� �ؽ�Ʈ�� ǥ���� ��. �翬�� ������ �ؽ�Ʈ�� maxhealth maxmana �޾ƿ� �߾ӿ� ǥ���ϰ�.
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
}

void UCubeAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // �������� ��������� �߻��� ���� �� �Ӽ�(attribute)���� ������ �������� �Լ�.
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, PhysicalPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, MagicalPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, MagicResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, MovementSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, ArmorPenetrationRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, MagicResistancePenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, MagicResistancePenetrationRate, COND_None, REPNOTIFY_Always);

	// Secondary
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, ArmorRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, MagicResistanceRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, MovementSpeedIncreaseRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);

	// Vital
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, Health, COND_None, REPNOTIFY_Always); // �Ӽ��� ��������� ����� �Ӽ��� ����(�׻�)�ϰ�, OnRep_�Լ��� ȣ����.
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCubeAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UCubeAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UCubeAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = ����Ʈ �ҽ� (����Ʈ�� ����� ����), Target = ����Ʈ�� ������ ��� (�� AS�� ����)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UCubeAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("Changed Health on %s, Health: %f"), *Props.TargetAvatarActor->GetName(), GetHealth());
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;
			if (bFatal) // ���� ������ ���ظ� �޾Ҵ°�?
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
				if (CombatInterface) // ��ȿ�� Ȯ��
				{
					CombatInterface->Die(); // ���
				}
			}
			else // �츸�Ѱ�?
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FCubeGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			const bool bCriticalHit = UCubeAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
			const bool bPhysicalHit = UCubeAbilitySystemLibrary::IsPhysicalHit(Props.EffectContextHandle);
			const bool bMagicalHit = UCubeAbilitySystemLibrary::IsMagicalHit(Props.EffectContextHandle);
			const bool bPureHit = UCubeAbilitySystemLibrary::IsPureHit(Props.EffectContextHandle);
			ShowFloatingText(Props, LocalIncomingDamage, bCriticalHit, bPhysicalHit, bMagicalHit, bPureHit);
		}
	}
}

void UCubeAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bCriticalHit, bool bPhysicalHit, bool bMagicalHit, bool bPureHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter) // ������ ������ �ƴϸ�
	{
		if (ACubePlayerController* PC = Cast<ACubePlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bCriticalHit, bPhysicalHit, bMagicalHit, bPureHit); // ��� ��ġ�� ������ �ؽ�Ʈ ǥ��
		}
	}
}

void UCubeAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const 
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, Health, OldHealth); // �������� �Ӽ��� ����Ǿ��ٸ�, Ŭ���̾�Ʈ���� �ش� �Ӽ��� ��������� ����.
}

void UCubeAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, MaxHealth, OldMaxHealth);
}

void UCubeAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, Mana, OldMana);
}

void UCubeAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, MaxMana, OldMaxMana);
}


// Primary
void UCubeAttributeSet::OnRep_PhysicalPower(const FGameplayAttributeData& OldPhysicalPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, PhysicalPower, OldPhysicalPower);
}

void UCubeAttributeSet::OnRep_MagicalPower(const FGameplayAttributeData& OldMagicalPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, MagicalPower, OldMagicalPower);
}

void UCubeAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, Armor, OldArmor);
}

void UCubeAttributeSet::OnRep_MagicResistance(const FGameplayAttributeData& OldMagicResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, MagicResistance, OldMagicResistance);
}

void UCubeAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldMovementSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, MovementSpeed, OldMovementSpeed);
}

void UCubeAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, CriticalChance, OldCriticalChance);
}

void UCubeAttributeSet::OnRep_CriticalDamage(const FGameplayAttributeData& OldCriticalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, CriticalDamage, OldCriticalDamage);
}

void UCubeAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, AttackSpeed, OldAttackSpeed);
}

void UCubeAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UCubeAttributeSet::OnRep_ArmorPenetrationRate(const FGameplayAttributeData& OldArmorPenetrationRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, ArmorPenetrationRate, OldArmorPenetrationRate);
}

void UCubeAttributeSet::OnRep_MagicResistancePenetration(const FGameplayAttributeData& OldMagicResistancePenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, MagicResistancePenetration, OldMagicResistancePenetration);
}

void UCubeAttributeSet::OnRep_MagicResistancePenetrationRate(const FGameplayAttributeData& OldMagicResistancePenetrationRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, MagicResistancePenetrationRate, OldMagicResistancePenetrationRate);
}

//Secondary
void UCubeAttributeSet::OnRep_ArmorRate(const FGameplayAttributeData& OldArmorRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, ArmorRate, OldArmorRate);
}

void UCubeAttributeSet::OnRep_MagicResistanceRate(const FGameplayAttributeData& OldMagicResistanceRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, ArmorRate, OldMagicResistanceRate);
}

void UCubeAttributeSet::OnRep_MovementSpeedIncreaseRate(const FGameplayAttributeData& OldMovementSpeedIncreaseRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, MovementSpeedIncreaseRate, OldMovementSpeedIncreaseRate);
}

void UCubeAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UCubeAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCubeAttributeSet, ManaRegeneration, OldManaRegeneration);
}