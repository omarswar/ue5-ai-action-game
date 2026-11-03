#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APickupBase.generated.h"

UCLASS()
class AIACTIONGAME_API AAPickupBase : public AActor
{
	GENERATED_BODY()

public:
	AAPickupBase();

protected:
	UPROPERTY(EditAnywhere, Category = "Pickup")
	class USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	class UStaticMeshComponent* Mesh;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
