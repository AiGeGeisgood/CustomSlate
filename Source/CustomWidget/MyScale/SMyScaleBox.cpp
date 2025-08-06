#include "SMyScaleBox.h"

#include "Framework/Application/SlateApplication.h"
#include "Layout/LayoutUtils.h"
#include "Misc/CoreDelegates.h"
#include "Widgets/SViewport.h"

/* SMyScaleBox interface
 *****************************************************************************/

void SMyScaleBox::Construct(const SMyScaleBox::FArguments& InArgs)
{
	bHasCustomPrepass = true;

	Stretch = InArgs._Stretch;

	StretchDirection = InArgs._StretchDirection;
	UserSpecifiedScale = InArgs._UserSpecifiedScale;
	IgnoreInheritedScale = InArgs._IgnoreInheritedScale;

	LastFinalOffset = FVector2D(0, 0);

	ChildSlot
	.HAlign(InArgs._HAlign)
	.VAlign(InArgs._VAlign)
	[
		InArgs._Content.Widget
	];

#if WITH_EDITOR
	OverrideScreenSize = InArgs._OverrideScreenSize;
	FSlateApplication::Get().OnDebugSafeZoneChanged.AddSP(this, &SMyScaleBox::DebugSafeAreaUpdated);
#endif

	RefreshSafeZoneScale();
	OnSafeFrameChangedHandle = FCoreDelegates::OnSafeFrameChangedEvent.AddSP(this, &SMyScaleBox::RefreshSafeZoneScale);
}

SMyScaleBox::~SMyScaleBox()
{
	FCoreDelegates::OnSafeFrameChangedEvent.Remove(OnSafeFrameChangedHandle);
}

bool SMyScaleBox::CustomPrepass(float LayoutScaleMultiplier)
{
	SWidget& ChildSlotWidget = ChildSlot.GetWidget().Get();

	const bool bNeedsNormalizingPrepassOrLocalGeometry = DoesScaleRequireNormalizingPrepassOrLocalGeometry();

	// If we need a normalizing prepass, or we've yet to give the child a chance to generate a desired
	// size, do that now.
	if (bNeedsNormalizingPrepassOrLocalGeometry || !LastAllocatedArea.IsSet())
	{
		ChildSlotWidget.SlatePrepass(LayoutScaleMultiplier);

		NormalizedContentDesiredSize = ChildSlotWidget.GetDesiredSize();
	}
	else
	{
		NormalizedContentDesiredSize.Reset();
	}

	TOptional<float> NewComputedContentScale;

	if (bNeedsNormalizingPrepassOrLocalGeometry)
	{
		if (LastAllocatedArea.IsSet())
		{
			NewComputedContentScale = ComputeContentScale(LastPaintGeometry.GetValue());
		}
	}
	else
	{
		// If we don't need the area, send a false geometry.
		static const FGeometry NullGeometry;
		NewComputedContentScale = ComputeContentScale(NullGeometry);
	}

	if (bNeedsNormalizingPrepassOrLocalGeometry)
	{
		ChildSlotWidget.Invalidate(EInvalidateWidgetReason::Layout);
		ChildSlotWidget.InvalidatePrepass();
	}

	// Extract the incoming scale out of the layout scale if 
	if (NewComputedContentScale.IsSet())
	{
		if (IgnoreInheritedScale.Get(false) && LayoutScaleMultiplier != 0)
		{
			NewComputedContentScale = NewComputedContentScale.GetValue() / LayoutScaleMultiplier;
		}
	}

	ComputedContentScale = NewComputedContentScale;

	return true;
}

bool SMyScaleBox::DoesScaleRequireNormalizingPrepassOrLocalGeometry() const
{
	const EMyStretch::Type CurrentStretch = Stretch.Get();
	switch (CurrentStretch)
	{
	case EMyStretch::None:
	case EMyStretch::Fill:
	case EMyStretch::ScaleBySafeZone:
	case EMyStretch::UserSpecified:
		return false;
	default:
		return true;
	}
}

bool SMyScaleBox::IsDesiredSizeDependentOnAreaAndScale() const
{
	const EMyStretch::Type CurrentStretch = Stretch.Get();
	switch (CurrentStretch)
	{
	case EMyStretch::ScaleToFitX:
	case EMyStretch::ScaleToFitY:
		return true;
	default:
		return false;
	}
}

float SMyScaleBox::ComputeContentScale(const FGeometry& PaintGeometry) const
{
	const EMyStretch::Type CurrentStretch = Stretch.Get();
	const EMyStretchDirection::Type CurrentStretchDirection = StretchDirection.Get();

	switch (CurrentStretch)
	{
	case EMyStretch::ScaleBySafeZone:
		return SafeZoneScale;
	case EMyStretch::UserSpecified:
		return UserSpecifiedScale.Get(1.0f);
	}

	float FinalScale = 1;

	const FVector2D ChildDesiredSize = ChildSlot.GetWidget()->GetDesiredSize();

	if (ChildDesiredSize.X != 0 && ChildDesiredSize.Y != 0)
	{
		switch (CurrentStretch)
		{
		case EMyStretch::ScaleToFit:
		{
			//FVector2D LocalSnappedArea = PaintGeometry.LocalToRoundedLocal(PaintGeometry.GetLocalSize()) - PaintGeometry.LocalToRoundedLocal(FVector2D(0, 0));
			//FinalScale = FMath::Min(LocalSnappedArea.X / ChildDesiredSize.X, LocalSnappedArea.Y / ChildDesiredSize.Y);
			FinalScale = FMath::Min(PaintGeometry.GetLocalSize().X / ChildDesiredSize.X, PaintGeometry.GetLocalSize().Y / ChildDesiredSize.Y);
			break;
		}
		case EMyStretch::ScaleToFitX:
			FinalScale = PaintGeometry.GetLocalSize().X / ChildDesiredSize.X;
			break;
		case EMyStretch::ScaleToFitY:
			FinalScale = PaintGeometry.GetLocalSize().Y / ChildDesiredSize.Y;
			break;
		case EMyStretch::Fill:
			break;
		case EMyStretch::ScaleToFill:
			FinalScale = FMath::Max(PaintGeometry.GetLocalSize().X / ChildDesiredSize.X, PaintGeometry.GetLocalSize().Y / ChildDesiredSize.Y);
			break;
		}

		switch (CurrentStretchDirection)
		{
		case EMyStretchDirection::DownOnly:
			FinalScale = FMath::Min(FinalScale, 1.0f);
			break;
		case EMyStretchDirection::UpOnly:
			FinalScale = FMath::Max(FinalScale, 1.0f);
			break;
		case EMyStretchDirection::Both:
			break;
		}
	}

	return FinalScale;
}

void SMyScaleBox::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	const EVisibility ChildVisibility = ChildSlot.GetWidget()->GetVisibility();
	if (ArrangedChildren.Accepts(ChildVisibility))
	{
		const FVector2D AreaSize = AllottedGeometry.GetLocalSize();

		const FVector2D CurrentWidgetDesiredSize = ChildSlot.GetWidget()->GetDesiredSize();
		FVector2D SlotWidgetDesiredSize = CurrentWidgetDesiredSize;

		const EMyStretch::Type CurrentStretch = Stretch.Get();
		const EMyStretchDirection::Type CurrentStretchDirection = StretchDirection.Get();

		if (CurrentStretch == EMyStretch::Fill)
		{
			SlotWidgetDesiredSize = AreaSize;
		}

		// This scale may not look right, the item being 
		// shown may need 2 frames to truly be drawn correctly,
		// but rather than have a blank frame, it's better for us to try
		// and fit the contents to our known geometry.
		const float TempComputedContentScale = ComputedContentScale.IsSet() ? ComputedContentScale.GetValue() : ComputeContentScale(AllottedGeometry);

		LastFinalOffset = FVector2D(0, 0);
		float FinalScale = TempComputedContentScale;

		// If we're just filling, there's no scale applied, we're just filling the area.
		if (CurrentStretch != EMyStretch::Fill)
		{
			const FMargin SlotPadding(ChildSlot.SlotPadding.Get());
			AlignmentArrangeResult XResult = AlignChild<Orient_Horizontal>(AreaSize.X, ChildSlot, SlotPadding, FinalScale, false);
			AlignmentArrangeResult YResult = AlignChild<Orient_Vertical>(AreaSize.Y, ChildSlot, SlotPadding, FinalScale, false);

			LastFinalOffset = FVector2D(XResult.Offset, YResult.Offset) / FinalScale;

			// If the layout horizontally is fill, then we need the desired size to be the whole size of the widget, 
			// but scale the inverse of the scale we're applying.
			if (ChildSlot.HAlignment == HAlign_Fill)
			{
				SlotWidgetDesiredSize.X = AreaSize.X / FinalScale;
			}

			// If the layout vertically is fill, then we need the desired size to be the whole size of the widget, 
			// but scale the inverse of the scale we're applying.
			if (ChildSlot.VAlignment == VAlign_Fill)
			{
				SlotWidgetDesiredSize.Y = AreaSize.Y / FinalScale;
			}
		}

		ArrangedChildren.AddWidget(ChildVisibility, AllottedGeometry.MakeChild(
			ChildSlot.GetWidget(),
			LastFinalOffset,
			SlotWidgetDesiredSize,
			FinalScale
		));
	}
}

int32 SMyScaleBox::OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const
{
	// We need another layout pass if the incoming allocated geometry is different from last frames.
	if (!LastAllocatedArea.IsSet() || !AllottedGeometry.GetLocalSize().Equals(LastAllocatedArea.GetValue()))
	{
		LastAllocatedArea = AllottedGeometry.GetLocalSize();
		LastPaintGeometry = AllottedGeometry;

		if (DoesScaleRequireNormalizingPrepassOrLocalGeometry())
		{
			const_cast<SMyScaleBox*>(this)->Invalidate(EInvalidateWidgetReason::Layout);
			const_cast<SMyScaleBox*>(this)->InvalidatePrepass();
		}
	}

	bool bClippingNeeded = false;

	if (GetClipping() == EWidgetClipping::Inherit)
	{
		const EMyStretch::Type CurrentStretch = Stretch.Get();

		// There are a few stretch modes that require we clip, even if the user didn't set the property.
		switch (CurrentStretch)
		{
		case EMyStretch::ScaleToFitX:
		case EMyStretch::ScaleToFitY:
		case EMyStretch::ScaleToFill:
			bClippingNeeded = true;
			break;
		}
	}

	if (bClippingNeeded)
	{
		OutDrawElements.PushClip(FSlateClippingZone(AllottedGeometry));
		FGeometry HitTestGeometry = AllottedGeometry;
		HitTestGeometry.AppendTransform(FSlateLayoutTransform(Args.GetWindowToDesktopTransform()));
	}

	int32 MaxLayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	if (bClippingNeeded)
	{
		OutDrawElements.PopClip();
	}

	return MaxLayerId;
}

void SMyScaleBox::SetContent(TSharedRef<SWidget> InContent)
{
	ChildSlot
	[
		InContent
	];
}

void SMyScaleBox::SetHAlign(EHorizontalAlignment HAlign)
{
	if(ChildSlot.HAlignment != HAlign)
	{
		ChildSlot.HAlignment = HAlign;
		Invalidate(EInvalidateWidgetReason::Layout);
	}
}

void SMyScaleBox::SetVAlign(EVerticalAlignment VAlign)
{
	if(ChildSlot.VAlignment != VAlign)
	{
		ChildSlot.VAlignment = VAlign;
		Invalidate(EInvalidateWidgetReason::Layout);
	}
}

void SMyScaleBox::SetStretchDirection(EMyStretchDirection::Type InStretchDirection)
{
	if (SetAttribute(StretchDirection, TAttribute<EMyStretchDirection::Type>(InStretchDirection), EInvalidateWidgetReason::Layout))
	{
		Invalidate(EInvalidateWidgetReason::Layout);
		InvalidatePrepass();
	}
}

void SMyScaleBox::SetStretch(EMyStretch::Type InStretch)
{
	if (SetAttribute(Stretch, TAttribute<EMyStretch::Type>(InStretch), EInvalidateWidgetReason::Layout))
	{
		// This function invalidates the prepass.
		RefreshSafeZoneScale();
		check(NeedsPrepass());
	}
}

void SMyScaleBox::SetUserSpecifiedScale(float InUserSpecifiedScale)
{
	if (SetAttribute(UserSpecifiedScale, TAttribute<float>(InUserSpecifiedScale), EInvalidateWidgetReason::Layout))
	{
		Invalidate(EInvalidateWidgetReason::Layout);
		InvalidatePrepass();
	}
}

void SMyScaleBox::SetIgnoreInheritedScale(bool InIgnoreInheritedScale)
{
	if (SetAttribute(IgnoreInheritedScale, TAttribute<bool>(InIgnoreInheritedScale), EInvalidateWidgetReason::Layout))
	{
		InvalidatePrepass();
	}
}

FVector2D SMyScaleBox::ComputeDesiredSize(float InScale) const
{
	if (DoesScaleRequireNormalizingPrepassOrLocalGeometry())
	{
		if (NormalizedContentDesiredSize.IsSet())
		{
			FVector2D ContentDesiredSizeValue = NormalizedContentDesiredSize.GetValue();

			if (IsDesiredSizeDependentOnAreaAndScale())
			{
				// SUPER SPECIAL CASE - 
				// In the special case that we're only fitting one dimension, we can have the opposite dimension desire the growth of the
				// expected scale, if we can get that extra space, awesome.
				if (ComputedContentScale.IsSet() && ComputedContentScale.GetValue() != 0)
				{
					const EMyStretch::Type CurrentStretch = Stretch.Get();

					switch (CurrentStretch)
					{
					case EMyStretch::ScaleToFitX:
						ContentDesiredSizeValue.Y = ContentDesiredSizeValue.Y * ComputedContentScale.GetValue();
						break;
					case EMyStretch::ScaleToFitY:
						ContentDesiredSizeValue.X = ContentDesiredSizeValue.X * ComputedContentScale.GetValue();
						break;
					}
				}
			}

			// If we require a normalizing pre-pass, we can never allow the scaled content's desired size to affect
			// the area we return that we need, otherwise, we'll be introducing hysteresis.
			return ContentDesiredSizeValue;
		}
	}
	// If we don't need a normalizing prepass, then we can safely just multiply
	// the desired size of the children by the computed content scale, so that we request the now larger or smaller
	// area that we need - this area is a constant scale, either by safezone or user scale.
	else if (ComputedContentScale.IsSet())
	{
		return SCompoundWidget::ComputeDesiredSize(InScale) * ComputedContentScale.GetValue();
	}
	
	return SCompoundWidget::ComputeDesiredSize(InScale);
}

float SMyScaleBox::GetRelativeLayoutScale(const FSlotBase& Child, float LayoutScaleMultiplier) const
{
	return ComputedContentScale.IsSet() ? ComputedContentScale.GetValue() : 1.0f;
}

void SMyScaleBox::RefreshSafeZoneScale()
{
	float ScaleDownBy = 0.f;
	FMargin SafeMargin(0, 0, 0, 0);
	FVector2D ScaleBy(1, 1);

#if WITH_EDITOR
	if (OverrideScreenSize.IsSet() && !OverrideScreenSize.GetValue().IsZero())
	{
		FSlateApplication::Get().GetSafeZoneSize(SafeMargin, OverrideScreenSize.GetValue());
		ScaleBy = OverrideScreenSize.GetValue();
	}
	else
#endif
	{
		if (Stretch.Get() == EMyStretch::ScaleBySafeZone)
		{
			TSharedPtr<SViewport> GameViewport = FSlateApplication::Get().GetGameViewport();
			if (GameViewport.IsValid())
			{
				TSharedPtr<ISlateViewport> ViewportInterface = GameViewport->GetViewportInterface().Pin();
				if (ViewportInterface.IsValid())
				{
					const FIntPoint ViewportSize = ViewportInterface->GetSize();

					FSlateApplication::Get().GetSafeZoneSize(SafeMargin, ViewportSize);
					ScaleBy = ViewportSize;
				}
			}
		}
	}

	const float SafeZoneScaleX = FMath::Max(SafeMargin.Left, SafeMargin.Right)/ (float)ScaleBy.X;
	const float SafeZoneScaleY = FMath::Max(SafeMargin.Top, SafeMargin.Bottom) / (float)ScaleBy.Y;

	// In order to deal with non-uniform safe-zones we take the largest scale as the amount to scale down by.
	ScaleDownBy = FMath::Max(SafeZoneScaleX, SafeZoneScaleY);

	SafeZoneScale = 1.f - ScaleDownBy;

	Invalidate(EInvalidateWidgetReason::Layout);
	InvalidatePrepass();
}

#if WITH_EDITOR

void SMyScaleBox::DebugSafeAreaUpdated(const FMargin& NewSafeZone, bool bShouldRecacheMetrics)
{
	RefreshSafeZoneScale();
}

void SMyScaleBox::SetOverrideScreenInformation(TOptional<FVector2D> InScreenSize)
{
	OverrideScreenSize = InScreenSize;
	RefreshSafeZoneScale();
}

#endif

