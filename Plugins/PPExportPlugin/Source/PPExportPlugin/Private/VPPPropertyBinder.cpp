#include "VPPPropertyBinder.h"
#include "UObject/Object.h"

bool FVPPPropertyBinder::SetFloatByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, float InValue )
{
    return SetPropertyByDisplayName<float, FFloatProperty>( InObject, InPPName, InDisplayName, InValue );
}

bool FVPPPropertyBinder::GetFloatByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, float& OutValue )
{
    return GetPropertyByDisplayName<float, FFloatProperty>( InObject, InPPName, InDisplayName, OutValue );
}

bool FVPPPropertyBinder::SetIntByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, int32 InValue )
{
    return SetPropertyByDisplayName<int32, FIntProperty>( InObject, InPPName, InDisplayName, InValue );
}

bool FVPPPropertyBinder::GetIntByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, int32& OutValue )
{
    return GetPropertyByDisplayName<int32, FIntProperty>( InObject, InPPName, InDisplayName, OutValue );
}

bool FVPPPropertyBinder::SetUIntByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, uint32 InValue )
{
    return SetPropertyByDisplayName<uint32, FUInt32Property>( InObject, InPPName, InDisplayName, InValue );
}

bool FVPPPropertyBinder::GetUIntByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, uint32& OutValue )
{
    return GetPropertyByDisplayName<uint32, FUInt32Property>( InObject, InPPName, InDisplayName, OutValue );
}

bool FVPPPropertyBinder::SetByteByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, uint8 InValue )
{
    return SetPropertyByDisplayName<uint8, FByteProperty>( InObject, InPPName, InDisplayName, InValue );
}

bool FVPPPropertyBinder::GetByteByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, uint8& OutValue )
{
    return GetPropertyByDisplayName<uint8, FByteProperty>( InObject, InPPName, InDisplayName, OutValue );
}

bool FVPPPropertyBinder::SetBoolByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, bool InValue )
{
    return SetPropertyByDisplayName<bool, FBoolProperty>( InObject, InPPName, InDisplayName, InValue );
}

bool FVPPPropertyBinder::GetBoolByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, bool& OutValue )
{
    return GetPropertyByDisplayName<bool, FBoolProperty>( InObject, InPPName, InDisplayName, OutValue );
}

bool FVPPPropertyBinder::SetVector4ByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, FVector4 InValue )
{
    return SetStructPropertyByDisplayName<FVector4>( InObject, InPPName, InDisplayName, InValue );
}

bool FVPPPropertyBinder::GetVector4ByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, FVector4& OutValue )
{
    return GetStructPropertyByDisplayName<FVector4>( InObject, InPPName, InDisplayName, OutValue );
}

bool FVPPPropertyBinder::SetLinearColorByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, FLinearColor InValue )
{
    return SetStructPropertyByDisplayName<FLinearColor>( InObject, InPPName, InDisplayName, InValue );
}

bool FVPPPropertyBinder::GetLinearColorByDisplayName( UObject* InObject, const FName& InPPName, const FName& InDisplayName, FLinearColor& OutValue )
{
    return GetStructPropertyByDisplayName<FLinearColor>( InObject, InPPName, InDisplayName, OutValue );
}
