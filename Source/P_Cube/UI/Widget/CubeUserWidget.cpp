// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Cube/UI/Widget/CubeUserWidget.h"

void UCubeUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
