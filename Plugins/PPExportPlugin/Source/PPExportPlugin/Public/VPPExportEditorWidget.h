// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "VPPExportEditorWidget.generated.h"

UCLASS( BlueprintType ) 
class PPEXPORTPLUGIN_API UVPPExportEditorWidget : public UEditorUtilityWidget
{
    GENERATED_UCLASS_BODY()
public:
    UFUNCTION( BlueprintCallable, Category="Export or Import" )
    void SetPostProcessVolume( class AActor* InPPVolume );
    UFUNCTION( BlueprintCallable, Category="Export or Import" )
    void SetJsonFilename( const FString& InJsonFilename );
    UFUNCTION( BlueprintCallable, Category="Export or Import" )
    void AddFieldData( const FName& InFieldName );
    UFUNCTION( BlueprintCallable, Category="Export or Import" )
    void ClearFieldDatas();

    UFUNCTION( BlueprintCallable, Category="Export or Import" )
    void Export();
    UFUNCTION( BlueprintCallable, Category="Export or Import" )
    void Import();

private:

private:
    UPROPERTY()
    class AActor* PostProcessVolume;

    //-------------------------------------------------------------------------

    TArray<FName> ExportPostProcessFields;
    FString JsonFilename;
};
