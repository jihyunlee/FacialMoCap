#pragma once

#include "Math/Quat.h"

#include "MillXMathBPLibrary.generated.h"

UCLASS()
class MILLXUTILS_API UMillXMathBPLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()

	// Quat
	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat MakeFromAxisAngleRad(const FVector& Axis, const float AngleRad);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat MakeFromAxisAngleDeg(const FVector& Axis, const float AngleDeg);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static void ToAxisAngleRad(const FQuat& Quat, FVector& Axis, float& AngleRad);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static void ToAxisAngleDeg(const FQuat& Quat, FVector& Axis, float& AngleDeg);

	UFUNCTION(BluePrintCallable, Category="MillX|Math|Quat")
	static FQuat ConvertEulerToQuat(const FVector& Euler);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FVector ConvertQuatToEuler(const FQuat& Quat);


	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat MakeFromX(const FVector& XAxis);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat MakeFromY(const FVector& YAxis);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat MakeFromZ(const FVector& ZAxis);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat MakeFromXY(const FVector& XAxis, const FVector& YAxis);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat MakeFromXZ(const FVector& XAxis, const FVector& ZAxis);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat MakeFromYX(const FVector& YAxis, const FVector& XAxis);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat MakeFromYZ(const FVector& YAxis, const FVector& ZAxis);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat MakeFromZX(const FVector& ZAxis, const FVector& XAxis);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat MakeFromZY(const FVector& ZAxis, const FVector& YAxis);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FVector GetForwardVector(const FQuat& Quat);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FVector GetRightVector(const FQuat& Quat);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FVector GetUpVector(const FQuat& Quat);


	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat Lerp(const FQuat& A, const FQuat& B, const float Alpha);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat Bilerp(const FQuat& P00, const FQuat& P10, const FQuat& P01, const FQuat& P11, float FracX, float FracY);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat Slerp(const FQuat &Quat1, const FQuat &Quat2, float Slerp, bool Normalized);


	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static FQuat Squad(const FQuat& Quat1, const FQuat& Tang1, const FQuat& Quat2, const FQuat& Tang2, const float Alpha);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static void CalcTangents(const FQuat& PrevP, const FQuat& P, const FQuat& NextP, float Tension, FQuat& OutTan);


	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static float Size(const FQuat& Quat);

	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static float SizeSquared(const FQuat& Quat);


	UFUNCTION(BluePrintCallable, Category = "MillX|Math|Quat")
	static void ToSwingTwist(const FQuat& Quat, const FVector& InTwistAxis, FQuat& OutSwing, FQuat& OutTwist);

};