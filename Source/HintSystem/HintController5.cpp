// Fill out your copyright notice in the Description page of Project Settings.

#include "HintController5.h"
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
extern int puzzle5PathStep;

//What progress the player is on
extern float puzzle5Progress;

//Whether the puzzle is complete or not
extern bool puzzle5Complete;

//Whether the previous puzzle is complete
extern bool puzzle4Complete;

//The score from the previous puzzle
extern int puzzle4FinalScore;

//The starting point for the puzzle
int puzzle5StartingPoint = 1;

//What stage the player is on
int puzzle5Stage = 11;

//Check if the puzzle has been started
bool puzzle5StartCheck = false;

//Don't allow hints to be reversed
bool puzzle5BeenOne = false;
bool puzzle5BeenTwo = false;

//The total completion time for the puzzle
int puzzle5TotalTimer = 0;

//The currently available hint
int puzzle5HintID = 0;

//Ensure the player doesn't exceed the total number of hints
bool puzzle5TempFinalHint = false;

//Ensure the puzzle only runs once
bool puzzle5RunOnce = false;

//Whether the player is in range of the puzzle
int puzzle5SlidingPuzzleRange = 0;

//The timer tracking the playtime of the puzzle
int puzzle5TimerCounter = 0;

// Sets default values
AHintController5::AHintController5()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set the puzzle's locations
	slidingPuzzleLoc = FVector(-2010.0f, -2000.0f, 310.0f);

	//Default trigonometry values
	trigValue = 6.2831853071795865;
	radiansToDegrees = 57.2957795130823209;

	//Default global values
	::puzzle5StartingPoint = 1;
	::puzzle5Stage = 11;
	::puzzle5StartCheck = false;
	::puzzle5BeenOne = false;
	::puzzle5BeenTwo = false;
	::puzzle5Complete = false;
	::puzzle5TotalTimer = 0;
	::puzzle5HintID = 0;
	::puzzle5TempFinalHint = false;
	::puzzle5RunOnce = false;
	::puzzle5SlidingPuzzleRange = 0;
	::puzzle5TimerCounter = 0;
}

// Called when the game starts or when spawned
void AHintController5::BeginPlay()
{
	Super::BeginPlay();

	//Begin a timer for total length of puzzle.
	GetWorldTimerManager().SetTimer(slidingPuzzleLengthTimer, this, &AHintController5::SlidingPuzzleTimer, 1.0f, true);
}

// Advance the sliding puzzle timer counter
void AHintController5::SlidingPuzzleTimer()
{
	//Advance the timer if in range of the sliding puzzle
	if (::puzzle5SlidingPuzzleRange == 1 && ::puzzle4Complete == true && ::puzzle5Complete == false) {
		::puzzle5TimerCounter++;
	}
}

