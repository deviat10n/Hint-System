// Fill out your copyright notice in the Description page of Project Settings.

#include "SlidingHints.h"
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

//The progress through puzzle 1
extern float puzzle1Progress;

//The active time for puzzle 1
extern int puzzle1ActiveTime;

//The Hint ID for puzzle 1
extern int puzzle1HintID;

//The total timer for puzzle 1
extern int puzzle1RunningTime;

//The total number of moves for puzzle 1
extern int puzzle1Moves;

//Whether puzzle 1 is complete
extern bool puzzle1Complete;

//The current hint for puzzle 1
int puzzle1CurrentHint;

//What the current visible hint is for puzzle 1
int puzzle1VisibleHint = 1;

//What move the player is on in puzzle 1
int puzzle1MoveValue = 0;

//The number of hints taken in puzzle 1
int puzzle1HintsTaken = 0;

//The total number of hints in puzzle 1
int puzzle1NumOfHints = 32;

//The score given based on completion time of puzzle 1
int puzzle1TimerScore = 0;

//The score given based on the number of moves in puzzle 1
int puzzle1MovesScore = 0;

//The score given based on the active time of puzzle 1
int puzzle1ActiveTimeScore = 0;

//The running total of scores for puzzle 1
int puzzle1RunningScore = 0;

//The final total score for puzzle 1
int puzzle1FinalScore = 0;

//The visible hint code
int puzzle1HintCode = 0;

//Prevent negative hint progression
int puzzle1BeenCheck = 0;

//Regulate the input of the enter key to take hints
bool puzzle1EnterTemp = false;

//Whether the first hint has been determined yet
bool puzzle1FirstHint = false;

//Input check for giving hints
bool puzzle1HintCheck2 = false;

//Remove old hints off screen
bool puzzle1RemoveHint = false;

// Sets default values
ASlidingHints::ASlidingHints()
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
	::puzzle1MoveValue = 0;
	::puzzle1RunningScore = 0;
	::puzzle1HintsTaken = 0;
	::puzzle1TimerScore = 0;
	::puzzle1FinalScore = 0;
	::puzzle1MovesScore = 0;
	::puzzle1ActiveTimeScore = 0;
	::puzzle1VisibleHint = 1;
	::puzzle1NumOfHints = 32;
	::puzzle1EnterTemp = false;
	::puzzle1HintCheck2 = false;
	::puzzle1FirstHint = false;
	::puzzle1RemoveHint = false;
	::puzzle1CurrentHint = 0;
	::puzzle1HintCode = 0;
	::puzzle1BeenCheck = 0;
}

// Called when the game starts or when spawned
void ASlidingHints::BeginPlay()
{
	Super::BeginPlay();
	
	//Don't block inputs
	bBlockInput = false;

	//Enable keyboard input
	PlayerController = GetWorld()->GetFirstPlayerController();
	EnableInput(PlayerController);
	InputComponent->bBlockInput = false;

	// Respond when keys are pressed
	FInputActionBinding& EnterBinding = InputComponent->BindAction("Enter", IE_Released, this, &ASlidingHints::EnterReleased);

	//Don't consume inputs
	EnterBinding.bConsumeInput = false;

	//Give each block an ID
	originalLocation = this->GetActorLocation();
	if (originalLocation == (FVector(1790.0f, -640.0f, 560.0f))) blockID = 1;
	if (originalLocation == (FVector(1790.0f, -780.0f, 560.0f))) blockID = 2;
	if (originalLocation == (FVector(1790.0f, -920.0f, 560.0f))) blockID = 3;
	if (originalLocation == (FVector(1790.0f, -1060.0f, 560.0f))) blockID = 4;
	if (originalLocation == (FVector(1790.0f, -1200.0f, 560.0f))) blockID = 5;
	if (originalLocation == (FVector(1790.0f, -1340.0f, 560.0f))) blockID = 6;
	if (originalLocation == (FVector(1790.0f, -1480.0f, 560.0f))) blockID = 7;
	if (originalLocation == (FVector(1790.0f, -1620.0f, 560.0f))) blockID = 8;
	if (originalLocation == (FVector(1790.0f, -640.0f, 420.0f))) blockID = 9;
	if (originalLocation == (FVector(1790.0f, -780.0f, 420.0f))) blockID = 10;
	if (originalLocation == (FVector(1790.0f, -920.0f, 420.0f))) blockID = 11;
	if (originalLocation == (FVector(1790.0f, -1060.0f, 420.0f))) blockID = 12;
	if (originalLocation == (FVector(1790.0f, -1200.0f, 420.0f))) blockID = 13;
	if (originalLocation == (FVector(1790.0f, -1340.0f, 420.0f))) blockID = 14;
	if (originalLocation == (FVector(1790.0f, -1480.0f, 420.0f))) blockID = 15;
	if (originalLocation == (FVector(1790.0f, -1620.0f, 420.0f))) blockID = 16;
	if (originalLocation == (FVector(1790.0f, -640.0f, 280.0f))) blockID = 17;
	if (originalLocation == (FVector(1790.0f, -780.0f, 280.0f))) blockID = 18;
	if (originalLocation == (FVector(1790.0f, -920.0f, 280.0f))) blockID = 19;
	if (originalLocation == (FVector(1790.0f, -1060.0f, 280.0f))) blockID = 20;
	if (originalLocation == (FVector(1790.0f, -1200.0f, 280.0f))) blockID = 21;
	if (originalLocation == (FVector(1790.0f, -1340.0f, 280.0f))) blockID = 22;
	if (originalLocation == (FVector(1790.0f, -1480.0f, 280.0f))) blockID = 23;
	if (originalLocation == (FVector(1790.0f, -1620.0f, 280.0f))) blockID = 24;
	if (originalLocation == (FVector(1790.0f, -640.0f, 140.0f))) blockID = 25;
	if (originalLocation == (FVector(1790.0f, -780.0f, 140.0f))) blockID = 26;
	if (originalLocation == (FVector(1790.0f, -920.0f, 140.0f))) blockID = 27;
	if (originalLocation == (FVector(1790.0f, -1060.0f, 140.0f))) blockID = 28;
	if (originalLocation == (FVector(1790.0f, -1200.0f, 140.0f))) blockID = 29;
	if (originalLocation == (FVector(1790.0f, -1340.0f, 140.0f))) blockID = 30;
	if (originalLocation == (FVector(1790.0f, -1480.0f, 140.0f))) blockID = 31;
	if (originalLocation == (FVector(1790.0f, -1620.0f, 140.0f))) blockID = 32;
}

