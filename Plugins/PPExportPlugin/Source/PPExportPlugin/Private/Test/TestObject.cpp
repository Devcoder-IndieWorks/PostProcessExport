#include "TestObject.h"

UTestObject::UTestObject( const FObjectInitializer& ObjectInitializer )
    : Super( ObjectInitializer )
{
    Data.FloatValue = 0.0f;
    Data.IntValue = 0;
    Data.BoolValue = false;
}

FTestStruct& UTestObject::GetTestStruct()
{
    return Data;
}
