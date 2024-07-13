// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CubeAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/*
- GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) : 클래스(ClassName)에서 사용할 attribute(PropertyName) 속성에 대한 접근자를 생성해 속성을 사용할 때 필요한 함수를 정의.
- GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) : 속성(attribute)의 현재 값을 반환하는 getter 함수 생성.
- GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) : 속성(attribute)값을 설정하는 setter 함수 생성.
- GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) : 속성의 초기값을 설정하는 초기화 함수를 생성. (Init~~())
*/



/**
 * 
 */
UCLASS()
class P_CUBE_API UCubeAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UCubeAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, Health); // Health 속성에 대한 접근자들을 정의함.

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCubeAttributeSet, MaxMana);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
};
