// BTTask_GetNextPatrolPoint.cpp
#include "BTTask_GetNextPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "EnemyAICharacter.h"

UBTTask_GetNextPatrolPoint::UBTTask_GetNextPatrolPoint()
{
    NodeName = TEXT("Get Next Patrol Point");
}

EBTNodeResult::Type UBTTask_GetNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (!AIController) return EBTNodeResult::Failed;

    AEnemyAICharacter* Enemy = Cast<AEnemyAICharacter>(AIController->GetPawn());
    if (!Enemy || Enemy->PatrolPoints.Num() == 0) return EBTNodeResult::Failed;

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;

    int32 CurrentIndex = Blackboard->GetValueAsInt("PatrolIndex");
    Blackboard->SetValueAsObject("PatrolPoint", Enemy->PatrolPoints[CurrentIndex]);

    int32 NextIndex = (CurrentIndex + 1) % Enemy->PatrolPoints.Num();
    Blackboard->SetValueAsInt("PatrolIndex", NextIndex);

    return EBTNodeResult::Succeeded;
}
