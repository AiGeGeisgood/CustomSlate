#pragma once
#include "Framework/Text/ITextDecorator.h"
#include "UMyRichTextBlockDecorator.generated.h"

class UMyRichTextBlock;

class FMyRichTextDecorator: public ITextDecorator
{
public:
	FMyRichTextDecorator(UMyRichTextBlock* InOwner);

	virtual ~FMyRichTextDecorator() {}

	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		return false;
	}

	virtual TSharedRef<ISlateRun> Create(const TSharedRef<class FTextLayout>& TextLayout, const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef< FString >& InOutModelText, const ISlateStyle* Style) override final;

protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& DefaultTextStyle) const;

	UMyRichTextBlock* Owner;
};

UCLASS(Abstract, Blueprintable)
class CUSTOMWIDGET_API UMyRichTextBlockDecorator : public UObject
{
	GENERATED_BODY()

public:
	UMyRichTextBlockDecorator(const FObjectInitializer& ObjectInitializer);

	virtual TSharedPtr<ITextDecorator> CreateDecorator(UMyRichTextBlock* InOwner);

	
};
