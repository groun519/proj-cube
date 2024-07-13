// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/UI/HUD/CubeHUD.h"

#include "P_Cube/UI/Widget/CubeUserWidget.h"
#include "P_Cube/UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* ACubeHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams); // 플레이어 컨트롤러, 플레이어 스테이트, 어빌리티 시스템, 어트리뷰트셋의 정보대로 위젯을 설정함.

		OverlayWidgetController->BindCallbacksToDependencies();
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void ACubeHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 만약 오버레이 위젯 클래스나 위젯 컨트롤러 클래스를 할당하지 않았다면, checkf문으로 로그를 통해 알려줌.
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UCubeUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController); // WidgetControllerParams에 담긴 플레이어의 정보를 토대로 유저위젯들을 설정함. (Event Widget Controller Set 블루프린트가 트리거 됨.)

	WidgetController->BroadcastInitialValues(); // 값 변경을 블루프린트로 브로드캐스팅 하는 함수
	Widget->AddToViewport(); // 뷰포트에 반영함.
}
