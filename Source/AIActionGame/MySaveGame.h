// MySaveGame.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

UCLASS()
class AIACTIONGAME_API UMySaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    FVector SavedLocation;

    UPROPERTY(BlueprintReadWrite)
    float SavedHealth;

    UPROPERTY(BlueprintReadWrite)
    bool bSavedFirstPerson;
};
