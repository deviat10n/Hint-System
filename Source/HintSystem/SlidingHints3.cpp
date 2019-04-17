// Fill out your copyright notice in the Description page of Project Settings.

#include "SlidingHints3.h"
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

//The progress through puzzle 3
extern float puzzle3Progress;

//The active time for puzzle 3
extern int puzzle3ActiveTime;

//The Hint ID for puzzle 3
extern int puzzle3HintID;

//The total timer for puzzle 3
extern int puzzle3RunningTime;

//The total number of moves for puzzle 3
extern int puzzle3Moves;

//The scores from previous puzzles
extern int puzzle1FinalScore;
extern int puzzle2FinalScore;

//Whether puzzle 3 is complete
extern bool puzzle3Complete;

//Whether the previous puzzle is complete
extern bool puzzle2Complete;

//The current hint for puzzle 3
int puzzle3CurrentHint;

//What the current visible hint is for puzzle 3
int puzzle3VisibleHint = 1;

//What move the player is on in puzzle 3
int puzzle3MoveValue = 0;

//The number of hints taken in puzzle 3
int puzzle3HintsTaken = 0;

//The total number of hints in puzzle 3
int puzzle3NumOfHints = 32;

//The score given based on completion time of puzzle 3
int puzzle3TimerScore = 0;

//The score given based on the number of moves in puzzle 3
int puzzle3MovesScore = 0;

//The score given based on the active time of puzzle 3
int puzzle3ActiveTimeScore = 0;

//The running total of scores for puzzle 3
int puzzle3RunningScore = 0;

//The final total score for puzzle 3
int puzzle3FinalScore = 0;

//The visible hint code
int puzzle3HintCode = 0;

//Regulate the input of the enter key to take hints
bool puzzle3EnterTemp = false;

//Whether the first hint has been determined yet
bool puzzle3FirstHint = false;

//Input check for giving hints
bool puzzle3HintCheck2 = false;

// Sets default values
ASlidingHints3::ASlidingHints3()
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
	::puzzle3MoveValue = 0;
	::puzzle3RunningScore = 0;
	::puzzle3HintsTaken = 0;
	::puzzle3TimerScore = 0;
	::puzzle3FinalScore = 0;
	::puzzle3MovesScore = 0;
	::puzzle3ActiveTimeScore = 0;
	::puzzle3VisibleHint = 1;
	::puzzle3NumOfHints = 32;
	::puzzle3EnterTemp = false;
	::puzzle3HintCheck2 = false;
	::puzzle3FirstHint = false;
	::puzzle3CurrentHint = 0;
	::puzzle3HintCode = 0;
}

