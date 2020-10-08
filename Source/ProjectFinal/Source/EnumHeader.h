#pragma once

#include "EnumHeader.generated.h"

UENUM(BlueprintType)
enum class EHitStop : uint8
{
	Light,
	Medium,
	Heavy,
	Custom
};