// Called every frame
void ASlidingHints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (::puzzle1CurrentHint == 1) ::puzzle1HintCode = 11;
	if (::puzzle1CurrentHint == 2) ::puzzle1HintCode = 21;
	if (::puzzle1CurrentHint == 3) ::puzzle1HintCode = 31;
	if (::puzzle1CurrentHint == 4) ::puzzle1HintCode = 12;
	if (::puzzle1CurrentHint == 5) ::puzzle1HintCode = 22;
	if (::puzzle1CurrentHint == 6) ::puzzle1HintCode = 32;
	if (::puzzle1CurrentHint == 7) ::puzzle1HintCode = 13;
	if (::puzzle1CurrentHint == 8) ::puzzle1HintCode = 23;
	if (::puzzle1CurrentHint >= 9) ::puzzle1HintCode = 33;

	puzzle1LocalEnter = ::puzzle1EnterTemp;
	if (::puzzle1Complete == true) {
		SetActorLocation(originalLocation);
		BoxVisual->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

	if (::puzzle1Complete == false) {

		puzzle1LocalTimerScore = ::puzzle1TimerScore;
		puzzle1LocalActiveTimerScore = ::puzzle1ActiveTimeScore;
		puzzle1LocalMovesScore = ::puzzle1MovesScore;
		puzzle1LocalHintsScore = (10 - ((::puzzle1HintsTaken / ::puzzle1NumOfHints) * 10));

		//Outputs
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, FString::Printf(TEXT("Puzzle 1 Running Time: %ds"), ::puzzle1RunningTime));
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Yellow, FString::Printf(TEXT("Puzzle 1 Running Time Score: %d/10"), ::puzzle1TimerScore));
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Yellow, FString::Printf(TEXT("Puzzle 1 Active Time: %ds"), ::puzzle1ActiveTime));
		GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Yellow, FString::Printf(TEXT("Puzzle 1 Active Time Score: %d/10"), ::puzzle1ActiveTimeScore));
		GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Yellow, FString::Printf(TEXT("Puzzle 1 Number of Moves: %d"), ::puzzle1Moves));
		GEngine->AddOnScreenDebugMessage(6, 5.f, FColor::Yellow, FString::Printf(TEXT("Puzzle 1 Number of Moves Score: %d/10"), ::puzzle1MovesScore));
		GEngine->AddOnScreenDebugMessage(7, 5.f, FColor::Yellow, FString::Printf(TEXT("Puzzle 1 Number of Hints Taken: %d"), ::puzzle1HintsTaken));
		GEngine->AddOnScreenDebugMessage(8, 5.f, FColor::Yellow, FString::Printf(TEXT("Puzzle 1 Running Score: %d/40"), ::puzzle1RunningScore));
		GEngine->AddOnScreenDebugMessage(9, 5.f, FColor::Yellow, FString::Printf(TEXT("Puzzle 1 Progress: %f/100"), ::puzzle1Progress));
		GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("VIsible Hint: %d"), ::puzzle1HintCode));

		//Output the solution
		GEngine->AddOnScreenDebugMessage(15, 5.f, FColor::Blue, FString::Printf(TEXT("Solution: DDRUULDRRDLLURRULDDRULUL")));

		//Give a score based on the completion time of puzzle 1
		if (::puzzle1RunningTime < 60) ::puzzle1TimerScore = 10;
		else if (::puzzle1RunningTime < 120) ::puzzle1TimerScore = 8;
		else if (::puzzle1RunningTime < 180) ::puzzle1TimerScore = 6;
		else if (::puzzle1RunningTime < 240) ::puzzle1TimerScore = 4;
		else ::puzzle1TimerScore = 2;

		//Give a score based on the active time of puzzle 1
		if (::puzzle1ActiveTime < 30) ::puzzle1ActiveTimeScore = 10;
		else if (::puzzle1ActiveTime < 60) ::puzzle1ActiveTimeScore = 8;
		else if (::puzzle1ActiveTime < 90) ::puzzle1ActiveTimeScore = 6;
		else if (::puzzle1ActiveTime < 120) ::puzzle1ActiveTimeScore = 4;
		else ::puzzle1ActiveTimeScore = 2;

		//Give a score based on the number of moves of puzzle 1
		if (::puzzle1Moves < 30) ::puzzle1MovesScore = 10;
		else if (::puzzle1Moves < 40) ::puzzle1MovesScore = 8;
		else if (::puzzle1Moves < 50) ::puzzle1MovesScore = 6;
		else if (::puzzle1Moves < 60) ::puzzle1MovesScore = 4;
		else ::puzzle1MovesScore = 2;

		//Calculate the player's running score
		::puzzle1RunningScore = ((10 - ((::puzzle1HintsTaken / ::puzzle1NumOfHints) * 10)) + ::puzzle1TimerScore + ::puzzle1MovesScore + ::puzzle1ActiveTimeScore);

		//Calculate the player's final score
		::puzzle1FinalScore = (::puzzle1RunningScore / 4);

		//Display the first hint
		if (::puzzle1FirstHint == false && ::puzzle1HintID >= 1 && ::puzzle1HintID < 4) {
			::puzzle1VisibleHint = 1;
			if (blockID == 1) {
				::puzzle1MoveValue = 1;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			::puzzle1FirstHint = true;
		}
		if (::puzzle1FirstHint == false && ::puzzle1HintID >= 4 && ::puzzle1HintID < 7) {
			::puzzle1VisibleHint = 4;
			::puzzle1BeenCheck = 1;
			if (blockID == 4) {
				::puzzle1MoveValue = 4;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			::puzzle1FirstHint = true;
		}
		if (::puzzle1FirstHint == false && ::puzzle1HintID >= 7) {
			::puzzle1VisibleHint = 7;
			::puzzle1BeenCheck = 2;
			if (blockID == 7) {
				::puzzle1MoveValue = 7;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			::puzzle1FirstHint = true;
		}

		//Get the player's location
		playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		//Display the correct hint to the player
		::puzzle1VisibleHint = ::puzzle1HintID;

		//Track what moves the player is making to determine hints
		::puzzle1MoveValue = ::puzzle1VisibleHint;

		//Reset input check
		::puzzle1HintCheck2 = false;

		//Display the hints to the player
		if (::puzzle1EnterTemp == true) {
			if (::puzzle1MoveValue == 1 && blockID == 1) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 2 && blockID == 2) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle1MoveValue != puzzle1CurrentHint) if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 3 && blockID == 3) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle1MoveValue != puzzle1CurrentHint) if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 4 && blockID == 4) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 5 && blockID == 5) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 6 && blockID == 6) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 7 && blockID == 7) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 8 && blockID == 8) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 9 && blockID == 9) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 10 && blockID == 10) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 11 && blockID == 11) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 12 && blockID == 12) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 13 && blockID == 13) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 14 && blockID == 14) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 15 && blockID == 15) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 16 && blockID == 16) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 17 && blockID == 17) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 18 && blockID == 18) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 19 && blockID == 19) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 20 && blockID == 20) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 21 && blockID == 21) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 22 && blockID == 22) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 23 && blockID == 23) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 24 && blockID == 24) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 25 && blockID == 25) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 26 && blockID == 26) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 27 && blockID == 27) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 28 && blockID == 28) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 29 && blockID == 29) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 30 && blockID == 30) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 31 && blockID == 31) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else if (::puzzle1MoveValue == 32 && blockID == 32) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if(::puzzle1MoveValue != puzzle1CurrentHint) ::puzzle1EnterTemp = false;
				if (::puzzle1HintsTaken < ::puzzle1MoveValue) ::puzzle1HintsTaken = ::puzzle1MoveValue;
				::puzzle1CurrentHint = ::puzzle1MoveValue;
			}
			else {
				SetActorLocation(originalLocation);
				BoxVisual->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}
	}
}

//When the enter button is pressed
void ASlidingHints::EnterReleased() {
	::puzzle1EnterTemp = true;
}