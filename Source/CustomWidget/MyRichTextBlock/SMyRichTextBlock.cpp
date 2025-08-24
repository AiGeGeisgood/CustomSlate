#include "SMyRichTextBlock.h"

#if WITH_FANCY_TEXT

#include "Framework/Text/RichTextMarkupProcessing.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"
#include "Types/ReflectionMetadata.h"
#include "Widgets/Text/SlateTextBlockLayout.h"

SMyRichTextBlock::SMyRichTextBlock()
{
}

SMyRichTextBlock::~SMyRichTextBlock()
{
	// Needed to avoid "deletion of pointer to incomplete type 'FSlateTextBlockLayout'; no destructor called" error when using TUniquePtr
}

void SMyRichTextBlock::Construct( const FArguments& InArgs )
{
	BoundText = InArgs._Text;
	HighlightText = InArgs._HighlightText;

	TextStyle = *InArgs._TextStyle;
	WrapTextAt = InArgs._WrapTextAt;
	AutoWrapText = InArgs._AutoWrapText;
	WrappingPolicy = InArgs._WrappingPolicy;
	Margin = InArgs._Margin;
	LineHeightPercentage = InArgs._LineHeightPercentage;
	Justification = InArgs._Justification;
	MinDesiredWidth = InArgs._MinDesiredWidth;

	{
		TSharedPtr<IRichTextMarkupParser> Parser = InArgs._Parser;
		if ( !Parser.IsValid() )
		{
			Parser = FDefaultRichTextMarkupParser::GetStaticInstance();
		}

		Marshaller = InArgs._Marshaller;
		if (!Marshaller.IsValid())
		{
			Marshaller = FRichTextLayoutMarshaller::Create(Parser, nullptr, InArgs._Decorators, InArgs._DecoratorStyleSet);
		}
		
		for (const TSharedRef< ITextDecorator >& Decorator : InArgs.InlineDecorators)
		{
			Marshaller->AppendInlineDecorator(Decorator);
		}

		TextLayoutCache = MakeUnique<FSlateTextBlockLayout>(this, TextStyle, InArgs._TextShapingMethod, InArgs._TextFlowDirection, InArgs._CreateSlateTextLayout, Marshaller.ToSharedRef(), nullptr);
		TextLayoutCache->SetDebugSourceInfo(TAttribute<FString>::Create(TAttribute<FString>::FGetter::CreateLambda([this]{ return FReflectionMetaData::GetWidgetDebugInfo(this); })));
	}

	SetCanTick(false);
}

int32 SMyRichTextBlock::OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const
{
	const FVector2D LastDesiredSize = TextLayoutCache->GetDesiredSize();

	// OnPaint will also update the text layout cache if required
	LayerId = TextLayoutCache->OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, ShouldBeEnabled(bParentEnabled));

	const FVector2D NewDesiredSize = TextLayoutCache->GetDesiredSize();

	// HACK: Due to the nature of wrapping and layout, we may have been arranged in a different box than what we were cached with.  Which
	// might update wrapping, so make sure we always set the desired size to the current size of the text layout, which may have changed
	// during paint.
	bool bCanWrap = WrapTextAt.Get() > 0 || AutoWrapText.Get();

	if (bCanWrap && !NewDesiredSize.Equals(LastDesiredSize))
	{
		const_cast<SMyRichTextBlock*>(this)->Invalidate(EInvalidateWidget::Layout);
	}

	return LayerId;
}

FVector2D SMyRichTextBlock::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	// ComputeDesiredSize will also update the text layout cache if required
	const FVector2D TextSize = TextLayoutCache->ComputeDesiredSize(
		FSlateTextBlockLayout::FWidgetArgs(BoundText, HighlightText, WrapTextAt, AutoWrapText, WrappingPolicy, Margin, LineHeightPercentage, Justification),
		LayoutScaleMultiplier, TextStyle
		);

	return FVector2D(FMath::Max(TextSize.X, MinDesiredWidth.Get()), TextSize.Y);
}

