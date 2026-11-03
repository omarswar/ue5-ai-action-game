#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "EnemyAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create and set up perception component
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
    SetPerceptionComponent(*AIPerceptionComponent);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 1000.f;
    SightConfig->LoseSightRadius = 1200.f;
    SightConfig->PeripheralVisionAngleDegrees = 90.f;
    SightConfig->SetMaxAge(5.f);

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    AIPerceptionComponent->ConfigureSense(*SightConfig);
    AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
    AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    AEnemyAICharacter* ControlledCharacter = Cast<AEnemyAICharacter>(GetPawn());
    if (ControlledCharacter && ControlledCharacter->GetBehaviorTree())
    {
        UseBlackboard(ControlledCharacter->GetBehaviorTree()->BlackboardAsset, BlackboardComp);
        RunBehaviorTree(ControlledCharacter->GetBehaviorTree());

        UE_LOG(LogTemp, Warning, TEXT("Behavior Tree Started"));
    }
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn || !BlackboardComp) return;

    float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), GetPawn()->GetActorLocation());
    bool bHasLOS = LineOfSightTo(PlayerPawn);

    BlackboardComp->SetValueAsBool("TargetVisible", bHasLOS);
    BlackboardComp->SetValueAsObject("TargetActor", PlayerPawn);

    if (bHasLOS)
    {
        BlackboardComp->SetValueAsBool("CanChase", Distance >= 120.0f && Distance <= 1000.0f);
        BlackboardComp->SetValueAsBool("CanAttack", Distance < 120.0f);
    }
    else
    {
        BlackboardComp->SetValueAsBool("CanChase", false);
        BlackboardComp->SetValueAsBool("CanAttack", false);
        BlackboardComp->ClearValue("TargetActor");
    }
}

void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn || !BlackboardComp) return;

    if (LineOfSightTo(PlayerPawn))
    {
        BlackboardComp->SetValueAsBool("TargetVisible", true);
        BlackboardComp->SetValueAsObject("TargetActor", PlayerPawn);
    }
    else
    {
        BlackboardComp->SetValueAsBool("TargetVisible", false);
        BlackboardComp->ClearValue("TargetActor");
    }
}
