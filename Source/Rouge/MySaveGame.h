// MySaveGame.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

UCLASS()
class ROUGE_API UMySaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UMySaveGame();

    UPROPERTY(VisibleAnywhere, Category = Basic)
        int32 HighScore;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        bool bGameFirstLoaded;
};
