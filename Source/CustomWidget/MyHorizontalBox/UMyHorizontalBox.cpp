// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyHorizontalBox.h"

#include "UMyHorizontalBoxSlot.h"
#include "Layout/ArrangedChildren.h"
#include "Components/CanvasPanelSlot.h"


#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UMyUWidget02

UMyHorizontalBox::UMyHorizontalBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	Visibility = ESlateVisibility::SelfHitTestInvisible;
}

void UMyHorizontalBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyHorizontalBox.Reset();
}

UClass* UMyHorizontalBox::GetSlotClass() const
{
	return UMyHorizontalBoxSlot::StaticClass();
}

void UMyHorizontalBox::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if ( MyHorizontalBox.IsValid() )
	{
		CastChecked<UMyHorizontalBoxSlot>(InSlot)->BuildSlot(MyHorizontalBox.ToSharedRef());
	}
}

void UMyHorizontalBox::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( MyHorizontalBox.IsValid() )
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if ( Widget.IsValid() )
		{
			MyHorizontalBox->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

UMyHorizontalBoxSlot* UMyHorizontalBox::AddChildToHorizontalBox(UWidget* Content)
{
	return Cast<UMyHorizontalBoxSlot>( Super::AddChild(Content) );
}

TSharedRef<SWidget> UMyHorizontalBox::RebuildWidget()
{
	MyHorizontalBox = SNew(SMyHorizontalBox);

	for ( UPanelSlot* PanelSlot : Slots )
	{
		if ( UMyHorizontalBoxSlot* TypedSlot = Cast<UMyHorizontalBoxSlot>(PanelSlot) )
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyHorizontalBox.ToSharedRef());
		}
	}

	return MyHorizontalBox.ToSharedRef();
}

#if WITH_EDITOR

const FText UMyHorizontalBox::GetPaletteCategory()
{
	return LOCTEXT("AggWidget", "AggWidget");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
