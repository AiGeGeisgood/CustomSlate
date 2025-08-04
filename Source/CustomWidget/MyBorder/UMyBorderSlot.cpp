// Fill out your copyright notice in the Description page of Project Settings.

/////////////////////////////////////////////////////
// UMyUSlot03

#include "UMyBorderSlot.h"
#include "ObjectEditorUtils.h"
#include "UMyBorder.h"
#include "Components/Widget.h"

/////////////////////////////////////////////////////
// UMyBorderSlot

UMyBorderSlot::UMyBorderSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Padding = FMargin(2, 2);

	HorizontalAlignment = HAlign_Fill;
	VerticalAlignment = VAlign_Fill;
}

void UMyBorderSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	Border.Reset();
}

void UMyBorderSlot::BuildSlot(TSharedRef<SMyBorder> InBorder)
{
	Border = InBorder;

	Border.Pin()->SetPadding(Padding);
	Border.Pin()->SetHAlign(HorizontalAlignment);
	Border.Pin()->SetVAlign(VerticalAlignment);

	Border.Pin()->SetContent(Content ? Content->TakeWidget() : SNullWidget::NullWidget);
}

void UMyBorderSlot::SetPadding(FMargin InPadding)
{
	CastChecked<UMyBorder>(Parent)->SetPadding(InPadding);
}

void UMyBorderSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	CastChecked<UMyBorder>(Parent)->SetHorizontalAlignment(InHorizontalAlignment);
}

void UMyBorderSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	CastChecked<UMyBorder>(Parent)->SetVerticalAlignment(InVerticalAlignment);
}

void UMyBorderSlot::SynchronizeProperties()
{
	if ( Border.IsValid() )
	{
		SetPadding(Padding);
		SetHorizontalAlignment(HorizontalAlignment);
		SetVerticalAlignment(VerticalAlignment);
	}
}

#if WITH_EDITOR

void UMyBorderSlot::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	static bool IsReentrant = false;

	if ( !IsReentrant )
	{
		IsReentrant = true;

		if ( PropertyChangedEvent.Property )
		{
			static const FName PaddingName("Padding");
			static const FName HorizontalAlignmentName("HorizontalAlignment");
			static const FName VerticalAlignmentName("VerticalAlignment");

			FName PropertyName = PropertyChangedEvent.Property->GetFName();

			if ( UMyBorder* ParentBorder = CastChecked<UMyBorder>(Parent) )
			{
				if ( PropertyName == PaddingName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, PaddingName, ParentBorder, PaddingName);
				}
				else if ( PropertyName == HorizontalAlignmentName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, HorizontalAlignmentName, ParentBorder, HorizontalAlignmentName);
				}
				else if ( PropertyName == VerticalAlignmentName)
				{
					FObjectEditorUtils::MigratePropertyValue(this, VerticalAlignmentName, ParentBorder, VerticalAlignmentName);
				}
			}
		}

		IsReentrant = false;
	}
}

#endif
