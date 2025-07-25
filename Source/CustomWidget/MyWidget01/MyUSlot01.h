// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MySWidget01.h"
#include "Components/PanelSlot.h"
#include "Layout/Geometry.h"
#include "Layout/Margin.h"
#include "Widgets/Layout/Anchors.h"
#include "MyUSlot01.generated.h"


USTRUCT(BlueprintType)
struct FMyAnchorData
{
public:
	GENERATED_USTRUCT_BODY()

public:

	/** Offset. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnchorData)
	FMargin Offsets;
	
	/** Anchors. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnchorData)
	FAnchors Anchors;

	/**
	 * Alignment is the pivot point of the widget.  Starting in the upper left at (0,0),
	 * ending in the lower right at (1,1).  Moving the alignment point allows you to move
	 * the origin of the widget.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AnchorData)
	FVector2D Alignment;

	/**
	* Compares this set of FAnchorData with another for equality.
	*
	* @param Other The other FAnchorData.
	* @return true if the two FAnchors are equal, false otherwise.
	*/
	bool operator==(const FMyAnchorData& Other) const
	{
		return Offsets == Other.Offsets && 
			   Anchors == Other.Anchors && 
			   Alignment == Other.Alignment;
	}

	/**
	* Compares this set of FAnchorData with another for inequality.
	*
	* @param Other The other FAnchorData.
	* @return true if the two FAnchors are not equal, false otherwise.
	*/
	bool operator!=(const FMyAnchorData& Other) const
	{
		return !(*this == Other);
	}
};


/**
 * 
 */
UCLASS()
class CUSTOMWIDGET_API UMyUSlot01 : public UPanelSlot
{
	GENERATED_UCLASS_BODY()

	public:
	
	/** The anchoring information for the slot */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Layout|Canvas Slot")
	FMyAnchorData LayoutData;

	/** When AutoSize is true we use the widget's desired size */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Layout|Canvas Slot", AdvancedDisplay, meta=( DisplayName="Size To Content" ))
	bool bAutoSize;

	/** The order priority this widget is rendered in.  Higher values are rendered last (and so they will appear to be on top). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Layout|Canvas Slot")
	int32 ZOrder;

public:
#if WITH_EDITOR
	virtual bool NudgeByDesigner(const FVector2D& NudgeDirection, const TOptional<int32>& GridSnapSize) override;
	virtual bool DragDropPreviewByDesigner(const FVector2D& LocalCursorPosition, const TOptional<int32>& XGridSnapSize, const TOptional<int32>& YGridSnapSize) override;
	virtual void SynchronizeFromTemplate(const UPanelSlot* const TemplateSlot) override;
#endif //WITH_EDITOR

	/** Sets the layout data of the slot */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	void SetLayout(const FMyAnchorData& InLayoutData);

	/** Gets the layout data of the slot */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	FMyAnchorData GetLayout() const;

	/** Sets the position of the slot */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	void SetPosition(FVector2D InPosition);

	/** Gets the position of the slot */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	FVector2D GetPosition() const;

	/** Sets the size of the slot */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	void SetSize(FVector2D InSize);

	/** Gets the size of the slot */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	FVector2D GetSize() const;

	/** Sets the offset data of the slot, which could be position and size, or margins depending on the anchor points */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	void SetOffsets(FMargin InOffset);

	/** Gets the offset data of the slot, which could be position and size, or margins depending on the anchor points */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	FMargin GetOffsets() const;
	
	/** Sets the anchors on the slot */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	void SetAnchors(FAnchors InAnchors);

	/** Gets the anchors on the slot */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	FAnchors GetAnchors() const;

	/** Sets the alignment on the slot */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	void SetAlignment(FVector2D InAlignment);

	/** Gets the alignment on the slot */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	FVector2D GetAlignment() const;

	/** Sets if the slot to be auto-sized */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	void SetAutoSize(bool InbAutoSize);

	/** Gets if the slot to be auto-sized */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	bool GetAutoSize() const;

	/** Sets the z-order on the slot */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	void SetZOrder(int32 InZOrder);

	/** Gets the z-order on the slot */
	UFUNCTION(BlueprintCallable, Category="Layout|Canvas Slot")
	int32 GetZOrder() const;

public:

	/** Sets the anchors on the slot */
	UFUNCTION()
	void SetMinimum(FVector2D InMinimumAnchors);

	/** Sets the anchors on the slot */
	UFUNCTION()
	void SetMaximum(FVector2D InMaximumAnchors);

public:

	void BuildSlot(TSharedRef<SMySWidget01> Canvas);

	// UPanelSlot interface
	virtual void SynchronizeProperties() override;
	// End of UPanelSlot interface

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	// UObject interface
	using Super::PreEditChange;
	virtual void PreEditChange(UProperty* PropertyThatWillChange) override;
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
	// End of UObject interface

	/** Stores the current layout information about the slot and parent canvas. */
	void SaveBaseLayout();

	void SetDesiredPosition(FVector2D InPosition);

	/** Compares the saved base layout against the current state.  Updates the necessary properties to maintain a stable position. */
	void RebaseLayout(bool PreserveSize = true);
#endif

private:
	SMySWidget01::FSlot* Slot;

#if WITH_EDITORONLY_DATA
	FGeometry PreEditGeometry;
	FMyAnchorData PreEditLayoutData;

	TOptional<FVector2D> DesiredPosition;
#endif
};
