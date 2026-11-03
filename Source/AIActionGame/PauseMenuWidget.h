#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

UCLASS()
class AIACTIONGAME_API UPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void OnResumeClicked();

    UFUNCTION(BlueprintCallable)
    void OnQuitClicked();

    UFUNCTION(BlueprintCallable)
    void OnSaveClicked();

    UFUNCTION(BlueprintCallable)
    void OnLoadClicked();

};
