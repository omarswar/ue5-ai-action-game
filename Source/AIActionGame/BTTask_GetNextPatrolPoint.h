#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetNextPatrolPoint.generated.h"

UCLASS()
class AIACTIONGAME_API UBTTask_GetNextPatrolPoint : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_GetNextPatrolPoint();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    // The key for the selected patrol point
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector PatrolPointKey;

    // The key for the current index in the patrol array
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector PatrolIndexKey;
};
