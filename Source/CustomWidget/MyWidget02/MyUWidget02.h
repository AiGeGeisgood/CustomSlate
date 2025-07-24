// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyUSlot02.h"
#include "Components/PanelWidget.h"
#include "MyUWidget02.generated.h"

/**
 * 
 */
UCLASS(meta = (ShortTooltip = "A layout panel for automatically laying child widgets out horizontally"))
class CUSTOMWIDGET_API UMyUWidget02 : public UPanelWidget
{
	GENERATED_UCLASS_BODY()
public:

	/**  */
	UFUNCTION(BlueprintCallable, Category="Widget")
	UMyUSlot02* AddChildToHorizontalBox(UWidget* Content);

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

	TSharedPtr<class SMySWidget02> MySWidget02;

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

};
