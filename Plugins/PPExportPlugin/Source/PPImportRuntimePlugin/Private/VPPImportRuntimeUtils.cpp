#include "VPPImportRuntimeUtils.h"
#include "UObject/UnrealTypePrivate.h"
#include "ObjectEditorUtils.h"
#include "JsonUtilities.h"

UVPPImportRuntimeUtils::UVPPImportRuntimeUtils( const FObjectInitializer& ObjectInitializer )
    : Super( ObjectInitializer )
{
}

//-----------------------------------------------------------------------------

namespace VPPImportRuntimeHelper
{
    template<typename Type> 
    typename TEnableIf<TIsDerivedFrom<Type, FField>::IsDerived, Type*>::Type 
    __FindPropertyByDisplayName( const UStruct* InOwner, FName InDisplayName )
    {
        static_assert( sizeof(Type) > 0, "Type must not be an incomplete type." );
        if ( InDisplayName.IsNone() )
            return nullptr;
    
        TArray<FString> displayNameSplit;
        InDisplayName.ToString().ParseIntoArray( displayNameSplit, TEXT( "," ) );
    
        for ( TFieldIterator<Type> It( InOwner ); It; ++It ) {
            if ( It->GetDisplayNameText().ToString() == displayNameSplit[1] ) {
                auto categoryName = FObjectEditorUtils::GetCategoryFName( *It );
                if ( categoryName.ToString() == displayNameSplit[0] )
                    return *It;
            }
        }
    
        return nullptr;
    }

    template<typename Type> 
    void __SetStructPropertyValue( FProperty* InProperty, void* InStructAddress, Type InValue )
    {
        *InProperty->ContainerPtrToValuePtr<Type>( InStructAddress ) = InValue;
    }

    FString __GetJsonFilePath( const FString& InFilename )
    {
        return FString::Printf( TEXT( "%sSaveGames/%s.txt" ), *FPaths::ProjectSavedDir(), *InFilename );
    }

    bool SetPropertyValueFromJsonData( UObject* InObject, const FName& InPPName, const FName& InDisplayName, const TSharedPtr<FJsonValue>& InJsonValue )
    {
        auto ppProp = FindFProperty<FStructProperty>( InObject->GetClass(), InPPName );
        if ( ensure( ppProp != nullptr && ppProp->Struct != nullptr ) ) {
            auto structAddress = ppProp->ContainerPtrToValuePtr<void>( InObject );
            auto prop = __FindPropertyByDisplayName<FProperty>( ppProp->Struct, InDisplayName );
            if ( ensure( prop != nullptr ) ) {
                FString overrideFieldName = TEXT( "bOverride_" ) + prop->GetName();
                auto overrideProp = FindFProperty<FBoolProperty>( ppProp->Struct, *overrideFieldName );
                if ( overrideProp != nullptr )
                    overrideProp->SetPropertyValue_InContainer( structAddress, true );

                if ( auto numericProp = CastField<FNumericProperty>( prop ) ) {
                    auto dataPtr = numericProp->ContainerPtrToValuePtr<void>( structAddress );

                    if ( numericProp->IsFloatingPoint() ) {
                        numericProp->SetFloatingPointPropertyValue( dataPtr, InJsonValue->AsNumber() );
                        return true;
                    }
                    else if ( numericProp->IsInteger() ) {
                        if ( InJsonValue->Type == EJson::String )
                            numericProp->SetIntPropertyValue( dataPtr, FCString::Atoi64( *InJsonValue->AsString() ) );
                        else
                            numericProp->SetIntPropertyValue( dataPtr, (int64)InJsonValue->AsNumber() );

                        return true;
                    }
                }
                else if ( auto boolProp = CastField<FBoolProperty>( prop ) ) {
                    boolProp->SetPropertyValue( structAddress, InJsonValue->AsBool() );
                    return true;
                }
                else if ( auto enumProp = CastField<FEnumProperty>( prop ) ) {
                    auto dataPtr = enumProp->ContainerPtrToValuePtr<void>( structAddress );

                    if ( InJsonValue->Type == EJson::String ) {
                        const auto enumPtr = enumProp->GetEnum();
                        auto strValue = InJsonValue->AsString();
                        auto intValue = enumPtr->GetValueByName( *strValue );
                        if ( ensure( intValue != INDEX_NONE ) )
                            enumProp->GetUnderlyingProperty()->SetIntPropertyValue( dataPtr, intValue );
                    }
                    else {
                        enumProp->GetUnderlyingProperty()->SetIntPropertyValue( dataPtr, (int64)InJsonValue->AsNumber() );
                    }

                    return true;
                }
                else if ( auto structProp = CastField<FStructProperty>( prop ) ) {
                    const TArray<TSharedPtr<FJsonValue>>* datas;
                    if ( ensure( InJsonValue->TryGetArray( datas ) ) ) {
                        TArray<float> arr;
                        for ( const auto& val : *datas )
                            arr.Add( (float)val->AsNumber() );

                        if ( structProp->Struct->GetFName() == NAME_Vector4 ) {
                            FVector4 vec( arr[0], arr[1], arr[2], arr[3] );
                            __SetStructPropertyValue( structProp, structAddress, vec );

                            return true;
                        }
                        else if ( structProp->Struct->GetFName() == NAME_LinearColor ) {
                            FLinearColor col( arr[0], arr[1], arr[2], arr[3] );
                            __SetStructPropertyValue( structProp, structAddress, col );

                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    bool StringToJsonObject( const FString& InJsonString, TSharedPtr<FJsonObject>& OutJsonObject )
    {
        auto reader = TJsonReaderFactory<TCHAR>::Create( *InJsonString );
        FJsonSerializer::Deserialize( reader, OutJsonObject );

        return OutJsonObject->Values.Num() > 0;
    }

    bool LoadJson( const FString& InFilename, FString& OutJsonString, bool InDeleteJson = false )
    {
        if ( FFileHelper::LoadFileToString( OutJsonString, *(__GetJsonFilePath( InFilename )) ) ) {
            if ( InDeleteJson )
                IFileManager::Get().Delete( *(__GetJsonFilePath( InFilename )) );
            return true;
        }

        return false;
    }
}

//-----------------------------------------------------------------------------

bool UVPPImportRuntimeUtils::ImportPostProcessSettings( const FString& InJsonFilename, UObject* InPostProcessVolume )
{
    FString jsonString;
    if ( ensure( VPPImportRuntimeHelper::LoadJson( InJsonFilename, jsonString ) ) ) {
        TSharedPtr<FJsonObject> jsonObject = MakeShareable( new FJsonObject() );
        if ( ensure( VPPImportRuntimeHelper::StringToJsonObject( jsonString, jsonObject ) ) ) {
            for ( const auto& pair : jsonObject->Values ) {
                auto displayName = pair.Key;
                auto jsonValue = pair.Value;
                VPPImportRuntimeHelper::SetPropertyValueFromJsonData( InPostProcessVolume, TEXT( "Settings" ), *(displayName), jsonValue );
            }

            return true;
        }
    }

    return false;
}
