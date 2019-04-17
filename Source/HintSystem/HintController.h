// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HintController.generated.h"

UCLASS()
class HINTSYSTEM_API AHintController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHintController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Advance the sliding puzzle timer counter
	virtual void SlidingPuzzleTimer();

	//Handle the timers
	FTimerHandle slidingPuzzleLengthTimer;

private:
	//A class to store the human player
	class APawn* Player;

	//The player's location
	FVector playerLoc;

	//The player's rotation
	FRotator playerRot;

	//The sliding puzzle's location
	FVector slidingPuzzleLoc;

	//The distance to the sliding puzzle from the player
	FVector slidingPuzzleLocVector;

	//Separated X and Y values from the slidingPuzzleLocVector
	int slidingPuzzleX;
	int slidingPuzzleY;

	//The direct line length between the player and sliding puzzle after Pythagoras
	int slidingPuzzleDistance;

	//The Yaw value of the player's rotation
	int playerRotYaw;

	//Values for calculating trigonometry
	double trigValue;
	double radiansToDegrees;

	//Theta for calculating trigonometry
	double theta;

	//Trigonometry result
	double dResult;

	//dResult to float
	int fResult;

	//The rotation away from the sliding puzzle
	int slidingPuzzleRotation;

	//Whether the player is in range of sliding puzzle
	int slidingPuzzleRange;	
};
