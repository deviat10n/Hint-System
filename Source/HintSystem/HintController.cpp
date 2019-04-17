// Fill out your copyright notice in the Description page of Project Settings.

#include "HintController.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine.h"
#include "EngineGlobals.h"
#include "iostream"
#include "SlidingPuzzle.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

//what step the player is on
extern int puzzle1PathStep;

//Prevent negative hint progress
extern int puzzle1BeenCheck;

//What progress the player is on
extern float puzzle1Progress;

//Whether the puzzle is complete or not
extern bool puzzle1Complete;

//The starting point for the puzzle
int puzzle1StartingPoint = 1;

//What stage the player is on
int puzzle1Stage = 11;

//Check if the puzzle has been started
bool puzzle1StartCheck = false;

//Don't allow hints to be reversed
bool puzzle1BeenOne = false;
bool puzzle1BeenTwo = false;

//The total completion time for the puzzle
int puzzle1TotalTimer = 0;

//The currently available hint
int puzzle1HintID = 0;

//Ensure the player doesn't exceed the total number of hints
bool puzzle1TempFinalHint = false;

//Ensure the puzzle only runs once
bool puzzle1RunOnce = false;

//Whether the player is in range of the puzzle
int puzzle1SlidingPuzzleRange = 0;

//The timer tracking the playtime of the puzzle
int puzzle1TimerCounter = 0;

// Sets default values
AHintController::AHintController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set the puzzle's locations
	slidingPuzzleLoc = FVector(-2010.0f, 680.0f, 310.0f);

	//Default trigonometry values
	trigValue = 6.2831853071795865;
	radiansToDegrees = 57.2957795130823209;

	//Default global values
	::puzzle1StartingPoint = 1;
	::puzzle1Stage = 11;
	::puzzle1StartCheck = false;
	::puzzle1BeenOne = false;
	::puzzle1BeenTwo = false;
	::puzzle1Complete = false;
	::puzzle1TotalTimer = 0;
	::puzzle1HintID = 0;
	::puzzle1TempFinalHint = false;
	::puzzle1RunOnce = false;
	::puzzle1SlidingPuzzleRange = 0;
	::puzzle1TimerCounter = 0;
}

// Called when the game starts or when spawned
void AHintController::BeginPlay()
{
	Super::BeginPlay();

	//Begin a timer for total length of puzzle.
	GetWorldTimerManager().SetTimer(slidingPuzzleLengthTimer, this, &AHintController::SlidingPuzzleTimer, 1.0f, true);
}

// Advance the sliding puzzle timer counter
void AHintController::SlidingPuzzleTimer()
{
	//Advance the timer if in range of the sliding puzzle
	if (::puzzle1SlidingPuzzleRange == 1) {
		::puzzle1TimerCounter++;
	}
}

