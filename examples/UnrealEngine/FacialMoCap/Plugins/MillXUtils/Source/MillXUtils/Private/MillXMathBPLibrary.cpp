#include "MillXMathBPLibrary.h"

UMillXMathBPLibrary::UMillXMathBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FQuat UMillXMathBPLibrary::MakeFromAxisAngleRad(const FVector& Axis, const float AngleRad)
{
	return FQuat::FQuat(Axis, AngleRad);
}

FQuat UMillXMathBPLibrary::MakeFromAxisAngleDeg(const FVector& Axis, const float AngleDeg)
{
	return FQuat::FQuat(Axis, FMath::DegreesToRadians(AngleDeg));
}

void UMillXMathBPLibrary::ToAxisAngleRad(const FQuat& Quat, FVector& Axis, float& AngleRad)
{
	Axis = Quat.GetRotationAxis();
	AngleRad = Quat.GetAngle();
}

void UMillXMathBPLibrary::ToAxisAngleDeg(const FQuat& Quat, FVector& Axis, float& AngleDeg)
{
	Axis = Quat.GetRotationAxis();
	AngleDeg = FMath::RadiansToDegrees(Quat.GetAngle());
}

FQuat UMillXMathBPLibrary::ConvertEulerToQuat(const FVector& Euler)
{
	return FQuat::MakeFromEuler(Euler);
}

FVector UMillXMathBPLibrary::ConvertQuatToEuler(const FQuat& Quat)
{
	return Quat.Euler();
}

FQuat UMillXMathBPLibrary::MakeFromX(const FVector& XAxis)
{
	return FRotationMatrix::MakeFromX(XAxis).ToQuat();
}

FQuat UMillXMathBPLibrary::MakeFromY(const FVector& YAxis)
{
	return FRotationMatrix::MakeFromY(YAxis).ToQuat();
}

FQuat UMillXMathBPLibrary::MakeFromZ(const FVector& ZAxis)
{
	return FRotationMatrix::MakeFromZ(ZAxis).ToQuat();
}

FQuat UMillXMathBPLibrary::MakeFromXY(const FVector& XAxis, const FVector& YAxis)
{
	return FRotationMatrix::MakeFromXY(XAxis, YAxis).ToQuat();
}

FQuat UMillXMathBPLibrary::MakeFromXZ(const FVector& XAxis, const FVector& ZAxis)
{
	return FRotationMatrix::MakeFromXZ(XAxis, ZAxis).ToQuat();
}

FQuat UMillXMathBPLibrary::MakeFromYX(const FVector& YAxis, const FVector& XAxis)
{
	return FRotationMatrix::MakeFromYX(YAxis, XAxis).ToQuat();
}

FQuat UMillXMathBPLibrary::MakeFromYZ(const FVector& YAxis, const FVector& ZAxis)
{
	return FRotationMatrix::MakeFromYZ(YAxis, ZAxis).ToQuat();
}

FQuat UMillXMathBPLibrary::MakeFromZX(const FVector& ZAxis, const FVector& XAxis)
{
	return FRotationMatrix::MakeFromZX(ZAxis, XAxis).ToQuat();
}

FQuat UMillXMathBPLibrary::MakeFromZY(const FVector& ZAxis, const FVector& YAxis)
{
	return FRotationMatrix::MakeFromZY(ZAxis, YAxis).ToQuat();
}

FVector UMillXMathBPLibrary::GetForwardVector(const FQuat& Quat)
{
	return Quat.GetForwardVector();
}

FVector UMillXMathBPLibrary::GetRightVector(const FQuat& Quat)
{
	return Quat.GetRightVector();
}

FVector UMillXMathBPLibrary::GetUpVector(const FQuat& Quat)
{
	return Quat.GetUpVector();
}

FQuat UMillXMathBPLibrary::Lerp(const FQuat& A, const FQuat& B, const float Alpha)
{
	return FQuat::FastLerp(A, B, Alpha);
}

FQuat UMillXMathBPLibrary::Bilerp(const FQuat& P00, const FQuat& P10, const FQuat& P01, const FQuat& P11, float FracX, float FracY)
{
	return FQuat::FastBilerp(P00, P10, P01, P11, FracX, FracY);
}

FQuat UMillXMathBPLibrary::Slerp(const FQuat &Quat1, const FQuat &Quat2, float Slerp, bool Normalized)
{
	if (Normalized)
		return FQuat::Slerp(Quat1, Quat2, Slerp);

	return FQuat::Slerp_NotNormalized(Quat1, Quat2, Slerp);
}

FQuat UMillXMathBPLibrary::Squad(const FQuat& Quat1, const FQuat& Tang1, const FQuat& Quat2, const FQuat& Tang2, const float Alpha)
{
	return FQuat::Squad(Quat1, Tang1, Quat2, Tang2, Alpha);
}

void UMillXMathBPLibrary::CalcTangents(const FQuat& PrevP, const FQuat& P, const FQuat& NextP, float Tension, FQuat& OutTan)
{
	FQuat::CalcTangents(PrevP, P, NextP, Tension, OutTan);
}

float UMillXMathBPLibrary::Size(const FQuat &Quat)
{
	return Quat.Size();
}

float UMillXMathBPLibrary::SizeSquared(const FQuat &Quat)
{
	return Quat.SizeSquared();
}

void UMillXMathBPLibrary::ToSwingTwist(const FQuat& Quat, const FVector& InTwistAxis, FQuat& OutSwing, FQuat& OutTwist)
{
	Quat.ToSwingTwist(InTwistAxis, OutSwing, OutTwist);
}
