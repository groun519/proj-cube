// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P_Cube/Player/CubePlayerController.h"
#include "P_Cube/Player/CubePlayerState.h"
#include "P_Cube/UI/HUD/CubeHUD.h"


#include "P_Cube/Weapon.h"
#include <Components/CapsuleComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>

#include <Blueprint/AIBlueprintHelperLibrary.h>

// Sets default values
ACubeCharacter::ACubeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;



	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = 800.0f;
	SpringArmComponent->SetRelativeRotation(FRotator(-60.0f, 45.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;


	HaveWeapon = false;
	WeaponNum = 0;
	n = 0;
}

void ACubeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 서버의 ability actor 정보 초기화 (AbilitySystemComponent, AttributeSet)
	InitAbilityActorInfo();
}

void ACubeCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 클라이언트의 ability actor 정보 초기화 (AbilitySystemComponent, AttributeSet)
	InitAbilityActorInfo();
}

void ACubeCharacter::InitAbilityActorInfo() // 어빌리티 시스템 컴포넌트, 어트리뷰트셋 초기화
{
	ACubePlayerState* CubePlayerState = GetPlayerState<ACubePlayerState>();
	check(CubePlayerState); // 잘 가져왔는지 체크
	CubePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CubePlayerState, this);
	AbilitySystemComponent = CubePlayerState->GetAbilitySystemComponent();
	AttributeSet = CubePlayerState->GetAttributeSet();

	if (ACubePlayerController* CubePlayerController = Cast<ACubePlayerController>(GetController()))
	{
		if (ACubeHUD* CubeHUD = Cast<ACubeHUD>(CubePlayerController->GetHUD()))
		{
			CubeHUD->InitOverlay(CubePlayerController, CubePlayerState, AbilitySystemComponent, AttributeSet);
		}
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
	// 바뀐 AWeapon의 메쉬는 자동적으로 적용됨
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
	// 몽타주가 현재 재생 중인지 여부를 반환하는 코드
	return GetMesh()->GetAnimInstance()->Montage_IsPlaying(BagicAttackMontages[_n]);
}