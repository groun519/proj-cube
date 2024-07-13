// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/UI/HUD/CubeHUD.h"

#include "P_Cube/UI/Widget/CubeUserWidget.h"
#include "P_Cube/UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* ACubeHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams); // �÷��̾� ��Ʈ�ѷ�, �÷��̾� ������Ʈ, �����Ƽ �ý���, ��Ʈ����Ʈ���� ������� ������ ������.

		OverlayWidgetController->BindCallbacksToDependencies();
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void ACubeHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// ���� �������� ���� Ŭ������ ���� ��Ʈ�ѷ� Ŭ������ �Ҵ����� �ʾҴٸ�, checkf������ �α׸� ���� �˷���.
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UCubeUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController); // WidgetControllerParams�� ��� �÷��̾��� ������ ���� ������������ ������. (Event Widget Controller Set �������Ʈ�� Ʈ���� ��.)

	WidgetController->BroadcastInitialValues(); // �� ������ �������Ʈ�� ��ε�ĳ���� �ϴ� �Լ�
	Widget->AddToViewport(); // ����Ʈ�� �ݿ���.
}
