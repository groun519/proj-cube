#include "CubePlayerController.h"

#include "P_Cube/Character/CubeCharacter.h"
#include "Blueprint/UserWidget.h"
#include "InputActionValue.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "P_Cube/Interaction/EnemyInterface.h"

ACubePlayerController::ACubePlayerController()
{
    bReplicates = true;
    bShowMouseCursor = true;
}

void ACubePlayerController::BeginPlay()
{
    Super::BeginPlay();
    check(CubeContext);

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    check(Subsystem);
    Subsystem->AddMappingContext(CubeContext, 0);

    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // ����Ʈ�� ���콺�� ������Ű�� �ʰ� ��.
    InputModeData.SetHideCursorDuringCapture(false); // ����Ʈ�� Ŭ���ϸ� Ŀ�� ������� ������ ������.
    SetInputMode(InputModeData); // ���� ��ǲ ������ Ȱ��ȭ��.

    Character = Cast<ACubeCharacter>(GetPawn());

    if (HUDOverlayAsset)
    {
        HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
    }

    HUDOverlay->AddToViewport(); // ����Ʈ�� ����
    HUDOverlay->SetVisibility(ESlateVisibility::Visible); // HUD ���̰�
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
        //UE_LOG(LogTemp, Warning, TEXT("success"));
    }
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

    UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(InputComponent);

    if (PEI)
    {
        PEI->BindAction(InputMouseRight, ETriggerEvent::Triggered, this, &ACubePlayerController::MouseRightPressed);
        PEI->BindAction(InputMouseRight, ETriggerEvent::Completed, this, &ACubePlayerController::MouseRightCompleted);
    }
    else
    {
        // Handle the case when the input component is null
        //UE_LOG(LogTemp, Error, TEXT("Enhanced Input Component is null!"));
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

        // Ÿ�ٰ��� �Ÿ��� 90 ������ ��
        if (FVector::Dist(TargetVec, PlayerLocation) <= 90.0f)
        {
            Character->Attack();
        }
    }
}
