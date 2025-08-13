// Fill out your copyright notice in the Description page of Project Settings.


#include "UMySizeBox.h"

#include "SMySizeBox.h"
#include "UMySizeBoxSlot.h"
#include "Layout/ArrangedChildren.h"
#include "Components/CanvasPanelSlot.h"


#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UMySizeBox

UMySizeBox::UMySizeBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	Visibility = ESlateVisibility::SelfHitTestInvisible;
	MinAspectRatio = 1;
	MaxAspectRatio = 1;
}

void UMySizeBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MySizeBox.Reset();
}

TSharedRef<SWidget> UMySizeBox::RebuildWidget()
{
	MySizeBox = SNew(SMySizeBox);

	if (GetChildrenCount() > 0)
	{
		Cast<UMySizeBoxSlot>(GetContentSlot())->BuildSlot(MySizeBox.ToSharedRef());
	}

	return MySizeBox.ToSharedRef();
}

void UMySizeBox::SetWidthOverride(float InWidthOverride)
{
	bOverride_WidthOverride = true;
	WidthOverride = InWidthOverride;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetWidthOverride(InWidthOverride);
	}
}

void UMySizeBox::ClearWidthOverride()
{
	bOverride_WidthOverride = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetWidthOverride(FOptionalSize());
	}
}

void UMySizeBox::SetHeightOverride(float InHeightOverride)
{
	bOverride_HeightOverride = true;
	HeightOverride = InHeightOverride;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetHeightOverride(InHeightOverride);
	}
}

void UMySizeBox::ClearHeightOverride()
{
	bOverride_HeightOverride = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetHeightOverride(FOptionalSize());
	}
}

void UMySizeBox::SetMinDesiredWidth(float InMinDesiredWidth)
{
	bOverride_MinDesiredWidth = true;
	MinDesiredWidth = InMinDesiredWidth;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMinDesiredWidth(InMinDesiredWidth);
	}
}

void UMySizeBox::ClearMinDesiredWidth()
{
	bOverride_MinDesiredWidth = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMinDesiredWidth(FOptionalSize());
	}
}

void UMySizeBox::SetMinDesiredHeight(float InMinDesiredHeight)
{
	bOverride_MinDesiredHeight = true;
	MinDesiredHeight = InMinDesiredHeight;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMinDesiredHeight(InMinDesiredHeight);
	}
}

void UMySizeBox::ClearMinDesiredHeight()
{
	bOverride_MinDesiredHeight = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMinDesiredHeight(FOptionalSize());
	}
}

void UMySizeBox::SetMaxDesiredWidth(float InMaxDesiredWidth)
{
	bOverride_MaxDesiredWidth = true;
	MaxDesiredWidth = InMaxDesiredWidth;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMaxDesiredWidth(InMaxDesiredWidth);
	}
}

void UMySizeBox::ClearMaxDesiredWidth()
{
	bOverride_MaxDesiredWidth = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMaxDesiredWidth(FOptionalSize());
	}
}

void UMySizeBox::SetMaxDesiredHeight(float InMaxDesiredHeight)
{
	bOverride_MaxDesiredHeight = true;
	MaxDesiredHeight = InMaxDesiredHeight;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMaxDesiredHeight(InMaxDesiredHeight);
	}
}

void UMySizeBox::ClearMaxDesiredHeight()
{
	bOverride_MaxDesiredHeight = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMaxDesiredHeight(FOptionalSize());
	}
}

void UMySizeBox::SetMinAspectRatio(float InMinAspectRatio)
{
	bOverride_MinAspectRatio = true;
	MinAspectRatio = InMinAspectRatio;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMinAspectRatio(InMinAspectRatio);
	}
}

void UMySizeBox::ClearMinAspectRatio()
{
	bOverride_MinAspectRatio = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMinAspectRatio(FOptionalSize());
	}
}

void UMySizeBox::SetMaxAspectRatio(float InMaxAspectRatio)
{
	bOverride_MaxAspectRatio = true;
	MaxAspectRatio = InMaxAspectRatio;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMaxAspectRatio(InMaxAspectRatio);
	}
}

void UMySizeBox::ClearMaxAspectRatio()
{
	bOverride_MaxAspectRatio = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMaxAspectRatio(FOptionalSize());
	}
}

void UMySizeBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (bOverride_WidthOverride)
	{
		SetWidthOverride(WidthOverride);
	}
	else
	{
		ClearWidthOverride();
	}

	if (bOverride_HeightOverride)
	{
		SetHeightOverride(HeightOverride);
	}
	else
	{
		ClearHeightOverride();
	}

	if (bOverride_MinDesiredWidth)
	{
		SetMinDesiredWidth(MinDesiredWidth);
	}
	else
	{
		ClearMinDesiredWidth();
	}

	if (bOverride_MinDesiredHeight)
	{
		SetMinDesiredHeight(MinDesiredHeight);
	}
	else
	{
		ClearMinDesiredHeight();
	}

	if (bOverride_MaxDesiredWidth)
	{
		SetMaxDesiredWidth(MaxDesiredWidth);
	}
	else
	{
		ClearMaxDesiredWidth();
	}

	if (bOverride_MaxDesiredHeight)
	{
		SetMaxDesiredHeight(MaxDesiredHeight);
	}
	else
	{
		ClearMaxDesiredHeight();
	}

	if (bOverride_MinAspectRatio)
	{
		SetMinAspectRatio(MinAspectRatio);
	}
	else
	{
		ClearMinAspectRatio();
	}

	if (bOverride_MaxAspectRatio)
	{
		SetMaxAspectRatio(MaxAspectRatio);
	}
	else
	{
		ClearMaxAspectRatio();
	}
}

UClass* UMySizeBox::GetSlotClass() const
{
	return UMySizeBoxSlot::StaticClass();
}

void UMySizeBox::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live slot if it already exists
	if (MySizeBox.IsValid())
	{
		CastChecked<UMySizeBoxSlot>(InSlot)->BuildSlot(MySizeBox.ToSharedRef());
	}
}

void UMySizeBox::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetContent(SNullWidget::NullWidget);
	}
}

#if WITH_EDITOR

const FText UMySizeBox::GetPaletteCategory()
{
	return LOCTEXT("AggWidget", "AggWidget");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE