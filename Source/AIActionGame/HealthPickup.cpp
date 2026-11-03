#include "HealthPickup.h"
#include "Components/StaticMeshComponent.h"
#include "MyCharacter1.h"
#include "Kismet/GameplayStatics.h"

AHealthPickup::AHealthPickup()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    // Setup collision for overlap
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Mesh->SetCollisionObjectType(ECC_WorldDynamic);
    Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Mesh->SetGenerateOverlapEvents(true);

    OnActorBeginOverlap.AddDynamic(this, &AHealthPickup::OnOverlap);

    bHasBeenPickedUp = false;
    SetLifeSpan(120.0f); // 2 minutes
}

void AHealthPickup::BeginPlay()
{
    Super::BeginPlay();
}

void AHealthPickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AHealthPickup::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (bHasBeenPickedUp || !OtherActor) return;

    if (AMyCharacter1* Player = Cast<AMyCharacter1>(OtherActor))
    {
        if (Player->Health < Player->MaxHealth)
        {
            bHasBeenPickedUp = true;
            Player->IncreaseHealth(HealAmount);

            UE_LOG(LogTemp, Warning, TEXT("Health pickup touched by: %s"), *OtherActor->GetName());
            UE_LOG(LogTemp, Warning, TEXT("Player health after pickup: %f"), Player->Health);

            Destroy();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Health already full. Pickup ignored."));
        }
    }
}