// Called every frame
void AHintController5::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (::puzzle4Complete == true && ::puzzle5Complete == false) {
		if (::puzzle4FinalScore < 3) ::puzzle5StartingPoint = 1;
		else if (::puzzle4FinalScore < 7) ::puzzle5StartingPoint = 2;
		else ::puzzle5StartingPoint = 3;

		//Reset control variable
		::puzzle5RunOnce = false;

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
			::puzzle5SlidingPuzzleRange = 1;
		}
		else {
			::puzzle5SlidingPuzzleRange = 0;
		}

		//Show whether the player is in range of the sliding puzzle
		if (::puzzle5SlidingPuzzleRange == 0) {
			GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("In range of sliding puzzle: FALSE")));
		}
		else {
			GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("In range of sliding puzzle: TRUE")));
		}

		//Determine starting positions
		if (::puzzle5StartCheck == false) {
			if (::puzzle5StartingPoint == 1) ::puzzle5Stage = 11;
			if (::puzzle5StartingPoint == 2) ::puzzle5Stage = 21;
			if (::puzzle5StartingPoint == 3) ::puzzle5Stage = 31;
			::puzzle5StartCheck = true;
		}

		//Move the player through stages
		if (::puzzle5Stage == 11 && ::puzzle5TimerCounter > 60 && ::puzzle5Progress < 20.0f) {
			::puzzle5Stage = 21;
		}
		if (::puzzle5Stage == 11 && ::puzzle5TimerCounter <= 60 && ::puzzle5Progress > 20.0f) {
			::puzzle5Stage = 12;
			::puzzle5BeenOne = true;
			::puzzle5TotalTimer = ::puzzle5TotalTimer + ::puzzle5TimerCounter;
			::puzzle5TimerCounter = 0;
		}
		if (::puzzle5Stage == 21 && ::puzzle5TimerCounter > 90 && ::puzzle5Progress < 20.0f) {
			::puzzle5Stage = 31;
		}
		if (::puzzle5Stage == 21 && ::puzzle5TimerCounter <= 60 && ::puzzle5Progress > 20.0f) {
			::puzzle5Stage = 12;
			::puzzle5BeenOne = true;
			::puzzle5TotalTimer = ::puzzle5TotalTimer + ::puzzle5TimerCounter;
			::puzzle5TimerCounter = 0;
		}
		if (::puzzle5Stage == 21 && ::puzzle5TimerCounter <= 90 && ::puzzle5Progress > 20.0f) {
			::puzzle5Stage = 22;
			::puzzle5BeenOne = true;
			::puzzle5TotalTimer = ::puzzle5TotalTimer + ::puzzle5TimerCounter;
			::puzzle5TimerCounter = 0;
		}
		if (::puzzle5Stage == 31 && ::puzzle5TimerCounter <= 60 && ::puzzle5Progress > 20.0f) {
			::puzzle5Stage = 12;
			::puzzle5BeenOne = true;
			::puzzle5TotalTimer = ::puzzle5TotalTimer + ::puzzle5TimerCounter;
			::puzzle5TimerCounter = 0;
		}
		if (::puzzle5Stage == 31 && ::puzzle5TimerCounter <= 90 && ::puzzle5Progress > 20.0f) {
			::puzzle5Stage = 22;
			::puzzle5BeenOne = true;
			::puzzle5TotalTimer = ::puzzle5TotalTimer + ::puzzle5TimerCounter;
			::puzzle5TimerCounter = 0;
		}
		if (::puzzle5Stage == 31 && ::puzzle5TimerCounter > 90 && ::puzzle5Progress > 20.0f) {
			::puzzle5Stage = 32;
			::puzzle5BeenOne = true;
			::puzzle5TotalTimer = ::puzzle5TotalTimer + ::puzzle5TimerCounter;
			::puzzle5TimerCounter = 0;
		}
		if (::puzzle5Stage == 12 && ::puzzle5TimerCounter > 120 && ::puzzle5Progress < 40.0f) {
			::puzzle5Stage = 22;
		}
		if (::puzzle5Stage == 12 && ::puzzle5TimerCounter <= 120 && ::puzzle5Progress > 40.0f) {
			::puzzle5Stage = 13;
			::puzzle5BeenTwo = true;
			::puzzle5TotalTimer = ::puzzle5TotalTimer + ::puzzle5TimerCounter;
			::puzzle5TimerCounter = 0;
		}
		if (::puzzle5Stage == 22 && ::puzzle5TimerCounter > 180 && ::puzzle5Progress < 40.0f) {
			::puzzle5Stage = 32;
		}
		if (::puzzle5Stage == 22 && ::puzzle5TimerCounter <= 120 && ::puzzle5Progress > 40.0f) {
			::puzzle5Stage = 13;
			::puzzle5BeenTwo = true;
			::puzzle5TotalTimer = ::puzzle5TotalTimer + ::puzzle5TimerCounter;
			::puzzle5TimerCounter = 0;
		}
		if (::puzzle5Stage == 22 && ::puzzle5TimerCounter <= 180 && ::puzzle5Progress > 40.0f) {
			::puzzle5Stage = 23;
			::puzzle5BeenTwo = true;
			::puzzle5TotalTimer = ::puzzle5TotalTimer + ::puzzle5TimerCounter;
			::puzzle5TimerCounter = 0;
		}
		if (::puzzle5Stage == 32 && ::puzzle5TimerCounter <= 60 && ::puzzle5Progress > 40.0f) {
			::puzzle5Stage = 13;
			::puzzle5BeenTwo = true;
			::puzzle5TotalTimer = ::puzzle5TotalTimer + ::puzzle5TimerCounter;
			::puzzle5TimerCounter = 0;
		}
		if (::puzzle5Stage == 32 && ::puzzle5TimerCounter <= 120 && ::puzzle5Progress > 40.0f) {
			::puzzle5Stage = 23;
			::puzzle5BeenTwo = true;
			::puzzle5TotalTimer = ::puzzle5TotalTimer + ::puzzle5TimerCounter;
			::puzzle5TimerCounter = 0;
		}
		if (::puzzle5Stage == 32 && ::puzzle5TimerCounter > 120 && ::puzzle5Progress > 40.0f) {
			::puzzle5Stage = 33;
			::puzzle5BeenTwo = true;
			::puzzle5TotalTimer = ::puzzle5TotalTimer + ::puzzle5TimerCounter;
			::puzzle5TimerCounter = 0;
		}
		if (::puzzle5Stage == 13 && ::puzzle5TimerCounter > 120 && ::puzzle5Progress < 100.0f) {
			::puzzle5Stage = 23;
		}
		if (::puzzle5Stage == 23 && ::puzzle5TimerCounter > 180 && ::puzzle5Progress < 100.0f) {
			::puzzle5Stage = 33;
		}
		if (::puzzle5Progress == 100.0f) ::puzzle5Complete = true;

		//Give hints to the player
		if (::puzzle5Stage == 11 && ::puzzle5TimerCounter > 30 && ::puzzle5TimerCounter < 60 && ::puzzle5BeenOne == false) {
			::puzzle5HintID = 1;
		}
		else if (::puzzle5Stage == 21 && ::puzzle5TimerCounter > 60 && ::puzzle5TimerCounter < 90 && ::puzzle5BeenOne == false) {
			::puzzle5HintID = 2;
		}
		else if (::puzzle5Stage == 31 && ::puzzle5TimerCounter > 90 && ::puzzle5BeenOne == false) {
			::puzzle5HintID = 3;
		}
		else if (::puzzle5Stage == 12 && ::puzzle5TimerCounter > 60 && ::puzzle5TimerCounter < 120 && ::puzzle5BeenTwo == false) {
			::puzzle5HintID = 4;
		}
		else if (::puzzle5Stage == 22 && ::puzzle5TimerCounter > 120 && ::puzzle5TimerCounter < 180 && ::puzzle5BeenTwo == false) {
			::puzzle5HintID = 5;
		}
		else if (::puzzle5Stage == 32 && ::puzzle5TimerCounter > 180 && ::puzzle5BeenTwo == false) {
			::puzzle5HintID = 6;
		}
		else if (::puzzle5Stage == 13 && ::puzzle5TimerCounter > 60 && ::puzzle5TimerCounter < 120) {
			::puzzle5HintID = 7;
		}
		else if (::puzzle5Stage == 23 && ::puzzle5TimerCounter > 120 && ::puzzle5TimerCounter < 180) {
			::puzzle5HintID = 8;
		}
		else if (::puzzle5Stage == 33 && ::puzzle5TimerCounter >= 180 && (::puzzle5TimerCounter % 30 == 0.0f && ::puzzle5RunOnce == false)) {
			if (puzzle5TempFinalHint == false) {
				::puzzle5HintID = 8;
				puzzle5TempFinalHint = true;
			}
			if (::puzzle5HintID < 32) {
				::puzzle5HintID++;
			}
			::puzzle5TimerCounter++;
			::puzzle5RunOnce = true;
		}
	}
}
