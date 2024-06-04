// MyGameInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS()
class ROUGE_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UMyGameInstance();

    UFUNCTION(BlueprintCallable, Category = "HighScore")
        int32 GetHighScore();

    UFUNCTION(BlueprintCallable, Category = "HighScore")
        void SetHighScore(int32 NewHighScore);

    UFUNCTION(BlueprintCallable, Category = "SaveGame")
        void SaveGame();

    UFUNCTION(BlueprintCallable, Category = "SaveGame")
        void LoadGame();

private:
    void InitializeHighScoreFromSaveGame();

    int32 HighScore;
};