FChildren* SMyRichTextBlock::GetChildren()
{
	return TextLayoutCache->GetChildren();
}

void SMyRichTextBlock::OnArrangeChildren( const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren ) const
{
	TextLayoutCache->ArrangeChildren( AllottedGeometry, ArrangedChildren );
}

void SMyRichTextBlock::SetText( const TAttribute<FText>& InTextAttr )
{
	BoundText = InTextAttr;
	Invalidate(EInvalidateWidget::LayoutAndVolatility);
}

void SMyRichTextBlock::SetHighlightText( const TAttribute<FText>& InHighlightText )
{
	HighlightText = InHighlightText;
	Invalidate(EInvalidateWidget::LayoutAndVolatility);
}

void SMyRichTextBlock::SetTextShapingMethod(const TOptional<ETextShapingMethod>& InTextShapingMethod)
{
	TextLayoutCache->SetTextShapingMethod(InTextShapingMethod);
	Invalidate(EInvalidateWidget::Layout);
}

void SMyRichTextBlock::SetTextFlowDirection(const TOptional<ETextFlowDirection>& InTextFlowDirection)
{
	TextLayoutCache->SetTextFlowDirection(InTextFlowDirection);
	Invalidate(EInvalidateWidget::Layout);
}

void SMyRichTextBlock::SetWrapTextAt(const TAttribute<float>& InWrapTextAt)
{
	SetAttribute(WrapTextAt, InWrapTextAt, EInvalidateWidgetReason::Layout);
}

void SMyRichTextBlock::SetAutoWrapText(const TAttribute<bool>& InAutoWrapText)
{
	SetAttribute(AutoWrapText, InAutoWrapText, EInvalidateWidgetReason::Layout);
}

void SMyRichTextBlock::SetWrappingPolicy(const TAttribute<ETextWrappingPolicy>& InWrappingPolicy)
{
	SetAttribute(WrappingPolicy, InWrappingPolicy, EInvalidateWidgetReason::Layout);
}

void SMyRichTextBlock::SetLineHeightPercentage(const TAttribute<float>& InLineHeightPercentage)
{
	SetAttribute(LineHeightPercentage, InLineHeightPercentage, EInvalidateWidgetReason::Layout);
}

void SMyRichTextBlock::SetMargin(const TAttribute<FMargin>& InMargin)
{
	SetAttribute(Margin, InMargin, EInvalidateWidgetReason::Layout);
}

void SMyRichTextBlock::SetJustification(const TAttribute<ETextJustify::Type>& InJustification)
{
	SetAttribute(Justification, InJustification, EInvalidateWidgetReason::Layout);
}

void SMyRichTextBlock::SetTextStyle(const FTextBlockStyle& InTextStyle)
{
	TextStyle = InTextStyle;
	Invalidate(EInvalidateWidget::Layout);
}

void SMyRichTextBlock::SetMinDesiredWidth(const TAttribute<float>& InMinDesiredWidth)
{
	SetAttribute(MinDesiredWidth, InMinDesiredWidth, EInvalidateWidgetReason::Layout);
}

void SMyRichTextBlock::SetDecoratorStyleSet(const ISlateStyle* NewDecoratorStyleSet)
{
	if (Marshaller.IsValid())
	{
		Marshaller->SetDecoratorStyleSet(NewDecoratorStyleSet);
		Refresh();
	}
}

void SMyRichTextBlock::Refresh()
{
	TextLayoutCache->DirtyContent();
	Invalidate(EInvalidateWidget::Layout);
}

bool SMyRichTextBlock::ComputeVolatility() const
{
	return SWidget::ComputeVolatility() 
		|| BoundText.IsBound()
		|| HighlightText.IsBound()
		|| WrapTextAt.IsBound()
		|| AutoWrapText.IsBound()
		|| WrappingPolicy.IsBound()
		|| Margin.IsBound()
		|| Justification.IsBound()
		|| LineHeightPercentage.IsBound()
		|| MinDesiredWidth.IsBound();
}

#endif //WITH_FANCY_TEXT
