// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMySizeBox.h"
#include "Components/PanelSlot.h"
#include "Components/SlateWrapperTypes.h"
#include "UMySizeBoxSlot.generated.h"


UCLASS()
class CUSTOMWIDGET_API UMySizeBoxSlot : public UPanelSlot
{
	GENERATED_UCLASS_BODY()

public:
	/** The padding area between the slot and the content it contains. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Layout|SizeBox Slot")
	FMargin Padding;

private:
	/** A pointer to the button to allow us to adjust the size, padding...etc at runtime. */
	TWeakPtr<SMySizeBox> SizeBox;

public:
	/** The alignment of the object horizontally. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Layout|SizeBox Slot")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	/** The alignment of the object vertically. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Layout|SizeBox Slot")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;


	UFUNCTION(BlueprintCallable, Category="Layout|SizeBox Slot")
	void SetPadding(FMargin InPadding);

	UFUNCTION(BlueprintCallable, Category="Layout|SizeBox Slot")
	void SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	UFUNCTION(BlueprintCallable, Category="Layout|SizeBox Slot")
	void SetVerticalAlignment(EVerticalAlignment InVerticalAlignment);

	// UPanelSlot interface
	virtual void SynchronizeProperties() override;
	// End of UPanelSlot interface

	/** Builds the underlying slot for the slate SizeBox. */
	void BuildSlot(TSharedRef<SMySizeBox> InSizeBox);

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

};
