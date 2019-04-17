// Fill out your copyright notice in the Description page of Project Settings.

#include "HintController3.h"
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
extern int puzzle3PathStep;

//What progress the player is on
extern float puzzle3Progress;

//Whether the puzzle is complete or not
extern bool puzzle3Complete;

//Whether the previous puzzle is complete
extern bool puzzle2Complete;

//The score from the previous puzzle
extern int puzzle2FinalScore;

//The starting point for the puzzle
int puzzle3StartingPoint = 1;

//What stage the player is on
int puzzle3Stage = 11;

//Check if the puzzle has been started
bool puzzle3StartCheck = false;

//Don't allow hints to be reversed
bool puzzle3BeenOne = false;
bool puzzle3BeenTwo = false;

//The total completion time for the puzzle
int puzzle3TotalTimer = 0;

//The currently available hint
int puzzle3HintID = 0;

//Ensure the player doesn't exceed the total number of hints
bool puzzle3TempFinalHint = false;

//Ensure the puzzle only runs once
bool puzzle3RunOnce = false;

//Whether the player is in range of the puzzle
int puzzle3SlidingPuzzleRange = 0;

//The timer tracking the playtime of the puzzle
int puzzle3TimerCounter = 0;

// Sets default values
AHintController3::AHintController3()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set the puzzle's locations
	slidingPuzzleLoc = FVector(-2010.0f, -660.0f, 310.0f);

	//Default trigonometry values
	trigValue = 6.2831853071795865;
	radiansToDegrees = 57.2957795130823209;

	//Default global values
	::puzzle3StartingPoint = 1;
	::puzzle3Stage = 11;
	::puzzle3StartCheck = false;
	::puzzle3BeenOne = false;
	::puzzle3BeenTwo = false;
	::puzzle3Complete = false;
	::puzzle3TotalTimer = 0;
	::puzzle3HintID = 0;
	::puzzle3TempFinalHint = false;
	::puzzle3RunOnce = false;
	::puzzle3SlidingPuzzleRange = 0;
	::puzzle3TimerCounter = 0;
}

// Called when the game starts or when spawned
void AHintController3::BeginPlay()
{
	Super::BeginPlay();

	//Begin a timer for total length of puzzle.
	GetWorldTimerManager().SetTimer(slidingPuzzleLengthTimer, this, &AHintController3::SlidingPuzzleTimer, 1.0f, true);
}

// Advance the sliding puzzle timer counter
void AHintController3::SlidingPuzzleTimer()
{
	//Advance the timer if in range of the sliding puzzle
	if (::puzzle3SlidingPuzzleRange == 1 && ::puzzle2Complete == true && ::puzzle3Complete == false) {
		::puzzle3TimerCounter++;
	}
}

