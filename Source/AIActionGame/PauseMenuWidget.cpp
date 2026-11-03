#include "PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "MyCharacter1.h"

void UPauseMenuWidget::OnResumeClicked()
{
    UGameplayStatics::SetGamePaused(GetWorld(), false);
    RemoveFromParent();

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }
}

void UPauseMenuWidget::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UPauseMenuWidget::OnSaveClicked()
{
    AMyCharacter1* Player = Cast<AMyCharacter1>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Player)
    {
        Player->SaveGameData();
    }
}

void UPauseMenuWidget::OnLoadClicked()
{
    AMyCharacter1* Player = Cast<AMyCharacter1>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Player)
    {
        Player->LoadGameData();
    }
}

