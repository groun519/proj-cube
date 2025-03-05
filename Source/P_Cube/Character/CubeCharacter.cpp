// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeCharacter.h"

#include "AbilitySystemComponent.h"
#include "P_Cube/CubeGameplayTags.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemLibrary.h"
#include "P_Cube/AbilitySystem/CubeAttributeSet.h"
#include "P_Cube/AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P_Cube/Player/CubePlayerController.h"
#include "P_Cube/Player/CubePlayerState.h"
#include "NiagaraComponent.h"
#include "P_Cube/UI/HUD/CubeHUD.h"

#include "P_Cube/UI/Widget/CubeUserWidget.h"
#include "Components/WidgetComponent.h"

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

	GetCharacterMovement()->bOrientRotationToMovement = true; // 캐릭터가 이동하는 방향으로 자동으로 화전하도록 설정
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // 초당 최대 720도로화전가능, 부드럽게 만드는 것
	GetCharacterMovement()->bConstrainToPlane = true; // 캐릭터의 움직임을 평면으로만
	GetCharacterMovement()->bSnapToPlaneAtStart = true; // 게임이 시작될때 지정된 평면에 맞추어 배치

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	// 컨트롤러의 회전을 캐릭터에 반영 x -> bOrientRotationToMovement = true; 가 있어서 이동방향으로 자동 회전 가능

	CharacterClass = ECharacterClass::Default;




	
	// 머리 위 위젯.
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar"); // 체력바 위젯 생성
	HealthBar->SetupAttachment(GetRootComponent());

	ClassArray.Add(ECharacterClass::Warrior);
	ClassArray.Add(ECharacterClass::Ranger);
	ClassArray.Add(ECharacterClass::Wizard);
	ClassArray.Add(ECharacterClass::Healer);
}

void ACubeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// InitAbilityActorInfo() : 서버의 ability actor 정보 초기화 (AbilitySystemComponent, AttributeSet)
	InitAbilityActorInfo();

	// 캐릭터 어빌리티 추가
	//AddCharacterAbilities(); <- wnddy
}

void ACubeCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 클라이언트의 ability actor 정보 초기화 (AbilitySystemComponent, AttributeSet)
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

	if (UCubeAbilitySystemComponent* CubeASC = Cast<UCubeAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		CubeASC->UpdateAbilityStatuses(CubePlayerState->GetPlayerLevel());
	}
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

int32 ACubeCharacter::GetMoney_Implementation() const
{
	ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>();
	check(CubePlayerState);
	return CubePlayerState->GetMoney();
}

int32 ACubeCharacter::GetSkillPoints_Implementation() const
{
	ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>();
	check(CubePlayerState);
	return CubePlayerState->GetSkillPoints();
}

void ACubeCharacter::ShowDecalEffectActor_Implementation(FVector Scale, UMaterialInterface* DecalMaterial)
{
	if ( ACubePlayerController* CubePlayerController = Cast<ACubePlayerController>(GetController()) )
	{
		CubePlayerController->ShowDecalEffectActor(Scale, DecalMaterial);
		CubePlayerController->bShowMouseCursor = false;
	}
}

void ACubeCharacter::HideDecalEffectActor_Implementation()
{
	if ( ACubePlayerController* CubePlayerController = Cast<ACubePlayerController>(GetController()) )
	{
		CubePlayerController->HideDecalEffectActor();
		CubePlayerController->bShowMouseCursor = true;
	}
}

int32 ACubeCharacter::GetPlayerLevel_Implementation()
{
	const ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>(); // state를 가져오고,
	check(CubePlayerState);
	return CubePlayerState->GetPlayerLevel(); // state에서 레벨 얻은 후 리턴.
}

void ACubeCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ACubeCharacter::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void ACubeCharacter::InitAbilityActorInfo() // 어빌리티 시스템 컴포넌트, 어트리뷰트셋 초기화
{
	ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>();
	check(CubePlayerState); // 잘 가져왔는지 체크
	CubePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CubePlayerState, this);
	Cast<UCubeAbilitySystemComponent>(CubePlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = CubePlayerState->GetAbilitySystemComponent();
	AttributeSet = CubePlayerState->GetAttributeSet();
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FCubeGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ACubeCharacter::StunTagChanged);

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

	// 체력이 변경될때마다 업데이트하는 로직
	if ( UCubeUserWidget* CubeUserWidget = Cast<UCubeUserWidget>(HealthBar->GetUserWidgetObject()) )
	{
		CubeUserWidget->SetWidgetController(this);
	}

	if ( const UCubeAttributeSet* CubeAS = Cast<UCubeAttributeSet>(AttributeSet) )
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CubeAS->GetHealthAttribute()).AddLambda(
			[ this ] (const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CubeAS->GetMaxHealthAttribute()).AddLambda(
			[ this ] (const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		OnHealthChanged.Broadcast(CubeAS->GetHealth());
		OnMaxHealthChanged.Broadcast(CubeAS->GetMaxHealth());
	}
}

void ACubeCharacter::OnRep_Stunned()
{
	if ( UCubeAbilitySystemComponent* CubeASC = Cast<UCubeAbilitySystemComponent>(AbilitySystemComponent) )
	{
		const FCubeGameplayTags& GameplayTags = FCubeGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if ( bIsStunned )
		{
			CubeASC->AddLooseGameplayTags(BlockedTags);
			//StunDebuffComponent->Activate();
		}
		else
		{
			CubeASC->RemoveLooseGameplayTags(BlockedTags);
			//StunDebuffComponent->Deactivate();
		}
	}
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
	// 몽타주가 현재 재생 중인지 여부를 반환하는 코드
	return GetMesh()->GetAnimInstance()->Montage_IsPlaying(BagicAttackMontages[_n]);
}