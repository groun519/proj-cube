// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "P_Cube/AbilitySystem/Data/CharacterClassInfo.h"
#include "P_Cube/Interaction/CombatInterface.h"
#include "P_Cube/AbilitySystem/Data/WeaponInfo.h"
#include "CubeCharacterBase.generated.h"

class UPassiveNiagaraComponent;
class UNiagaraSystem;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FStartupAbilities
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
};

UCLASS()
class P_CUBE_API ACubeCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACubeCharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // AbilitySytem getter.
	UAttributeSet* GetAttributeSet() const { return AttributeSet; } // AttributeSet getter.

	/** Combat Interface */
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncremenetMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual void SetBaseWeapon_Implementation(FGameplayTag NewWeaponTag) override;
	virtual void ChangeWeapon_Implementation(FGameplayTag NewWeaponTag) override;
	virtual void ResetWeapon_Implementation() override;
	virtual FOnASCRegistered GetOnASCRegisteredDelegate() override;
	/** end Combat Interface */

	FOnASCRegistered OnAscRegistered;

	FOnDeathSignature OnDeathDelegate;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY(ReplicatedUsing = OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;
	
	UFUNCTION()
	virtual void OnRep_Stunned();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	USoundBase* DeathSound;

	/* Minions */

	int32 MinionCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Dummy;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;

	UPROPERTY(EditAnywhere, Category = "Interact")
	TObjectPtr<UAnimMontage> InteractMontage;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interact")
	void PlayInteractMontage(FGameplayTag InteractTag);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	TSubclassOf<UGameplayAbility> AttackAbility;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon; // 무기 스켈레탈 메시

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMesh> BaseWeaponMesh; // 무기 스켈레탈 메시
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FTransform BaseWeaponOffset; // 무기 스켈레탈 메시

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName; // 이펙트 발사 위치.

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;

	bool bDead = false;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	/** Movement Speed **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float BaseSpeed = 500.f;

	UPROPERTY(ReplicatedUsing = OnRep_MaxWalkSpeed)
	float ReplicatedMaxWalkSpeed;

	UFUNCTION()
	void OnRep_MaxWalkSpeed();

	UFUNCTION(Server, Reliable)
	void Server_UpdateMovementSpeed(UCharacterMovementComponent* CharacterMovementComp);

	UFUNCTION()
	void UpdateMovementSpeed(UCharacterMovementComponent* CharacterMovementComp);


	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateMovementSpeed(float NewSpeed);
	/** Movement Speed **/

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent; // ASC

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet; // AS

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes; // 기본 능력치(Primary) 초기화 효과

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	UFUNCTION(BlueprintCallable)
	void AddCharacterAbilities();

	/* Dissolve Effects */

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* BloodEffect;


	/** weapon settings **/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon DataAsset")
	TObjectPtr<UWeaponInfo> WeaponInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Settings")
	FGameplayTag WeaponTag = FGameplayTag();

public:
	UFUNCTION()
	void SetWeaponTag(FGameplayTag NewTag);

private:
	UFUNCTION(Server, Reliable)
	void Server_SetWeaponMesh(USkeletalMesh* NewMesh, FTransform NewTransform);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetWeaponMesh(USkeletalMesh* NewMesh, FTransform NewTransform);

	UFUNCTION(Server, Reliable)
	void Server_ApplyWeaponEffect(float Range, float MovementSpeed_Coef, float AttackSpeed_Coef);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyWeaponEffect(float Range, float MovementSpeed_Coef, float AttackSpeed_Coef);

	/** end weapon settings **/

private:

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TMap<FName, FStartupAbilities> StartupAbilitiesMap;
	//TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	
	
};