// Called every frame
void AHintController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (::puzzle1BeenCheck == 1) ::puzzle1BeenOne = true;
	if (::puzzle1BeenCheck == 2) ::puzzle1BeenTwo = true;

	//Reset control variable
	::puzzle1RunOnce = false;

	//Get the player's location
	playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	//Get the player's rotation
	playerRot = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation();

	//Get the Yaw value from the FRotator
	playerRotYaw = playerRot.Yaw + 180;

	//Calculate angle from the puzzle to the player
	theta = atan2(playerLoc.X - slidingPuzzleLoc.X, slidingPuzzleLoc.Y - playerLoc.Y);
	if (theta < 0.0)
		theta += trigValue;
	dResult = radiansToDegrees * theta;
	fResult = (float)dResult;

	slidingPuzzleRotation = (fResult - playerRotYaw) - 90;

	if (slidingPuzzleRotation < -180) {
		slidingPuzzleRotation = 180 - (abs(slidingPuzzleRotation) - 180);
	}

	//Calculate distance to the sliding puzzle
	slidingPuzzleLocVector = slidingPuzzleLoc - playerLoc;

	//Separate the X and Y values
	slidingPuzzleX = slidingPuzzleLocVector.X;
	slidingPuzzleY = slidingPuzzleLocVector.Y;

	//calculate direct distance to the sliding puzzle
	slidingPuzzleDistance = sqrt((slidingPuzzleX*slidingPuzzleX) + (slidingPuzzleY*slidingPuzzleY));

	//Check if the player is in range of the sliding puzzle
	if ((slidingPuzzleDistance < 1000) && (slidingPuzzleRotation > -30) && (slidingPuzzleRotation < 30)) {
		::puzzle1SlidingPuzzleRange = 1;
	}
	else {
		::puzzle1SlidingPuzzleRange = 0;
	}

	//Show whether the player is in range of the sliding puzzle
	if (::puzzle1SlidingPuzzleRange == 0) {
		GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("In range of sliding puzzle: FALSE")));
	}
	else {
		GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("In range of sliding puzzle: TRUE")));
	}

	//Determine starting positions
	if (::puzzle1StartCheck == false) {
		if (::puzzle1StartingPoint == 1) ::puzzle1Stage = 11;
		if (::puzzle1StartingPoint == 2) ::puzzle1Stage = 21;
		if (::puzzle1StartingPoint == 3) ::puzzle1Stage = 31;
		::puzzle1StartCheck = true;
	}

	//Move the player through stages
	if (::puzzle1Stage == 11 && ::puzzle1TimerCounter > 60 && ::puzzle1Progress < 20.0f) {
		::puzzle1Stage = 21;
	}
	if (::puzzle1Stage == 11 && ::puzzle1TimerCounter <= 60 && ::puzzle1Progress > 20.0f) {
		::puzzle1Stage = 12;
		::puzzle1BeenOne = true;
		::puzzle1TotalTimer = ::puzzle1TotalTimer + ::puzzle1TimerCounter;
		::puzzle1TimerCounter = 0;
	}
	if (::puzzle1Stage == 21 && ::puzzle1TimerCounter > 90 && ::puzzle1Progress < 20.0f) {
		::puzzle1Stage = 31;
	}
	if (::puzzle1Stage == 21 && ::puzzle1TimerCounter <= 60 && ::puzzle1Progress > 20.0f) {
		::puzzle1Stage = 12;
		::puzzle1BeenOne = true;
		::puzzle1TotalTimer = ::puzzle1TotalTimer + ::puzzle1TimerCounter;
		::puzzle1TimerCounter = 0;
	}
	if (::puzzle1Stage == 21 && ::puzzle1TimerCounter <= 90 && ::puzzle1Progress > 20.0f) {
		::puzzle1Stage = 22;
		::puzzle1BeenOne = true;
		::puzzle1TotalTimer = ::puzzle1TotalTimer + ::puzzle1TimerCounter;
		::puzzle1TimerCounter = 0;
	}
	if (::puzzle1Stage == 31 && ::puzzle1TimerCounter <= 60 && ::puzzle1Progress > 20.0f) {
		::puzzle1Stage = 12;
		::puzzle1BeenOne = true;
		::puzzle1TotalTimer = ::puzzle1TotalTimer + ::puzzle1TimerCounter;
		::puzzle1TimerCounter = 0;
	}
	if (::puzzle1Stage == 31 && ::puzzle1TimerCounter <= 90 && ::puzzle1Progress > 20.0f) {
		::puzzle1Stage = 22;
		::puzzle1BeenOne = true;
		::puzzle1TotalTimer = ::puzzle1TotalTimer + ::puzzle1TimerCounter;
		::puzzle1TimerCounter = 0;
	}
	if (::puzzle1Stage == 31 && ::puzzle1TimerCounter > 90 && ::puzzle1Progress > 20.0f) {
		::puzzle1Stage = 32;
		::puzzle1BeenOne = true;
		::puzzle1TotalTimer = ::puzzle1TotalTimer + ::puzzle1TimerCounter;
		::puzzle1TimerCounter = 0;
	}
	if (::puzzle1Stage == 12 && ::puzzle1TimerCounter > 120 && ::puzzle1Progress < 40.0f) {
		::puzzle1Stage = 22;
	}
	if (::puzzle1Stage == 12 && ::puzzle1TimerCounter <= 120 && ::puzzle1Progress > 40.0f) {
		::puzzle1Stage = 13;
		::puzzle1BeenTwo = true;
		::puzzle1TotalTimer = ::puzzle1TotalTimer + ::puzzle1TimerCounter;
		::puzzle1TimerCounter = 0;
	}
	if (::puzzle1Stage == 22 && ::puzzle1TimerCounter > 180 && ::puzzle1Progress < 40.0f) {
		::puzzle1Stage = 32;
	}
	if (::puzzle1Stage == 22 && ::puzzle1TimerCounter <= 120 && ::puzzle1Progress > 40.0f) {
		::puzzle1Stage = 13;
		::puzzle1BeenTwo = true;
		::puzzle1TotalTimer = ::puzzle1TotalTimer + ::puzzle1TimerCounter;
		::puzzle1TimerCounter = 0;
	}
	if (::puzzle1Stage == 22 && ::puzzle1TimerCounter <= 180 && ::puzzle1Progress > 40.0f) {
		::puzzle1Stage = 23;
		::puzzle1BeenTwo = true;
		::puzzle1TotalTimer = ::puzzle1TotalTimer + ::puzzle1TimerCounter;
		::puzzle1TimerCounter = 0;
	}
	if (::puzzle1Stage == 32 && ::puzzle1TimerCounter <= 60 && ::puzzle1Progress > 40.0f) {
		::puzzle1Stage = 13;
		::puzzle1BeenTwo = true;
		::puzzle1TotalTimer = ::puzzle1TotalTimer + ::puzzle1TimerCounter;
		::puzzle1TimerCounter = 0;
	}
	if (::puzzle1Stage == 32 && ::puzzle1TimerCounter <= 120 && ::puzzle1Progress > 40.0f) {
		::puzzle1Stage = 23;
		::puzzle1BeenTwo = true;
		::puzzle1TotalTimer = ::puzzle1TotalTimer + ::puzzle1TimerCounter;
		::puzzle1TimerCounter = 0;
	}
	if (::puzzle1Stage == 32 && ::puzzle1TimerCounter > 120 && ::puzzle1Progress > 40.0f) {
		::puzzle1Stage = 33;
		::puzzle1BeenTwo = true;
		::puzzle1TotalTimer = ::puzzle1TotalTimer + ::puzzle1TimerCounter;
		::puzzle1TimerCounter = 0;
	}
	if (::puzzle1Stage == 13 && ::puzzle1TimerCounter > 120 && ::puzzle1Progress < 100.0f) {
		::puzzle1Stage = 23;
	}
	if (::puzzle1Stage == 23 && ::puzzle1TimerCounter > 180 && ::puzzle1Progress < 100.0f) {
		::puzzle1Stage = 33;
	}
	if (::puzzle1Progress == 100.0f) ::puzzle1Complete = true;

	//Give hints to the player
	if (::puzzle1Stage == 11 && ::puzzle1TimerCounter > 30 && ::puzzle1TimerCounter < 60 && ::puzzle1BeenOne == false) {
		::puzzle1HintID = 1;
	}
	else if (::puzzle1Stage == 21 && ::puzzle1TimerCounter > 60 && ::puzzle1TimerCounter < 90 && ::puzzle1BeenOne == false) {
		::puzzle1HintID = 2;
	}
	else if (::puzzle1Stage == 31 && ::puzzle1TimerCounter > 90 && ::puzzle1BeenOne == false) {
		::puzzle1HintID = 3;
	}
	else if (::puzzle1Stage == 12 && ::puzzle1TimerCounter > 60 && ::puzzle1TimerCounter < 120 && ::puzzle1BeenTwo == false) {
		::puzzle1HintID = 4;
	}
	else if (::puzzle1Stage == 22 && ::puzzle1TimerCounter > 120 && ::puzzle1TimerCounter < 180 && ::puzzle1BeenTwo == false) {
		::puzzle1HintID = 5;
	}
	else if (::puzzle1Stage == 32 && ::puzzle1TimerCounter > 180 && ::puzzle1BeenTwo == false) {
		::puzzle1HintID = 6;
	}
	else if (::puzzle1Stage == 13 && ::puzzle1TimerCounter > 60 && ::puzzle1TimerCounter < 120) {
		::puzzle1HintID = 7;
	}
	else if (::puzzle1Stage == 23 && ::puzzle1TimerCounter > 120 && ::puzzle1TimerCounter < 180) {
		::puzzle1HintID = 8;
	} 
	else if (::puzzle1Stage == 33 && ::puzzle1TimerCounter >= 180 && (::puzzle1TimerCounter % 30 == 0.0f && ::puzzle1RunOnce == false)) {
		if (puzzle1TempFinalHint == false) {
			::puzzle1HintID = 8;
			puzzle1TempFinalHint = true;
		}
		if (::puzzle1HintID < 32) {
			::puzzle1HintID++;
		}
		::puzzle1TimerCounter++;
		::puzzle1RunOnce = true;
	}
}