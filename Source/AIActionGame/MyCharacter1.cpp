#include "MyCharacter1.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DamageEvents.h" 
#include "Components/CapsuleComponent.h"

#include "MySaveGame.h"
#include "PauseMenuWidget.h"
#include "EnemyAICharacter.h"
class UPauseMenuWidget;

AMyCharacter1::AMyCharacter1()
{
    PrimaryActorTick.bCanEverTick = true;
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    // Setup Follow Camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Setup First-Person Camera
    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(RootComponent);
    FirstPersonCamera->bUsePawnControlRotation = true;
    FirstPersonCamera->SetRelativeLocation(FVector(0.f, 0.f, 60.f)); // Adjust height
    FirstPersonCamera->SetAutoActivate(false); // Start with third-person active

    // Character Movement Settings
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    bIsFirstPerson = false;

    bUseControllerRotationYaw = true;
    CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
    CharacterMesh->SetupAttachment(RootComponent);
    CharacterMesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f)); // Adjust height
    CharacterMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
    CharacterMesh->SetOwnerNoSee(true); // Hide in first-person
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetCapsuleComponent()->SetGenerateOverlapEvents(true);



}
void AMyCharacter1::BeginPlay()
{
  
    Super::BeginPlay();
    if (GetMesh())
    {
        GetMesh()->SetVisibility(true);
        GetMesh()->SetHiddenInGame(false);
    }

    if (HealthWidgetClass)
    {
        UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), HealthWidgetClass);
        if (Widget)
        {
            Widget->AddToViewport();
        }
    }

}

void AMyCharacter1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update speed
    Speed = GetVelocity().Size();

    // Calculate movement direction
    FVector Velocity = GetVelocity();
    FVector ForwardVector = GetActorForwardVector();
    FVector RightVector = GetActorRightVector();

    float ForwardSpeed = FVector::DotProduct(Velocity, ForwardVector);
    float RightSpeed = FVector::DotProduct(Velocity, RightVector);
    Direction = FMath::Atan2(RightSpeed, ForwardSpeed) * (180.f / PI);

    bIsInAir = GetCharacterMovement()->IsFalling(); // Set bIsInAir based on character movement

    UE_LOG(LogTemp, Warning, TEXT("Speed: %f, Direction: %f, IsInAir: %s"), Speed, Direction, bIsInAir ? TEXT("True") : TEXT("False"));
}

void AMyCharacter1::MoveForward(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        // get forward vector
        FVector Direction1 = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction1, Value);
        // Correct for backward movement (S key)
        if (Value < 0.0f) {
            Direction = -Direction; // Negate the direction for backward movement
        }

        // Use 1.0f as magnitude since Direction handles sign

         // Rotate character to face the movement direction
        FRotator NewRotation = Direction1.Rotation();
        NewRotation.Pitch = 0;
        NewRotation.Roll = 0;

        FRotator CurrentRotation = GetActorRotation();
        FRotator InterpRotation = FMath::RInterpTo(CurrentRotation, NewRotation, GetWorld()->GetDeltaSeconds(), 10.0f); // Adjust 10.0f for speed

        SetActorRotation(InterpRotation);
    }
}
void AMyCharacter1::MoveRight(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        // get right vector
        const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // Correct for left movement (A key)
        FVector Direction1 = RightVector * Value;

        // Use the modified Direction vector for movement
        AddMovementInput(Direction1, 1.0f); // Use 1.0f as magnitude since Direction handles sign

        // Rotate character to face the movement direction
        FRotator NewRotation = Direction1.Rotation();
        NewRotation.Pitch = 0;
        NewRotation.Roll = 0;

        FRotator CurrentRotation = GetActorRotation();
        FRotator InterpRotation = FMath::RInterpTo(CurrentRotation, NewRotation, GetWorld()->GetDeltaSeconds(), 10.0f); // Adjust 10.0f for speed

        SetActorRotation(InterpRotation);
    }
}

void AMyCharacter1::SwitchCamera()
{
    if (bIsFirstPerson)
    {
        // Switch to Third-Person
        FirstPersonCamera->Deactivate();
        FollowCamera->Activate();
        CameraBoom->SetRelativeRotation(FRotator(0.f, 0.f, 0.f)); // Reset camera rotation
    }
    else
    {
        // Switch to First-Person
        FollowCamera->Deactivate();
        FirstPersonCamera->Activate();
    }

    // Toggle state
    bIsFirstPerson = !bIsFirstPerson;
}


void AMyCharacter1::StartJump()
{
    UE_LOG(LogTemp, Warning, TEXT("Jump Pressed"));
    ACharacter::Jump();
    UE_LOG(LogTemp, Warning, TEXT("After Jump: IsInAir: %s"), GetCharacterMovement()->IsFalling() ? TEXT("True") : TEXT("False"));
}

void AMyCharacter1::StopJump()
{
    UE_LOG(LogTemp, Warning, TEXT("Jump Released"));
    ACharacter::StopJumping();
}

