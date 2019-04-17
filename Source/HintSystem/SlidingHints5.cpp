// Fill out your copyright notice in the Description page of Project Settings.

#include "SlidingHints5.h"
#include "HintController.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine.h"
#include "TimerManager.h"
#include "iostream"
#include "cstdio"
#include "ctime"
#include "Components/InputComponent.h"

//The progress through puzzle 5
extern float puzzle5Progress;

//The active time for puzzle 5
extern int puzzle5ActiveTime;

//The Hint ID for puzzle 5
extern int puzzle5HintID;

//The total timer for puzzle 5
extern int puzzle5RunningTime;

//The total number of moves for puzzle 5
extern int puzzle5Moves;

//The scores from previous puzzles
extern int puzzle1FinalScore;
extern int puzzle2FinalScore;
extern int puzzle3FinalScore;
extern int puzzle4FinalScore;

//Whether puzzle 5 is complete
extern bool puzzle5Complete;

//Whether the previous puzzle is complete
extern bool puzzle4Complete;

//The current hint for puzzle 5
int puzzle5CurrentHint;

//What the current visible hint is for puzzle 5
int puzzle5VisibleHint = 1;

//What move the player is on in puzzle 5
int puzzle5MoveValue = 0;

//The number of hints taken in puzzle 5
int puzzle5HintsTaken = 0;

//The total number of hints in puzzle 5
int puzzle5NumOfHints = 32;

//The score given based on completion time of puzzle 5
int puzzle5TimerScore = 0;

//The score given based on the number of moves in puzzle 5
int puzzle5MovesScore = 0;

//The score given based on the active time of puzzle 5
int puzzle5ActiveTimeScore = 0;

//The running total of scores for puzzle 5
int puzzle5RunningScore = 0;

//The final total score for puzzle 5
int puzzle5FinalScore = 0;

//The visible hint code
int puzzle5HintCode = 0;

//Regulate the input of the enter key to take hints
bool puzzle5EnterTemp = false;

//Whether the first hint has been determined yet
bool puzzle5FirstHint = false;

//Input check for giving hints
bool puzzle5HintCheck2 = false;

// Sets default values
ASlidingHints5::ASlidingHints5()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and attach a static mesh component.
	BoxVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));

	// Set the mesh to the root component.
	RootComponent = BoxVisual;

	// Set the mesh for the static mesh component.
	ConstructorHelpers::FObjectFinder<UStaticMesh> BoxAsset(TEXT("StaticMesh'/Game/SlidingPuzzle/Shape_Cube.Shape_Cube'"));

	// If the mesh was found set it and set properties.
	if (BoxAsset.Succeeded())
	{
		BoxVisual->SetStaticMesh(BoxAsset.Object);
	}

	//Set default values
	::puzzle5MoveValue = 0;
	::puzzle5RunningScore = 0;
	::puzzle5HintsTaken = 0;
	::puzzle5TimerScore = 0;
	::puzzle5FinalScore = 0;
	::puzzle5MovesScore = 0;
	::puzzle5ActiveTimeScore = 0;
	::puzzle5VisibleHint = 1;
	::puzzle5NumOfHints = 32;
	::puzzle5EnterTemp = false;
	::puzzle5HintCheck2 = false;
	::puzzle5FirstHint = false;
	::puzzle5CurrentHint = 0;
	::puzzle5HintCode = 0;
}

