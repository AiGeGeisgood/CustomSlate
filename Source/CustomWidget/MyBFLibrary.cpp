// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBFLibrary.h"

#include "Components/Image.h"
#include "Engine/AssetManager.h"

void UMyBFLibrary::SetSyncLoadTexture(UImage* Image, TSoftObjectPtr<UTexture2D> SoftTexture)
{
	// FSoftObjectPath DefaultImage(TEXT("Texture2D'/Game/f12.f12'"));
	TSharedPtr<FStreamableHandle> Texture = UAssetManager::GetStreamableManager().RequestSyncLoad(SoftTexture.ToSoftObjectPath());

	// auto Texture = LoadObject<UTexture2D>(nullptr,TEXT("Texture2D'/Game/f12.f12'"));
	Image->Brush.SetResourceObject(Texture->GetLoadedAsset());
}

void UMyBFLibrary::SetASyncLoadTexture(UImage* Image, TSoftObjectPtr<UTexture2D> SoftTexture)
{
	// FSoftObjectPath DefaultImage(TEXT("Texture2D'/Game/f12.f12'"));
	TSharedPtr<FStreamableHandle> Texture = UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftTexture.ToSoftObjectPath(), [=]()
	{
		// auto Texture = LoadObject<UTexture2D>(nullptr,TEXT("Texture2D'/Game/f12.f12'"));
		Image->Brush.SetResourceObject(SoftTexture.ToSoftObjectPath().ResolveObject());
	});
}