// Called every frame
void AHintController3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (::puzzle2Complete == true && ::puzzle3Complete == false) {
		if (::puzzle2FinalScore < 3) ::puzzle3StartingPoint = 1;
		else if (::puzzle2FinalScore < 7) ::puzzle3StartingPoint = 2;
		else ::puzzle3StartingPoint = 3;

		//Reset control variable
		::puzzle3RunOnce = false;

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
			::puzzle3SlidingPuzzleRange = 1;
		}
		else {
			::puzzle3SlidingPuzzleRange = 0;
		}

		//Show whether the player is in range of the sliding puzzle
		if (::puzzle3SlidingPuzzleRange == 0) {
			GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("In range of sliding puzzle: FALSE")));
		}
		else {
			GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("In range of sliding puzzle: TRUE")));
		}

		//Determine starting positions
		if (::puzzle3StartCheck == false) {
			if (::puzzle3StartingPoint == 1) ::puzzle3Stage = 11;
			if (::puzzle3StartingPoint == 2) ::puzzle3Stage = 21;
			if (::puzzle3StartingPoint == 3) ::puzzle3Stage = 31;
			::puzzle3StartCheck = true;
		}

		//Move the player through stages
		if (::puzzle3Stage == 11 && ::puzzle3TimerCounter > 60 && ::puzzle3Progress < 20.0f) {
			::puzzle3Stage = 21;
		}
		if (::puzzle3Stage == 11 && ::puzzle3TimerCounter <= 60 && ::puzzle3Progress > 20.0f) {
			::puzzle3Stage = 12;
			::puzzle3BeenOne = true;
			::puzzle3TotalTimer = ::puzzle3TotalTimer + ::puzzle3TimerCounter;
			::puzzle3TimerCounter = 0;
		}
		if (::puzzle3Stage == 21 && ::puzzle3TimerCounter > 90 && ::puzzle3Progress < 20.0f) {
			::puzzle3Stage = 31;
		}
		if (::puzzle3Stage == 21 && ::puzzle3TimerCounter <= 60 && ::puzzle3Progress > 20.0f) {
			::puzzle3Stage = 12;
			::puzzle3BeenOne = true;
			::puzzle3TotalTimer = ::puzzle3TotalTimer + ::puzzle3TimerCounter;
			::puzzle3TimerCounter = 0;
		}
		if (::puzzle3Stage == 21 && ::puzzle3TimerCounter <= 90 && ::puzzle3Progress > 20.0f) {
			::puzzle3Stage = 22;
			::puzzle3BeenOne = true;
			::puzzle3TotalTimer = ::puzzle3TotalTimer + ::puzzle3TimerCounter;
			::puzzle3TimerCounter = 0;
		}
		if (::puzzle3Stage == 31 && ::puzzle3TimerCounter <= 60 && ::puzzle3Progress > 20.0f) {
			::puzzle3Stage = 12;
			::puzzle3BeenOne = true;
			::puzzle3TotalTimer = ::puzzle3TotalTimer + ::puzzle3TimerCounter;
			::puzzle3TimerCounter = 0;
		}
		if (::puzzle3Stage == 31 && ::puzzle3TimerCounter <= 90 && ::puzzle3Progress > 20.0f) {
			::puzzle3Stage = 22;
			::puzzle3BeenOne = true;
			::puzzle3TotalTimer = ::puzzle3TotalTimer + ::puzzle3TimerCounter;
			::puzzle3TimerCounter = 0;
		}
		if (::puzzle3Stage == 31 && ::puzzle3TimerCounter > 90 && ::puzzle3Progress > 20.0f) {
			::puzzle3Stage = 32;
			::puzzle3BeenOne = true;
			::puzzle3TotalTimer = ::puzzle3TotalTimer + ::puzzle3TimerCounter;
			::puzzle3TimerCounter = 0;
		}
		if (::puzzle3Stage == 12 && ::puzzle3TimerCounter > 120 && ::puzzle3Progress < 40.0f) {
			::puzzle3Stage = 22;
		}
		if (::puzzle3Stage == 12 && ::puzzle3TimerCounter <= 120 && ::puzzle3Progress > 40.0f) {
			::puzzle3Stage = 13;
			::puzzle3BeenTwo = true;
			::puzzle3TotalTimer = ::puzzle3TotalTimer + ::puzzle3TimerCounter;
			::puzzle3TimerCounter = 0;
		}
		if (::puzzle3Stage == 22 && ::puzzle3TimerCounter > 180 && ::puzzle3Progress < 40.0f) {
			::puzzle3Stage = 32;
		}
		if (::puzzle3Stage == 22 && ::puzzle3TimerCounter <= 120 && ::puzzle3Progress > 40.0f) {
			::puzzle3Stage = 13;
			::puzzle3BeenTwo = true;
			::puzzle3TotalTimer = ::puzzle3TotalTimer + ::puzzle3TimerCounter;
			::puzzle3TimerCounter = 0;
		}
		if (::puzzle3Stage == 22 && ::puzzle3TimerCounter <= 180 && ::puzzle3Progress > 40.0f) {
			::puzzle3Stage = 23;
			::puzzle3BeenTwo = true;
			::puzzle3TotalTimer = ::puzzle3TotalTimer + ::puzzle3TimerCounter;
			::puzzle3TimerCounter = 0;
		}
		if (::puzzle3Stage == 32 && ::puzzle3TimerCounter <= 60 && ::puzzle3Progress > 40.0f) {
			::puzzle3Stage = 13;
			::puzzle3BeenTwo = true;
			::puzzle3TotalTimer = ::puzzle3TotalTimer + ::puzzle3TimerCounter;
			::puzzle3TimerCounter = 0;
		}
		if (::puzzle3Stage == 32 && ::puzzle3TimerCounter <= 120 && ::puzzle3Progress > 40.0f) {
			::puzzle3Stage = 23;
			::puzzle3BeenTwo = true;
			::puzzle3TotalTimer = ::puzzle3TotalTimer + ::puzzle3TimerCounter;
			::puzzle3TimerCounter = 0;
		}
		if (::puzzle3Stage == 32 && ::puzzle3TimerCounter > 120 && ::puzzle3Progress > 40.0f) {
			::puzzle3Stage = 33;
			::puzzle3BeenTwo = true;
			::puzzle3TotalTimer = ::puzzle3TotalTimer + ::puzzle3TimerCounter;
			::puzzle3TimerCounter = 0;
		}
		if (::puzzle3Stage == 13 && ::puzzle3TimerCounter > 120 && ::puzzle3Progress < 100.0f) {
			::puzzle3Stage = 23;
		}
		if (::puzzle3Stage == 23 && ::puzzle3TimerCounter > 180 && ::puzzle3Progress < 100.0f) {
			::puzzle3Stage = 33;
		}
		if (::puzzle3Progress == 100.0f) ::puzzle3Complete = true;

		//Give hints to the player
		if (::puzzle3Stage == 11 && ::puzzle3TimerCounter > 30 && ::puzzle3TimerCounter < 60 && ::puzzle3BeenOne == false) {
			::puzzle3HintID = 1;
		}
		else if (::puzzle3Stage == 21 && ::puzzle3TimerCounter > 60 && ::puzzle3TimerCounter < 90 && ::puzzle3BeenOne == false) {
			::puzzle3HintID = 2;
		}
		else if (::puzzle3Stage == 31 && ::puzzle3TimerCounter > 90 && ::puzzle3BeenOne == false) {
			::puzzle3HintID = 3;
		}
		else if (::puzzle3Stage == 12 && ::puzzle3TimerCounter > 60 && ::puzzle3TimerCounter < 120 && ::puzzle3BeenTwo == false) {
			::puzzle3HintID = 4;
		}
		else if (::puzzle3Stage == 22 && ::puzzle3TimerCounter > 120 && ::puzzle3TimerCounter < 180 && ::puzzle3BeenTwo == false) {
			::puzzle3HintID = 5;
		}
		else if (::puzzle3Stage == 32 && ::puzzle3TimerCounter > 180 && ::puzzle3BeenTwo == false) {
			::puzzle3HintID = 6;
		}
		else if (::puzzle3Stage == 13 && ::puzzle3TimerCounter > 60 && ::puzzle3TimerCounter < 120) {
			::puzzle3HintID = 7;
		}
		else if (::puzzle3Stage == 23 && ::puzzle3TimerCounter > 120 && ::puzzle3TimerCounter < 180) {
			::puzzle3HintID = 8;
		}
		else if (::puzzle3Stage == 33 && ::puzzle3TimerCounter >= 180 && (::puzzle3TimerCounter % 30 == 0.0f && ::puzzle3RunOnce == false)) {
			if (puzzle3TempFinalHint == false) {
				::puzzle3HintID = 8;
				puzzle3TempFinalHint = true;
			}
			if (::puzzle3HintID < 32) {
				::puzzle3HintID++;
			}
			::puzzle3TimerCounter++;
			::puzzle3RunOnce = true;
		}
	}
}