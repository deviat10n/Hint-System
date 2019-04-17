// Fill out your copyright notice in the Description page of Project Settings.

#include "HintController4.h"
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
extern int puzzle4PathStep;

//What progress the player is on
extern float puzzle4Progress;

//Whether the puzzle is complete or not
extern bool puzzle4Complete;

//Whether the previous puzzle is complete
extern bool puzzle3Complete;

//The score from the previous puzzle
extern int puzzle3FinalScore;

//The starting point for the puzzle
int puzzle4StartingPoint = 1;

//What stage the player is on
int puzzle4Stage = 11;

//Check if the puzzle has been started
bool puzzle4StartCheck = false;

//Don't allow hints to be reversed
bool puzzle4BeenOne = false;
bool puzzle4BeenTwo = false;

//The total completion time for the puzzle
int puzzle4TotalTimer = 0;

//The currently available hint
int puzzle4HintID = 0;

//Ensure the player doesn't exceed the total number of hints
bool puzzle4TempFinalHint = false;

//Ensure the puzzle only runs once
bool puzzle4RunOnce = false;

//Whether the player is in range of the puzzle
int puzzle4SlidingPuzzleRange = 0;

//The timer tracking the playtime of the puzzle
int puzzle4TimerCounter = 0;

// Sets default values
AHintController4::AHintController4()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set the puzzle's locations
	slidingPuzzleLoc = FVector(-2010.0f, -1330.0f, 310.0f);

	//Default trigonometry values
	trigValue = 6.2831853071795865;
	radiansToDegrees = 57.2957795130823209;

	//Default global values
	::puzzle4StartingPoint = 1;
	::puzzle4Stage = 11;
	::puzzle4StartCheck = false;
	::puzzle4BeenOne = false;
	::puzzle4BeenTwo = false;
	::puzzle4Complete = false;
	::puzzle4TotalTimer = 0;
	::puzzle4HintID = 0;
	::puzzle4TempFinalHint = false;
	::puzzle4RunOnce = false;
	::puzzle4SlidingPuzzleRange = 0;
	::puzzle4TimerCounter = 0;
}

// Called when the game starts or when spawned
void AHintController4::BeginPlay()
{
	Super::BeginPlay();

	//Begin a timer for total length of puzzle.
	GetWorldTimerManager().SetTimer(slidingPuzzleLengthTimer, this, &AHintController4::SlidingPuzzleTimer, 1.0f, true);
}

// Advance the sliding puzzle timer counter
void AHintController4::SlidingPuzzleTimer()
{
	//Advance the timer if in range of the sliding puzzle
	if (::puzzle4SlidingPuzzleRange == 1 && ::puzzle3Complete == true && ::puzzle4Complete == false) {
		::puzzle4TimerCounter++;
	}
}

// Called every frame
void AHintController4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (::puzzle3Complete == true && ::puzzle4Complete == false) {
		if (::puzzle3FinalScore < 3) ::puzzle4StartingPoint = 1;
		else if (::puzzle3FinalScore < 7) ::puzzle4StartingPoint = 2;
		else ::puzzle4StartingPoint = 3;

		//Reset control variable
		::puzzle4RunOnce = false;

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
			::puzzle4SlidingPuzzleRange = 1;
		}
		else {
			::puzzle4SlidingPuzzleRange = 0;
		}

		//Show whether the player is in range of the sliding puzzle
		if (::puzzle4SlidingPuzzleRange == 0) {
			GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("In range of sliding puzzle: FALSE")));
		}
		else {
			GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("In range of sliding puzzle: TRUE")));
		}

		//Determine starting positions
		if (::puzzle4StartCheck == false) {
			if (::puzzle4StartingPoint == 1) ::puzzle4Stage = 11;
			if (::puzzle4StartingPoint == 2) ::puzzle4Stage = 21;
			if (::puzzle4StartingPoint == 3) ::puzzle4Stage = 31;
			::puzzle4StartCheck = true;
		}

		//Move the player through stages
		if (::puzzle4Stage == 11 && ::puzzle4TimerCounter > 60 && ::puzzle4Progress < 20.0f) {
			::puzzle4Stage = 21;
		}
		if (::puzzle4Stage == 11 && ::puzzle4TimerCounter <= 60 && ::puzzle4Progress > 20.0f) {
			::puzzle4Stage = 12;
			::puzzle4BeenOne = true;
			::puzzle4TotalTimer = ::puzzle4TotalTimer + ::puzzle4TimerCounter;
			::puzzle4TimerCounter = 0;
		}
		if (::puzzle4Stage == 21 && ::puzzle4TimerCounter > 90 && ::puzzle4Progress < 20.0f) {
			::puzzle4Stage = 31;
		}
		if (::puzzle4Stage == 21 && ::puzzle4TimerCounter <= 60 && ::puzzle4Progress > 20.0f) {
			::puzzle4Stage = 12;
			::puzzle4BeenOne = true;
			::puzzle4TotalTimer = ::puzzle4TotalTimer + ::puzzle4TimerCounter;
			::puzzle4TimerCounter = 0;
		}
		if (::puzzle4Stage == 21 && ::puzzle4TimerCounter <= 90 && ::puzzle4Progress > 20.0f) {
			::puzzle4Stage = 22;
			::puzzle4BeenOne = true;
			::puzzle4TotalTimer = ::puzzle4TotalTimer + ::puzzle4TimerCounter;
			::puzzle4TimerCounter = 0;
		}
		if (::puzzle4Stage == 31 && ::puzzle4TimerCounter <= 60 && ::puzzle4Progress > 20.0f) {
			::puzzle4Stage = 12;
			::puzzle4BeenOne = true;
			::puzzle4TotalTimer = ::puzzle4TotalTimer + ::puzzle4TimerCounter;
			::puzzle4TimerCounter = 0;
		}
		if (::puzzle4Stage == 31 && ::puzzle4TimerCounter <= 90 && ::puzzle4Progress > 20.0f) {
			::puzzle4Stage = 22;
			::puzzle4BeenOne = true;
			::puzzle4TotalTimer = ::puzzle4TotalTimer + ::puzzle4TimerCounter;
			::puzzle4TimerCounter = 0;
		}
		if (::puzzle4Stage == 31 && ::puzzle4TimerCounter > 90 && ::puzzle4Progress > 20.0f) {
			::puzzle4Stage = 32;
			::puzzle4BeenOne = true;
			::puzzle4TotalTimer = ::puzzle4TotalTimer + ::puzzle4TimerCounter;
			::puzzle4TimerCounter = 0;
		}
		if (::puzzle4Stage == 12 && ::puzzle4TimerCounter > 120 && ::puzzle4Progress < 40.0f) {
			::puzzle4Stage = 22;
		}
		if (::puzzle4Stage == 12 && ::puzzle4TimerCounter <= 120 && ::puzzle4Progress > 40.0f) {
			::puzzle4Stage = 13;
			::puzzle4BeenTwo = true;
			::puzzle4TotalTimer = ::puzzle4TotalTimer + ::puzzle4TimerCounter;
			::puzzle4TimerCounter = 0;
		}
		if (::puzzle4Stage == 22 && ::puzzle4TimerCounter > 180 && ::puzzle4Progress < 40.0f) {
			::puzzle4Stage = 32;
		}
		if (::puzzle4Stage == 22 && ::puzzle4TimerCounter <= 120 && ::puzzle4Progress > 40.0f) {
			::puzzle4Stage = 13;
			::puzzle4BeenTwo = true;
			::puzzle4TotalTimer = ::puzzle4TotalTimer + ::puzzle4TimerCounter;
			::puzzle4TimerCounter = 0;
		}
		if (::puzzle4Stage == 22 && ::puzzle4TimerCounter <= 180 && ::puzzle4Progress > 40.0f) {
			::puzzle4Stage = 23;
			::puzzle4BeenTwo = true;
			::puzzle4TotalTimer = ::puzzle4TotalTimer + ::puzzle4TimerCounter;
			::puzzle4TimerCounter = 0;
		}
		if (::puzzle4Stage == 32 && ::puzzle4TimerCounter <= 60 && ::puzzle4Progress > 40.0f) {
			::puzzle4Stage = 13;
			::puzzle4BeenTwo = true;
			::puzzle4TotalTimer = ::puzzle4TotalTimer + ::puzzle4TimerCounter;
			::puzzle4TimerCounter = 0;
		}
		if (::puzzle4Stage == 32 && ::puzzle4TimerCounter <= 120 && ::puzzle4Progress > 40.0f) {
			::puzzle4Stage = 23;
			::puzzle4BeenTwo = true;
			::puzzle4TotalTimer = ::puzzle4TotalTimer + ::puzzle4TimerCounter;
			::puzzle4TimerCounter = 0;
		}
		if (::puzzle4Stage == 32 && ::puzzle4TimerCounter > 120 && ::puzzle4Progress > 40.0f) {
			::puzzle4Stage = 33;
			::puzzle4BeenTwo = true;
			::puzzle4TotalTimer = ::puzzle4TotalTimer + ::puzzle4TimerCounter;
			::puzzle4TimerCounter = 0;
		}
		if (::puzzle4Stage == 13 && ::puzzle4TimerCounter > 120 && ::puzzle4Progress < 100.0f) {
			::puzzle4Stage = 23;
		}
		if (::puzzle4Stage == 23 && ::puzzle4TimerCounter > 180 && ::puzzle4Progress < 100.0f) {
			::puzzle4Stage = 33;
		}
		if (::puzzle4Progress == 100.0f) ::puzzle4Complete = true;

		//Give hints to the player
		if (::puzzle4Stage == 11 && ::puzzle4TimerCounter > 30 && ::puzzle4TimerCounter < 60 && ::puzzle4BeenOne == false) {
			::puzzle4HintID = 1;
		}
		else if (::puzzle4Stage == 21 && ::puzzle4TimerCounter > 60 && ::puzzle4TimerCounter < 90 && ::puzzle4BeenOne == false) {
			::puzzle4HintID = 2;
		}
		else if (::puzzle4Stage == 31 && ::puzzle4TimerCounter > 90 && ::puzzle4BeenOne == false) {
			::puzzle4HintID = 3;
		}
		else if (::puzzle4Stage == 12 && ::puzzle4TimerCounter > 60 && ::puzzle4TimerCounter < 120 && ::puzzle4BeenTwo == false) {
			::puzzle4HintID = 4;
		}
		else if (::puzzle4Stage == 22 && ::puzzle4TimerCounter > 120 && ::puzzle4TimerCounter < 180 && ::puzzle4BeenTwo == false) {
			::puzzle4HintID = 5;
		}
		else if (::puzzle4Stage == 32 && ::puzzle4TimerCounter > 180 && ::puzzle4BeenTwo == false) {
			::puzzle4HintID = 6;
		}
		else if (::puzzle4Stage == 13 && ::puzzle4TimerCounter > 60 && ::puzzle4TimerCounter < 120) {
			::puzzle4HintID = 7;
		}
		else if (::puzzle4Stage == 23 && ::puzzle4TimerCounter > 120 && ::puzzle4TimerCounter < 180) {
			::puzzle4HintID = 8;
		}
		else if (::puzzle4Stage == 33 && ::puzzle4TimerCounter >= 180 && (::puzzle4TimerCounter % 30 == 0.0f && ::puzzle4RunOnce == false)) {
			if (puzzle4TempFinalHint == false) {
				::puzzle4HintID = 8;
				puzzle4TempFinalHint = true;
			}
			if (::puzzle4HintID < 32) {
				::puzzle4HintID++;
			}
			::puzzle4TimerCounter++;
			::puzzle4RunOnce = true;
		}
	}
}