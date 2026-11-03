#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "MyCharacter1.h"
#include "Kismet/GameplayStatics.h"

void UHealthBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PlayerRef = Cast<AMyCharacter1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void UHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!PlayerRef)
    {
        PlayerRef = Cast<AMyCharacter1>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    }

    if (PlayerRef && HealthProgressBar)
    {
        float HealthPercent = PlayerRef->Health / PlayerRef->MaxHealth;
        HealthProgressBar->SetPercent(HealthPercent);
    }
}
