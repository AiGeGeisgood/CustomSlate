// Fill out your copyright notice in the Description page of Project Settings.

/////////////////////////////////////////////////////
// UMyUSlot03

#include "UMyScaleBoxSlot.h"
#include "ObjectEditorUtils.h"
#include "UMyScaleBox.h"
#include "Components/Widget.h"

/////////////////////////////////////////////////////
// UMyScaleBoxSlot

UMyScaleBoxSlot::UMyScaleBoxSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Padding = FMargin(0, 0);

	HorizontalAlignment = HAlign_Center;
	VerticalAlignment = VAlign_Center;
}

void UMyScaleBoxSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	ScaleBox.Reset();
}

void UMyScaleBoxSlot::BuildSlot(TSharedRef<SMyScaleBox> InScaleBox)
{
	ScaleBox = InScaleBox;

	//ScaleBox->SetPadding(Padding);
	ScaleBox.Pin()->SetHAlign(HorizontalAlignment);
	ScaleBox.Pin()->SetVAlign(VerticalAlignment);

	ScaleBox.Pin()->SetContent(Content ? Content->TakeWidget() : SNullWidget::NullWidget);
}

void UMyScaleBoxSlot::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if ( ScaleBox.IsValid() )
	{
		//ScaleBox.Pin()->SetPadding(InPadding);
	}
}

void UMyScaleBoxSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if ( ScaleBox.IsValid() )
	{
		ScaleBox.Pin()->SetHAlign(InHorizontalAlignment);
	}
}

void UMyScaleBoxSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if ( ScaleBox.IsValid() )
	{
		ScaleBox.Pin()->SetVAlign(InVerticalAlignment);
	}
}

void UMyScaleBoxSlot::SynchronizeProperties()
{
	SetPadding(Padding);
	SetHorizontalAlignment(HorizontalAlignment);
	SetVerticalAlignment(VerticalAlignment);
}
