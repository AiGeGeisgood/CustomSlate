// Fill out your copyright notice in the Description page of Project Settings.

#include "MyUSlot02.h"
#include "Components/Widget.h"

UMyUSlot02::UMyUSlot02(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HorizontalAlignment = HAlign_Fill;
	VerticalAlignment = VAlign_Fill;
	Slot = NULL;
	Size = FSlateChildSize(ESlateSizeRule::Automatic);
}

void UMyUSlot02::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Slot = NULL;
}

void UMyUSlot02::BuildSlot(TSharedRef<SMySWidget02> MySWidget02)
{
	Slot = &MySWidget02->AddSlot()
		.HAlign(HorizontalAlignment)
		.VAlign(VerticalAlignment)
		.Padding(Padding)
		[
			Content == NULL ? SNullWidget::NullWidget : Content->TakeWidget()
		];

	Slot->SizeParam = UWidget::ConvertSerializedSizeParamToRuntime(Size);
}

void UMyUSlot02::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if ( Slot )
	{
		Slot->Padding(InPadding);
	}
}

void UMyUSlot02::SetSize(FSlateChildSize InSize)
{
	Size = InSize;
	if ( Slot )
	{
		Slot->SizeParam = UWidget::ConvertSerializedSizeParamToRuntime(InSize);
	}
}

void UMyUSlot02::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if ( Slot )
	{
		Slot->HAlign(InHorizontalAlignment);
	}
}

void UMyUSlot02::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if ( Slot )
	{
		Slot->VAlign(InVerticalAlignment);
	}
}

void UMyUSlot02::SynchronizeProperties()
{
	SetPadding(Padding);
	SetSize(Size);
	SetHorizontalAlignment(HorizontalAlignment);
	SetVerticalAlignment(VerticalAlignment);
}