// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMyOverlaySlot.h"
#include "Components/PanelWidget.h"
#include "UMyOverlay.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMWIDGET_API UMyOverlay : public UPanelWidget
{
	GENERATED_UCLASS_BODY()
public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	/**  */
	UFUNCTION(BlueprintCallable, Category="Widget")
	UMyOverlaySlot* AddChildToOverlay(UWidget* Content);

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	// End UPanelWidget

protected:
	TSharedPtr<class SMyOverlay> MyOverlay;

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface
};
