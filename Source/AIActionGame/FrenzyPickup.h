#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FrenzyPickup.generated.h"

UCLASS()
class AIACTIONGAME_API AFrenzyPickup : public AActor
{
    GENERATED_BODY()

public:
    AFrenzyPickup();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    class UStaticMeshComponent* PickupMesh;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float RotationSpeed = 180.f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float Lifespan = 15.f;

public:
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
