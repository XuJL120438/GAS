// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Test_BPFunc.generated.h"

/**
 * 
 */


UCLASS()
class TEST_GAME_API UTest_BPFunc : public UUserWidget
{
	GENERATED_BODY()
private:
	TWeakPtr<SWindow>  WindowWidget;
	TSharedPtr<SWidget> Window;
public:
	void SetWindowWidget(const TSharedRef<SWindow> InWindows);
	TSharedRef<SWidget> RebuildWidget() override;
};
