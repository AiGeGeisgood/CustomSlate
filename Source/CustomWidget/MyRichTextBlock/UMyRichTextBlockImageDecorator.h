#pragma once
#include "UMyRichTextBlockDecorator.h"
#include "Engine/DataTable.h"
#include "UMyRichTextBlockImageDecorator.generated.h"

class ISlateStyle;

/** Simple struct for rich text styles */
USTRUCT(Blueprintable, BlueprintType)
struct CUSTOMWIDGET_API FMyRichImageRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Appearance)
	FSlateBrush Brush;
};

/**
 * Allows you to setup an image decorator that can be configured
 * to map certain keys to certain images.  We recommend you subclass this
 * as a blueprint to configure the instance.
 *
 * Understands the format <img id="NameOfBrushInTable"></>
 */
UCLASS(Abstract, Blueprintable)
class CUSTOMWIDGET_API UMyRichTextBlockImageDecorator : public UMyRichTextBlockDecorator
{
	GENERATED_BODY()

public:
	UMyRichTextBlockImageDecorator(const FObjectInitializer& ObjectInitializer);

	virtual TSharedPtr<ITextDecorator> CreateDecorator(UMyRichTextBlock* InOwner) override;

	virtual const FSlateBrush* FindImageBrush(FName TagOrId, bool bWarnIfMissing);

protected:

	FMyRichImageRow* FindImageRow(FName TagOrId, bool bWarnIfMissing);

	UPROPERTY(EditAnywhere, Category=Appearance, meta=(RowType="RichImageRow"))
	class UDataTable* ImageSet;
};
