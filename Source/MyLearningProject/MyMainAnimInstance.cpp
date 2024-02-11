// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyMainCharacter.h"

void UMyMainAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Main = Cast<AMyMainCharacter>(Pawn);
		}
	}
}

void UMyMainAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();

	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		if (Main == nullptr)
		{
			Main = Cast<AMyMainCharacter>(Pawn);
		}
		
		bIsCrouching = Main->bCrouching;
		bIsSprinting = Main->bSprinting;
		bAnimHasDoubleJumped = Main->bHasDoubleJumped;
	}
}