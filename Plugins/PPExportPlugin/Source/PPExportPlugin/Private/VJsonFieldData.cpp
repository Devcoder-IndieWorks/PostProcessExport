#include "VJsonFieldData.h"
#include "VJsonUtils.h"

FVJsonFieldData::FVJsonFieldData()
{
    Data = MakeShareable( new FJsonObject() );
}

FString FVJsonFieldData::GetContents()
{
    FString outString;
    TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<TCHAR>::Create( &outString );
    WriteObject( jsonWriter, TEXT( "" ), new FJsonValueObject( Data ) );
    jsonWriter->Close();

    //return VJsonUtils::RemoveEscapeCharsFromJsonString( outString );
    return outString;
}

void FVJsonFieldData::WriteObject( TSharedRef<TJsonWriter<TCHAR>> InWriter, const FString& InKey, FJsonValue* InValue )
{
    switch ( InValue->Type ) {
        case EJson::String:
            if ( InKey.Len() > 0 )
                InWriter->WriteValue( InKey, InValue->AsString() );
            else
                InWriter->WriteValue( InValue->AsString() );
            break;
        case EJson::Number:
            if ( InKey.Len() > 0 )
                InWriter->WriteValue( InKey, InValue->AsNumber() );
            else
                InWriter->WriteValue( InValue->AsNumber() );
            break;
        case EJson::Boolean:
            if ( InKey.Len() > 0 )
                InWriter->WriteValue( InKey, InValue->AsBool() );
            else
                InWriter->WriteValue( InValue->AsBool() );
            break;
        case EJson::Object: {
            if ( InKey.Len() > 0 )
                InWriter->WriteObjectStart( InKey );
            else
                InWriter->WriteObjectStart();
        
            TSharedPtr<FJsonObject> objectData = InValue->AsObject();
            for ( auto objectValue = objectData->Values.CreateIterator(); objectValue; ++objectValue )
                WriteObject( InWriter, objectValue.Key(), objectValue.Value().Get() );
        
            InWriter->WriteObjectEnd();
            break;
        }
        case EJson::Array: {
            InWriter->WriteArrayStart( InKey );

            TArray<TSharedPtr<FJsonValue>> objectArray = InValue->AsArray();
            for ( int32 i = 0; i < objectArray.Num(); ++i )
                WriteObject( InWriter, TEXT( "" ), objectArray[i].Get() );

            InWriter->WriteArrayEnd();
            break;
        }
    }
}

FVJsonFieldData& FVJsonFieldData::SetString( const FString& InKey, const FString& InValue )
{
    Data->SetStringField( InKey, InValue );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetName( const FString& InKey, const FName& InValue )
{
    Data->SetStringField( InKey, InValue.ToString() );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetByte( const FString& InKey, uint8 InValue )
{
    Data->SetNumberField( InKey, InValue );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetBool( const FString& InKey, bool InValue )
{
    Data->SetBoolField( InKey, InValue );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetInt32( const FString& InKey, int32 InValue )
{
    Data->SetNumberField( InKey, InValue );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetInt64( const FString& InKey, int64 InValue )
{
    Data->SetNumberField( InKey, InValue );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetFloat( const FString& InKey, float InValue )
{
    Data->SetNumberField( InKey, InValue );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetObject( const FString& InKey, const TSharedPtr<FJsonObject>& InValue )
{
    Data->SetObjectField( InKey, InValue );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetStringArray( const FString& InKey, const TArray<FString>& InValues )
{
    TArray<TSharedPtr<FJsonValue>> datas;
    VJsonUtils::ForEach<FString>( InValues, [&datas]( FString data ){
        datas.Add( MakeShareable( new FJsonValueString( data ) ) );
    } );
    Data->SetArrayField( InKey, datas );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetNameArray( const FString& InKey, const TArray<FName>& InValues )
{
    TArray<TSharedPtr<FJsonValue>> datas;
    VJsonUtils::ForEach<FName>( InValues, [&datas]( FName data ){
        datas.Add( MakeShareable( new FJsonValueString( data.ToString() ) ) );
    } );
    Data->SetArrayField( InKey, datas );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetByteArray( const FString& InKey, const TArray<uint8>& InValues )
{
    TArray<TSharedPtr<FJsonValue>> datas;
    VJsonUtils::ForEach<uint8>( InValues, [&datas]( uint8 data ){
        datas.Add( MakeShareable( new FJsonValueNumber( data ) ) );
    } );
    Data->SetArrayField( InKey, datas );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetBoolArray( const FString& InKey, const TArray<bool>& InValues )
{
    TArray<TSharedPtr<FJsonValue>> datas;
    VJsonUtils::ForEach<bool>( InValues, [&datas]( bool data ){
        datas.Add( MakeShareable( new FJsonValueBoolean( data ) ) );
    } );
    Data->SetArrayField( InKey, datas );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetInt32Array( const FString& InKey, const TArray<int32>& InValues )
{
    TArray<TSharedPtr<FJsonValue>> datas;
    VJsonUtils::ForEach<int32>( InValues, [&datas]( int32 data ){
        datas.Add( MakeShareable( new FJsonValueNumber( data ) ) );
    } );
    Data->SetArrayField( InKey, datas );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetInt64Array( const FString& InKey, const TArray<int64>& InValues )
{
    TArray<TSharedPtr<FJsonValue>> datas;
    VJsonUtils::ForEach<int64>( InValues, [&datas]( int64 data ){
        datas.Add( MakeShareable( new FJsonValueNumber( data ) ) );
    } );
    Data->SetArrayField( InKey, datas );
    return *this;
}

FVJsonFieldData& FVJsonFieldData::SetFloatArray( const FString& InKey, const TArray<float>& InValues )
{
    TArray<TSharedPtr<FJsonValue>> datas;
    VJsonUtils::ForEach<float>( InValues, [&datas]( float data ){
        datas.Add( MakeShareable( new FJsonValueNumber( data ) ) );
    } );
    Data->SetArrayField( InKey, datas );
    return *this;
}
