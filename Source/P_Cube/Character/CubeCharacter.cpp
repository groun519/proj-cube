// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeCharacter.h"

#include "AbilitySystemComponent.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P_Cube/Player/CubePlayerController.h"
#include "P_Cube/Player/CubePlayerState.h"
#include "NiagaraComponent.h"
#include "P_Cube/UI/HUD/CubeHUD.h"


#include "P_Cube/Weapon.h"
#include <Components/CapsuleComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>

#include <Blueprint/AIBlueprintHelperLibrary.h>

#include "Components/WidgetComponent.h"

// Sets default values
ACubeCharacter::ACubeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);



	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bEnableCameraLag = true;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CharacterClass = ECharacterClass::Berserker;




	
	// �Ӹ� �� ����.
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	HaveWeapon = false;
	WeaponNum = 0;
	n = 0;
}

void ACubeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// InitAbilityActorInfo() : ������ ability actor ���� �ʱ�ȭ (AbilitySystemComponent, AttributeSet)
	InitAbilityActorInfo();

	// ĳ���� �����Ƽ �߰�
	AddCharacterAbilities();
}

void ACubeCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Ŭ���̾�Ʈ�� ability actor ���� �ʱ�ȭ (AbilitySystemComponent, AttributeSet)
	InitAbilityActorInfo();
}

void ACubeCharacter::AddToXP_Implementation(int32 InXP)
{
	ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>();
	check(CubePlayerState);
	CubePlayerState->AddToXP(InXP);
}

void ACubeCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

int32 ACubeCharacter::GetXP_Implementation() const
{
	const ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>();
	check(CubePlayerState);
	return CubePlayerState->GetXP();
}

int32 ACubeCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>();
	check(CubePlayerState);
	return CubePlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 ACubeCharacter::GetMoneyReward_Implementation(int32 Level) const
{
	const ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>();
	check(CubePlayerState);
	return CubePlayerState->LevelUpInfo->LevelUpInformation[Level].MoneyAward;
}

int32 ACubeCharacter::GetSkillPointsReward_Implementation(int32 Level) const
{
	const ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>();
	check(CubePlayerState);
	return CubePlayerState->LevelUpInfo->LevelUpInformation[Level].SkillPointAward;
}

void ACubeCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>();
	check(CubePlayerState);
	CubePlayerState->AddToLevel(InPlayerLevel);
}

void ACubeCharacter::AddToMoney_Implementation(int32 InMoney)
{
	ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>();
	check(CubePlayerState);
	CubePlayerState->AddToMoney(InMoney);
}

void ACubeCharacter::AddToSkillPoints_Implementation(int32 InSkillPoints)
{
	ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>();
	check(CubePlayerState);
	CubePlayerState->AddToSkillPoints(InSkillPoints);
}

int32 ACubeCharacter::GetPlayerLevel_Implementation()
{
	const ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>(); // state�� ��������,
	check(CubePlayerState);
	return CubePlayerState->GetPlayerLevel(); // state���� ���� ���� �� ����.
}

void ACubeCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ACubeCharacter::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void ACubeCharacter::InitAbilityActorInfo() // �����Ƽ �ý��� ������Ʈ, ��Ʈ����Ʈ�� �ʱ�ȭ
{
	ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>();
	check(CubePlayerState); // �� �����Դ��� üũ
	CubePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CubePlayerState, this);
	Cast<UCubeAbilitySystemComponent>(CubePlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = CubePlayerState->GetAbilitySystemComponent();
	AttributeSet = CubePlayerState->GetAttributeSet();

	if (ACubePlayerController* CubePlayerController = Cast<ACubePlayerController>(GetController()))
	{
		if (ACubeHUD* CubeHUD = Cast<ACubeHUD>(CubePlayerController->GetHUD()))
		{
			CubeHUD->InitOverlay(CubePlayerController, CubePlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}

void ACubeCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		LevelUpNiagaraComponent->Activate(true);
	}
}

void ACubeCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FName WeaponSocket = TEXT("RightHand");
	CurWeapon = GetWorld()->SpawnActor<AWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);
		CurWeapon->Weapon->SetVisibility(HaveWeapon);
	}

	//WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	//WeaponMesh->SetupAttachment(GetMesh(), WeaponSocket);

	/*WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetCanEverAffectNavigation(false);
	WeaponMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);*/
}

void ACubeCharacter::SetWeaponVisibility()
{
	CurWeapon->Weapon->SetVisibility(HaveWeapon);
	CurWeapon->ChangeWeaponMesh(WeaponNum);
	// �ٲ� AWeapon�� �޽��� �ڵ������� �����
}

void ACubeCharacter::Attack()
{
	if (!IsPlayingMontage(n))
	{
		float PlayRate = 0.7f;
		PlayAnimMontage(BagicAttackMontages[n], PlayRate);
		//UE_LOG(LogTemp, Warning, TEXT("Attack success"));
		//UE_LOG(LogTemp, Warning, TEXT("Montage Name: %s"), *BagicAttackMontages[n]->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("AnimNum: %d"), n);
	}

	// For example, play attack animation, apply damage to the target, etc.
}

void ACubeCharacter::MoveDest(const FVector Destination)
{
	float const Distance = FVector::Dist(Destination, GetActorLocation());
	if (Distance > 120.0f)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Destination);
	}
}

bool ACubeCharacter::IsPlayingMontage(int _n)
{
	// ��Ÿ�ְ� ���� ��� ������ ���θ� ��ȯ�ϴ� �ڵ�
	return GetMesh()->GetAnimInstance()->Montage_IsPlaying(BagicAttackMontages[_n]);
}