// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyCanvas.h"

#include "UMyCanvasSlot.h"
#include "Layout/ArrangedChildren.h"
#include "Components/CanvasPanelSlot.h"

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UCanvasPanel

UMyCanvas::UMyCanvas(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	Visibility = ESlateVisibility::SelfHitTestInvisible;
}

void UMyCanvas::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyCanvas.Reset();
}

UClass* UMyCanvas::GetSlotClass() const
{
	return UMyCanvasSlot::StaticClass();
}

void UMyCanvas::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if ( MyCanvas.IsValid() )
	{
		CastChecked<UMyCanvasSlot>(InSlot)->BuildSlot(MyCanvas.ToSharedRef());
	}
}

void UMyCanvas::OnSlotRemoved(UPanelSlot* InSlot)
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

TSharedRef<SWidget> UMyCanvas::RebuildWidget()
{
	MyCanvas = SNew(SMyCanvas);

	for ( UPanelSlot* PanelSlot : Slots )
	{
		if ( UMyCanvasSlot* TypedSlot = Cast<UMyCanvasSlot>(PanelSlot) )
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyCanvas.ToSharedRef());
		}
	}

	return MyCanvas.ToSharedRef();
}

UMyCanvasSlot* UMyCanvas::AddChildToCanvas(UWidget* Content)
{
	return Cast<UMyCanvasSlot>( Super::AddChild(Content) );
}

TSharedPtr<SMyCanvas> UMyCanvas::GetCanvasWidget() const
{
	return MyCanvas;
}

bool UMyCanvas::GetGeometryForSlot(int32 SlotIndex, FGeometry& ArrangedGeometry) const
{
	UMyCanvasSlot* PanelSlot = CastChecked<UMyCanvasSlot>(Slots[SlotIndex]);
	return GetGeometryForSlot(PanelSlot, ArrangedGeometry);
}

bool UMyCanvas::GetGeometryForSlot(UMyCanvasSlot* InSlot, FGeometry& ArrangedGeometry) const
{
	if ( InSlot->Content == nullptr )
	{
		return false;
	}

	TSharedPtr<SMyCanvas> Canvas = GetCanvasWidget();
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

const FText UMyCanvas::GetPaletteCategory()
{
	return LOCTEXT("AggWidget", "AggWidget");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