// Called when the game starts or when spawned
void ASlidingHints5::BeginPlay()
{
	Super::BeginPlay();

	//Don't block inputs
	bBlockInput = false;

	//Enable keyboard input to control the puzzle
	PlayerController = GetWorld()->GetFirstPlayerController();
	EnableInput(PlayerController);
	InputComponent->bBlockInput = false;

	// Respond when keys are pressed
	FInputActionBinding& EnterBinding = InputComponent->BindAction("Enter", IE_Released, this, &ASlidingHints5::EnterReleased);

	//Don't consume inputs
	EnterBinding.bConsumeInput = false;

	//Give each block an ID
	originalLocation = this->GetActorLocation();
	if (originalLocation == (FVector(2470.0f, -640.0f, 560.0f))) blockID = 1;
	if (originalLocation == (FVector(2470.0f, -780.0f, 560.0f))) blockID = 2;
	if (originalLocation == (FVector(2470.0f, -920.0f, 560.0f))) blockID = 3;
	if (originalLocation == (FVector(2470.0f, -1060.0f, 560.0f))) blockID = 4;
	if (originalLocation == (FVector(2470.0f, -1200.0f, 560.0f))) blockID = 5;
	if (originalLocation == (FVector(2470.0f, -1340.0f, 560.0f))) blockID = 6;
	if (originalLocation == (FVector(2470.0f, -1480.0f, 560.0f))) blockID = 7;
	if (originalLocation == (FVector(2470.0f, -1620.0f, 560.0f))) blockID = 8;
	if (originalLocation == (FVector(2470.0f, -640.0f, 420.0f))) blockID = 9;
	if (originalLocation == (FVector(2470.0f, -780.0f, 420.0f))) blockID = 10;
	if (originalLocation == (FVector(2470.0f, -920.0f, 420.0f))) blockID = 11;
	if (originalLocation == (FVector(2470.0f, -1060.0f, 420.0f))) blockID = 12;
	if (originalLocation == (FVector(2470.0f, -1200.0f, 420.0f))) blockID = 13;
	if (originalLocation == (FVector(2470.0f, -1340.0f, 420.0f))) blockID = 14;
	if (originalLocation == (FVector(2470.0f, -1480.0f, 420.0f))) blockID = 15;
	if (originalLocation == (FVector(2470.0f, -1620.0f, 420.0f))) blockID = 16;
	if (originalLocation == (FVector(2470.0f, -640.0f, 280.0f))) blockID = 17;
	if (originalLocation == (FVector(2470.0f, -780.0f, 280.0f))) blockID = 18;
	if (originalLocation == (FVector(2470.0f, -920.0f, 280.0f))) blockID = 19;
	if (originalLocation == (FVector(2470.0f, -1060.0f, 280.0f))) blockID = 20;
	if (originalLocation == (FVector(2470.0f, -1200.0f, 280.0f))) blockID = 21;
	if (originalLocation == (FVector(2470.0f, -1340.0f, 280.0f))) blockID = 22;
	if (originalLocation == (FVector(2470.0f, -1480.0f, 280.0f))) blockID = 23;
	if (originalLocation == (FVector(2470.0f, -1620.0f, 280.0f))) blockID = 24;
	if (originalLocation == (FVector(2470.0f, -640.0f, 140.0f))) blockID = 25;
	if (originalLocation == (FVector(2470.0f, -780.0f, 140.0f))) blockID = 26;
	if (originalLocation == (FVector(2470.0f, -920.0f, 140.0f))) blockID = 27;
	if (originalLocation == (FVector(2470.0f, -1060.0f, 140.0f))) blockID = 28;
	if (originalLocation == (FVector(2470.0f, -1200.0f, 140.0f))) blockID = 29;
	if (originalLocation == (FVector(2470.0f, -1340.0f, 140.0f))) blockID = 30;
	if (originalLocation == (FVector(2470.0f, -1480.0f, 140.0f))) blockID = 31;
	if (originalLocation == (FVector(2470.0f, -1620.0f, 140.0f))) blockID = 32;
}

// Called every frame
void ASlidingHints5::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (::puzzle5CurrentHint == 1) ::puzzle5HintCode = 11;
	if (::puzzle5CurrentHint == 2) ::puzzle5HintCode = 21;
	if (::puzzle5CurrentHint == 3) ::puzzle5HintCode = 31;
	if (::puzzle5CurrentHint == 4) ::puzzle5HintCode = 12;
	if (::puzzle5CurrentHint == 5) ::puzzle5HintCode = 22;
	if (::puzzle5CurrentHint == 6) ::puzzle5HintCode = 32;
	if (::puzzle5CurrentHint == 7) ::puzzle5HintCode = 13;
	if (::puzzle5CurrentHint == 8) ::puzzle5HintCode = 23;
	if (::puzzle5CurrentHint >= 9) ::puzzle5HintCode = 33;

	if (::puzzle5Complete == true) {
		SetActorLocation(originalLocation);
		BoxVisual->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

	if (::puzzle4Complete == true && ::puzzle5Complete == false) {

		puzzle5LocalEnter = ::puzzle5EnterTemp;

		puzzle5LocalTimerScore = ::puzzle5TimerScore;
		puzzle5LocalActiveTimerScore = ::puzzle5ActiveTimeScore;
		puzzle5LocalMovesScore = ::puzzle5MovesScore;
		puzzle5LocalHintsScore = (10 - ((::puzzle5HintsTaken / ::puzzle5NumOfHints) * 10));
		puzzle1LocalFinalScore = ::puzzle1FinalScore;
		puzzle2LocalFinalScore = ::puzzle2FinalScore;
		puzzle3LocalFinalScore = ::puzzle3FinalScore;
		puzzle4LocalFinalScore = ::puzzle4FinalScore;

		//Outputs
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle5RunningTime: %d"), ::puzzle5RunningTime));
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle5TimerScore: %d"), ::puzzle5TimerScore));
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle5ActiveTime: %d"), ::puzzle5ActiveTime));
		GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle5ActiveTimeScore: %d"), ::puzzle5ActiveTimeScore));
		GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle5Moves: %d"), ::puzzle5Moves));
		GEngine->AddOnScreenDebugMessage(6, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle5MovesScore: %d"), ::puzzle5MovesScore));
		GEngine->AddOnScreenDebugMessage(7, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle5HintsTaken: %d"), ::puzzle5HintsTaken));
		GEngine->AddOnScreenDebugMessage(8, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle5RunningScore: %d"), ::puzzle5RunningScore));
		GEngine->AddOnScreenDebugMessage(9, 5.f, FColor::Yellow, FString::Printf(TEXT("Puzzle 5 Progress: %f/100"), ::puzzle5Progress));
		GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("VIsible Hint: %d"), ::puzzle5HintCode));

		//Output the solution
		GEngine->AddOnScreenDebugMessage(12, 5.f, FColor::Blue, FString::Printf(TEXT("Solution: DDRUULDRRDLLURRULDDRULUL")));

		//Give a score based on the completion time of puzzle 5
		if (::puzzle5RunningTime < 60) ::puzzle5TimerScore = 10;
		else if (::puzzle5RunningTime < 120) ::puzzle5TimerScore = 8;
		else if (::puzzle5RunningTime < 180) ::puzzle5TimerScore = 6;
		else if (::puzzle5RunningTime < 240) ::puzzle5TimerScore = 4;
		else ::puzzle5TimerScore = 2;

		//Give a score based on the active time of puzzle 5
		if (::puzzle5ActiveTime < 30) ::puzzle5ActiveTimeScore = 10;
		else if (::puzzle5ActiveTime < 60) ::puzzle5ActiveTimeScore = 8;
		else if (::puzzle5ActiveTime < 90) ::puzzle5ActiveTimeScore = 6;
		else if (::puzzle5ActiveTime < 120) ::puzzle5ActiveTimeScore = 4;
		else ::puzzle5ActiveTimeScore = 2;

		//Give a score based on the number of moves of puzzle 5
		if (::puzzle5Moves < 30) ::puzzle5MovesScore = 10;
		else if (::puzzle5Moves < 40) ::puzzle5MovesScore = 8;
		else if (::puzzle5Moves < 50) ::puzzle5MovesScore = 6;
		else if (::puzzle5Moves < 60) ::puzzle5MovesScore = 4;
		else ::puzzle5MovesScore = 2;

		//Calculate the player's running score
		::puzzle5RunningScore = ((10 - ((::puzzle5HintsTaken / ::puzzle5NumOfHints) * 10)) + ::puzzle5TimerScore + ::puzzle5MovesScore + ::puzzle5ActiveTimeScore + ::puzzle1FinalScore + ::puzzle2FinalScore + ::puzzle3FinalScore + ::puzzle4FinalScore);

		//Calculate the player's final score
		::puzzle5FinalScore = (::puzzle5RunningScore / 8);

		//Display the first hint
		if (::puzzle5FirstHint == false && ::puzzle5HintID >= 1) {
			::puzzle5VisibleHint = 1;
			if (blockID == 1) {
				::puzzle5MoveValue = 1;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			::puzzle5FirstHint = true;
		}

		//Get the player's location
		playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		//Display the correct hint to the player
		::puzzle5VisibleHint = ::puzzle5HintID;

		//Track what moves the player is making to determine hints
		::puzzle5MoveValue = ::puzzle5VisibleHint;

		//Reset input check
		::puzzle5HintCheck2 = false;

		//Display the hints to the player
		if (::puzzle5EnterTemp == true) {
			if (::puzzle5MoveValue == 1 && blockID == 1) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;

			}
			else if (::puzzle5MoveValue == 2 && blockID == 2) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 3 && blockID == 3) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 4 && blockID == 4) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 5 && blockID == 5) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 6 && blockID == 6) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 7 && blockID == 7) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 8 && blockID == 8) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 9 && blockID == 9) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 10 && blockID == 10) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 11 && blockID == 11) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 12 && blockID == 12) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 13 && blockID == 13) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 14 && blockID == 14) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 15 && blockID == 15) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 16 && blockID == 16) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 17 && blockID == 17) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 18 && blockID == 18) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 19 && blockID == 19) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 20 && blockID == 20) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 21 && blockID == 21) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 22 && blockID == 22) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 23 && blockID == 23) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 24 && blockID == 24) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 25 && blockID == 25) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 26 && blockID == 26) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 27 && blockID == 27) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 28 && blockID == 28) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 29 && blockID == 29) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 30 && blockID == 30) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 31 && blockID == 31) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else if (::puzzle5MoveValue == 32 && blockID == 32) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle5MoveValue != puzzle5CurrentHint) if (::puzzle5MoveValue != puzzle5CurrentHint) ::puzzle5EnterTemp = false;
				if (::puzzle5HintsTaken < ::puzzle5MoveValue) ::puzzle5HintsTaken = ::puzzle5MoveValue;
				::puzzle5CurrentHint = ::puzzle5MoveValue;
			}
			else {
				SetActorLocation(originalLocation);
				BoxVisual->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}
	}
}

//When the enter button is pressed
void ASlidingHints5::EnterReleased() {
	if (::puzzle4Complete == true && ::puzzle5Complete == false) {
		::puzzle5EnterTemp = true;
	}
}