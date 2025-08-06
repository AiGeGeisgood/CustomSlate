// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyNameSlot.h"
#include "SMyNameSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/Text/STextBlock.h"


#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UMyNameSlot

UMyNameSlot::UMyNameSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = true;
	Visibility = ESlateVisibility::SelfHitTestInvisible;
}

void UMyNameSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyBox.Reset();
}

TSharedRef<SWidget> UMyNameSlot::RebuildWidget()
{
	MyBox = SNew(SMyNameSlot);

	if ( IsDesignTime() )
	{
		MyBox->SetContent(
			SNew(SMyNameSlot)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromName(GetFName()))
			]
		);
	}

	// Add any existing content to the new slate box
	if ( GetChildrenCount() > 0 )
	{
		UPanelSlot* ContentSlot = GetContentSlot();
		if ( ContentSlot->Content )
		{
			MyBox->SetContent(ContentSlot->Content->TakeWidget());
		}
	}

	return MyBox.ToSharedRef();
}

void UMyNameSlot::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live slot if it already exists
	if ( MyBox.IsValid() && InSlot->Content )
	{
		MyBox->SetContent(InSlot->Content->TakeWidget());
	}
}

void UMyNameSlot::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( MyBox.IsValid() )
	{
		MyBox->SetContent(SNullWidget::NullWidget);

		if ( IsDesignTime() )
		{
			MyBox->SetContent(
				SNew(SMyNameSlot)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromName(GetFName()))
				]
			);
		}
	}
}

#if WITH_EDITOR

const FText UMyNameSlot::GetPaletteCategory()
{
	return LOCTEXT("AggWidget", "AggWidget");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE