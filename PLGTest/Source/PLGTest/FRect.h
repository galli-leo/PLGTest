

#pragma once

struct FRect{
	float	width,
		height,
		x,
		y;

	/*FString Description()
	{
	return "width: " + FString::SanitizeFloat(width) + ", height: " + FString::SanitizeFloat(height);
	}*/
	FORCEINLINE FRect();

	FORCEINLINE FRect(float inWidth, float inHeight);

	FORCEINLINE FRect(float inWidth, float inHeight, float inX, float inY);

	//FString ASDFRectDescription();

};

FORCEINLINE FRect::FRect()
{

}


FORCEINLINE FRect::FRect(float inWidth, float inHeight) : width(inWidth), height(inHeight)
{

}

FORCEINLINE FRect::FRect(float inWidth, float inHeight, float inX, float inY) : width(inWidth), height(inHeight), x(inX), y(inY)
{

}
/*
FString FRect::ASDFRectDescription()
{
	return FString::SanitizeFloat(width) + "," + FString::SanitizeFloat(height) + "," + FString::SanitizeFloat(x) + "," + FString::SanitizeFloat(y);
}*/