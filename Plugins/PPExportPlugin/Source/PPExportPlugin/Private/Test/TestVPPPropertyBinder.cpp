#include "Engine.h"
#include "TestObject.h"
#include "VPPPropertyBinder.h"
#include "VLog.h"

#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST( FTestVPPPropertyBinder, "PPExportPlugin.PPPropertyBinder", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter )
bool FTestVPPPropertyBinder::RunTest( const FString& InParams )
{
    VLOG( Log, TEXT( "#### Begin test VPPPropertyBinder ####" ) );

    {
        auto obj = NewObject<UTestObject>();
        obj->AddToRoot();

        FVPPPropertyBinder::SetFloatByDisplayName( obj, TEXT( "Data" ), TEXT( "TestFloat" ), 10.0f );
        float floatValue = 0.0f;
        FVPPPropertyBinder::GetFloatByDisplayName( obj, TEXT( "Data" ), TEXT( "TestFloat" ), floatValue );
        TestEqual( TEXT( "FloatValue not equal." ), floatValue, 10.0f );

        FVector4 vec(10.0f, 10.0f, 10.0f, 10.0f);
        FVPPPropertyBinder::SetVector4ByDisplayName( obj, TEXT( "Data" ), TEXT( "TestVector4" ), vec );
        FVector4 vecValue;
        FVPPPropertyBinder::GetVector4ByDisplayName( obj, TEXT( "Data" ), TEXT( "TestVector4" ), vecValue );
        TestEqual( TEXT( "Vector4Value.X not equal" ), vecValue.X, vec.X );

        obj->RemoveFromRoot();
    }

    VLOG( Log, TEXT( "#### End test VPPPropertyBinder ####" ) );
    return true;
}
#endif
