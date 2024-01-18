// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyMainCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class MYLEARNINGPROJECT_API AMyMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyMainCharacter();

	//virtual void Jump() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

public:	

	bool bAttacking;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom Property -> Movement")
	float CharacterMovementSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom Property -> Movement")
	bool bCrouching;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Custom Property -> Movement")
	bool bSprinting;

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

	void Move(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

	void Crouch();

	void Sprint();
};
