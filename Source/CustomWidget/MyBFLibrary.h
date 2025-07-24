// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBFLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMWIDGET_API UMyBFLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable,Category = MyFBLibrary)
	static void SetSyncLoadTexture(class UImage* Image,TSoftObjectPtr<UTexture2D> SoftTexture);


	UFUNCTION(BlueprintCallable,Category = MyFBLibrary)
	static void SetASyncLoadTexture(class UImage* Image,TSoftObjectPtr<UTexture2D> SoftTexture);
};
