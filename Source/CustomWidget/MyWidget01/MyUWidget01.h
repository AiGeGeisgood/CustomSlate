// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyUSlot01.h"
#include "Components/PanelWidget.h"
#include "MyUWidget01.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMWIDGET_API UMyUWidget01 : public UPanelWidget
{
	GENERATED_UCLASS_BODY()
public:

	/**  */
	UFUNCTION(BlueprintCallable, Category="Canvas Panel")
	UMyUSlot01* AddChildToCanvas(UWidget* Content);

	/** Gets the underlying native canvas widget if it has been constructed */
	TSharedPtr<class SMySWidget01> GetCanvasWidget() const;

	/** Computes the geometry for a particular slot based on the current geometry of the canvas. */
	bool GetGeometryForSlot(int32 SlotIndex, FGeometry& ArrangedGeometry) const;

	/** Computes the geometry for a particular slot based on the current geometry of the canvas. */
	bool GetGeometryForSlot(UMyUSlot01* Slot, FGeometry& ArrangedGeometry) const;

	void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	// UWidget interface
	virtual const FText GetPaletteCategory() override;
	// End UWidget interface

	// UWidget interface
	virtual bool LockToPanelOnDrag() const override
	{
		return true;
	}
	// End UWidget interface
#endif

protected:
 
	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	// End UPanelWidget

protected:

	TSharedPtr<class SMySWidget01> MyCanvas;

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface
};
