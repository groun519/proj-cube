#include "CubePlayerController.h"

#include "P_Cube/Character/CubeCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "P_Cube/Interaction/EnemyInterface.h"

ACubePlayerController::ACubePlayerController()
{
    bReplicates = true;
    bShowMouseCursor = true;
}

void ACubePlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    CursorTrace();

    if (bClickRightMouse)
    {
        SetTarget();
        MoveToMouseCursor();
    }

    if (HaveTarget)
    {
        TraceHitTarget();
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

    Character = Cast<ACubeCharacter>(GetPawn());
}

void ACubePlayerController::CursorTrace()
{
    FHitResult CursorHit;
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
    if (!CursorHit.bBlockingHit) return;

    LastActor = ThisActor;
    ThisActor = CursorHit.GetActor();

    if (LastActor == nullptr)
    {
        if (ThisActor != nullptr)
        {
            ThisActor->HighlightActor();
        }
        else
        {

        }
    }
    else
    {
        if (ThisActor == nullptr)
        {
            LastActor->UnHighlightActor();
        }
        else
        {
            if (LastActor != ThisActor)
            {
                LastActor->UnHighlightActor();
                ThisActor->HighlightActor();
            }
            else
            {

            }
        }
    }
}

void ACubePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    if (EnhancedInputComponent)
    {
        EnhancedInputComponent->BindAction(InputMouseRight, ETriggerEvent::Triggered, this, &ACubePlayerController::MouseRightPressed);
        EnhancedInputComponent->BindAction(InputMouseRight, ETriggerEvent::Completed, this, &ACubePlayerController::MouseRightCompleted);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Enhanced Input Component is null!"));
    }
}

void ACubePlayerController::MouseRightPressed(const FInputActionValue& Value) // mouse right click down
{
    bClickRightMouse = true;
}
void ACubePlayerController::MouseRightCompleted(const FInputActionValue& Value) // mouse right click up
{
    bClickRightMouse = false;
}



void ACubePlayerController::MoveToMouseCursor() // mouse cursor 
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        Character->MoveDest(Hit.ImpactPoint);

        AActor* HitActor = Hit.GetActor();
        if (HitActor)
        {
            TArray<FName> ActorTags = HitActor->Tags;
            if (ActorTags.Num() > 0)
            {
                FName ActorTag = ActorTags[0];
                //UE_LOG(LogTemp, Warning, TEXT("Hit Actor Tag: %s"), *ActorTag.ToString());
            }
            else
            {
                //UE_LOG(LogTemp, Warning, TEXT("Hit Actor has no tags"));
            }
        }
        else
        {
            //UE_LOG(LogTemp, Warning, TEXT("Hit Actor is null"));
        }
    }
    else
    {
        //UE_LOG(LogTemp, Warning, TEXT("No blocking hit detected"));
    }
}





void ACubePlayerController::SetTarget()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit) {
        if (Hit.GetActor()->Tags.Contains("Monster"))
        {
            Target = Hit.GetActor();
            TargetVec = Hit.GetActor()->GetActorLocation();
            Character-> MoveDest(TargetVec);
            HaveTarget = true;
            //UE_LOG(LogTemp, Warning, TEXT("TargetVec: %s"), *TargetVec.ToString());
            //UE_LOG(LogTemp, Warning, TEXT("that's monster!!!"));
        }
        else {
            HaveTarget = false;
            //UE_LOG(LogTemp, Warning, TEXT("That's not monster TT"));
        }
    }
}

void ACubePlayerController::TraceHitTarget()
{
    //UE_LOG(LogTemp, Warning, TEXT("Trace start"));
    //AClickMoveCharacter* MyCharacter = Cast<AClickMoveCharacter>(GetPawn());

    TargetVec = Target->GetActorLocation();
    //UE_LOG(LogTemp, Warning, TEXT("Target Actor Vec: %s"), *TargetVec.ToString());

    Character->MoveDest(TargetVec);

    if (Character && HaveTarget)
    {

        FVector PlayerLocation = Character->GetActorLocation();

        //UE_LOG(LogTemp, Warning, TEXT("dist: %f"), FVector::Dist(TargetVec, PlayerLocation));

        // 타겟과의 거리가 90 이하일 때
        if (FVector::Dist(TargetVec, PlayerLocation) <= 90.0f)
        {
            Character->Attack();
        }
    }
}
