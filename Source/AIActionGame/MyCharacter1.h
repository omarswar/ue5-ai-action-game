#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DamageEvents.h" 
#include "GameFramework/DamageType.h"
#include "EnemyAICharacter.h"
#include "PauseMenuWidget.h"
#include "MySaveGame.h"


#include "MyCharacter1.generated.h" 


UCLASS()

class AIACTIONGAME_API AMyCharacter1 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY()
	UPauseMenuWidget* PauseMenuInstance;
	
	FTimerHandle FrenzyTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackMultiplier = 1.0f;
public:
	// Called every frame


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void TogglePauseMenu();


	UPROPERTY(BlueprintReadWrite)
	bool bIsInvulnerable = false;



	UFUNCTION(BlueprintCallable)
	void SaveGameData();

	UFUNCTION(BlueprintCallable)
	void LoadGameData();

	UFUNCTION()
	void PerformAttack();

	void ActivateFrenzy();
	void DeactivateFrenzy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Health = 100.0f;





	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UPauseMenuWidget> PauseMenuClass;

	// Health Variables
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	// Health Functions
	UFUNCTION(BlueprintCallable, Category = "Health")
	void IncreaseHealth(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakePlayerDamage(float DamageAmount);



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bIsInAir; // Variable to track if character is in the air

	UFUNCTION(BlueprintPure, Category = "Animation")
	bool GetIsInAir() const; // Getter function for Blueprint

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HealthWidgetClass;



	/** Camera boom positioning the camera behind the Character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	/** First-Person Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

	/** Boolean to track first-person mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bIsFirstPerson;

	/** Function to switch between first-person and third-person */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SwitchCamera();


	void MoveForward(float Value);
	void MoveRight(float Value);

	/** Sprinting and Slow-motion functions */
	void StartSprint();
	void StopSprint();
	void StartSlowmo();
	void StopSlowmo();
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void StartJump();  // Function to start jumping
	void StopJump();   // Function to stop jumping
	/** Interaction */


	/** Speed Variables */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float NormalSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SlowSpeed = 50.f;

};
