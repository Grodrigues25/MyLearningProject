// Fill out your copyright notice in the Description page of Project Settings.

#include "MyMainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// TODO: Understand why the crouching movement speed sums up when going diagonally and correct it

// Sets default values
AMyMainCharacter::AMyMainCharacter()
{
	// Bools related to Character Movement
	bShiftKeyDown = false;
	bAttacking = false;
	bCrouching = false;
	bHasDoubleJumped = false;
	bIsInAir = false;
	bPressedDodge = false;

	// Enums
	MovementStatus = EMovementStatus::EMS_Walking;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	/// Character Stats Variables
	// MovementSpeeds
	SprintingSpeed = 950.f;
	WalkingSpeed = 400.f;
	CrouchingSpeed = 200.f;

	// Health and Stamina
	MaxHealth = 100.f;
	Health = 80.f;
	MaxStamina = 200.f;
	Stamina = 100.f;
	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Camera Boom (pulls towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.f; // Camera follows at a distance
	CameraBoom->bUsePawnControlRotation = true; // Rotate arm based on controller

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Attach the Camera to the end of the boom and let the boom adjust to match 
	// the controller orientation
	FollowCamera->bUsePawnControlRotation = false;

	// Set our turn rates for input
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	// Don't rotate the character when the controller rotates. Just affects the camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	// Set size for colision capsule
	GetCapsuleComponent()->SetCapsuleSize(30.f, 90.f);

}

// Called when the game starts or when spawned
void AMyMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) 
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) 
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
	
}

// Called every frame
void AMyMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaStamina = StaminaDrainRate * DeltaTime;

	// Switch for the different behaviours and appearances of the stamina bar when Sprinting
	if (MovementStatus != EMovementStatus::EMS_Crouching)
	{
		switch (StaminaStatus)
		{
		case EStaminaStatus::ESS_Normal:
			if (bShiftKeyDown)
			{
				if (Stamina - DeltaStamina <= MinSprintStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
					Stamina -= DeltaStamina;
				}
				else
				{
					Stamina -= DeltaStamina;
				}
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
			else
			{
				if (Stamina + DeltaStamina >= MaxStamina)
				{
					Stamina = MaxStamina;
				}
				else
				{
					Stamina += DeltaStamina;
				}
				SetMovementStatus(EMovementStatus::EMS_Walking);
			}
			break;

		case EStaminaStatus::ESS_BelowMinimum:
			if (bShiftKeyDown)
			{
				if (Stamina - DeltaStamina <= 0.f)
				{
					SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
					Stamina = 0.f;
					SetMovementStatus(EMovementStatus::EMS_Walking);
				}
				else
				{
					Stamina -= DeltaStamina;
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
			}
			else
			{
				if (Stamina + DeltaStamina >= MinSprintStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_Normal);
					Stamina += DeltaStamina;
				}
				else
				{
					Stamina += DeltaStamina;
				}
				SetMovementStatus(EMovementStatus::EMS_Walking);
			}
			break;

		case EStaminaStatus::ESS_Exhausted:
			if (bShiftKeyDown)
			{
				Stamina = 0.f;
			}
			else
			{
				SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Walking);
			break;

		case EStaminaStatus::ESS_ExhaustedRecovering:
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Walking);
			break;

		default:
			;
		}
	}
}

// Called to bind functionality to input
void AMyMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AMyMainCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyMainCharacter::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyMainCharacter::DoubleJump);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMyMainCharacter::Crouch);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMyMainCharacter::ShiftKeyDown);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &AMyMainCharacter::ShiftKeyUp);

		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AMyMainCharacter::DodgeKeyDown);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Canceled, this, &AMyMainCharacter::DodgeKeyUp);
	}

}

// Called for forward/backward input
void AMyMainCharacter::Move(const FInputActionValue& value)
{
	// Get movement vector from the input and then normalize it to make its magnitude 1 in all directions
	// This avoids faster movement diagonally
	const FVector2D MovementVector = value.Get<FVector2D>();
	const FVector2D NormalizedMovementVector = MovementVector / MovementVector.Length();

	if (Controller != nullptr)
	{
		// Find out which way is forward
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, rotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, NormalizedMovementVector.Y);
		AddMovementInput(RightDirection, NormalizedMovementVector.X);
		
	}
}

void AMyMainCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisVector = value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void AMyMainCharacter::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
		UE_LOG(LogTemp, Warning, TEXT("MovementStatus -> %s"), *UEnum::GetValueAsString(MovementStatus))
		//GetCapsuleComponent()->SetCapsuleSize(30.f, 90.f);
		//FVector MeshLocation(0.f, 5.f, -90.5f);
		//GetMesh()->SetRelativeLocation(MeshLocation);
	}
	else if (MovementStatus == EMovementStatus::EMS_Crouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchingSpeed;
		UE_LOG(LogTemp, Warning, TEXT("MovementStatus -> %s"), *UEnum::GetValueAsString(MovementStatus))
		//GetCapsuleComponent()->SetCapsuleSize(40.f, 70.f);
		//FVector MeshLocation(0.f, 5.f, -72.5f);
		//GetMesh()->SetRelativeLocation(MeshLocation);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
		UE_LOG(LogTemp, Warning, TEXT("MovementStatus -> %s"), *UEnum::GetValueAsString(MovementStatus))
		//GetCapsuleComponent()->SetCapsuleSize(30.f, 90.f);
		//FVector MeshLocation(0.f, 5.f, -90.5f);
		//GetMesh()->SetRelativeLocation(MeshLocation);
	}
}

void AMyMainCharacter::Crouch()
{
	bCrouching = !bCrouching;
	if (bCrouching)
	{
		SetMovementStatus(EMovementStatus::EMS_Crouching);
	}
	else
	{
		SetMovementStatus(EMovementStatus::EMS_Walking);
	}
		

}

void AMyMainCharacter::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMyMainCharacter::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMyMainCharacter::DoubleJump()
{
	if (!(GetMovementComponent()->IsFalling()))
	{
		ACharacter::Jump();
		bHasDoubleJumped = false;
	}
	else if(GetMovementComponent()->IsFalling() && !bHasDoubleJumped)
	{
		ACharacter::Jump();
		bHasDoubleJumped = true;
	}
}

void AMyMainCharacter::DodgeKeyDown()
{
	bPressedDodge = true;
	//if (!bPressedDodge)
	//{
	//	bPressedDodge = true;
	//}
	//else
	//{
	//	bPressedDodge = false;
	//}
}

void AMyMainCharacter::DodgeKeyUp()
{
	bPressedDodge = false;
}

void AMyMainCharacter::DecrementHealth(float Amount)
{
	if (Health - Amount <= 0.f)
	{
		Die();
	}
	else
	{
		Health -= Amount;
	}
	
}

void AMyMainCharacter::Die()
{

}
