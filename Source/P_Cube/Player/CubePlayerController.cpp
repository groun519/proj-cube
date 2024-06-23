#include "CubePlayerController.h"
#include "P_Cube/Character/CubeCharacter.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"

ACubePlayerController::ACubePlayerController(const FObjectInitializer& ObjectInitalizer)
    : APlayerController(ObjectInitalizer)
{
    bShowMouseCursor = true;
}

void ACubePlayerController::BeginPlay()
{
    Super::BeginPlay();

    Character = Cast<ACubeCharacter>(GetPawn());

    if (HUDOverlayAsset)
    {
        HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
    }
    HUDOverlay->AddToViewport(); // 뷰포트에 생성
    HUDOverlay->SetVisibility(ESlateVisibility::Visible); // HUD 보이게

    // Add Input Mapping Context
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->ClearAllMappings();
        Subsystem->AddMappingContext(InputMapping, 0);
    }
}

void ACubePlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if (bClickRightMouse)
    {
        SetTarget();
        MoveToMouseCursor();
    }

    if (HaveTarget)
    {
        TraceHitTarget();
        UE_LOG(LogTemp, Warning, TEXT("success"));
    }
}

void ACubePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(InputComponent);

    if (PEI)
    {
        PEI->BindAction(InputMouseRight, ETriggerEvent::Triggered, this, &ACubePlayerController::MouseRightPressed);
        PEI->BindAction(InputMouseRight, ETriggerEvent::Completed, this, &ACubePlayerController::MouseRightCompleted);
    }
    else
    {
        // Handle the case when the input component is null
        UE_LOG(LogTemp, Error, TEXT("Enhanced Input Component is null!"));
    }
}

void ACubePlayerController::MouseRightPressed(const FInputActionValue& Value) // mouse right click down
{
    bClickRightMouse = true;
    //UE_LOG(LogTemp, Error, TEXT("Mouse Right Pressed!"));
}
void ACubePlayerController::MouseRightCompleted(const FInputActionValue& Value) // mouse right click up
{
    bClickRightMouse = false;
    //UE_LOG(LogTemp, Error, TEXT("Mouse Right Pressed!"));
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
                UE_LOG(LogTemp, Warning, TEXT("Hit Actor Tag: %s"), *ActorTag.ToString());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Hit Actor has no tags"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Hit Actor is null"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No blocking hit detected"));
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
            UE_LOG(LogTemp, Warning, TEXT("that's monster!!!"));
        }
        else {
            HaveTarget = false;
            UE_LOG(LogTemp, Warning, TEXT("That's not monster TT"));
        }
    }
}

void ACubePlayerController::TraceHitTarget()
{
    UE_LOG(LogTemp, Warning, TEXT("Trace start"));
    //AClickMoveCharacter* MyCharacter = Cast<AClickMoveCharacter>(GetPawn());

    TargetVec = Target->GetActorLocation();
    UE_LOG(LogTemp, Warning, TEXT("Target Actor Vec: %s"), *TargetVec.ToString());

    Character->MoveDest(TargetVec);

    if (Character && HaveTarget)
    {

        FVector PlayerLocation = Character->GetActorLocation();

        UE_LOG(LogTemp, Warning, TEXT("dist: %f"), FVector::Dist(TargetVec, PlayerLocation));

        // 타겟과의 거리가 90 이하일 때
        if (FVector::Dist(TargetVec, PlayerLocation) <= 90.0f)
        {
            Character->Attack();
        }
    }
}