// Called when the game starts or when spawned
void ASlidingHints3::BeginPlay()
{
	Super::BeginPlay();

	//Don't block inputs
	bBlockInput = false;

	//Enable keyboard input to control the puzzle
	PlayerController = GetWorld()->GetFirstPlayerController();
	EnableInput(PlayerController);
	InputComponent->bBlockInput = false;

	// Respond when keys are pressed
	FInputActionBinding& EnterBinding = InputComponent->BindAction("Enter", IE_Released, this, &ASlidingHints3::EnterReleased);

	//Don't consume inputs
	EnterBinding.bConsumeInput = false;

	//Give each block an ID
	originalLocation = this->GetActorLocation();
	if (originalLocation == (FVector(2130.0f, -640.0f, 560.0f))) blockID = 1;
	if (originalLocation == (FVector(2130.0f, -780.0f, 560.0f))) blockID = 2;
	if (originalLocation == (FVector(2130.0f, -920.0f, 560.0f))) blockID = 3;
	if (originalLocation == (FVector(2130.0f, -1060.0f, 560.0f))) blockID = 4;
	if (originalLocation == (FVector(2130.0f, -1200.0f, 560.0f))) blockID = 5;
	if (originalLocation == (FVector(2130.0f, -1340.0f, 560.0f))) blockID = 6;
	if (originalLocation == (FVector(2130.0f, -1480.0f, 560.0f))) blockID = 7;
	if (originalLocation == (FVector(2130.0f, -1620.0f, 560.0f))) blockID = 8;
	if (originalLocation == (FVector(2130.0f, -640.0f, 420.0f))) blockID = 9;
	if (originalLocation == (FVector(2130.0f, -780.0f, 420.0f))) blockID = 10;
	if (originalLocation == (FVector(2130.0f, -920.0f, 420.0f))) blockID = 11;
	if (originalLocation == (FVector(2130.0f, -1060.0f, 420.0f))) blockID = 12;
	if (originalLocation == (FVector(2130.0f, -1200.0f, 420.0f))) blockID = 13;
	if (originalLocation == (FVector(2130.0f, -1340.0f, 420.0f))) blockID = 14;
	if (originalLocation == (FVector(2130.0f, -1480.0f, 420.0f))) blockID = 15;
	if (originalLocation == (FVector(2130.0f, -1620.0f, 420.0f))) blockID = 16;
	if (originalLocation == (FVector(2130.0f, -640.0f, 280.0f))) blockID = 17;
	if (originalLocation == (FVector(2130.0f, -780.0f, 280.0f))) blockID = 18;
	if (originalLocation == (FVector(2130.0f, -920.0f, 280.0f))) blockID = 19;
	if (originalLocation == (FVector(2130.0f, -1060.0f, 280.0f))) blockID = 20;
	if (originalLocation == (FVector(2130.0f, -1200.0f, 280.0f))) blockID = 21;
	if (originalLocation == (FVector(2130.0f, -1340.0f, 280.0f))) blockID = 22;
	if (originalLocation == (FVector(2130.0f, -1480.0f, 280.0f))) blockID = 23;
	if (originalLocation == (FVector(2130.0f, -1620.0f, 280.0f))) blockID = 24;
	if (originalLocation == (FVector(2130.0f, -640.0f, 140.0f))) blockID = 25;
	if (originalLocation == (FVector(2130.0f, -780.0f, 140.0f))) blockID = 26;
	if (originalLocation == (FVector(2130.0f, -920.0f, 140.0f))) blockID = 27;
	if (originalLocation == (FVector(2130.0f, -1060.0f, 140.0f))) blockID = 28;
	if (originalLocation == (FVector(2130.0f, -1200.0f, 140.0f))) blockID = 29;
	if (originalLocation == (FVector(2130.0f, -1340.0f, 140.0f))) blockID = 30;
	if (originalLocation == (FVector(2130.0f, -1480.0f, 140.0f))) blockID = 31;
	if (originalLocation == (FVector(2130.0f, -1620.0f, 140.0f))) blockID = 32;
}

