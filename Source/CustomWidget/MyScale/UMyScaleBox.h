// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMyScaleBox.h"
#include "UObject/ObjectMacros.h"
#include "Widgets/SWidget.h"
#include "Components/ContentWidget.h"
#include "UMyScaleBox.generated.h"

/**
 * A border is a container widget that can contain one child widget, providing an opportunity 
 * to surround it with a background image and adjustable padding.
 *
 * * Single Child
 * * Image
 */


UCLASS(Config=Engine)
class CUSTOMWIDGET_API UMyScaleBox : public UContentWidget
{
	GENERATED_UCLASS_BODY()

public:
	/** The stretching rule to apply when content is stretched */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stretching")
	TEnumAsByte<EMyStretch::Type> Stretch;

	/** Controls in what direction content can be scaled */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stretching")
	TEnumAsByte<EMyStretchDirection::Type> StretchDirection;

	/** Optional scale that can be specified by the User. Used only for UserSpecified stretching. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stretching")
	float UserSpecifiedScale;

	/** Optional bool to ignore the inherited scale. Applies inverse scaling to counteract parents before applying the local scale operation. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stretching")
	bool IgnoreInheritedScale;

public:
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetStretch(EMyStretch::Type InStretch);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetStretchDirection(EMyStretchDirection::Type InStretchDirection);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetUserSpecifiedScale(float InUserSpecifiedScale);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetIgnoreInheritedScale(bool bInIgnoreInheritedScale);

public:
	// UWidget interface
	virtual void SynchronizeProperties() override;
	// End of UWidget interface

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
	virtual bool CanEditChange(const UProperty* InProperty) const override;
	virtual void OnDesignerChanged(const FDesignerChangedEventArgs& EventArgs) override;
#endif

	virtual void Serialize(FArchive& Ar) override;

protected:
	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	// End UPanelWidget

protected:
	TSharedPtr<SMyScaleBox> MyScaleBox;

#if WITH_EDITOR
	TOptional<FVector2D> DesignerSize;
#endif

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface
};
