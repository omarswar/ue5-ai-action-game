#include "BTTask_AttackTarget.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Engine/DamageEvents.h" 
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter1.h" // Assuming this is your main character

UBTTask_AttackTarget::UBTTask_AttackTarget()
{
    NodeName = "Attack Target";
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return EBTNodeResult::Failed;

    ACharacter* AIPawn = Cast<ACharacter>(AICon->GetPawn());
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (AIPawn && PlayerPawn)
    {
        AMyCharacter1* Player = Cast<AMyCharacter1>(PlayerPawn);
        if (Player)
        {
            // Respect invulnerability
            if (!Player->bIsInvulnerable)
            {
                Player->TakePlayerDamage(10.0f); // Use your own function
                UE_LOG(LogTemp, Warning, TEXT("AI attacked the player. Remaining HP: %f"), Player->Health);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Attack blocked: player is invulnerable!"));
            }

            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}
