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
        UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass); // ������ �ؽ�Ʈ ������Ʈ ����
        DamageText->RegisterComponent(); // ������Ʈ�� ���� ���忡 ���
        DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform); // ������Ʈ�� ��Ʈ ������Ʈ �ڽ����� ����. ��Ʈ ������Ʈ�� Ʈ���������� ��ġ�� ������.
        DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform); // ���� ��ǥ�� ������ ���·�, ������Ʈ�� �и�.
        DamageText->SetDamageText(DamageAmount, bCriticalHit, bPhysicalHit, bMagicalHit, bPureHit); // ������ ���� ǥ���ϵ��� ������ �ؽ�Ʈ ����
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
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // ����Ʈ�� ���콺�� ������Ű�� �ʰ� ��.
    InputModeData.SetHideCursorDuringCapture(false); // ����Ʈ�� Ŭ���ϸ� Ŀ�� ������� ������ ������.
    SetInputMode(InputModeData); // ���� ��ǲ ������ Ȱ��ȭ��.
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
    if (InputTag.MatchesTagExact(FCubeGameplayTags::Get().InputTag_RMB)) // RMB�� Ű�� ��ġ�ϸ�, �±� �ο�
    {
        bTargeting = ThisActor ? true : false; // ThisActor�� �����ϸ� T, �ƴϸ� F �� bTargeting�� �Ҵ�.
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
    if (!InputTag.MatchesTagExact(FCubeGameplayTags::Get().InputTag_RMB)) // RMB�� �ƴ� ���
    {
        if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
        return; // ����
    }

    if (bTargeting) // �Ʊ� Pressed���� ���� bTargeting bool�� T��, (��� Ŭ�� ���ϰ� ��� ���� ���콺 ���� ä ȣ���� �ϴ� �� ����)
    {
        if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
    }
    else // F��
    {
        FollowTime += GetWorld()->GetDeltaSeconds(); // �̵�

        if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint; // ��ǥ������ ���콺 �Ʒ� ���ͷ� ����.

        if (APawn* ControlledPawn = GetPawn()) // ControlledPawn�� ������ ����.
        {
            const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
            ControlledPawn->AddMovementInput(WorldDirection); // WorldDirection �������� �̵�
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