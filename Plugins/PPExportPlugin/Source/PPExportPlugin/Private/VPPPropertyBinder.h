// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "Core.h"
#include "UObject/UnrealTypePrivate.h"
#include "ObjectEditorUtils.h"
#include "VLog.h"

class FVPPPropertyBinder final
{
public:
    static bool SetFloatByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, float InValue );
    static bool GetFloatByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, float& OutValue );

    static bool SetIntByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, int32 InValue );
    static bool GetIntByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, int32& OutValue );

    static bool SetUIntByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, uint32 InValue );
    static bool GetUIntByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, uint32& OutValue );

    static bool SetByteByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, uint8 InValue );
    static bool GetByteByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, uint8& OutValue );

    static bool SetBoolByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, bool InValue );
    static bool GetBoolByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, bool& OutValue );

    static bool SetVector4ByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, FVector4 InValue );
    static bool GetVector4ByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, FVector4& OutValue );

    static bool SetLinearColorByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, FLinearColor InValue );
    static bool GetLinearColorByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, FLinearColor& OutValue );

public:
    template<typename Type> 
    static typename TEnableIf<TIsDerivedFrom<Type, FField>::IsDerived, Type*>::Type 
    FindFPropertyByDisplayName( const UStruct* InOwner, FName InDisplayName )
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

            //auto categoryName = FObjectEditorUtils::GetCategoryFName( *It );
            //VLOG( Log, TEXT( "#### Field iterator display name: [%s] Category name: [%s] ####" ), 
            //    *(It->GetDisplayNameText().ToString()), *(categoryName.ToString()) );
        }

        //VLOG( Log, TEXT( "#### Input display nanme: [%s] ####" ), *(InDisplayName.ToString()) );
        return nullptr;
    }

private:
    template<typename Type, typename TypeProperty> 
    static bool SetPropertyByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, Type InValue )
    {
        // Reference: FrameCaptureUtilities.cpp 파일의 UFrameCaptureUtilities::SetCameraScalarProperty()에서.
        // 포스트프로세서볼륨에서 포스트프로세서 구조체 필드이름(InPPName)으로 포스트프로세서 구조체 필드를 얻어옴.
        auto structProp = FindFProperty<FStructProperty>( InObject->GetClass(), InPPName );
        if ( ensure( structProp != nullptr && structProp->Struct != nullptr ) ) {
            auto prop = FindFPropertyByDisplayName<TypeProperty>( structProp->Struct, InDisplayName );
            if ( ensure( prop != nullptr ) ) {
                void* structAddress = structProp->ContainerPtrToValuePtr<void>( InObject );

                FString overrideFieldName = TEXT( "bOverride_" ) + prop->GetName();
                auto overrideProp = FindFProperty<FBoolProperty>( structProp->Struct, *overrideFieldName );
                if ( overrideProp != nullptr )
                    overrideProp->SetPropertyValue_InContainer( structAddress, true );

                prop->SetPropertyValue_InContainer( structAddress, InValue );
                return true;
            }
        }
        return false;
    }

    template<typename Type> 
    static bool SetStructPropertyByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, Type InValue )
    {
        auto structProp = FindFProperty<FStructProperty>( InObject->GetClass(), InPPName );
        if ( ensure( structProp != nullptr && structProp->Struct != nullptr ) ) {
            auto prop = FindFPropertyByDisplayName<FStructProperty>( structProp->Struct, InDisplayName );
            if ( ensure( prop != nullptr ) ) {
                void* structAddress = structProp->ContainerPtrToValuePtr<void>( InObject );

                FString overrideFieldName = TEXT( "bOverride_" ) + prop->GetName();
                auto overrideProp = FindFProperty<FBoolProperty>( structProp->Struct, *overrideFieldName );
                if ( overrideProp != nullptr )
                    overrideProp->SetPropertyValue_InContainer( structAddress, true );

                *prop->ContainerPtrToValuePtr<Type>( structAddress ) = InValue;
                return true;
            }
        }
        return false;
    }

    template<typename Type, typename TypeProperty> 
    static bool GetPropertyByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, Type& OutValue )
    {
        auto structProp = FindFProperty<FStructProperty>( InObject->GetClass(), InPPName );
        if ( ensure( structProp != nullptr && structProp->Struct != nullptr ) ) {
            auto prop = FindFPropertyByDisplayName<TypeProperty>( structProp->Struct, InDisplayName );
            if ( ensure( prop != nullptr ) ) {
                void* structAddress = structProp->ContainerPtrToValuePtr<void>( InObject );
                Type propValue = prop->GetPropertyValue_InContainer( structAddress );
                OutValue = propValue;
                return true;
            }
        }
        return false;
    }

    template<typename Type> 
    static bool GetStructPropertyByDisplayName( class UObject* InObject, const FName& InPPName, const FName& InDisplayName, Type& OutValue )
    {
        auto structProp = FindFProperty<FStructProperty>( InObject->GetClass(), InPPName );
        if ( ensure( structProp != nullptr && structProp->Struct != nullptr ) ) {
            auto prop = FindFPropertyByDisplayName<FStructProperty>( structProp->Struct, InDisplayName );
            if ( ensure( prop != nullptr ) ) {
                void* structAddress = structProp->ContainerPtrToValuePtr<void>( InObject );
                Type propValue = *prop->ContainerPtrToValuePtr<Type>( structAddress );
                OutValue = propValue;
                return true;
            }
        }
        return false;
    }
};
