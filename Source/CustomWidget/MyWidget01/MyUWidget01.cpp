// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUWidget01.h"

#include "MyUSlot01.h"
#include "Layout/ArrangedChildren.h"
#include "Components/CanvasPanelSlot.h"

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UCanvasPanel

UMyUWidget01::UMyUWidget01(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	Visibility = ESlateVisibility::SelfHitTestInvisible;
}

void UMyUWidget01::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyCanvas.Reset();
}

UClass* UMyUWidget01::GetSlotClass() const
{
	return UMyUSlot01::StaticClass();
}

void UMyUWidget01::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if ( MyCanvas.IsValid() )
	{
		CastChecked<UMyUSlot01>(InSlot)->BuildSlot(MyCanvas.ToSharedRef());
	}
}

void UMyUWidget01::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( MyCanvas.IsValid() )
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if ( Widget.IsValid() )
		{
			MyCanvas->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

TSharedRef<SWidget> UMyUWidget01::RebuildWidget()
{
	MyCanvas = SNew(SMySWidget01);

	for ( UPanelSlot* PanelSlot : Slots )
	{
		if ( UMyUSlot01* TypedSlot = Cast<UMyUSlot01>(PanelSlot) )
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyCanvas.ToSharedRef());
		}
	}

	return MyCanvas.ToSharedRef();
}

UMyUSlot01* UMyUWidget01::AddChildToCanvas(UWidget* Content)
{
	return Cast<UMyUSlot01>( Super::AddChild(Content) );
}

TSharedPtr<SMySWidget01> UMyUWidget01::GetCanvasWidget() const
{
	return MyCanvas;
}

bool UMyUWidget01::GetGeometryForSlot(int32 SlotIndex, FGeometry& ArrangedGeometry) const
{
	UMyUSlot01* PanelSlot = CastChecked<UMyUSlot01>(Slots[SlotIndex]);
	return GetGeometryForSlot(PanelSlot, ArrangedGeometry);
}

bool UMyUWidget01::GetGeometryForSlot(UMyUSlot01* InSlot, FGeometry& ArrangedGeometry) const
{
	if ( InSlot->Content == nullptr )
	{
		return false;
	}

	TSharedPtr<SMySWidget01> Canvas = GetCanvasWidget();
	if ( Canvas.IsValid() )
	{
		FArrangedChildren ArrangedChildren(EVisibility::All);
		Canvas->ArrangeChildren(Canvas->GetCachedGeometry(), ArrangedChildren);

		for ( int32 ChildIndex = 0; ChildIndex < ArrangedChildren.Num(); ChildIndex++ )
		{
			if ( ArrangedChildren[ChildIndex].Widget == InSlot->Content->GetCachedWidget() )
			{
				ArrangedGeometry = ArrangedChildren[ChildIndex].Geometry;
				return true;
			}
		}
	}

	return false;
}

#if WITH_EDITOR

const FText UMyUWidget01::GetPaletteCategory()
{
	return LOCTEXT("AMyWidget", "AMyWidget");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
