// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MediaPlayer.h"
#include "MediaPlayer.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */

struct FLocal_Component
{
	FString ComponentName;
	TArray<TSharedPtr<FLocal_Component>> Children;
	FString VideoPath;
	FLocal_Component(FString Name, FString VideoPath)
	{
		ComponentName = Name;
		this->VideoPath = VideoPath;
	}
};

class TEST_GAME_API STest_UI : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STest_UI)
		{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FLocal_Component> Local_Component, const TSharedRef<STableViewBase>& TableViewBase);
	void OnGetChildrenForTree( TSharedPtr<FLocal_Component> InItem, TArray<TSharedPtr<FLocal_Component>>& OutChildren);
	void OnSequenceSelectionChanged(TSharedPtr<FLocal_Component> InItem, ESelectInfo::Type SelectInfo);
	void InitComponentConfig();
	FReply OnClicked();
	TSharedPtr<STreeView<TSharedPtr<FLocal_Component>>> mCharView;
	TSharedPtr<SSearchBox> mEditableText;
	TArray<TSharedPtr<FLocal_Component>> mComponentDatas;
	TArray<TSharedPtr<FLocal_Component>> mSearchDatas;
	TArray<TSharedPtr<FLocal_Component>> mTotalDatas;
	FString FinalVedioPath;
	void OnMouseButtonDoubleClick(const TSharedPtr<FLocal_Component> Item );
	void OnTextChanged(const FText& InText, ETextCommit::Type CommitType);
};
