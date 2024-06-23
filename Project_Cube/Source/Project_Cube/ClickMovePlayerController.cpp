
#include "ClickMovePlayerController.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>

AClickMovePlayerController::AClickMovePlayerController() {
	bShowMouseCursor = true;
}

void AClickMovePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AClickMovePlayerController::InputLeftMouseButtonPressed);
	InputComponent->BindAction("LeftClick", IE_Released, this, &AClickMovePlayerController::InputLeftMouseButtonReleased);
}

void AClickMovePlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	if (bClickLeftMouse) {
		MoveToMouseCursor();
	}
}

void AClickMovePlayerController::InputLeftMouseButtonPressed() {
	bClickLeftMouse = true;
}

void AClickMovePlayerController::InputLeftMouseButtonReleased() {
	bClickLeftMouse = false;
}

void AClickMovePlayerController::SetNewDestination(const FVector Destination) {
	APawn* const MyPawn = GetPawn();

	if (MyPawn) {
		float const Distance = FVector::Dist(Destination, MyPawn->GetActorLocation());
		if (Distance > 120.0f) {
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Destination);
		}
	}
}

void AClickMovePlayerController::MoveToMouseCursor() {
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit) {
		SetNewDestination(Hit.ImpactPoint);
	}
}