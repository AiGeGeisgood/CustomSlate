// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ContentWidget.h"
#include "Components/PanelWidget.h"
#include "UMyNameSlot.generated.h"

/**
 * Allows you to expose an external slot for your user widget.  When others reuse your user control, 
 * they can put whatever they want in this named slot.
 */
UCLASS(meta = (ShortTooltip = "A layout panel for automatically laying child widgets out horizontally"))
class CUSTOMWIDGET_API UMyNameSlot : public UContentWidget
{
	GENERATED_UCLASS_BODY()

public:

	// UPanelWidget interface
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	// End of UPanelWidget interface

	// UVisual interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	// End of UVisual interface

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

protected:
	TSharedPtr<class SMyNameSlot> MyBox;
};
