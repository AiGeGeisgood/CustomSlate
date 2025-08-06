// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyScaleBox.h"
#include "ObjectEditorUtils.h"
#include "UMyScaleBoxSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Slate/SlateBrushAsset.h"


#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UMyScaleBox

UMyScaleBox::UMyScaleBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	Visibility = ESlateVisibility::SelfHitTestInvisible;

	StretchDirection = EMyStretchDirection::Both;
	Stretch = EMyStretch::ScaleToFit;
	UserSpecifiedScale = 1.0f;
	IgnoreInheritedScale = false;
}

void UMyScaleBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyScaleBox.Reset();
}

TSharedRef<SWidget> UMyScaleBox::RebuildWidget()
{
	MyScaleBox = SNew(SMyScaleBox)
#if WITH_EDITOR
		.OverrideScreenSize(DesignerSize)
#endif
	;

	if ( GetChildrenCount() > 0 )
	{
		CastChecked<UMyScaleBoxSlot>(GetContentSlot())->BuildSlot(MyScaleBox.ToSharedRef());
	}

	return MyScaleBox.ToSharedRef();
}

void UMyScaleBox::SetStretch(EMyStretch::Type InStretch)
{
	Stretch = InStretch;
	if(MyScaleBox.IsValid())
	{
		MyScaleBox->SetStretch(InStretch);
	}
}

void UMyScaleBox::SetStretchDirection(EMyStretchDirection::Type InStretchDirection)
{
	StretchDirection = InStretchDirection;
	if (MyScaleBox.IsValid())
	{
		MyScaleBox->SetStretchDirection(InStretchDirection);
	}
}

void UMyScaleBox::SetUserSpecifiedScale(float InUserSpecifiedScale)
{
	UserSpecifiedScale = InUserSpecifiedScale;
	if (MyScaleBox.IsValid())
	{
		MyScaleBox->SetUserSpecifiedScale(InUserSpecifiedScale);
	}
}

void UMyScaleBox::SetIgnoreInheritedScale(bool bInIgnoreInheritedScale)
{
	IgnoreInheritedScale = bInIgnoreInheritedScale;
	if (MyScaleBox.IsValid())
	{
		MyScaleBox->SetIgnoreInheritedScale(bInIgnoreInheritedScale);
	}
}

void UMyScaleBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyScaleBox->SetStretchDirection(StretchDirection);
	MyScaleBox->SetStretch(Stretch);
	MyScaleBox->SetUserSpecifiedScale(UserSpecifiedScale);
	MyScaleBox->SetIgnoreInheritedScale(IgnoreInheritedScale);
}

UClass* UMyScaleBox::GetSlotClass() const
{
	return UMyScaleBoxSlot::StaticClass();
}

void UMyScaleBox::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live slot if it already exists
	if ( MyScaleBox.IsValid() )
	{
		CastChecked<UMyScaleBoxSlot>(InSlot)->BuildSlot(MyScaleBox.ToSharedRef());
	}
}

void UMyScaleBox::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( MyScaleBox.IsValid() )
	{
		MyScaleBox->SetContent(SNullWidget::NullWidget);
	}
}

void UMyScaleBox::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FEditorObjectVersion::GUID);
}

#if WITH_EDITOR

const FText UMyScaleBox::GetPaletteCategory()
{
	return LOCTEXT("AggWidget", "AggWidget");
}

bool UMyScaleBox::CanEditChange(const UProperty* InProperty) const
{
	bool bIsEditable = Super::CanEditChange(InProperty);
	if (bIsEditable && InProperty)
	{
		const FName PropertyName = InProperty->GetFName();

		if (PropertyName == GET_MEMBER_NAME_CHECKED(UMyScaleBox, StretchDirection))
		{
			return Stretch != EMyStretch::None && Stretch != EMyStretch::ScaleBySafeZone && Stretch != EMyStretch::UserSpecified;
		}

		if (PropertyName == GET_MEMBER_NAME_CHECKED(UMyScaleBox, UserSpecifiedScale))
		{
			return Stretch == EMyStretch::UserSpecified;
		}
	}

	return bIsEditable;
}


void UMyScaleBox::OnDesignerChanged(const FDesignerChangedEventArgs& EventArgs)
{
	if (EventArgs.bScreenPreview)
	{
		DesignerSize = EventArgs.Size;
	}
	else
	{
		DesignerSize = FVector2D(0, 0);
	}

	if (MyScaleBox.IsValid())
	{
		MyScaleBox->SetOverrideScreenInformation(DesignerSize);
	}
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
