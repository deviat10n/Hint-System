// Fill out your copyright notice in the Description page of Project Settings.

#include "HintController2.h"
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
extern int puzzle2PathStep;

//What progress the player is on
extern float puzzle2Progress;

//Whether the puzzle is complete or not
extern bool puzzle2Complete;

//Whether the previous puzzle is complete
extern bool puzzle1Complete;

//The score from the previous puzzle
extern int puzzle1FinalScore;

//The starting point for the puzzle
int puzzle2StartingPoint = 1;

//What stage the player is on
int puzzle2Stage = 11;

//Check if the puzzle has been started
bool puzzle2StartCheck = false;

//Don't allow hints to be reversed
bool puzzle2BeenOne = false;
bool puzzle2BeenTwo = false;

//The total completion time for the puzzle
int puzzle2TotalTimer = 0;

//The currently available hint
int puzzle2HintID = 0;

//Ensure the player doesn't exceed the total number of hints
bool puzzle2TempFinalHint = false;

//Ensure the puzzle only runs once
bool puzzle2RunOnce = false;

//Whether the player is in range of the puzzle
int puzzle2SlidingPuzzleRange = 0;

//The timer tracking the playtime of the puzzle
int puzzle2TimerCounter = 0;

// Sets default values
AHintController2::AHintController2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set the puzzle's locations
	slidingPuzzleLoc = FVector(-2010.0f, 10.0f, 310.0f);

	//Default trigonometry values
	trigValue = 6.2831853071795865;
	radiansToDegrees = 57.2957795130823209;

	//Default global values
	::puzzle2StartingPoint = 1;
	::puzzle2Stage = 11;
	::puzzle2StartCheck = false;
	::puzzle2BeenOne = false;
	::puzzle2BeenTwo = false;
	::puzzle2Complete = false;
	::puzzle2TotalTimer = 0;
	::puzzle2HintID = 0;
	::puzzle2TempFinalHint = false;
	::puzzle2RunOnce = false;
	::puzzle2SlidingPuzzleRange = 0;
	::puzzle2TimerCounter = 0;
}

// Called when the game starts or when spawned
void AHintController2::BeginPlay()
{
	Super::BeginPlay();

	//Begin a timer for total length of puzzle.
	GetWorldTimerManager().SetTimer(slidingPuzzleLengthTimer, this, &AHintController2::SlidingPuzzleTimer, 1.0f, true);
}

// Advance the sliding puzzle timer counter
void AHintController2::SlidingPuzzleTimer()
{
	//Advance the timer if in range of the sliding puzzle
	if (::puzzle2SlidingPuzzleRange == 1 && ::puzzle1Complete == true && ::puzzle2Complete == false) {
		::puzzle2TimerCounter++;
	}
}

