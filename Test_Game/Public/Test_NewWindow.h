// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MediaPlayer.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class TEST_GAME_API STest_NewWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STest_NewWindow)
		{
		}
	SLATE_ARGUMENT(FString, mVedioPath)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	TSharedPtr<SImage> mImage;
	UMediaPlayer* MediaPalyer;
	const FSlateBrush* Image;
	FString mVedioPath;
};
