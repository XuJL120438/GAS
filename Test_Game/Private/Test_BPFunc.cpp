// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_BPFunc.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Test_UI.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SGridPanel.h"

#define LOCTEXT_NAMESPACE "UTest_BPFunc"

void UTest_BPFunc::SetWindowWidget(const TSharedRef<SWindow> InWindows)
{
	WindowWidget = InWindows;
	InWindows->SetContent(Window.ToSharedRef());
}

TSharedRef<SWidget> UTest_BPFunc::RebuildWidget()
{
	TSharedPtr<SWidget> Widget = SNew(STest_UI);
	return Widget.ToSharedRef();
}