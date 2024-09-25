#include "CubePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "P_Cube/CubeGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "P_Cube/AbilitySystem/CubeAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "P_Cube/Input/CubeInputComponent.h"
#include "P_Cube/Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
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
}

void ACubePlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bCriticalHit, bool bPhysicalHit, bool bMagicalHit, bool bPureHit)
{
    if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
    {
        UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass); // 데미지 텍스트 컴포넌트 생성
        DamageText->RegisterComponent(); // 컴포넌트를 게임 월드에 등록
        DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); // 컴포넌트를 루트 컴포넌트 자식으로 설정. 루트 컴포넌트의 트랜스폼으로 위치가 수정됨.
        DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform); // 월드 좌표를 유지한 상태로, 컴포넌트를 분리.
        DamageText->SetDamageText(DamageAmount, bCriticalHit, bPhysicalHit, bMagicalHit, bPureHit); // 데미지 양을 표시하도록 데미지 텍스트 설정
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

void ACubePlayerController::CursorTrace()
{
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
    if (!CursorHit.bBlockingHit) return;

    LastActor = ThisActor;
    ThisActor = CursorHit.GetActor();

    if (LastActor != ThisActor)
    {
        if (LastActor) LastActor->UnHighlightActor();
        if (ThisActor) ThisActor->HighlightActor();
    }
}

void ACubePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (InputTag.MatchesTagExact(FCubeGameplayTags::Get().InputTag_RMB)) // RMB와 키가 일치하면, 태그 부여
    {
        bTargeting = ThisActor ? true : false; // ThisActor가 존재하면 T, 아니면 F 를 bTargeting에 할당.
        bAutoRunning = false;
    }
}

void ACubePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (!InputTag.MatchesTagExact(FCubeGameplayTags::Get().InputTag_RMB))
    {
        if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
        return;
    }

    if (bTargeting)
    {
        if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
    }
    else
    {
        const APawn* ControlledPawn = GetPawn();
        if (FollowTime <= ShortPressThreshold && ControlledPawn)
        {
            if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
            {
                Spline->ClearSplinePoints();
                for (const FVector& PointLoc : NavPath->PathPoints)
                {
                    Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
                    //DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
                }
                if (NavPath->PathPoints.Num() > 0)
                {
                    CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
                    bAutoRunning = true;
                }
            }
        }
        FollowTime = 0.f;
        bTargeting = false;
    }
}

void ACubePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (!InputTag.MatchesTagExact(FCubeGameplayTags::Get().InputTag_RMB)) // RMB가 아닐 경우
    {
        if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
        return; // 나가
    }

    if (bTargeting) // 아까 Pressed에서 받은 bTargeting bool이 T면, (대상 클릭 안하고 대상 위로 마우스 누른 채 호버링 하는 것 방지)
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

void ACubePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UCubeInputComponent* CubeInputComponent = CastChecked<UCubeInputComponent>(InputComponent);
       
    //CubeInputComponent->BindAction(InputMouseRight, ETriggerEvent::Triggered, this, &ACubePlayerController::MouseRightPressed);
    //CubeInputComponent->BindAction(InputMouseRight, ETriggerEvent::Completed, this, &ACubePlayerController::MouseRightCompleted);
    CubeInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}