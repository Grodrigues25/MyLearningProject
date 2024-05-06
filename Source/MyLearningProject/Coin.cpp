// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "MyMainCharacter.h"

ACoin::ACoin()
{
	bRotate = true;
}

void ACoin::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("Coin: On Overlap Begin"));

	if (OtherActor)
	{
		AMyMainCharacter* Main = Cast<AMyMainCharacter>(OtherActor);

		if (Main)
		{
			Main->IncrementCoins();

			Destroy();
		}
	}
}

void ACoin::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	UE_LOG(LogTemp, Warning, TEXT("Coin: On Overlap End"));

}