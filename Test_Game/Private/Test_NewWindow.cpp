// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_NewWindow.h"

#include "MediaPlayer.h"
#include "SlateOptMacros.h"
#include "Test_BPFunc.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STest_NewWindow::Construct(const FArguments& InArgs)
{
	FSlateBrush* ImageBrush = new FSlateBrush;
	MediaPalyer = LoadObject<UMediaPlayer>(nullptr, TEXT("/Game/MP_MedioPlayer"));
	UMaterial* Mat = LoadObject<UMaterial>(nullptr, TEXT("/Game/M_Medio"));
	MediaPalyer->OpenFile(InArgs._mVedioPath);
	if (Mat)
	{
		ImageBrush->SetResourceObject(Mat);
		Image = ImageBrush;
	}
	else
		Image = FAppStyle::GetBrush(TEXT("Menu.Background"));
		
	ChildSlot
	[
			SAssignNew(mImage, SImage)
			.Image(Image)
	];
	
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
