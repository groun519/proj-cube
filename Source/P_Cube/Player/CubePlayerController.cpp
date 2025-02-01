#include "CubePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "P_Cube/CubeGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "P_Cube/Actor/DecalEffectActor.h"
#include "P_Cube/P_Cube.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "P_Cube/Input/CubeInputComponent.h"
#include "P_Cube/Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "P_Cube/Interaction/HighlightInterface.h"
#include "P_Cube/UI/Widget/DamageTextComponent.h"

ACubePlayerController::ACubePlayerController()
{
    bReplicates = true;
    Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void ACubePlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    CursorTrace();
    AutoRun();
    UpdateDecalEffectActorLocation();
}

void ACubePlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bCriticalHit, bool bPhysicalHit, bool bMagicalHit, bool bPureHit, bool bHealHit)
{
    if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
    {
        UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass); // 데미지 텍스트 컴포넌트 생성
        DamageText->RegisterComponent(); // 컴포넌트를 게임 월드에 등록
        DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); // 컴포넌트를 루트 컴포넌트 자식으로 설정. 루트 컴포넌트의 트랜스폼으로 위치가 수정됨.
        DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform); // 월드 좌표를 유지한 상태로, 컴포넌트를 분리.
        DamageText->SetDamageText(DamageAmount, bCriticalHit, bPhysicalHit, bMagicalHit, bPureHit, bHealHit); // 데미지 양을 표시하도록 데미지 텍스트 설정
    }
}

void ACubePlayerController::ShowDecalEffectActor(FVector Scale, UMaterialInterface* DecalMaterial)
{
    if ( !IsValid(DecalEffectActor) )
    {
        DecalEffectActor = GetWorld()->SpawnActor<ADecalEffectActor>(DecalEffectActorClass);
        DecalEffectActor->SetActorRelativeScale3D(Scale);
        if ( DecalMaterial )
        {
            DecalEffectActor->DecalComp->SetMaterial(0, DecalMaterial);
        }
    }
}

void ACubePlayerController::HideDecalEffectActor()
{
    if ( IsValid(DecalEffectActor) )
    {
        DecalEffectActor->Destroy();
    }
}

void ACubePlayerController::BeginPlay()
{
    Super::BeginPlay();
    check(CubeContext);

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        Subsystem->AddMappingContext(CubeContext, 0);
    }

    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 뷰포트에 마우스를 고정시키지 않게 함.
    InputModeData.SetHideCursorDuringCapture(false); // 뷰포트를 클릭하면 커서 사라지는 현상을 방지함.
    SetInputMode(InputModeData); // 위의 인풋 세팅을 활성화함.
}

void ACubePlayerController::HighlightActor(AActor* InActor)
{
    if ( IsValid(InActor) && InActor->Implements<UHighlightInterface>() )
    {
        IHighlightInterface::Execute_HighlightActor(InActor);
    }
}

void ACubePlayerController::UnHighlightActor(AActor * InActor)
{
    if ( IsValid(InActor) && InActor->Implements<UHighlightInterface>() )
    {
        IHighlightInterface::Execute_UnHighlightActor(InActor);
    }
}

void ACubePlayerController::CursorTrace()
{
    if ( GetASC() && GetASC()->HasMatchingGameplayTag(FCubeGameplayTags::Get().Player_Block_CursorTrace) )
    {
        UnHighlightActor(LastActor);
		UnHighlightActor(ThisActor);
		if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())

        LastActor = nullptr;
        ThisActor = nullptr;
        return;
    }

    const ECollisionChannel TraceChannel = IsValid(DecalEffectActor) ? ECC_ExcludePlayers : ECC_Visibility;
    GetHitResultUnderCursor(TraceChannel, false, CursorHit);
    if ( !CursorHit.bBlockingHit ) return;

    LastActor = ThisActor;
    if ( IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>() )
    {
        ThisActor = CursorHit.GetActor();
    }
    else
    {
        ThisActor = nullptr;
    }

    if ( LastActor != ThisActor )
    {
        UnHighlightActor(LastActor);
        HighlightActor(ThisActor);
    }
}

void ACubePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if ( GetASC() && GetASC()->HasMatchingGameplayTag(FCubeGameplayTags::Get().Player_Block_InputPressed) )
    {
        return;
    }
    if (InputTag.MatchesTagExact(FCubeGameplayTags::Get().InputTag_RMB)) // RMB와 키가 일치하면, 태그 부여
    {
        if ( IsValid(ThisActor) )
        {
            TargetingStatus = ThisActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
        }
        else
        {
            TargetingStatus = ETargetingStatus::NotTargeting;
        }
        bAutoRunning = false;
    }
    if ( GetASC() ) GetASC()->AbilityInputTagPressed(InputTag);
}

void ACubePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if ( GetASC() && GetASC()->HasMatchingGameplayTag(FCubeGameplayTags::Get().Player_Block_InputReleased) )
    {
        return;
    }
    if (!InputTag.MatchesTagExact(FCubeGameplayTags::Get().InputTag_RMB))
    {
        if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
        return;
    }

    if ( GetASC() ) GetASC()->AbilityInputTagReleased(InputTag);

    if ( TargetingStatus != ETargetingStatus::TargetingEnemy )
    {
        const APawn* ControlledPawn = GetPawn();
        if ( FollowTime <= ShortPressThreshold && ControlledPawn )
        {
            if ( IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>() )
            {
                IHighlightInterface::Execute_SetMoveToLocation(ThisActor, CachedDestination);
            }
            else if ( GetASC() && !GetASC()->HasMatchingGameplayTag(FCubeGameplayTags::Get().Player_Block_InputPressed) )
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
            }
            if ( UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination) )
            {
                Spline->ClearSplinePoints();
                for ( const FVector& PointLoc : NavPath->PathPoints )
                {
                    Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
                    //DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 3.f);
                }
                if ( NavPath->PathPoints.Num() > 0 )
                {
                    CachedDestination = NavPath->PathPoints[ NavPath->PathPoints.Num() - 1 ];
                    bAutoRunning = true;
                }
            }
        }
        FollowTime = 0.f;
        TargetingStatus = ETargetingStatus::NotTargeting;
    }
}

void ACubePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if ( GetASC() && GetASC()->HasMatchingGameplayTag(FCubeGameplayTags::Get().Player_Block_InputHeld) )
    {
        return;
    }
    if (!InputTag.MatchesTagExact(FCubeGameplayTags::Get().InputTag_RMB)) // RMB가 아닐 경우
    {
        if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
        return; // 나가
    }

    if ( TargetingStatus == ETargetingStatus::TargetingEnemy ) // 아까 Pressed에서 받은 bTargeting bool이 T면, (대상 클릭 안하고 대상 위로 마우스 누른 채 호버링 하는 것 방지)
    {
        if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
    }
    else // F면
    {
        FollowTime += GetWorld()->GetDeltaSeconds(); // 이동

        if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint; // 목표지점을 마우스 아래 벡터로 설정.

        if (APawn* ControlledPawn = GetPawn()) // ControlledPawn에 본인을 넣음.
        {
            const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
            ControlledPawn->AddMovementInput(WorldDirection); // WorldDirection 방향으로 이동
        }
    }
}

UCubeAbilitySystemComponent* ACubePlayerController::GetASC()
{
    if (CubeAbilitySystemComponent == nullptr)
    {
        CubeAbilitySystemComponent = Cast<UCubeAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
    }
    return CubeAbilitySystemComponent;
}

void ACubePlayerController::AutoRun()
{
    if (!bAutoRunning) return;
    if (APawn* ControlledPawn = GetPawn())
    {
        const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
        const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
        ControlledPawn->AddMovementInput(Direction);

        const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
        if (DistanceToDestination <= AutoRunAcceptanceRadius)
        {
            bAutoRunning = false;
        }
    }
}

void ACubePlayerController::UpdateDecalEffectActorLocation()
{
    if ( IsValid(DecalEffectActor) )
    {
        DecalEffectActor->SetActorLocation(CursorHit.ImpactPoint);
    }
}

void ACubePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UCubeInputComponent* CubeInputComponent = CastChecked<UCubeInputComponent>(InputComponent);
       
    //CubeInputComponent->BindAction(InputMouseRight, ETriggerEvent::Triggered, this, &ACubePlayerController::MouseRightPressed);
    //CubeInputComponent->BindAction(InputMouseRight, ETriggerEvent::Completed, this, &ACubePlayerController::MouseRightCompleted);
    CubeInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}