#include "APickupBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AAPickupBase::AAPickupBase()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    RootComponent = CollisionComp;
    CollisionComp->SetSphereRadius(50.f);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
}

void AAPickupBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
    // To be implemented in child classes
}
