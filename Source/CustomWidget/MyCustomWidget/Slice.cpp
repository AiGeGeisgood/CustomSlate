#include "Slice.h"

#include "Widgets/DeclarativeSyntaxSupport.h"

#define LOCTEXT_NAMESPACE "UMG"

void USlice::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	MySlice->SetBrush(&Brush);
	MySlice->SetAngle(Angle);
	MySlice->SetArcSize(ArcSize);
}

void USlice::ReleaseSlateResources(bool bReleaseChildren)
{
	MySlice.Reset();
}

void USlice::SetAngle(float InAngle)
{
	Angle = InAngle;
	if (MySlice)
	{
		MySlice->SetAngle(Angle);
	}
}

void USlice::SetArcSize(float InArcSize)
{
	ArcSize = InArcSize;
	if (MySlice)
	{
		MySlice->SetArcSize(ArcSize);
	}
}

#if WITH_EDITOR
const FText USlice::GetPaletteCategory()
{
	return LOCTEXT("AMyWidget", "AMyWidget");
}
#endif

TSharedRef<SWidget> USlice::RebuildWidget()
{
	MySlice = SNew(SSlateSlice)
		.Brush(&Brush)
		.Angle(Angle)
		.ArcSize(ArcSize);
	return MySlice.ToSharedRef();
}
