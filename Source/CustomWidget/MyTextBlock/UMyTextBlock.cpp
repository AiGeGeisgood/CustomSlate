// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyTextBlock.h"
#include "SMyTextBlock.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SInvalidationPanel.h"
#include "Materials/MaterialInstanceDynamic.h"


#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UMyTextBlock

UMyTextBlock::UMyTextBlock(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	bWrapWithInvalidationPanel = false;
	ShadowOffset = FVector2D(1.0f, 1.0f);
	ColorAndOpacity = FLinearColor::White;
	ShadowColorAndOpacity = FLinearColor::Transparent;
	bAutoWrapText_DEPRECATED = false;

	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*UWidget::GetDefaultFontName());
		Font = FSlateFontInfo(RobotoFontObj.Object, 24, FName("Bold"));
	}

#if WITH_EDITORONLY_DATA
	AccessibleBehavior = ESlateAccessibleBehavior::Auto;
	bCanChildrenBeAccessible = false;
#endif
}

void UMyTextBlock::PostLoad()
{
	Super::PostLoad();

	if (bAutoWrapText_DEPRECATED)
	{
		AutoWrapText = bAutoWrapText_DEPRECATED;
		bAutoWrapText_DEPRECATED = false;
	}
}

void UMyTextBlock::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyTextBlock.Reset();
}

void UMyTextBlock::SetColorAndOpacity(FSlateColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
	if( MyTextBlock.IsValid() )
	{
		MyTextBlock->SetColorAndOpacity( InColorAndOpacity );
	}
}

void UMyTextBlock::SetOpacity(float InOpacity)
{
	FLinearColor CurrentColor = ColorAndOpacity.GetSpecifiedColor();
	CurrentColor.A = InOpacity;
	
	SetColorAndOpacity(FSlateColor(CurrentColor));
}

void UMyTextBlock::SetShadowColorAndOpacity(FLinearColor InShadowColorAndOpacity)
{
	ShadowColorAndOpacity = InShadowColorAndOpacity;
	if( MyTextBlock.IsValid() )
	{
		MyTextBlock->SetShadowColorAndOpacity(InShadowColorAndOpacity);
	}
}

void UMyTextBlock::SetShadowOffset(FVector2D InShadowOffset)
{
	ShadowOffset = InShadowOffset;
	if( MyTextBlock.IsValid() )
	{
		MyTextBlock->SetShadowOffset(ShadowOffset);
	}
}

void UMyTextBlock::SetFont(FSlateFontInfo InFontInfo)
{
	Font = InFontInfo;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetFont(Font);
	}
}

void UMyTextBlock::SetStrikeBrush(FSlateBrush InStrikeBrush)
{
	StrikeBrush = InStrikeBrush;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetStrikeBrush(&StrikeBrush);
	}
}

void UMyTextBlock::SetJustification(ETextJustify::Type InJustification)
{
	Super::SetJustification(InJustification);
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetJustification(InJustification);
	}
}

void UMyTextBlock::SetMinDesiredWidth(float InMinDesiredWidth)
{
	MinDesiredWidth = InMinDesiredWidth;
	if (MyTextBlock.IsValid())
	{
		MyTextBlock->SetMinDesiredWidth(MinDesiredWidth);
	}
}

void UMyTextBlock::SetAutoWrapText(bool InAutoWrapText)
{
	AutoWrapText = InAutoWrapText;
	if(MyTextBlock.IsValid())
	{
		MyTextBlock->SetAutoWrapText(InAutoWrapText);
	}
}

UMaterialInstanceDynamic* UMyTextBlock::GetDynamicFontMaterial()
{
	if (ensure(Font.FontMaterial))
	{
		UMaterialInterface* Material = CastChecked<UMaterialInterface>(Font.FontMaterial);

		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Font.FontMaterial = DynamicMaterial;

			SetFont(Font);
		}

		return DynamicMaterial;
	}

	return nullptr;
}

UMaterialInstanceDynamic* UMyTextBlock::GetDynamicOutlineMaterial()
{
	if (ensure(Font.OutlineSettings.OutlineMaterial))
	{
		UMaterialInterface* Material = CastChecked<UMaterialInterface>(Font.OutlineSettings.OutlineMaterial);

		UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(Material);

		if (!DynamicMaterial)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
			Font.OutlineSettings.OutlineMaterial = DynamicMaterial;

			SetFont(Font);
		}

		return DynamicMaterial;
	}

	return nullptr;
}

TSharedRef<SWidget> UMyTextBlock::RebuildWidget()
{
 	if (bWrapWithInvalidationPanel && !IsDesignTime())
 	{
 		TSharedPtr<SWidget> RetWidget = SNew(SInvalidationPanel)
 		[
 			SAssignNew(MyTextBlock, SMyTextBlock)
			.SimpleTextMode(bSimpleTextMode)
 		];
 		return RetWidget.ToSharedRef();
 	}
 	else
	{
		MyTextBlock =
			SNew(SMyTextBlock)
			.SimpleTextMode(bSimpleTextMode);

		//if (IsDesignTime())
		//{
		//	return SNew(SOverlay)

		//	+ SOverlay::Slot()
		//	[
		//		MyTextBlock.ToSharedRef()
		//	]

		//	+ SOverlay::Slot()
		//	.VAlign(VAlign_Top)
		//	.HAlign(HAlign_Right)
		//	[
		//		SNew(SImage)
		//		.Image(FCoreStyle::Get().GetBrush("Icons.Warning"))
		//		.Visibility_UObject(this, &ThisClass::GetTextWarningImageVisibility)
		//		.ToolTipText(LOCTEXT("TextNotLocalizedWarningToolTip", "This text is marked as 'culture invariant' and won't be gathered for localization.\nYou can change this by editing the advanced text settings."))
		//	];
		//}
		
		return MyTextBlock.ToSharedRef();
	}
}

