// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MySWidget02.h"
#include "Components/PanelSlot.h"
#include "Components/SlateWrapperTypes.h"
#include "Layout/Margin.h"
#include "MyUSlot02.generated.h"


UCLASS()
class CUSTOMWIDGET_API UMyUSlot02 : public UPanelSlot
{
	GENERATED_UCLASS_BODY()

private:
	SMySWidget02::FSlot* Slot;

public:
	
	/** The amount of padding between the slots parent and the content. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Layout|Horizontal Box Slot")
	FMargin Padding;

	/** How much space this slot should occupy in the direction of the panel. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Layout|Horizontal Box Slot")
	FSlateChildSize Size;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Layout|Horizontal Box Slot")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Layout|Horizontal Box Slot")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;

	UFUNCTION(BlueprintCallable, Category="Layout|Horizontal Box Slot")
	void SetPadding(FMargin InPadding);

	UFUNCTION(BlueprintCallable, Category="Layout|Horizontal Box Slot")
	void SetSize(FSlateChildSize InSize);

	UFUNCTION(BlueprintCallable, Category="Layout|Horizontal Box Slot")
	void SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	UFUNCTION(BlueprintCallable, Category="Layout|Horizontal Box Slot")
	void SetVerticalAlignment(EVerticalAlignment InVerticalAlignment);

	void BuildSlot(TSharedRef<SMySWidget02> MySWidget02);

	// UPanelSlot interface
	virtual void SynchronizeProperties() override;
	// End of UPanelSlot interface

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

};
