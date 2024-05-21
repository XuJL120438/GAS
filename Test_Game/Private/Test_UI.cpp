// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_UI.h"

#include "DesktopPlatformModule.h"
#include "FileMediaSource.h"
#include "SlateOptMacros.h"
#include "Test_NewWindow.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Layout/SGridPanel.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


#define LOCTEXT_NAMESPACE "STest_UI"

void STest_UI::Construct(const FArguments& InArgs)
{
	InitComponentConfig();
	this->ChildSlot
	[
		SNew(SBox)
		.WidthOverride(50.f)
		.HeightOverride(50.f)
		[
			SNew(SBorder)
			.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SNew(SGridPanel)
					.FillColumn(0, 0.125f)
					.FillColumn(1, 0.125f)
						
					+ SGridPanel::Slot(0, 0).ColumnSpan(1).RowSpan(1).Padding(5.f, 10.f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).Padding(5).AutoHeight()
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("演示区域")))
							]
							+ SVerticalBox::Slot().HAlign(HAlign_Right).VAlign(VAlign_Center).Padding(5).AutoHeight()
							[
								SNew(SBox)
								.HeightOverride(500)
								.WidthOverride(400)
								[
									SNew(SBorder)
									.Padding(5)
									.BorderBackgroundColor(FLinearColor(0.8f, 0.8f, 0.8f, 1.f))
									.BorderImage(FAppStyle::GetBrush(TEXT("Menu.Background")))
									
								]
							]
						]
						+ SHorizontalBox::Slot()
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).Padding(5).AutoHeight()
							[
								SNew(STextBlock)
								.Text(FText::FromString(TEXT("选择需要演示的部件")))
							]
							+ SVerticalBox::Slot().Padding(5).AutoHeight()
							[
								SNew(SBox)
								.HeightOverride(500)
								.WidthOverride(400)
								[
									SNew(SBorder)
									.Padding(5)
									.BorderBackgroundColor(FLinearColor(0.8f, 0.8f, 0.8f, 1.f))
									.BorderImage(FAppStyle::GetBrush(TEXT("Menu.Background")))
									[
										SNew(SVerticalBox)
										+ SVerticalBox::Slot().Padding(0).AutoHeight()
										[
											SNew(SBox)
											.HeightOverride(50)
											[
												SNew(SBorder)
												.Padding(5)
												.BorderBackgroundColor(FLinearColor(0.8f, 0.8f, 0.8f, 1.f))
												.BorderImage(FAppStyle::GetBrush(TEXT("Menu.Background")))
												[
													SAssignNew(mEditableText, SSearchBox)
													.HintText(FText::FromString(TEXT("请输入需要搜寻的部件")))
													.OnTextCommitted(this, &STest_UI::OnTextChanged)
												]
											]
										]
										+ SVerticalBox::Slot().Padding(5)
										[
											SAssignNew(mCharView, STreeView<TSharedPtr<FLocal_Component>>)
											.ItemHeight(10)
											.SelectionMode(ESelectionMode::Multi)
											.OnGenerateRow( this, &STest_UI::OnGenerateRow)
											.OnGetChildren(this, &STest_UI::OnGetChildrenForTree)
											.TreeItemsSource(&mComponentDatas)
											.OnSelectionChanged(this, &STest_UI::OnSequenceSelectionChanged)
											.OnMouseButtonDoubleClick(this, &STest_UI::OnMouseButtonDoubleClick)
										]	
									]
								]
							]
							+ SVerticalBox::Slot().Padding(5).AutoHeight()
							[
								SNew(SBox)
								.HeightOverride(50)
								.WidthOverride(50)
								[
									SNew(SButton)
									.HAlign(HAlign_Center)
									.VAlign(VAlign_Center)
									.OnClicked(this, &STest_UI::OnClicked)
									[
										SNew(STextBlock)
										.Text(FText::FromString(TEXT("确认演示")))
									]
								]
							]
						]
						+ SHorizontalBox::Slot()
					]
				]
			]
		]
	];
}

TSharedRef<ITableRow> STest_UI::OnGenerateRow(TSharedPtr<FLocal_Component> Local_Component, const TSharedRef<STableViewBase>& TableViewBase)
{
	return SNew(STableRow<TSharedPtr<FLocal_Component>>, TableViewBase)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(STextBlock)
			.Text(FText::FromString(Local_Component->ComponentName))
			.Font(FCoreStyle::GetDefaultFontStyle("Roboto", 12))
			.ColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)))
		]
	];
}

