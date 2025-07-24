// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUWidget02.h"

#include "MyUSlot02.h"
#include "Layout/ArrangedChildren.h"
#include "Components/CanvasPanelSlot.h"


#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UMyUWidget02

UMyUWidget02::UMyUWidget02(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	Visibility = ESlateVisibility::SelfHitTestInvisible;
}

void UMyUWidget02::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MySWidget02.Reset();
}

UClass* UMyUWidget02::GetSlotClass() const
{
	return UMyUSlot02::StaticClass();
}

void UMyUWidget02::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live canvas if it already exists
	if ( MySWidget02.IsValid() )
	{
		CastChecked<UMyUSlot02>(InSlot)->BuildSlot(MySWidget02.ToSharedRef());
	}
}

void UMyUWidget02::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( MySWidget02.IsValid() )
	{
		TSharedPtr<SWidget> Widget = InSlot->Content->GetCachedWidget();
		if ( Widget.IsValid() )
		{
			MySWidget02->RemoveSlot(Widget.ToSharedRef());
		}
	}
}

UMyUSlot02* UMyUWidget02::AddChildToHorizontalBox(UWidget* Content)
{
	return Cast<UMyUSlot02>( Super::AddChild(Content) );
}

TSharedRef<SWidget> UMyUWidget02::RebuildWidget()
{
	MySWidget02 = SNew(SMySWidget02);

	for ( UPanelSlot* PanelSlot : Slots )
	{
		if ( UMyUSlot02* TypedSlot = Cast<UMyUSlot02>(PanelSlot) )
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MySWidget02.ToSharedRef());
		}
	}

	return MySWidget02.ToSharedRef();
}

#if WITH_EDITOR

const FText UMyUWidget02::GetPaletteCategory()
{
	return LOCTEXT("Panel", "Panel");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
