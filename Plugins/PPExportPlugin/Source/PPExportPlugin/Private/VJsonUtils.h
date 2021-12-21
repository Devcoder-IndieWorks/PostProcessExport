// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "JsonUtilities.h"

namespace VJsonUtils
{
    template<typename TStruct> 
    bool GetJsonStringFromStruct( const TStruct& InFilledStruct, FString& OutJsonString )
    {
        FString outString;
        auto ret = FJsonObjectConverter::UStructToJsonObjectString( TStruct::StaticStruct(), &InFilledStruct, outString, 0, 0 );
        if ( ret )
            OutJsonString = RemoveEscapeCharsFromJsonString( outString );

        return ret;
    }

    FString RemoveEscapeCharsFromJsonString( const FString& InJsonString )
    {
        if ( InJsonString.IsEmpty() )
            return InJsonString;

        auto str = InJsonString.Replace( LINE_TERMINATOR, TEXT( "" ) );
        return str.Replace( TEXT( "\t" ), TEXT( "" ) );
    }

    template<typename TStruct> 
    TSharedPtr<FJsonObject> GetJsonObjectFromStruct( const TStruct& InFilledStruct )
    {
        TSharedRef<FJsonObject> jsonObject = MakeShareable( new FJsonObject() );
        return FJsonObjectConverter::UStructToJsonObject( TStruct::StaticStruct(), &InFilledStruct, jsonObject, 0, 0 ) 
            ? jsonObject : TSharedPtr<FJsonObject>();
    }

    template<typename TStruct> 
    bool GetStructFromJsonString( const FString& InJsonString, TStruct& OutStruct )
    {
        return FJsonObjectConverter::JsonObjectStringToUStruct<TStruct>( InJsonString, &OutStruct, 0, 0 );
    }

    template<typename TStruct> 
    bool GetStructFromJsonObject( const TSharedRef<FJsonObject>& InJsonObject, TStruct& OutStruct )
    {
        return FJsonObjectConverter::JsonObjectToUStruct<TStruct>( InJsonObject, &OutStruct, 0, 0 );
    }

    //-------------------------------------------------------------------------

    template<typename Type> 
    void ForEach( TArray<Type>& InContainer, TFunctionRef<void(Type&)> InAction )
    {
        if ( InContainer.Num() == 0 )
            return;

        for ( Type& data : InContainer )
            InAction( data );
    }

    template<typename Type> 
    void ForEach( const TArray<Type>& InContainer, TFunctionRef<void(Type)> InAction )
    {
        if ( InContainer.Num() == 0 )
            return;

        for ( const Type& data : InContainer )
            InAction( data );
    }

    template<typename Type> 
    void ForEach( TArray<Type>& InContainer, TFunctionRef<void(Type&)> InAction, TFunctionRef<bool(const Type&)> InPredicate )
    {
        if ( InContainer.Num() == 0 )
            return;

        for ( Type& data : InContainer ) {
            if ( !InPredicate( data ) )
                continue;

            InAction( data );
        }
    }

    template<typename Type> 
    void ForEach( const TArray<Type>& InContainer, TFunctionRef<void(Type)> InAction, TFunctionRef<bool(Type)> InPredicate )
    {
        if ( InContainer.Num() == 0 )
            return;

        for ( const Type& data : InContainer ) {
            if ( !InPredicate( data ) )
                continue;

            InAction( data );
        }
    }

    //-------------------------------------------------------------------------

    bool ExportJson( const FString& InFilename, class UObject* InObject, const FName& InPostProcessName, const TArray<FName>& InExportDatas );
    bool ImportJson( const FString& InFilename, class UObject* InObject, const FName& InPostProcessName );
}
