// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMyHorizontalBoxSlot.h"
#include "Components/PanelWidget.h"
#include "UMyHorizontalBox.generated.h"

/**
 * 
 */
UCLASS(meta = (ShortTooltip = "A layout panel for automatically laying child widgets out horizontally"))
class CUSTOMWIDGET_API UMyHorizontalBox : public UPanelWidget
{
	GENERATED_UCLASS_BODY()
public:

	/**  */
	UFUNCTION(BlueprintCallable, Category="Widget")
	UMyHorizontalBoxSlot* AddChildToHorizontalBox(UWidget* Content);

#if WITH_EDITOR
	// UWidget interface
	virtual const FText GetPaletteCategory() override;
	// End UWidget interface
#endif

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:

	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	// End UPanelWidget

protected:

	TSharedPtr<class SMyHorizontalBox> MyHorizontalBox;

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

};
