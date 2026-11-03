#include "FloatingScalePickup.h"
#include "GameFramework/Actor.h"
#include "MyCharacter1.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AFloatingScalePickup::AFloatingScalePickup()
{
    PrimaryActorTick.bCanEverTick = true;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    Mesh->SetGenerateOverlapEvents(true);
    OnActorBeginOverlap.AddDynamic(this, &AFloatingScalePickup::OnOverlap);

    InitialLifeSpan = 300.0f; // lifespan
    bHasBeenPickedUp = false;

}

void AFloatingScalePickup::BeginPlay()
{
    Super::BeginPlay();
    InitialLocation = GetActorLocation();
}

void AFloatingScalePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FVector NewLocation = InitialLocation;
    NewLocation.Z += FMath::Sin(GetWorld()->TimeSeconds * FloatSpeed) * FloatAmplitude;
    SetActorLocation(NewLocation);
}

void AFloatingScalePickup::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (bHasBeenPickedUp) return;

    if (AMyCharacter1* Player = Cast<AMyCharacter1>(OtherActor))
    {
        bHasBeenPickedUp = true;

        FVector Scale = Player->GetActorScale3D();
        Player->SetActorScale3D(Scale * 1.2f); // Increase size
        Destroy();
    }
}
