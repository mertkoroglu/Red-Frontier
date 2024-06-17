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

    UFUNCTION(BlueprintCallable, Category = "SaveGame")
        bool GetGameLoaded();

    UFUNCTION(BlueprintCallable, Category = "SaveGame")
        void SetGameLoaded();


private:
    void InitializeSaveGame();

    int32 HighScore;
    bool bGameFirstLoaded;
};
