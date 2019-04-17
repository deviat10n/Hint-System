// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "SlidingPuzzle5.generated.h"

UCLASS()
class HINTSYSTEM_API ASlidingPuzzle5 : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlidingPuzzle5();

	// Direction vector
	UPROPERTY(EditAnywhere)
		FVector movementDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		USceneComponent* OurVisibleComponent;

	//Movement controls
	void UpReleased();
	void DownReleased();
	void LeftReleased();
	void RightReleased();

	//The boxes location
	FVector Location;

	//Blocks to spawn
	ASlidingPuzzle5* puzzle5Block1;
	ASlidingPuzzle5* puzzle5Block2;
	ASlidingPuzzle5* puzzle5Block3;
	ASlidingPuzzle5* puzzle5Block4;
	ASlidingPuzzle5* puzzle5Block5;
	ASlidingPuzzle5* puzzle5Block6;
	ASlidingPuzzle5* puzzle5Block7;
	ASlidingPuzzle5* puzzle5Block8;

	//The movement direction
	float moveDirection;

	//The Player's controller
	APlayerController* PlayerController;

	UTextRenderComponent* TimerText;

	void UpdateTimerDisplay();

	void AdvanceTimer();

	UFUNCTION(BlueprintNativeEvent)
		void TimerHasFinished();

	virtual void TimerHasFinished_Implementation();

	FTimerHandle TimerTimerHandle;

private:
	// A static mesh component. The visual representation of our actor
	class UStaticMeshComponent* BoxVisual;

	// A class to store the human player
	class APawn* Player;

	//The box slots
	FVector puzzle5Slot1;
	FVector puzzle5Slot2;
	FVector puzzle5Slot3;
	FVector puzzle5Slot4;
	FVector puzzle5Slot5;
	FVector puzzle5Slot6;
	FVector puzzle5Slot7;
	FVector puzzle5Slot8;
	FVector puzzle5Slot9;

	// The actors location
	FVector puzzle5ActorLoc;

	//The ID for each block
	int puzzle5ID;

	//The player's location
	FVector playerLoc;
};