#include "FrenzyPickup.h"
#include "Components/StaticMeshComponent.h"
#include "MyCharacter1.h"

AFrenzyPickup::AFrenzyPickup()
{
    PrimaryActorTick.bCanEverTick = true;

    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
    RootComponent = PickupMesh;

    PickupMesh->SetGenerateOverlapEvents(true);
    PickupMesh->SetCollisionProfileName("OverlapAll");
    PickupMesh->OnComponentBeginOverlap.AddDynamic(this, &AFrenzyPickup::OnOverlapBegin);

    InitialLifeSpan = Lifespan;
}

void AFrenzyPickup::BeginPlay()
{
    Super::BeginPlay();
}

void AFrenzyPickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FRotator Rotation = FRotator(0, RotationSpeed * DeltaTime, 0);
    AddActorLocalRotation(Rotation);
}

void AFrenzyPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AMyCharacter1* Player = Cast<AMyCharacter1>(OtherActor);
    if (Player)
    {
        Player->ActivateFrenzy();
        UE_LOG(LogTemp, Warning, TEXT("Frenzy activated! Player will do 1.5x damage for 30 seconds."));
        Destroy();
    }
}
