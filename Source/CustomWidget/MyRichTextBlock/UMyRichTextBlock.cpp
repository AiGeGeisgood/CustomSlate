// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyRichTextBlock.h"

#include "RenderingThread.h"
#include "SMyRichTextBlock.h"
#include "UMyRichTextBlockDecorator.h"
#include "Editor/WidgetCompilerLog.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"
#include "Framework/Text/RichTextMarkupProcessing.h"
#include "Styling/SlateStyle.h"


#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UMyRichTextBlock

template< class ObjectType >
struct FMyDeferredDeletor : public FDeferredCleanupInterface
{
public:
	FMyDeferredDeletor(ObjectType* InInnerObjectToDelete)
		: InnerObjectToDelete(InInnerObjectToDelete)
	{
	}

	virtual ~FMyDeferredDeletor()
	{
		delete InnerObjectToDelete;
	}

private:
	ObjectType* InnerObjectToDelete;
};

template< class ObjectType >
FORCEINLINE SharedPointerInternals::FRawPtrProxy< ObjectType > MakeShareableMyDeferredCleanup(ObjectType* InObject)
{
	return MakeShareable(InObject, [](ObjectType* ObjectToDelete) { BeginCleanup(new FMyDeferredDeletor<ObjectType>(ObjectToDelete)); });
}

UMyRichTextBlock::UMyRichTextBlock(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Visibility = ESlateVisibility::SelfHitTestInvisible;
}

void UMyRichTextBlock::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyRichTextBlock.Reset();
	StyleInstance.Reset();
}

TSharedRef<SWidget> UMyRichTextBlock::RebuildWidget()
{
	UpdateStyleData();

	TArray< TSharedRef< class ITextDecorator > > CreatedDecorators;
	CreateDecorators(CreatedDecorators);

	TSharedRef<FRichTextLayoutMarshaller> Marshaller = FRichTextLayoutMarshaller::Create(CreateMarkupParser(), CreateMarkupWriter(), CreatedDecorators, StyleInstance.Get());

	MyRichTextBlock =
		SNew(SMyRichTextBlock)
		.TextStyle(bOverrideDefaultStyle ? &DefaultTextStyleOverride : &DefaultTextStyle)
		.Marshaller(Marshaller);
	
	return MyRichTextBlock.ToSharedRef();
}

void UMyRichTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyRichTextBlock->SetText(Text);

	Super::SynchronizeTextLayoutProperties( *MyRichTextBlock );
}

void UMyRichTextBlock::UpdateStyleData()
{
	if (IsDesignTime())
	{
		InstanceDecorators.Reset();
	}

	if (!StyleInstance.IsValid())
	{
		RebuildStyleInstance();

		for (TSubclassOf<UMyRichTextBlockDecorator> DecoratorClass : DecoratorClasses)
		{
			if (UClass* ResolvedClass = DecoratorClass.Get())
			{
				if (!ResolvedClass->HasAnyClassFlags(CLASS_Abstract))
				{
					UMyRichTextBlockDecorator* Decorator = NewObject<UMyRichTextBlockDecorator>(this, ResolvedClass);
					InstanceDecorators.Add(Decorator);
				}
			}
		}
	}
}

FText UMyRichTextBlock::GetText() const
{
	if (MyRichTextBlock.IsValid())
	{
		return MyRichTextBlock->GetText();
	}

	return Text;
}

void UMyRichTextBlock::SetText(const FText& InText)
{
	Text = InText;
	if (MyRichTextBlock.IsValid())
	{
		MyRichTextBlock->SetText(InText);
	}
}

void UMyRichTextBlock::RebuildStyleInstance()
{
	StyleInstance = MakeShareableMyDeferredCleanup(new FSlateStyleSet(TEXT("RichTextStyle")));

	if (TextStyleSet && TextStyleSet->GetRowStruct()->IsChildOf(FMyRichTextStyleRow::StaticStruct()))
	{
		for (const auto& Entry : TextStyleSet->GetRowMap())
		{
			FName SubStyleName = Entry.Key;
			FMyRichTextStyleRow* RichTextStyle = (FMyRichTextStyleRow*)Entry.Value;

			if (SubStyleName == FName(TEXT("Default")))
			{
				DefaultTextStyle = RichTextStyle->TextStyle;
			}

			StyleInstance->Set(SubStyleName, RichTextStyle->TextStyle);
		}
	}
}

void UMyRichTextBlock::SetTextStyleSet(class UDataTable* NewTextStyleSet)
{
	if (TextStyleSet != NewTextStyleSet)
	{
		TextStyleSet = NewTextStyleSet;

		RebuildStyleInstance();

		MyRichTextBlock->SetDecoratorStyleSet(StyleInstance.Get());
		MyRichTextBlock->SetTextStyle(DefaultTextStyle);
	}
}

const FTextBlockStyle& UMyRichTextBlock::GetDefaultTextStyle() const
{
	ensure(StyleInstance.IsValid());
	return DefaultTextStyle;
}

const FTextBlockStyle& UMyRichTextBlock::GetCurrentDefaultTextStyle() const
{
	ensure(StyleInstance.IsValid());
	return bOverrideDefaultStyle ? DefaultTextStyleOverride : DefaultTextStyle;
}

UMyRichTextBlockDecorator* UMyRichTextBlock::GetDecoratorByClass(TSubclassOf<UMyRichTextBlockDecorator> DecoratorClass)
{
	for (UMyRichTextBlockDecorator* Decorator : InstanceDecorators)
	{
		if (Decorator->IsA(DecoratorClass))
		{
			return Decorator;
		}
	}

	return nullptr;
}

