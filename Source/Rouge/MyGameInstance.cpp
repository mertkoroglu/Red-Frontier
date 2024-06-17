// MyGameInstance.cpp

#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MySaveGame.h"

UMyGameInstance::UMyGameInstance(): 
    HighScore(0),
    bGameFirstLoaded(false)
{
    InitializeSaveGame();
}

int32 UMyGameInstance::GetHighScore()
{
    return HighScore;
}

void UMyGameInstance::SetHighScore(int32 NewHighScore)
{
    HighScore = NewHighScore;
    SaveGame();
}

void UMyGameInstance::SaveGame()
{
    UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
    if (SaveGameInstance)
    {
        SaveGameInstance->HighScore = HighScore;
        SaveGameInstance->bGameFirstLoaded = bGameFirstLoaded;
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("SaveGameSlot"), 0);
    }
}

void UMyGameInstance::LoadGame()
{
    UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SaveGameSlot"), 0));
    if (LoadGameInstance)
    {
        HighScore = LoadGameInstance->HighScore;
        bGameFirstLoaded = LoadGameInstance->bGameFirstLoaded;
    }
}

bool UMyGameInstance::GetGameLoaded()
{
    return bGameFirstLoaded;
}

void UMyGameInstance::SetGameLoaded()
{
    bGameFirstLoaded = true;
    SaveGame();
}

void UMyGameInstance::InitializeSaveGame()
{
    LoadGame();
}
