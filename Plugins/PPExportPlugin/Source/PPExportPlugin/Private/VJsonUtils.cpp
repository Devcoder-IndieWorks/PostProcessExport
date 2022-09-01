#include "VJsonUtils.h"
#include "VJsonFieldData.h"
//#include "VPPPropertyBinder.h"
#include "UObject/UnrealTypePrivate.h"
#include "ObjectEditorUtils.h"
#include "VLog.h"

namespace VJsonUtils
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

    // 참고: https://penguinofdev.tistory.com/25
    FString __GetEnumToString( int64 InEnumValue, const FString& InEnumType )
    {
        const auto enumPtr = FindObject<UEnum>( ANY_PACKAGE, *InEnumType, true );
        if ( ensure( enumPtr != nullptr ) )
            return enumPtr->GetNameStringByValue( InEnumValue );

        return FString( TEXT( "Invalid" ) );
    }

    template<typename Type> 
    Type __GetStructPropertyValue( FProperty* InProperty, void* InDataPtr )
    {
        return *InProperty->ContainerPtrToValuePtr<Type>( InDataPtr );
    }

    template<typename Type> 
    void __SetStructPropertyValue( FProperty* InProperty, void* InStructAddress, Type InValue )
    {
        *InProperty->ContainerPtrToValuePtr<Type>( InStructAddress ) = InValue;
    }

    //-------------------------------------------------------------------------

    FString RemoveEscapeCharsFromJsonString( const FString& InJsonString )
    {
        if ( InJsonString.IsEmpty() )
            return InJsonString;

        auto str = InJsonString.Replace( LINE_TERMINATOR, TEXT( "" ) );
        return str.Replace( TEXT( "\t" ), TEXT( "" ) );
    }

    // 참고: https://github.com/getnamo/socketio-client-ue4/blob/master/Source/SIOJson/Private/SIOJConvert.cpp
    bool GetPropertyValueToJsonData( UObject* InObject, const FName& InPPName, const FName& InDisplayName, FVJsonFieldData* InOutJsonData )
    {
        auto ppProp = FindFProperty<FStructProperty>( InObject->GetClass(), InPPName );
        if ( ensure( ppProp != nullptr && ppProp->Struct != nullptr ) ) {
            auto structAddress = ppProp->ContainerPtrToValuePtr<void>( InObject );
            auto prop = __FindPropertyByDisplayName<FProperty>( ppProp->Struct, InDisplayName );
            if ( ensure( prop != nullptr ) ) {
                if ( auto numericProp = CastField<FNumericProperty>( prop ) ) {
                    auto dataPtr = numericProp->ContainerPtrToValuePtr<void>( structAddress );

                    if ( numericProp->IsFloatingPoint() ) {
                        auto val = numericProp->GetFloatingPointPropertyValue( dataPtr );
                        InOutJsonData->SetFloat( InDisplayName.ToString(), (float)val );

                        return true;
                    }
                    else if ( numericProp->IsInteger() ) {
                        auto val = numericProp->GetSignedIntPropertyValue( dataPtr );
                        InOutJsonData->SetInt64( InDisplayName.ToString(), val );

                        return true;
                    }

                    VLOG( Error, TEXT( "#### Not supported numeric type: %s. Display name: %s. ####" ), *(prop->GetClass()->GetName()), *(InDisplayName.ToString()) );
                }
                else if ( auto boolProp = CastField<FBoolProperty>( prop ) ) {
                    auto val = boolProp->GetPropertyValue_InContainer( structAddress );
                    InOutJsonData->SetBool( InDisplayName.ToString(), val );

                    return true;
                }
                else if ( auto enumProperty = CastField<FEnumProperty>( prop ) ) {
                    auto dataPtr = enumProperty->ContainerPtrToValuePtr<void>( structAddress );
                    auto val = enumProperty->GetUnderlyingProperty()->GetSignedIntPropertyValue( dataPtr );
                    auto enumType = enumProperty->GetName();
                    InOutJsonData->SetString( InDisplayName.ToString(), __GetEnumToString( val, enumType ) );

                    return true;
                }
                else if ( auto structProp = CastField<FStructProperty>( prop ) ) {
                    if ( structProp->Struct->GetFName() == NAME_Vector4 ) {
                        auto val = __GetStructPropertyValue<FVector4>( structProp, structAddress );
                        TArray<float> arr;
                        arr.Add( val.X );
                        arr.Add( val.Y );
                        arr.Add( val.Z );
                        arr.Add( val.W );
                        InOutJsonData->SetFloatArray( InDisplayName.ToString(), arr );

                        return true;
                    }
                    else if ( structProp->Struct->GetFName() == NAME_LinearColor ) {
                        auto val = __GetStructPropertyValue<FLinearColor>( structProp, structAddress );
                        TArray<float> arr;
                        arr.Add( val.R );
                        arr.Add( val.G );
                        arr.Add( val.B );
                        arr.Add( val.A );
                        InOutJsonData->SetFloatArray( InDisplayName.ToString(), arr );

                        return true;
                    }

                    VLOG( Error, TEXT( "#### Not supported struct type: %s. Display name: %s. ####" ), *(structProp->Struct->GetName()), *(InDisplayName.ToString()) );
                }
            }
        }

        return false;
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

                    VLOG( Error, TEXT( "#### Not supported numeric type: %s. Display name: %s. ####" ), *(prop->GetClass()->GetName()), *(InDisplayName.ToString()) );
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

                        VLOG( Error, TEXT( "#### Not supported struct type: %s. Display name: %s ####" ), *(structProp->Struct->GetName()), (*InDisplayName.ToString()) );
                    }
                }
            }
        }

        return false;
    }

    //-------------------------------------------------------------------------

    FString __GetJsonFilePath( const FString& InFilename )
    {
        return FString::Printf( TEXT( "%sSaveGames/%s.txt" ), *FPaths::ProjectSavedDir(), *InFilename );
    }

    // 참고: https://ballbot.tistory.com/45
    bool __StringToJsonObject( const FString& InJsonString, TSharedPtr<FJsonObject>& OutJsonObject )
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

    bool SaveJson( const FString& InFilename, const FString& InJsonString )
    {
        return FFileHelper::SaveStringToFile( InJsonString, *(__GetJsonFilePath( InFilename )), FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM );
    }

    //-------------------------------------------------------------------------

    bool ExportJson( const FString& InFilename, UObject* InObject, const FName& InPostProcessName, const TArray<FName>& InExportDatas )
    {
        if ( ensure( InExportDatas.Num() > 0 ) ) {
            FVJsonFieldData jsonFileData;
            ForEach<FName>( InExportDatas, [&jsonFileData, InObject, InPostProcessName]( FName InName ){
                if ( !GetPropertyValueToJsonData( InObject, InPostProcessName, InName, &jsonFileData ) )
                    return;
            } );

            auto jsonString = jsonFileData.GetContents();
            if ( ensure( !jsonString.IsEmpty() ) )
                return SaveJson( InFilename, jsonString );
        }

        return false;
    }

    bool ImportJson( const FString& InFilename, UObject* InObject, const FName& InPostProcessName )
    {
        FString jsonString;
        if ( ensure( LoadJson( InFilename, jsonString ) ) ) {
            TSharedPtr<FJsonObject> jsonObject = MakeShareable( new FJsonObject() );
            if ( ensure( __StringToJsonObject( jsonString, jsonObject ) ) ) {
                for ( const auto& pair : jsonObject->Values ) {
                    auto displayName = pair.Key;
                    auto jsonValue = pair.Value;
                    auto res = SetPropertyValueFromJsonData( InObject, InPostProcessName, *(displayName), jsonValue );

                    VCLOG( !res, Error, TEXT( "#### Failed. import property value. Display name: %s. ####" ), *(displayName) );
                }

                return true;
            }
        }

        return false;
    }
}