void UMyRichTextBlock::CreateDecorators(TArray< TSharedRef< class ITextDecorator > >& OutDecorators)
{
	for (UMyRichTextBlockDecorator* Decorator : InstanceDecorators)
	{
		if (Decorator)
		{
			TSharedPtr<ITextDecorator> TextDecorator = Decorator->CreateDecorator(this);
			if (TextDecorator.IsValid())
			{
				OutDecorators.Add(TextDecorator.ToSharedRef());
			}
		}
	}
}

TSharedPtr< IRichTextMarkupParser > UMyRichTextBlock::CreateMarkupParser()
{
	return FDefaultRichTextMarkupParser::GetStaticInstance();
}

TSharedPtr< IRichTextMarkupWriter > UMyRichTextBlock::CreateMarkupWriter()
{
	return FDefaultRichTextMarkupWriter::Create();
}

void UMyRichTextBlock::BeginDefaultStyleOverride()
{
	if (!bOverrideDefaultStyle)
	{
		// If we aren't already overriding, make sure override style starts off matching the existing default
		bOverrideDefaultStyle = true;
		DefaultTextStyleOverride = DefaultTextStyle;
	}
}

#if WITH_EDITOR

const FText UMyRichTextBlock::GetPaletteCategory()
{
	return LOCTEXT("AggWidget", "AggWidget");
}

void UMyRichTextBlock::OnCreationFromPalette()
{
	//Decorators.Add(NewObject<UMyRichTextBlockDecorator>(this, NAME_None, RF_Transactional));
}

void UMyRichTextBlock::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (TextStyleSet && !TextStyleSet->GetRowStruct()->IsChildOf(FMyRichTextStyleRow::StaticStruct()))
	{
		CompileLog.Warning(FText::Format(
			LOCTEXT("RichTextBlock_InvalidTextStyle", "{0} Text Style Set property expects a Data Table with a Rich Text Style Row structure (currently set to {1})."), 
			FText::FromString(GetName()), 
			FText::AsCultureInvariant(TextStyleSet->GetPathName())));
	}
}

#endif //if WITH_EDITOR

void UMyRichTextBlock::SetDefaultTextStyle(const FTextBlockStyle& InDefaultTextStyle)
{
	BeginDefaultStyleOverride();
	DefaultTextStyleOverride = InDefaultTextStyle;
	ApplyUpdatedDefaultTextStyle();
}

void UMyRichTextBlock::ClearAllDefaultStyleOverrides()
{
	if (bOverrideDefaultStyle)
	{
		bOverrideDefaultStyle = false;
		ApplyUpdatedDefaultTextStyle();
	}
}

void UMyRichTextBlock::SetDefaultColorAndOpacity(FSlateColor InColorAndOpacity)
{
	BeginDefaultStyleOverride();
	DefaultTextStyleOverride.ColorAndOpacity = InColorAndOpacity;
	ApplyUpdatedDefaultTextStyle();
}


void UMyRichTextBlock::SetDefaultShadowColorAndOpacity(FLinearColor InShadowColorAndOpacity)
{
	BeginDefaultStyleOverride();
	DefaultTextStyleOverride.ShadowColorAndOpacity = InShadowColorAndOpacity;
	ApplyUpdatedDefaultTextStyle();
}

void UMyRichTextBlock::SetDefaultShadowOffset(FVector2D InShadowOffset)
{
	BeginDefaultStyleOverride();
	DefaultTextStyleOverride.ShadowOffset = InShadowOffset;
	ApplyUpdatedDefaultTextStyle();
}

void UMyRichTextBlock::SetDefaultFont(FSlateFontInfo InFontInfo)
{
	BeginDefaultStyleOverride();
	DefaultTextStyleOverride.Font = InFontInfo;
	ApplyUpdatedDefaultTextStyle();
}

void UMyRichTextBlock::SetDefaultStrikeBrush(FSlateBrush& InStrikeBrush)
{
	BeginDefaultStyleOverride();
	DefaultTextStyleOverride.StrikeBrush = InStrikeBrush;
	ApplyUpdatedDefaultTextStyle();
}

void UMyRichTextBlock::SetJustification(ETextJustify::Type InJustification)
{
	Super::SetJustification(InJustification);
	if (MyRichTextBlock.IsValid())
	{
		MyRichTextBlock->SetJustification(InJustification);
	}
}

void UMyRichTextBlock::SetMinDesiredWidth(float InMinDesiredWidth)
{
	MinDesiredWidth = InMinDesiredWidth;
	if (MyRichTextBlock.IsValid())
	{
		MyRichTextBlock->SetMinDesiredWidth(InMinDesiredWidth);
	}
}

void UMyRichTextBlock::SetAutoWrapText(bool InAutoTextWrap)
{
	AutoWrapText = InAutoTextWrap;
	if (MyRichTextBlock.IsValid())
	{
		MyRichTextBlock->SetAutoWrapText(InAutoTextWrap);
	}
}

void UMyRichTextBlock::ApplyUpdatedDefaultTextStyle()
{
	if (MyRichTextBlock.IsValid())
	{
		MyRichTextBlock->SetTextStyle(bOverrideDefaultStyle ? DefaultTextStyleOverride : DefaultTextStyle);
	}
}

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

