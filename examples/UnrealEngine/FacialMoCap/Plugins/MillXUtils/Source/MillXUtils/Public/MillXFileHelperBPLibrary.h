#pragma once

#include "MillXFileHelperBPLibrary.generated.h"

UCLASS()
class MILLXUTILS_API UMillXFileHelperBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To String (Bytes)", BlueprintAutocast), Category = "MillX|CoreUtility|Conversion")
	static FString Conv_BytesToString(const TArray<uint8>& InBytes);

	/**
	*	Convert string to UTF8 bytes
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "To Bytes (String)", BlueprintAutocast), Category = "MillX|CoreUtility|Conversion")
	static TArray<uint8> Conv_StringToBytes(FString InString);
};