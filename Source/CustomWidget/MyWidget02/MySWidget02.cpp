#include "MySWidget02.h"



void SMySWidget02::Construct( const SMySWidget02::FArguments& InArgs )
{
	const int32 NumSlots = InArgs.Slots.Num();
	for ( int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex )
	{
		Children.Add( InArgs.Slots[SlotIndex] );
	}
}