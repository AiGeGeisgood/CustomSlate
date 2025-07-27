// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUWidget03.h"

#include "MyUSlot03.h"
#include "Layout/ArrangedChildren.h"
#include "Components/CanvasPanelSlot.h"


#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UMyUWidget03

UMyUWidget03::UMyUWidget03(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	Visibility = ESlateVisibility::SelfHitTestInvisible;
}

void UMyUWidget03::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MySWidget03.Reset();
}

UClass* UMyUWidget03::GetSlotClass() const
{
	return UMyUSlot03::StaticClass();
}

void UMyUWidget03::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if ( MySWidget03.IsValid() )
	{
		CastChecked<UMyUSlot03>(InSlot)->BuildSlot(MySWidget03.ToSharedRef());
	}
}

void UMyUWidget03::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( MySWidget03.IsValid() )
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if ( Widget.IsValid() )
		{
			MySWidget03->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

UMyUSlot03* UMyUWidget03::AddChildToVerticalBox(UWidget* Content)
{
	return Cast<UMyUSlot03>(Super::AddChild(Content));
}

TSharedRef<SWidget> UMyUWidget03::RebuildWidget()
{
	MySWidget03 = SNew(SMySWidget03);

	for ( UPanelSlot* PanelSlot : Slots )
	{
		if ( UMyUSlot03* TypedSlot = Cast<UMyUSlot03>(PanelSlot) )
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MySWidget03.ToSharedRef());
		}
	}

	return MySWidget03.ToSharedRef();
}

#if WITH_EDITOR

const FText UMyUWidget03::GetPaletteCategory()
{
	return LOCTEXT("Panel", "Panel");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE