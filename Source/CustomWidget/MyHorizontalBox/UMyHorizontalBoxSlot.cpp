// Fill out your copyright notice in the Description page of Project Settings.

#include "UMyHorizontalBoxSlot.h"
#include "Components/Widget.h"

UMyHorizontalBoxSlot::UMyHorizontalBoxSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HorizontalAlignment = HAlign_Fill;
	VerticalAlignment = VAlign_Fill;
	Slot = NULL;
	Size = FSlateChildSize(ESlateSizeRule::Automatic);
}

void UMyHorizontalBoxSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Slot = NULL;
}

void UMyHorizontalBoxSlot::BuildSlot(TSharedRef<SMyHorizontalBox> MyHorizontalBox)
{
	Slot = &MyHorizontalBox->AddSlot()
		.HAlign(HorizontalAlignment)
		.VAlign(VerticalAlignment)
		.Padding(Padding)
		[
			Content == NULL ? SNullWidget::NullWidget : Content->TakeWidget()
		];

	Slot->SizeParam = UWidget::ConvertSerializedSizeParamToRuntime(Size);
}

void UMyHorizontalBoxSlot::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if ( Slot )
	{
		Slot->Padding(InPadding);
	}
}

void UMyHorizontalBoxSlot::SetSize(FSlateChildSize InSize)
{
	Size = InSize;
	if ( Slot )
	{
		Slot->SizeParam = UWidget::ConvertSerializedSizeParamToRuntime(InSize);
	}
}

void UMyHorizontalBoxSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if ( Slot )
	{
		Slot->HAlign(InHorizontalAlignment);
	}
}

void UMyHorizontalBoxSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if ( Slot )
	{
		Slot->VAlign(InVerticalAlignment);
	}
}

void UMyHorizontalBoxSlot::SynchronizeProperties()
{
	SetPadding(Padding);
	SetSize(Size);
	SetHorizontalAlignment(HorizontalAlignment);
	SetVerticalAlignment(VerticalAlignment);
}