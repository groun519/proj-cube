// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeEnemy.h"

#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemLibrary.h"
#include "P_Cube/AbilitySystem/CubeAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "P_Cube/P_Cube.h"
#include "P_Cube/UI/Widget/CubeUserWidget.h"
#include "P_Cube/CubeGameplayTags.h"
#include "P_Cube/AI/CubeAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACubeEnemy::ACubeEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UCubeAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UCubeAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar"); // 체력바 위젯 생성
	HealthBar->SetupAttachment(GetRootComponent()); 
}

void ACubeEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	CubeAIController = Cast<ACubeAIController>(NewController);
	CubeAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	CubeAIController->RunBehaviorTree(BehaviorTree);
	CubeAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	CubeAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::MeleeEnemy || CharacterClass != ECharacterClass::TankEnemy);
}

void ACubeEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ACubeEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 ACubeEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}

void ACubeEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	if (CubeAIController) CubeAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);

	Super::Die();
}

void ACubeEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ACubeEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void ACubeEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (CubeAIController && CubeAIController->GetBlackboardComponent())
	{
		CubeAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void ACubeEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UCubeAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	// 체력이 변경될때마다 업데이트하는 로직
	if (UCubeUserWidget* CubeUserWidget = Cast<UCubeUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		CubeUserWidget->SetWidgetController(this);
	}

	if (const UCubeAttributeSet* CubeAS = Cast<UCubeAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CubeAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CubeAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(FCubeGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&ACubeEnemy::HitReactTagChanged
		);

		OnHealthChanged.Broadcast(CubeAS->GetHealth());
		OnMaxHealthChanged.Broadcast(CubeAS->GetMaxHealth());
	}
}

void ACubeEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UCubeAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes(); // 몬스터 attribute 초기화. (CubeCharacterBase 함수.)
	}
}

void ACubeEnemy::InitializeDefaultAttributes() const
{
	UCubeAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