EVisibility UMyTextBlock::GetTextWarningImageVisibility() const
{
	return Text.IsCultureInvariant() ? EVisibility::Visible : EVisibility::Collapsed;
}

#if WITH_ACCESSIBILITY
TSharedPtr<SWidget> UMyTextBlock::GetAccessibleWidget() const
{
	return MyTextBlock;
}
#endif

void UMyTextBlock::OnBindingChanged(const FName& Property)
{
	Super::OnBindingChanged(Property);

	if ( MyTextBlock.IsValid() )
	{
		static const FName TextProperty(TEXT("TextDelegate"));
		static const FName ColorAndOpacityProperty(TEXT("ColorAndOpacityDelegate"));
		static const FName ShadowColorAndOpacityProperty(TEXT("ShadowColorAndOpacityDelegate"));

		if ( Property == TextProperty )
		{
			TAttribute<FText> TextBinding = GetDisplayText();
			MyTextBlock->SetText(TextBinding);
		}
		else if ( Property == ColorAndOpacityProperty )
		{
			TAttribute<FSlateColor> ColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, ColorAndOpacity);
			MyTextBlock->SetColorAndOpacity(ColorAndOpacityBinding);
		}
		else if ( Property == ShadowColorAndOpacityProperty )
		{
			TAttribute<FLinearColor> ShadowColorAndOpacityBinding = PROPERTY_BINDING(FLinearColor, ShadowColorAndOpacity);
			MyTextBlock->SetShadowColorAndOpacity(ShadowColorAndOpacityBinding);
		}
	}
}

void UMyTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	TAttribute<FText> TextBinding = GetDisplayText();
	TAttribute<FSlateColor> ColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, ColorAndOpacity);
	TAttribute<FLinearColor> ShadowColorAndOpacityBinding = PROPERTY_BINDING(FLinearColor, ShadowColorAndOpacity);

	if ( MyTextBlock.IsValid() )
	{
		MyTextBlock->SetText( TextBinding );
		MyTextBlock->SetFont( Font );
		MyTextBlock->SetStrikeBrush( &StrikeBrush );
		MyTextBlock->SetColorAndOpacity( ColorAndOpacityBinding );
		MyTextBlock->SetShadowOffset( ShadowOffset );
		MyTextBlock->SetShadowColorAndOpacity( ShadowColorAndOpacityBinding );
		MyTextBlock->SetMinDesiredWidth( MinDesiredWidth );
		Super::SynchronizeTextLayoutProperties( *MyTextBlock );
	}
}

/// @cond DOXYGEN_WARNINGS

FText UMyTextBlock::GetText() const
{
	if (MyTextBlock.IsValid())
	{
		return MyTextBlock->GetText();
	}

	return Text;
}

/// @endcond

void UMyTextBlock::SetText(FText InText)
{
	Text = InText;
	TextDelegate.Unbind();
	if ( MyTextBlock.IsValid() )
	{
		TAttribute<FText> TextBinding = GetDisplayText();
		MyTextBlock->SetText(TextBinding);
	}
}

TAttribute<FText> UMyTextBlock::GetDisplayText()
{
	return PROPERTY_BINDING(FText, Text);
}

#if WITH_EDITOR

FString UMyTextBlock::GetLabelMetadata() const
{
	const int32 MaxSampleLength = 15;

	FString TextStr = Text.ToString().Replace(TEXT("\n"), TEXT(" "));
	TextStr = TextStr.Len() <= MaxSampleLength ? TextStr : TextStr.Left(MaxSampleLength - 2) + TEXT("..");
	return TEXT(" \"") + TextStr + TEXT("\"");
}

void UMyTextBlock::HandleTextCommitted(const FText& InText, ETextCommit::Type CommitteType)
{
	//TODO UMG How will this migrate to the template?  Seems to me we need the previews to have access to their templates!
	//TODO UMG How will the user click the editable area?  There is an overlay blocking input so that other widgets don't get them.
	//     Need a way to recognize one particular widget and forward things to them!
}

const FText UMyTextBlock::GetPaletteCategory()
{
	return LOCTEXT("AggWidget", "AggWidget");
}

void UMyTextBlock::OnCreationFromPalette()
{
	Text = LOCTEXT("TextBlockDefaultValue", "Text Block");
}

bool UMyTextBlock::CanEditChange(const UProperty* InProperty) const
{
	if (bSimpleTextMode && InProperty)
	{
		static TArray<FName> InvalidPropertiesInSimpleMode =
		{
			GET_MEMBER_NAME_CHECKED(UMyTextBlock, ShapedTextOptions),
			GET_MEMBER_NAME_CHECKED(UMyTextBlock, Justification),
			GET_MEMBER_NAME_CHECKED(UMyTextBlock, WrappingPolicy),
			GET_MEMBER_NAME_CHECKED(UMyTextBlock, AutoWrapText),
			GET_MEMBER_NAME_CHECKED(UMyTextBlock, WrapTextAt),
			GET_MEMBER_NAME_CHECKED(UMyTextBlock, Margin),
			GET_MEMBER_NAME_CHECKED(UMyTextBlock, LineHeightPercentage),
			GET_MEMBER_NAME_CHECKED(UMyTextBlock, AutoWrapText),
		};

		return !InvalidPropertiesInSimpleMode.Contains(InProperty->GetFName());
	}

	return Super::CanEditChange(InProperty);
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