// Called every frame
void ASlidingHints3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (::puzzle3CurrentHint == 1) ::puzzle3HintCode = 11;
	if (::puzzle3CurrentHint == 2) ::puzzle3HintCode = 21;
	if (::puzzle3CurrentHint == 3) ::puzzle3HintCode = 31;
	if (::puzzle3CurrentHint == 4) ::puzzle3HintCode = 12;
	if (::puzzle3CurrentHint == 5) ::puzzle3HintCode = 22;
	if (::puzzle3CurrentHint == 6) ::puzzle3HintCode = 32;
	if (::puzzle3CurrentHint == 7) ::puzzle3HintCode = 13;
	if (::puzzle3CurrentHint == 8) ::puzzle3HintCode = 23;
	if (::puzzle3CurrentHint >= 9) ::puzzle3HintCode = 33;

	if (::puzzle3Complete == true) {
		SetActorLocation(originalLocation);
		BoxVisual->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

	if (::puzzle2Complete == true && ::puzzle3Complete == false) {

		puzzle3LocalEnter = ::puzzle3EnterTemp;

		puzzle3LocalTimerScore = ::puzzle3TimerScore;
		puzzle3LocalActiveTimerScore = ::puzzle3ActiveTimeScore;
		puzzle3LocalMovesScore = ::puzzle3MovesScore;
		puzzle3LocalHintsScore = (10 - ((::puzzle3HintsTaken / ::puzzle3NumOfHints) * 10));
		puzzle1LocalFinalScore = ::puzzle1FinalScore;
		puzzle2LocalFinalScore = ::puzzle2FinalScore;

		//Outputs
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle3RunningTime: %d"), ::puzzle3RunningTime));
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle3TimerScore: %d"), ::puzzle3TimerScore));
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle3ActiveTime: %d"), ::puzzle3ActiveTime));
		GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle3ActiveTimeScore: %d"), ::puzzle3ActiveTimeScore));
		GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle3Moves: %d"), ::puzzle3Moves));
		GEngine->AddOnScreenDebugMessage(6, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle3MovesScore: %d"), ::puzzle3MovesScore));
		GEngine->AddOnScreenDebugMessage(7, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle3HintsTaken: %d"), ::puzzle3HintsTaken));
		GEngine->AddOnScreenDebugMessage(8, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle3RunningScore: %d"), ::puzzle3RunningScore));
		GEngine->AddOnScreenDebugMessage(9, 5.f, FColor::Yellow, FString::Printf(TEXT("Puzzle 3 Progress: %f/100"), ::puzzle3Progress));
		GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("VIsible Hint: %d"), ::puzzle3HintCode));

		//Output the solution
		GEngine->AddOnScreenDebugMessage(12, 5.f, FColor::Blue, FString::Printf(TEXT("Solution: DDRUULDRRDLLURRULDDRULUL")));
		
		//Give a score based on the completion time of puzzle 3
		if (::puzzle3RunningTime < 60) ::puzzle3TimerScore = 10;
		else if (::puzzle3RunningTime < 120) ::puzzle3TimerScore = 8;
		else if (::puzzle3RunningTime < 180) ::puzzle3TimerScore = 6;
		else if (::puzzle3RunningTime < 240) ::puzzle3TimerScore = 4;
		else ::puzzle3TimerScore = 2;

		//Give a score based on the active time of puzzle 3
		if (::puzzle3ActiveTime < 30) ::puzzle3ActiveTimeScore = 10;
		else if (::puzzle3ActiveTime < 60) ::puzzle3ActiveTimeScore = 8;
		else if (::puzzle3ActiveTime < 90) ::puzzle3ActiveTimeScore = 6;
		else if (::puzzle3ActiveTime < 120) ::puzzle3ActiveTimeScore = 4;
		else ::puzzle3ActiveTimeScore = 2;

		//Give a score based on the number of moves of puzzle 3
		if (::puzzle3Moves < 30) ::puzzle3MovesScore = 10;
		else if (::puzzle3Moves < 40) ::puzzle3MovesScore = 8;
		else if (::puzzle3Moves < 50) ::puzzle3MovesScore = 6;
		else if (::puzzle3Moves < 60) ::puzzle3MovesScore = 4;
		else ::puzzle3MovesScore = 2;

		//Calculate the player's running score
		::puzzle3RunningScore = ((10 - ((::puzzle3HintsTaken / ::puzzle3NumOfHints) * 10)) + ::puzzle3TimerScore + ::puzzle3MovesScore + ::puzzle3ActiveTimeScore + ::puzzle1FinalScore + ::puzzle2FinalScore);

		//Calculate the player's final score
		::puzzle3FinalScore = (::puzzle3RunningScore / 6);

		//Display the first hint
		if (::puzzle3FirstHint == false && ::puzzle3HintID >= 1) {
			::puzzle3VisibleHint = 1;
			if (blockID == 1) {
				::puzzle3MoveValue = 1;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			::puzzle3FirstHint = true;
		}

		//Get the player's location
		playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		//Display the correct hint to the player
		::puzzle3VisibleHint = ::puzzle3HintID;

		//Track what moves the player is making to determine hints
		::puzzle3MoveValue = ::puzzle3VisibleHint;

		//Reset input check
		::puzzle3HintCheck2 = false;

		//Display the hints to the player
		if (::puzzle3EnterTemp == true) {
			if (::puzzle3MoveValue == 1 && blockID == 1) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 2 && blockID == 2) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 3 && blockID == 3) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 4 && blockID == 4) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 5 && blockID == 5) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 6 && blockID == 6) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 7 && blockID == 7) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 8 && blockID == 8) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 9 && blockID == 9) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 10 && blockID == 10) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 11 && blockID == 11) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 12 && blockID == 12) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 13 && blockID == 13) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 14 && blockID == 14) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 15 && blockID == 15) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 16 && blockID == 16) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 17 && blockID == 17) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 18 && blockID == 18) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 19 && blockID == 19) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 20 && blockID == 20) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 21 && blockID == 21) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 22 && blockID == 22) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 23 && blockID == 23) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 24 && blockID == 24) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 25 && blockID == 25) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 26 && blockID == 26) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 27 && blockID == 27) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 28 && blockID == 28) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 29 && blockID == 29) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 30 && blockID == 30) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 31 && blockID == 31) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else if (::puzzle3MoveValue == 32 && blockID == 32) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle3MoveValue != puzzle3CurrentHint) if (::puzzle3MoveValue != puzzle3CurrentHint) ::puzzle3EnterTemp = false;
				if (::puzzle3HintsTaken < ::puzzle3MoveValue) ::puzzle3HintsTaken = ::puzzle3MoveValue;
				::puzzle3CurrentHint = ::puzzle3MoveValue;
			}
			else {
				SetActorLocation(originalLocation);
				BoxVisual->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}
	}
}

//When the enter button is pressed
void ASlidingHints3::EnterReleased() {
	if (::puzzle2Complete == true && ::puzzle3Complete == false) {
		::puzzle3EnterTemp = true;
	}
}