#include "UMyRichTextBlockDecorator.h"
#include "UMyRichTextBlock.h"
#include "Fonts/FontMeasure.h"
#include "Framework/Application/SlateApplication.h"


#define LOCTEXT_NAMESPACE "UMG"

FMyRichTextDecorator::FMyRichTextDecorator(UMyRichTextBlock* InOwner)
	: Owner(InOwner)
{
}

TSharedRef<ISlateRun> FMyRichTextDecorator::Create(const TSharedRef<class FTextLayout>& TextLayout, const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef< FString >& InOutModelText, const ISlateStyle* Style)
{
	FTextRange ModelRange;
	ModelRange.BeginIndex = InOutModelText->Len();

	FTextRunInfo RunInfo(RunParseResult.Name, FText::FromString(OriginalText.Mid(RunParseResult.ContentRange.BeginIndex, RunParseResult.ContentRange.EndIndex - RunParseResult.ContentRange.BeginIndex)));
	for (const TPair<FString, FTextRange>& Pair : RunParseResult.MetaData)
	{
		RunInfo.MetaData.Add(Pair.Key, OriginalText.Mid(Pair.Value.BeginIndex, Pair.Value.EndIndex - Pair.Value.BeginIndex));
	}

	const FTextBlockStyle& TextStyle = Owner->GetDefaultTextStyle();
	// TODO Allow universal mods?


	TSharedPtr<ISlateRun> SlateRun;
	TSharedPtr<SWidget> DecoratorWidget = CreateDecoratorWidget(RunInfo, TextStyle);
	if (DecoratorWidget.IsValid())
	{
		*InOutModelText += TEXT('\u200B'); // Zero-Width Breaking Space
		ModelRange.EndIndex = InOutModelText->Len();

		// Calculate the baseline of the text within the owning rich text
		const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
		int16 WidgetBaseline = FontMeasure->GetBaseline(TextStyle.Font) - FMath::Min(0.0f, TextStyle.ShadowOffset.Y);

		FSlateWidgetRun::FWidgetRunInfo WidgetRunInfo(DecoratorWidget.ToSharedRef(), WidgetBaseline);
		SlateRun = FSlateWidgetRun::Create(TextLayout, RunInfo, InOutModelText, WidgetRunInfo, ModelRange);
	}

	return SlateRun.ToSharedRef();
}

TSharedPtr<SWidget> FMyRichTextDecorator::CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& DefaultTextStyle) const
{
	return TSharedPtr<SWidget>();
}

/////////////////////////////////////////////////////
// UMyRichTextBlockDecorator

UMyRichTextBlockDecorator::UMyRichTextBlockDecorator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedPtr<ITextDecorator> UMyRichTextBlockDecorator::CreateDecorator(UMyRichTextBlock* InOwner)
{
	return TSharedPtr<ITextDecorator>();
}

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE