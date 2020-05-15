#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "MillXNoiseBPLibrary.generated.h"

UCLASS()
class MILLXUTILS_API UMillXNoiseBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "MillX|Utils|Noise", meta = (DisplayName = "Perlin Noise 2D"))
	static float PerlinNoise2D(const FVector2D& Location);

};