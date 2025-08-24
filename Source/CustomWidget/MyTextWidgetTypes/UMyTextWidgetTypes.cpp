// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyTextWidgetTypes.h"

FMyShapedTextOptions::FMyShapedTextOptions()
{
	bOverride_TextShapingMethod = false;
	TextShapingMethod = ETextShapingMethod::Auto;

	bOverride_TextFlowDirection = false;
	TextFlowDirection = ETextFlowDirection::Auto;
}


UMyTextLayoutWidget::UMyTextLayoutWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Justification = ETextJustify::Left;
	AutoWrapText = false;
	WrapTextAt = 0.0f;
	WrappingPolicy = ETextWrappingPolicy::DefaultWrapping;
	Margin = FMargin(0.0f);
	LineHeightPercentage = 1.0f;
}
