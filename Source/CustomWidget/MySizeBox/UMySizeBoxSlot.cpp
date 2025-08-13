// Fill out your copyright notice in the Description page of Project Settings.


#include "UMySizeBoxSlot.h"
#include "Components/Widget.h"


/////////////////////////////////////////////////////
// UMySizeBoxSlot

UMySizeBoxSlot::UMySizeBoxSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Padding = FMargin(0, 0);

	HorizontalAlignment = HAlign_Fill;
	VerticalAlignment = VAlign_Fill;
}

void UMySizeBoxSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	SizeBox.Reset();
}

void UMySizeBoxSlot::BuildSlot(TSharedRef<SMySizeBox> InSizeBox)
{
	SizeBox = InSizeBox;

	SynchronizeProperties();

	SizeBox.Pin()->SetContent(Content ? Content->TakeWidget() : SNullWidget::NullWidget);
}

void UMySizeBoxSlot::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if ( SizeBox.IsValid() )
	{
		SizeBox.Pin()->SetPadding(InPadding);
	}
}

void UMySizeBoxSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if ( SizeBox.IsValid() )
	{
		SizeBox.Pin()->SetHAlign(InHorizontalAlignment);
	}
}

void UMySizeBoxSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if ( SizeBox.IsValid() )
	{
		SizeBox.Pin()->SetVAlign(InVerticalAlignment);
	}
}

void UMySizeBoxSlot::SynchronizeProperties()
{
	SetPadding(Padding);
	SetHorizontalAlignment(HorizontalAlignment);
	SetVerticalAlignment(VerticalAlignment);
}
