// Copyright ViveStudios. All Rights Reserved.
#pragma once
#include "UObject/Object.h"
#include "TestObject.generated.h"

USTRUCT( BlueprintType ) 
struct FTestStruct
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY( BlueprintReadWrite, Category="Test", meta=(DisplayName="TestFloat") )
    float FloatValue;
    UPROPERTY( BlueprintReadWrite, Category="Test", meta=(DisplayName="TestInt") )
    int32 IntValue;
    UPROPERTY( BlueprintReadWrite, Category="Test", meta=(DisplayName="TestBool") )
    uint8 BoolValue:1;
    UPROPERTY( BlueprintReadWrite, Category="Test", meta=(DisplayName="TestVector4") )
    FVector4 Vector4Value;
    UPROPERTY( BlueprintReadWrite, Category="Test", meta=(DisplayName="TestLinearColor") )
    FLinearColor LinearColorValue;
};

UCLASS() 
class UTestObject : public UObject
{
    GENERATED_UCLASS_BODY()
public:
    FTestStruct& GetTestStruct();

private:
    UPROPERTY()
    FTestStruct Data;
};
