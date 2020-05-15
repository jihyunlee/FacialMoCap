#include "MillXFileHelperBPLibrary.h"
#include "Misc/FileHelper.h"

UMillXFileHelperBPLibrary::UMillXFileHelperBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FString UMillXFileHelperBPLibrary::Conv_BytesToString(const TArray<uint8>& InArray)
{
	FString ResultString;
	FFileHelper::BufferToString(ResultString, InArray.GetData(), InArray.Num());
	return ResultString;
}

TArray<uint8> UMillXFileHelperBPLibrary::Conv_StringToBytes(FString InString)
{
	TArray<uint8> ResultBytes;
	ResultBytes.Append((uint8*)TCHAR_TO_UTF8(*InString), InString.Len());
	return ResultBytes;
}