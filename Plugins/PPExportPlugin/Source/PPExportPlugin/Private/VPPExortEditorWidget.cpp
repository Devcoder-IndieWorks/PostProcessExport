#include "VPPExportEditorWidget.h"
#include "GameFramework/Actor.h"
#include "VJsonUtils.h"
#include "VLog.h"

UVPPExportEditorWidget::UVPPExportEditorWidget( const FObjectInitializer& ObjectInitializer )
    : Super( ObjectInitializer )
{
}

void UVPPExportEditorWidget::SetPostProcessVolume( AActor* InPPVolume )
{
    if ( IsValid( InPPVolume ) )
        PostProcessVolume = InPPVolume;
}

void UVPPExportEditorWidget::SetJsonFilename( const FString& InJsonFilename )
{
    if ( !InJsonFilename.IsEmpty() )
        JsonFilename = InJsonFilename;
}

void UVPPExportEditorWidget::AddFieldData( const FName& InFieldName )
{
    if ( !InFieldName.IsNone() )
        ExportPostProcessFields.Add( InFieldName );
}

void UVPPExportEditorWidget::ClearFieldDatas()
{
    ExportPostProcessFields.Empty();
}

void UVPPExportEditorWidget::Export()
{
    bool valid = IsValid( PostProcessVolume ) && !JsonFilename.IsEmpty() && (ExportPostProcessFields.Num() > 0);
    if ( ensure( valid ) ) {
        bool result = VJsonUtils::ExportJson( JsonFilename, PostProcessVolume, TEXT( "Settings" ), ExportPostProcessFields );
        VCLOG( !result, Error, TEXT( "#### Failed. export json file from APostProcessVolume. ####" ) );
    }
}

void UVPPExportEditorWidget::Import()
{
    bool valid = IsValid( PostProcessVolume ) && !JsonFilename.IsEmpty();
    if ( ensure( valid ) ) {
        bool result = VJsonUtils::ImportJson( JsonFilename, PostProcessVolume, TEXT( "Settings" ) );
        VCLOG( !result, Error, TEXT( "#### Failed. import json file to APostProcessVolume. ####" ) );
    }
}
