// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VPPImportRuntimeUtils.generated.h"

UCLASS() 
class PPIMPORTRUNTIMEPLUGIN_API UVPPImportRuntimeUtils : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()
public:
    UFUNCTION( BlueprintCallable, Category="PostProcess Settings Import" )
    static bool ImportPostProcessSettings( const FString& InJsonFilename, UObject* InPostProcessVolume );
};
