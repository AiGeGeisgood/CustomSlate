// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyButton.h"
#include "UMyButtonSlot.h"
#include "Slate/SlateBrushAsset.h"


#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UMyButton

static FButtonStyle* DefaultButtonStyle = nullptr;

UMyButton::UMyButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (DefaultButtonStyle == nullptr)
	{
		// HACK: THIS SHOULD NOT COME FROM CORESTYLE AND SHOULD INSTEAD BE DEFINED BY ENGINE TEXTURES/PROJECT SETTINGS
		DefaultButtonStyle = new FButtonStyle(FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("MyButton"));

		// Unlink UMG default colors from the editor settings colors.
		DefaultButtonStyle->UnlinkColors();
	}

	WidgetStyle = *DefaultButtonStyle;

	ColorAndOpacity = FLinearColor::White;
	BackgroundColor = FLinearColor::White;

	ClickMethod = EButtonClickMethod::DownAndUp;
	TouchMethod = EButtonTouchMethod::DownAndUp;

	IsFocusable = true;

#if WITH_EDITORONLY_DATA
	AccessibleBehavior = ESlateAccessibleBehavior::Summary;
	bCanChildrenBeAccessible = false;
#endif
}

void UMyButton::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyButton.Reset();
}

TSharedRef<SWidget> UMyButton::RebuildWidget()
{
	MyButton = SNew(SMyButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
		.OnHovered_UObject( this, &ThisClass::SlateHandleHovered )
		.OnUnhovered_UObject( this, &ThisClass::SlateHandleUnhovered )
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		;

	if ( GetChildrenCount() > 0 )
	{
		Cast<UMyButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}
	
	return MyButton.ToSharedRef();
}

void UMyButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyButton->SetColorAndOpacity( ColorAndOpacity );
	MyButton->SetBorderBackgroundColor( BackgroundColor );
}

UClass* UMyButton::GetSlotClass() const
{
	return UMyButtonSlot::StaticClass();
}

void UMyButton::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live slot if it already exists
	if ( MyButton.IsValid() )
	{
		CastChecked<UMyButtonSlot>(InSlot)->BuildSlot(MyButton.ToSharedRef());
	}
}

void UMyButton::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if ( MyButton.IsValid() )
	{
		MyButton->SetContent(SNullWidget::NullWidget);
	}
}

void UMyButton::SetStyle(const FButtonStyle& InStyle)
{
	WidgetStyle = InStyle;
	if ( MyButton.IsValid() )
	{
		MyButton->SetButtonStyle(&WidgetStyle);
	}
}

void UMyButton::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
	if ( MyButton.IsValid() )
	{
		MyButton->SetColorAndOpacity(InColorAndOpacity);
	}
}

void UMyButton::SetBackgroundColor(FLinearColor InBackgroundColor)
{
	BackgroundColor = InBackgroundColor;
	if ( MyButton.IsValid() )
	{
		MyButton->SetBorderBackgroundColor(InBackgroundColor);
	}
}

bool UMyButton::IsPressed() const
{
	if ( MyButton.IsValid() )
	{
		return MyButton->IsPressed();
	}

	return false;
}

void UMyButton::SetClickMethod(EButtonClickMethod::Type InClickMethod)
{
	ClickMethod = InClickMethod;
	if ( MyButton.IsValid() )
	{
		MyButton->SetClickMethod(ClickMethod);
	}
}

void UMyButton::SetTouchMethod(EButtonTouchMethod::Type InTouchMethod)
{
	TouchMethod = InTouchMethod;
	if ( MyButton.IsValid() )
	{
		MyButton->SetTouchMethod(TouchMethod);
	}
}

void UMyButton::SetPressMethod(EButtonPressMethod::Type InPressMethod)
{
	PressMethod = InPressMethod;
	if ( MyButton.IsValid() )
	{
		MyButton->SetPressMethod(PressMethod);
	}
}

void UMyButton::PostLoad()
{
	Super::PostLoad();

	if ( GetChildrenCount() > 0 )
	{
		//TODO UMG Pre-Release Upgrade, now buttons have slots of their own.  Convert existing slot to new slot.
		if ( UPanelSlot* PanelSlot = GetContentSlot() )
		{
			UMyButtonSlot* ButtonSlot = Cast<UMyButtonSlot>(PanelSlot);
			if ( ButtonSlot == NULL )
			{
				ButtonSlot = NewObject<UMyButtonSlot>(this);
				ButtonSlot->Content = GetContentSlot()->Content;
				ButtonSlot->Content->Slot = ButtonSlot;
				Slots[0] = ButtonSlot;
			}
		}
	}

	if( GetLinkerUE4Version() < VER_UE4_DEPRECATE_UMG_STYLE_ASSETS && Style_DEPRECATED != nullptr )
	{
		const FButtonStyle* StylePtr = Style_DEPRECATED->GetStyle<FButtonStyle>();
		if(StylePtr != nullptr)
		{
			WidgetStyle = *StylePtr;
		}

		Style_DEPRECATED = nullptr;
	}
}

FReply UMyButton::SlateHandleClicked()
{
	OnClicked.Broadcast();

	return FReply::Handled();
}

void UMyButton::SlateHandlePressed()
{
	OnPressed.Broadcast();
}

void UMyButton::SlateHandleReleased()
{
	OnReleased.Broadcast();
}

void UMyButton::SlateHandleHovered()
{
	OnHovered.Broadcast();
}

void UMyButton::SlateHandleUnhovered()
{
	OnUnhovered.Broadcast();
}

#if WITH_ACCESSIBILITY
TSharedPtr<SWidget> UMyButton::GetAccessibleWidget() const
{
	return MyButton;
}
#endif

#if WITH_EDITOR

const FText UMyButton::GetPaletteCategory()
{
	return LOCTEXT("AggWidget", "AggWidget");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE