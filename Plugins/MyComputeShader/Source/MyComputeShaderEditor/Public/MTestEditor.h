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
public:
    UFUNCTION(BlueprintCallable)
    static void MTest();
};
