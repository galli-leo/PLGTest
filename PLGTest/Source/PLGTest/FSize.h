

#pragma once

//#include "Engine/UserDefinedStruct.h"
//#include "FSize.generated.h"

/**
 * 
 */
/*UCLASS()
class UFSize : public UUserDefinedStruct
{
	GENERATED_UCLASS_BODY()*/
//USTRUCT()
struct FSize{
	//GENERATED_USTRUCT_BODY()
		//UPROPERTY(VisibleAnywhere, Blueprintreadonly, Category = Size)
		float	width;
	//UPROPERTY(VisibleAnywhere, Blueprintreadonly, Category = Size)
		float			height;

		/*FString Description()
		{
			return "width: " + FString::SanitizeFloat(width) + ", height: " + FString::SanitizeFloat(height);
		}*/
		FORCEINLINE FSize();

		FORCEINLINE FSize(float inWidth, float inHeight);
	
};

FORCEINLINE FSize::FSize()
{

}


FORCEINLINE FSize::FSize(float inWidth, float inHeight) : width(inWidth), height(inHeight)
{

}
