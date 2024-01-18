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
	bAttacking = false;
	bCrouching = false;

	// Character Movement Speeds
	CharacterMovementSpeed = 1100.f;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Camera Boom (pulls towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; // Camera follows at a distance
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

	// Set size for colision capsule
	GetCapsuleComponent()->SetCapsuleSize(30.f, 95.f);

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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMyMainCharacter::Crouch);
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

		if (bCrouching) {
			AddMovementInput(ForwardDirection, NormalizedMovementVector.Y/2);
			AddMovementInput(RightDirection, NormalizedMovementVector.X/2);
		}
		else 
		{
			AddMovementInput(ForwardDirection, NormalizedMovementVector.Y);
			AddMovementInput(RightDirection, NormalizedMovementVector.X);
		}
		
	}
}

void AMyMainCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisVector = value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void AMyMainCharacter::Crouch()
{
	if (!bCrouching) {
		
		bCrouching = true;

	}
	else
	{
		bCrouching = false;

	}
	
}


