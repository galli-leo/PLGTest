

#pragma once
#include "FLevelCell.h"



struct UGridRow{
	TArray<FLevelCell *> columns;

	
	FORCEINLINE UGridRow();

	

};

FORCEINLINE UGridRow::UGridRow()
{

}



