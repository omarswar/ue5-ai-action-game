#include "EnemyAICharacter.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"




void AEnemyAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AEnemyAICharacter::AEnemyAICharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Tell the engine which AI Controller to use
    AIControllerClass = AEnemyAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyAICharacter::TakeDamage(float DamageAmount)
{
    Health -= DamageAmount;
    if (Health <= 0)
    {
        Destroy(); // or play death animation
    }
}
float AEnemyAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Health -= DamageAmount;
    UE_LOG(LogTemp, Warning, TEXT("AI took damage: %f, remaining health: %f"), DamageAmount, Health);
    if (Health <= 0.0f)
    {
        Destroy(); // Or custom death logic
    }

    return DamageAmount;
}

