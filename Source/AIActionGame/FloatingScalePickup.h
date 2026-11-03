#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingScalePickup.generated.h"

UCLASS()
class AIACTIONGAME_API AFloatingScalePickup : public AActor
{
    GENERATED_BODY()

public:
    AFloatingScalePickup();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere)
    float FloatAmplitude = 20.0f;

    UPROPERTY(EditAnywhere)
    float FloatSpeed = 2.0f;

    FVector InitialLocation;

    bool bHasBeenPickedUp;

    UFUNCTION()
    void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