void STest_UI::OnGetChildrenForTree(TSharedPtr<FLocal_Component> InItem, TArray<TSharedPtr<FLocal_Component>>& OutChildren)
{
	OutChildren = InItem->Children;
}

void STest_UI::OnSequenceSelectionChanged(TSharedPtr<FLocal_Component> InItem, ESelectInfo::Type SelectInfo)
{
	if (!InItem)
		return;
	FinalVedioPath = InItem->VideoPath;
}

void STest_UI::InitComponentConfig()
{
	TArray<FString> OutPath;
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	FString Title = TEXT("Choose a .ini file to init Component"),
			FileType = TEXT("*.ini;*.INI"),
			DefaultFile = TEXT("");
	const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	DesktopPlatform->OpenFileDialog(
			FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
			LOCTEXT("StatsLoadTitle", "Load EQS stats").ToString(),
			TEXT(""),
			TEXT(""),
			FileType,
			EFileDialogFlags::None,
			OutPath
			);
	// FString name = FPaths::ConvertRelativePathToFull(*OutPath[0]);
	if (OutPath.IsEmpty())
		return;
	TArray<FString>JsonPath;
	GConfig->GetArray(TEXT("MainCompoent"), TEXT("Component"), JsonPath, *OutPath[0]);
	for (auto Comp : JsonPath)
	{
		TArray<FString> NameAndPath;
		Comp.ParseIntoArray(NameAndPath, TEXT(";"));
		TSharedPtr<FLocal_Component> MainComp = MakeShareable(new FLocal_Component(NameAndPath[0], NameAndPath[1]));
		TArray<FString> Children;
		GConfig->GetArray(*NameAndPath[0], TEXT("Component"), Children, *OutPath[0]);
		for (auto Chile : Children)
		{
			TArray<FString> SNameAndPath;
			Chile.ParseIntoArray(SNameAndPath, TEXT(";"));
			TSharedPtr<FLocal_Component> ChilePtr = MakeShareable(new FLocal_Component(SNameAndPath[0], SNameAndPath[1]));
			MainComp->Children.Add(ChilePtr);
		}
		mComponentDatas.Add(MainComp);
		mTotalDatas.Add(MainComp);
	}
	
}

FReply STest_UI::OnClicked()
{
	TSharedPtr<SWindow> Window = SNew(SWindow)
		.Title(FText::FromString(TEXT("部件演示视频")))
		.ClientSize(FVector2D(1500, 980))
		.SupportsMinimize(false).SupportsMaximize(false);
	TSharedRef<STest_NewWindow> WidgetInstance = SNew(STest_NewWindow)
		.mVedioPath(*FinalVedioPath);
	Window->SetContent(WidgetInstance);
	FSlateApplication::Get().AddWindow(Window.ToSharedRef());
	Window->BringToFront(true);
	
	return FReply::Handled();
}

void STest_UI::OnMouseButtonDoubleClick(const TSharedPtr<FLocal_Component> Item )
{
	TSharedPtr<SWindow> Window = SNew(SWindow)
		.Title(FText::FromString(TEXT("部件演示视频")))
		.ClientSize(FVector2D(1500, 980))
		.SupportsMinimize(false).SupportsMaximize(false);
	TSharedRef<STest_NewWindow> WidgetInstance = SNew(STest_NewWindow)
		.mVedioPath(*Item->VideoPath);
	Window->SetContent(WidgetInstance);
	FSlateApplication::Get().AddWindow(Window.ToSharedRef());
	Window->BringToFront(true);
}

void STest_UI::OnTextChanged(const FText& InText, ETextCommit::Type CommitType)
{
	if (InText.IsEmpty())
	{
		mCharView->SetTreeItemsSource(&mComponentDatas);
		mCharView->RequestTreeRefresh();
		return;
	}
	mSearchDatas.Empty();
	for(auto Component : mComponentDatas)
	{
		if (Component->ComponentName.Contains(InText.ToString()))
		{
			mSearchDatas.AddUnique(Component);
		}
		for(auto Chiledren : Component->Children)
		{
			if (Chiledren->ComponentName.Contains(InText.ToString()))
			{
				mSearchDatas.AddUnique(Component);
			}
		}
	}
	mCharView->SetTreeItemsSource(&mSearchDatas);
	mCharView->RequestTreeRefresh();
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