bool AMyCharacter1::GetIsInAir() const
{
    return GetCharacterMovement()->IsFalling(); // Checks if the character is in the air
}
void AMyCharacter1::TogglePauseMenu()
{
    if (PauseMenuClass && !UGameplayStatics::IsGamePaused(GetWorld()))
    {
        if (!IsValid(PauseMenuInstance))
        {
            PauseMenuInstance = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuClass);
        }

        if (PauseMenuInstance && !PauseMenuInstance->IsInViewport())
        {
            PauseMenuInstance->AddToViewport();
        }

        UGameplayStatics::SetGamePaused(GetWorld(), true);
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        PC->SetInputMode(FInputModeUIOnly());
        PC->bShowMouseCursor = true;

    }
    UE_LOG(LogTemp, Warning, TEXT("TAB PRESSED: Pause triggered"));



}





void AMyCharacter1::SaveGameData()
{
    UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
    if (SaveGameInstance)
    {
        SaveGameInstance->SavedLocation = GetActorLocation();
        SaveGameInstance->SavedHealth = Health;
        SaveGameInstance->bSavedFirstPerson = bIsFirstPerson;

        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("PlayerSaveSlot"), 0);
        UE_LOG(LogTemp, Warning, TEXT("Game Saved"));
    }
}


void AMyCharacter1::LoadGameData()
{
    if (UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveSlot"), 0))
    {
        UMySaveGame* LoadedGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));
        if (LoadedGame)
        {
            SetActorLocation(LoadedGame->SavedLocation);
            Health = LoadedGame->SavedHealth;
            bIsFirstPerson = LoadedGame->bSavedFirstPerson;

            if (bIsFirstPerson)
            {
                FollowCamera->Deactivate();
                FirstPersonCamera->Activate();
            }
            else
            {
                FirstPersonCamera->Deactivate();
                FollowCamera->Activate();
            }

            UE_LOG(LogTemp, Warning, TEXT("Game Loaded"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Save Game Found"));
    }
}

void AMyCharacter1::PerformAttack()
{
    FVector Start = GetActorLocation();
    FVector End = Start + GetActorForwardVector() * 200.0f;

    // Draw a debug line to visualize the attack range
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);  // Ignore self

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params))
    {
        UE_LOG(LogTemp, Warning, TEXT("LineTrace hit actor: %s"), *Hit.GetActor()->GetName());

        AEnemyAICharacter* Enemy = Cast<AEnemyAICharacter>(Hit.GetActor());
        if (Enemy)
        {
            float FinalDamage = 50.0f * AttackMultiplier;
            UE_LOG(LogTemp, Warning, TEXT("Applying damage to AI: %f"), FinalDamage);

            Enemy->TakeDamage(FinalDamage, FDamageEvent(), GetController(), this);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Hit actor is not an EnemyAICharacter."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Attack missed: no actor hit."));
    }
}


void AMyCharacter1::ActivateFrenzy()
{
    AttackMultiplier = 1.5f;
    UE_LOG(LogTemp, Warning, TEXT("Frenzy activated!"));

    // Set timer to end frenzy after 30 seconds
    GetWorldTimerManager().SetTimer(FrenzyTimerHandle, this, &AMyCharacter1::DeactivateFrenzy, 30.0f, false);
}

void AMyCharacter1::DeactivateFrenzy()
{
    AttackMultiplier = 1.0f;
    UE_LOG(LogTemp, Warning, TEXT("Frenzy ended."));
}






void AMyCharacter1::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AMyCharacter1::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void AMyCharacter1::StartSlowmo()
{
    GetCharacterMovement()->MaxWalkSpeed = SlowSpeed;
}

void AMyCharacter1::StopSlowmo()
{
    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void AMyCharacter1::TurnAtRate(float Rate)
{
    AddControllerYawInput(Rate);
}

void AMyCharacter1::LookUpAtRate(float Rate)
{
    AddControllerPitchInput(Rate);
}

void AMyCharacter1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter1::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter1::MoveRight);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter1::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter1::StopSprint);
    PlayerInputComponent->BindAction("SlowMo", IE_Pressed, this, &AMyCharacter1::StartSlowmo);
    PlayerInputComponent->BindAction("SlowMo", IE_Released, this, &AMyCharacter1::StopSlowmo);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter1::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter1::StopJump);
    PlayerInputComponent->BindAxis("Turn", this, &AMyCharacter1::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter1::LookUpAtRate);
    PlayerInputComponent->BindAction("SwitchCam", IE_Pressed, this, &AMyCharacter1::SwitchCamera);
    PlayerInputComponent->BindAction("PauseGame", IE_Pressed, this, &AMyCharacter1::TogglePauseMenu);
    PlayerInputComponent->BindAction("SaveGame", IE_Pressed, this, &AMyCharacter1::SaveGameData);
    PlayerInputComponent->BindAction("LoadGame", IE_Pressed, this, &AMyCharacter1::LoadGameData);
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyCharacter1::PerformAttack);




}
void AMyCharacter1::IncreaseHealth(float Amount)
{
    float OldHealth = Health;
    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
    UE_LOG(LogTemp, Warning, TEXT("Health increased from: %f to: %f"), OldHealth, Health);
}



void AMyCharacter1::TakePlayerDamage(float DamageAmount)
{
    if (!bIsInvulnerable)
    {
        Health -= DamageAmount;
        if (Health <= 0.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player died!"));
            // Handle death
        }
    }
}


  