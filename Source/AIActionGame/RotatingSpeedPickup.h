#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingSpeedPickup.generated.h"

UCLASS()
class AIACTIONGAME_API ARotatingSpeedPickup : public AActor
{
    GENERATED_BODY()

public:
    ARotatingSpeedPickup();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere)
    float RotationSpeed = 90.f;

    UPROPERTY(EditAnywhere)
    float SpeedMultiplier = 1.2f;

    UPROPERTY(EditAnywhere)
    float Lifespan = 300.f;

    UFUNCTION()
    void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);

    bool bHasBeenPickedUp;

};
