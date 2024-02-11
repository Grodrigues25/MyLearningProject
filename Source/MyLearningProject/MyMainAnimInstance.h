// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyMainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYLEARNINGPROJECT_API UMyMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
	float MovementSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
	bool bIsCrouching;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
	bool bIsSprinting;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	bool bAnimHasDoubleJumped;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class AMyMainCharacter* Main;
};
