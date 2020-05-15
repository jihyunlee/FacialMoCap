#include "MillXNoiseBPLibrary.h"

UMillXNoiseBPLibrary::UMillXNoiseBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

float UMillXNoiseBPLibrary::PerlinNoise2D(const FVector2D& Location)
{
	return  FMath::PerlinNoise2D(Location);
}
