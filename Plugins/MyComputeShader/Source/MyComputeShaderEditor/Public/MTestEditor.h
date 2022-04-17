// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MTestEditor.generated.h"

/**
 * 
 */
UCLASS()
class MYCOMPUTESHADEREDITOR_API UMTestEditor : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	static UTexture2D* MGetRTTexture(UTextureRenderTarget2D* result);
	static UTexture2D* MCreatNewTexture(TArray<FColor>& mdata, uint32 sizex, uint32 sizey);
	static UTextureRenderTarget2D* MCreatNewTexRT(uint32 sizex, uint32 sizey, FLinearColor clearColor);
	static bool MExportUTexture2D(UTexture2D* tex, const FString& path);
	static UTexture2D* LoadImageFromLocalUrl(const FString& ImageUrl);
public:
	UFUNCTION(BlueprintCallable)
		static void MTest();


};
