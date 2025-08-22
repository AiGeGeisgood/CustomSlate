// Fill out your copyright notice in the Description page of Project Settings.

/////////////////////////////////////////////////////
// UMyUSlot03

#include "UMyButtonSlot.h"
#include "ObjectEditorUtils.h"
#include "UMyButton.h"
#include "Components/Widget.h"

/////////////////////////////////////////////////////
// UMyButtonSlot

UMyButtonSlot::UMyButtonSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Padding = FMargin(0, 0);

	HorizontalAlignment = HAlign_Center;
	VerticalAlignment = VAlign_Center;
}

void UMyButtonSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Button.Reset();
}

void UMyButtonSlot::BuildSlot(TSharedRef<SMyButton> InButton)
{
	Button = InButton;

	Button.Pin()->SetContentPadding(Padding);
	Button.Pin()->SetHAlign(HorizontalAlignment);
	Button.Pin()->SetVAlign(VerticalAlignment);

	Button.Pin()->SetContent(Content ? Content->TakeWidget() : SNullWidget::NullWidget);
}

void UMyButtonSlot::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if ( Button.IsValid() )
	{
		Button.Pin()->SetContentPadding(InPadding);
	}
}

void UMyButtonSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if ( Button.IsValid() )
	{
		Button.Pin()->SetHAlign(InHorizontalAlignment);
	}
}

void UMyButtonSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if ( Button.IsValid() )
	{
		Button.Pin()->SetVAlign(InVerticalAlignment);
	}
}

void UMyButtonSlot::SynchronizeProperties()
{
	SetPadding(Padding);
	SetHorizontalAlignment(HorizontalAlignment);
	SetVerticalAlignment(VerticalAlignment);
}