// Called every frame
void AHintController2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (::puzzle1Complete == true && ::puzzle2Complete == false) {
		if(::puzzle1FinalScore < 3) ::puzzle2StartingPoint = 1;
		else if (::puzzle1FinalScore < 7) ::puzzle2StartingPoint = 2;
		else ::puzzle2StartingPoint = 3;

		//Reset control variable
		::puzzle2RunOnce = false;

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
			::puzzle2SlidingPuzzleRange = 1;
		}
		else {
			::puzzle2SlidingPuzzleRange = 0;
		}

		//Show whether the player is in range of the sliding puzzle
		if (::puzzle2SlidingPuzzleRange == 0) {
			GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("In range of sliding puzzle: FALSE")));
		}
		else {
			GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("In range of sliding puzzle: TRUE")));
		}

		//Determine starting positions
		if (::puzzle2StartCheck == false) {
			if (::puzzle2StartingPoint == 1) ::puzzle2Stage = 11;
			if (::puzzle2StartingPoint == 2) ::puzzle2Stage = 21;
			if (::puzzle2StartingPoint == 3) ::puzzle2Stage = 31;
			::puzzle2StartCheck = true;
		}

		//Move the player through stages
		if (::puzzle2Stage == 11 && ::puzzle2TimerCounter > 60 && ::puzzle2Progress < 20.0f) {
			::puzzle2Stage = 21;
		}
		if (::puzzle2Stage == 11 && ::puzzle2TimerCounter <= 60 && ::puzzle2Progress > 20.0f) {
			::puzzle2Stage = 12;
			::puzzle2BeenOne = true;
			::puzzle2TotalTimer = ::puzzle2TotalTimer + ::puzzle2TimerCounter;
			::puzzle2TimerCounter = 0;
		}
		if (::puzzle2Stage == 21 && ::puzzle2TimerCounter > 90 && ::puzzle2Progress < 20.0f) {
			::puzzle2Stage = 31;
		}
		if (::puzzle2Stage == 21 && ::puzzle2TimerCounter <= 60 && ::puzzle2Progress > 20.0f) {
			::puzzle2Stage = 12;
			::puzzle2BeenOne = true;
			::puzzle2TotalTimer = ::puzzle2TotalTimer + ::puzzle2TimerCounter;
			::puzzle2TimerCounter = 0;
		}
		if (::puzzle2Stage == 21 && ::puzzle2TimerCounter <= 90 && ::puzzle2Progress > 20.0f) {
			::puzzle2Stage = 22;
			::puzzle2BeenOne = true;
			::puzzle2TotalTimer = ::puzzle2TotalTimer + ::puzzle2TimerCounter;
			::puzzle2TimerCounter = 0;
		}
		if (::puzzle2Stage == 31 && ::puzzle2TimerCounter <= 60 && ::puzzle2Progress > 20.0f) {
			::puzzle2Stage = 12;
			::puzzle2BeenOne = true;
			::puzzle2TotalTimer = ::puzzle2TotalTimer + ::puzzle2TimerCounter;
			::puzzle2TimerCounter = 0;
		}
		if (::puzzle2Stage == 31 && ::puzzle2TimerCounter <= 90 && ::puzzle2Progress > 20.0f) {
			::puzzle2Stage = 22;
			::puzzle2BeenOne = true;
			::puzzle2TotalTimer = ::puzzle2TotalTimer + ::puzzle2TimerCounter;
			::puzzle2TimerCounter = 0;
		}
		if (::puzzle2Stage == 31 && ::puzzle2TimerCounter > 90 && ::puzzle2Progress > 20.0f) {
			::puzzle2Stage = 32;
			::puzzle2BeenOne = true;
			::puzzle2TotalTimer = ::puzzle2TotalTimer + ::puzzle2TimerCounter;
			::puzzle2TimerCounter = 0;
		}
		if (::puzzle2Stage == 12 && ::puzzle2TimerCounter > 120 && ::puzzle2Progress < 40.0f) {
			::puzzle2Stage = 22;
		}
		if (::puzzle2Stage == 12 && ::puzzle2TimerCounter <= 120 && ::puzzle2Progress > 40.0f) {
			::puzzle2Stage = 13;
			::puzzle2BeenTwo = true;
			::puzzle2TotalTimer = ::puzzle2TotalTimer + ::puzzle2TimerCounter;
			::puzzle2TimerCounter = 0;
		}
		if (::puzzle2Stage == 22 && ::puzzle2TimerCounter > 180 && ::puzzle2Progress < 40.0f) {
			::puzzle2Stage = 32;
		}
		if (::puzzle2Stage == 22 && ::puzzle2TimerCounter <= 120 && ::puzzle2Progress > 40.0f) {
			::puzzle2Stage = 13;
			::puzzle2BeenTwo = true;
			::puzzle2TotalTimer = ::puzzle2TotalTimer + ::puzzle2TimerCounter;
			::puzzle2TimerCounter = 0;
		}
		if (::puzzle2Stage == 22 && ::puzzle2TimerCounter <= 180 && ::puzzle2Progress > 40.0f) {
			::puzzle2Stage = 23;
			::puzzle2BeenTwo = true;
			::puzzle2TotalTimer = ::puzzle2TotalTimer + ::puzzle2TimerCounter;
			::puzzle2TimerCounter = 0;
		}
		if (::puzzle2Stage == 32 && ::puzzle2TimerCounter <= 60 && ::puzzle2Progress > 40.0f) {
			::puzzle2Stage = 13;
			::puzzle2BeenTwo = true;
			::puzzle2TotalTimer = ::puzzle2TotalTimer + ::puzzle2TimerCounter;
			::puzzle2TimerCounter = 0;
		}
		if (::puzzle2Stage == 32 && ::puzzle2TimerCounter <= 120 && ::puzzle2Progress > 40.0f) {
			::puzzle2Stage = 23;
			::puzzle2BeenTwo = true;
			::puzzle2TotalTimer = ::puzzle2TotalTimer + ::puzzle2TimerCounter;
			::puzzle2TimerCounter = 0;
		}
		if (::puzzle2Stage == 32 && ::puzzle2TimerCounter > 120 && ::puzzle2Progress > 40.0f) {
			::puzzle2Stage = 33;
			::puzzle2BeenTwo = true;
			::puzzle2TotalTimer = ::puzzle2TotalTimer + ::puzzle2TimerCounter;
			::puzzle2TimerCounter = 0;
		}
		if (::puzzle2Stage == 13 && ::puzzle2TimerCounter > 120 && ::puzzle2Progress < 100.0f) {
			::puzzle2Stage = 23;
		}
		if (::puzzle2Stage == 23 && ::puzzle2TimerCounter > 180 && ::puzzle2Progress < 100.0f) {
			::puzzle2Stage = 33;
		}
		if (::puzzle2Progress == 100.0f) ::puzzle2Complete = true;

		//Give hints to the player
		if (::puzzle2Stage == 11 && ::puzzle2TimerCounter > 30 && ::puzzle2TimerCounter < 60 && ::puzzle2BeenOne == false) {
			::puzzle2HintID = 1;
		}
		else if (::puzzle2Stage == 21 && ::puzzle2TimerCounter > 60 && ::puzzle2TimerCounter < 90 && ::puzzle2BeenOne == false) {
			::puzzle2HintID = 2;
		}
		else if (::puzzle2Stage == 31 && ::puzzle2TimerCounter > 90 && ::puzzle2BeenOne == false) {
			::puzzle2HintID = 3;
		}
		else if (::puzzle2Stage == 12 && ::puzzle2TimerCounter > 60 && ::puzzle2TimerCounter < 120 && ::puzzle2BeenTwo == false) {
			::puzzle2HintID = 4;
		}
		else if (::puzzle2Stage == 22 && ::puzzle2TimerCounter > 120 && ::puzzle2TimerCounter < 180 && ::puzzle2BeenTwo == false) {
			::puzzle2HintID = 5;
		}
		else if (::puzzle2Stage == 32 && ::puzzle2TimerCounter > 180 && ::puzzle2BeenTwo == false) {
			::puzzle2HintID = 6;
		}
		else if (::puzzle2Stage == 13 && ::puzzle2TimerCounter > 60 && ::puzzle2TimerCounter < 120) {
			::puzzle2HintID = 7;
		}
		else if (::puzzle2Stage == 23 && ::puzzle2TimerCounter > 120 && ::puzzle2TimerCounter < 180) {
			::puzzle2HintID = 8;
		}
		else if (::puzzle2Stage == 33 && ::puzzle2TimerCounter >= 180 && (::puzzle2TimerCounter % 30 == 0.0f && ::puzzle2RunOnce == false)) {
			if (puzzle2TempFinalHint == false) {
				::puzzle2HintID = 8;
				puzzle2TempFinalHint = true;
			}
			if (::puzzle2HintID < 32) {
				::puzzle2HintID++;
			}
			::puzzle2TimerCounter++;
			::puzzle2RunOnce = true;
		}
	}
}