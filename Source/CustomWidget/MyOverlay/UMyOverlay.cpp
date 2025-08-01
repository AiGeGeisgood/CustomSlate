// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyOverlay.h"

#include "SMyOverlay.h"
#include "UMyOverlaySlot.h"
#include "Layout/ArrangedChildren.h"
#include "Components/CanvasPanelSlot.h"


#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UMyOverlay

UMyOverlay::UMyOverlay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	Visibility = ESlateVisibility::SelfHitTestInvisible;
}

void UMyOverlay::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyOverlay.Reset();
}

UMyOverlaySlot* UMyOverlay::AddChildToOverlay(UWidget* Content)
{
	return Cast<UMyOverlaySlot>(Super::AddChild(Content));
}

UClass* UMyOverlay::GetSlotClass() const
{
	return UMyOverlaySlot::StaticClass();
}

void UMyOverlay::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if ( MyOverlay.IsValid() )
	{
		CastChecked<UMyOverlaySlot>(InSlot)->BuildSlot(MyOverlay.ToSharedRef());
	}
}

void UMyOverlay::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( MyOverlay.IsValid() )
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if ( Widget.IsValid() )
		{
			MyOverlay->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

TSharedRef<SWidget> UMyOverlay::RebuildWidget()
{
	MyOverlay = SNew(SMyOverlay);

	for ( UPanelSlot* PanelSlot : Slots )
	{
		if ( UMyOverlaySlot* TypedSlot = Cast<UMyOverlaySlot>(PanelSlot) )
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyOverlay.ToSharedRef());
		}
	}

	return MyOverlay.ToSharedRef();
}

#if WITH_EDITOR

const FText UMyOverlay::GetPaletteCategory()
{
	return LOCTEXT("AggWidget", "AggWidget");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE