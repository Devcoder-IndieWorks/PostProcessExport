// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Serialization/JsonTypes.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonValue.h"
#include "Dom/JsonObject.h"

class FVJsonFieldData final
{
public:
    FVJsonFieldData();

public:
    FVJsonFieldData& SetString( const FString& InKey, const FString& InValue );
    FVJsonFieldData& SetName( const FString& InKey, const FName& InValue );
    FVJsonFieldData& SetByte( const FString& InKey, uint8 InValue );
    FVJsonFieldData& SetBool( const FString& InKey, bool InValue );
    FVJsonFieldData& SetInt32( const FString& InKey, int32 InValue );
    FVJsonFieldData& SetInt64( const FString& InKey, int64 InValue );
    FVJsonFieldData& SetFloat( const FString& InKey, float InValue );
    FVJsonFieldData& SetObject( const FString& InKey, const TSharedPtr<FJsonObject>& InValue );

    FVJsonFieldData& SetStringArray( const FString& InKey, const TArray<FString>& InValues );
    FVJsonFieldData& SetNameArray( const FString& InKey, const TArray<FName>& InValues );
    FVJsonFieldData& SetByteArray( const FString& InKey, const TArray<uint8>& InValues );
    FVJsonFieldData& SetBoolArray( const FString& InKey, const TArray<bool>& InValues );
    FVJsonFieldData& SetInt32Array( const FString& InKey, const TArray<int32>& InValues );
    FVJsonFieldData& SetInt64Array( const FString& InKey, const TArray<int64>& InValues );
    FVJsonFieldData& SetFloatArray( const FString& InKey, const TArray<float>& InValues );

    FString GetContents();

private:
    void WriteObject( TSharedRef<TJsonWriter<TCHAR>> InWriter, const FString& InKey, FJsonValue* InValue );

private:
    TSharedPtr<FJsonObject> Data;
};
