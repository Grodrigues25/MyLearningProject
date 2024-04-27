// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyMainCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Walking UMETA(DisplayName = "Walking"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Crouching UMETA(DisplayName = "Crouching"),
	EMS_Dodging UMETA(DisplayName = "Dodging"),

	EMS_Max UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EStaminaStatus :uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinumum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),

	ESS_MAX UMETA(DisplayName = "DefaultMax")
};

// EnhancedInput
class UInputMappingContext;
class UInputAction;

UCLASS()
class MYLEARNINGPROJECT_API AMyMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyMainCharacter();

	//ENUMS for character states and statuses
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom Property -> Movement")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom Property -> Movement")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	// Set Movement and running speed
	void SetMovementStatus(EMovementStatus Status);

	// Variables related to character stats and metrics
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats | Movement")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats | Movement")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats | Movement")
	float SprintingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats | Movement")
	float WalkingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats | Movement")
	float CrouchingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats | Movement")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats | Movement")
	float MinSprintStamina;

	//Variables for collectibles
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats | Coins")
	int32 CoinTotal;

	// Character related bools used for gameplay mechanics
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom Property -> Movement")
	bool bCrouching;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom Property -> Movement")
	bool bSprintKeyDown;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom Property -> Movement")
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom Property -> Movement")
	bool bHasDoubleJumped;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom Property -> Movement")
	bool bPressedDodge;

	bool bAttacking;

	// Camera settings
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Camera Boom positioning the camera behind the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Property -> Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Follow Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Property -> Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// Base turn rates to scale turning functions for the Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Property -> Camera")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Property -> Camera")
	float BaseLookUpRate;


	// Functions
	void Move(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

	void Crouch();

	void SprintKeyDown();

	void SprintKeyUp();

	void DoubleJump();

	void DodgeKeyDown();

	void DodgeKeyUp();

	void DecrementHealth(float Amount);

	void Die();

	void IncrementCoins();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Input Mappings and Actions for input configuration
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Property -> Enhanced Input")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Property -> Enhanced Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Property -> Enhanced Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Property -> Enhanced Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Property -> Enhanced Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Property -> Enhanced Input")
	UInputAction* DodgeAction;
};
