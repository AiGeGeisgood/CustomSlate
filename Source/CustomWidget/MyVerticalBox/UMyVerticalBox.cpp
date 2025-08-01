// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyVerticalBox.h"

#include "UMyVerticalBoxSlot.h"
#include "Layout/ArrangedChildren.h"
#include "Components/CanvasPanelSlot.h"


#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UMyUWidget03

UMyVerticalBox::UMyVerticalBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	Visibility = ESlateVisibility::SelfHitTestInvisible;
}

void UMyVerticalBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyVerticalBox.Reset();
}

UClass* UMyVerticalBox::GetSlotClass() const
{
	return UMyVerticalBoxSlot::StaticClass();
}

void UMyVerticalBox::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if ( MyVerticalBox.IsValid() )
	{
		CastChecked<UMyVerticalBoxSlot>(InSlot)->BuildSlot(MyVerticalBox.ToSharedRef());
	}
}

void UMyVerticalBox::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( MyVerticalBox.IsValid() )
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if ( Widget.IsValid() )
		{
			MyVerticalBox->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

UMyVerticalBoxSlot* UMyVerticalBox::AddChildToVerticalBox(UWidget* Content)
{
	return Cast<UMyVerticalBoxSlot>(Super::AddChild(Content));
}

TSharedRef<SWidget> UMyVerticalBox::RebuildWidget()
{
	MyVerticalBox = SNew(SMyVerticalBox);

	for ( UPanelSlot* PanelSlot : Slots )
	{
		if ( UMyVerticalBoxSlot* TypedSlot = Cast<UMyVerticalBoxSlot>(PanelSlot) )
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyVerticalBox.ToSharedRef());
		}
	}

	return MyVerticalBox.ToSharedRef();
}

#if WITH_EDITOR

const FText UMyVerticalBox::GetPaletteCategory()
{
	return LOCTEXT("AggWidget", "AggWidget");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE