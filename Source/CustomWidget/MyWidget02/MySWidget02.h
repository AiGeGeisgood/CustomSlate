#pragma once
#include "SlotBase.h"
#include "Layout/Children.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/Anchors.h"

class CUSTOMWIDGET_API SMySWidget02 : public SBoxPanel
{
public:
	class FSlot : public SBoxPanel::FSlot
	{
		public:
		FSlot()
		: SBoxPanel::FSlot()
		{
		}

		FSlot& AutoWidth()
		{
			SizeParam = FAuto();
			return *this;
		}

		FSlot& MaxWidth( const TAttribute< float >& InMaxWidth )
		{
			MaxSize = InMaxWidth;
			return *this;
		}

		FSlot& FillWidth( const TAttribute< float >& StretchCoefficient )
		{
			SizeParam = FStretch( StretchCoefficient );
			return *this;
		}
		FSlot& Padding( float Uniform )
		{
			SlotPadding = FMargin(Uniform);
			return *this;
		}

		FSlot& Padding( float Horizontal, float Vertical )
		{
			SlotPadding = FMargin(Horizontal, Vertical);
			return *this;
		}

		FSlot& Padding( float Left, float Top, float Right, float Bottom )
		{
			SlotPadding = FMargin(Left, Top, Right, Bottom);
			return *this;
		}
		
		FSlot& Padding( TAttribute<FMargin> InPadding )
		{
			SlotPadding = InPadding;
			return *this;
		}

		FSlot& HAlign( EHorizontalAlignment InHAlignment )
		{
			HAlignment = InHAlignment;
			return *this;
		}

		FSlot& VAlign( EVerticalAlignment InVAlignment )
		{
			VAlignment = InVAlignment;
			return *this;
		}

		FSlot& operator[]( TSharedRef<SWidget> InWidget )
		{
			SBoxPanel::FSlot::operator[](InWidget);
			return *this;
		}

		FSlot& Expose( FSlot*& OutVarToInit )
		{
			OutVarToInit = this;
			return *this;
		}
	};

	static FSlot& Slot()
	{
		return *(new FSlot());
	}

	SLATE_BEGIN_ARGS( SMySWidget02 )
	{
		_Visibility = EVisibility::SelfHitTestInvisible;
	}

		SLATE_SUPPORTS_SLOT(SMySWidget02::FSlot)

	SLATE_END_ARGS()

	FSlot& AddSlot()
	{
		SMySWidget02::FSlot& NewSlot = *new SMySWidget02::FSlot();
		this->Children.Add( &NewSlot );

		Invalidate(EInvalidateWidget::Layout);

		return NewSlot;
	}

	FSlot& InsertSlot(int32 Index = INDEX_NONE)
	{
		if (Index == INDEX_NONE)
		{
			return AddSlot();
		}
		SMySWidget02::FSlot& NewSlot = *new SMySWidget02::FSlot();
		this->Children.Insert(&NewSlot, Index);

		Invalidate(EInvalidateWidget::Layout);

		return NewSlot;
	}

	int32 NumSlots() const
	{
		return this->Children.Num();
	}

	FORCENOINLINE SMySWidget02()
	: SBoxPanel( Orient_Horizontal )
	{
		SetCanTick(false);
		bCanSupportFocus = false;
	}

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct( const FArguments& InArgs );
};
