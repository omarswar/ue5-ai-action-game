#include "RotatingSpeedPickup.h"
#include "Components/StaticMeshComponent.h"
#include "MyCharacter1.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARotatingSpeedPickup::ARotatingSpeedPickup()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    Mesh->SetGenerateOverlapEvents(true);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

    OnActorBeginOverlap.AddDynamic(this, &ARotatingSpeedPickup::OnOverlap);

    InitialLifeSpan = Lifespan;
    bHasBeenPickedUp = false;
}

void ARotatingSpeedPickup::BeginPlay()
{
    Super::BeginPlay();
}

void ARotatingSpeedPickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
}

void ARotatingSpeedPickup::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (bHasBeenPickedUp) return;

    if (AMyCharacter1* Player = Cast<AMyCharacter1>(OtherActor))
    {
        bHasBeenPickedUp = true;

        float CurrentSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;
        Player->GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed * SpeedMultiplier;

        UE_LOG(LogTemp, Warning, TEXT("Speed pickup used by: %s | New Speed: %f"), *Player->GetName(), Player->GetCharacterMovement()->MaxWalkSpeed);

        Destroy();
    }
}
