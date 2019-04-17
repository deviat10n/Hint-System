// Fill out your copyright notice in the Description page of Project Settings.

#include "SlidingHints4.h"
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

//The progress through puzzle 4
extern float puzzle4Progress;

//The active time for puzzle 4
extern int puzzle4ActiveTime;

//The Hint ID for puzzle 4
extern int puzzle4HintID;

//The total timer for puzzle 4
extern int puzzle4RunningTime;

//The total number of moves for puzzle 4
extern int puzzle4Moves;

//The scores from previous puzzles
extern int puzzle1FinalScore;
extern int puzzle2FinalScore;
extern int puzzle3FinalScore;

//Whether puzzle 4 is complete
extern bool puzzle4Complete;

//Whether the previous puzzle is complete
extern bool puzzle3Complete;

//The current hint for puzzle 4
int puzzle4CurrentHint;

//What the current visible hint is for puzzle 4
int puzzle4VisibleHint = 1;

//What move the player is on in puzzle 4
int puzzle4MoveValue = 0;

//The number of hints taken in puzzle 4
int puzzle4HintsTaken = 0;

//The total number of hints in puzzle 4
int puzzle4NumOfHints = 32;

//The score given based on completion time of puzzle 4
int puzzle4TimerScore = 0;

//The score given based on the number of moves in puzzle 4
int puzzle4MovesScore = 0;

//The score given based on the active time of puzzle 4
int puzzle4ActiveTimeScore = 0;

//The running total of scores for puzzle 4
int puzzle4RunningScore = 0;

//The final total score for puzzle 4
int puzzle4FinalScore = 0;

//The visible hint code
int puzzle4HintCode = 0;

//Regulate the input of the enter key to take hints
bool puzzle4EnterTemp = false;

//Whether the first hint has been determined yet
bool puzzle4FirstHint = false;

//Input check for giving hints
bool puzzle4HintCheck2 = false;

// Sets default values
ASlidingHints4::ASlidingHints4()
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
	::puzzle4MoveValue = 0;
	::puzzle4RunningScore = 0;
	::puzzle4HintsTaken = 0;
	::puzzle4TimerScore = 0;
	::puzzle4FinalScore = 0;
	::puzzle4MovesScore = 0;
	::puzzle4ActiveTimeScore = 0;
	::puzzle4VisibleHint = 1;
	::puzzle4NumOfHints = 32;
	::puzzle4EnterTemp = false;
	::puzzle4HintCheck2 = false;
	::puzzle4FirstHint = false;
	::puzzle4CurrentHint = 0;
	::puzzle4HintCode = 0;
}

// Called when the game starts or when spawned
void ASlidingHints4::BeginPlay()
{
	Super::BeginPlay();

	//Don't block inputs
	bBlockInput = false;

	//Enable keyboard input to control the puzzle
	PlayerController = GetWorld()->GetFirstPlayerController();
	EnableInput(PlayerController);
	InputComponent->bBlockInput = false;

	// Respond when keys are pressed
	FInputActionBinding& EnterBinding = InputComponent->BindAction("Enter", IE_Released, this, &ASlidingHints4::EnterReleased);

	//Don't consume inputs
	EnterBinding.bConsumeInput = false;

	//Give each block an ID
	originalLocation = this->GetActorLocation();
	if (originalLocation == (FVector(2300.0f, -640.0f, 560.0f))) blockID = 1;
	if (originalLocation == (FVector(2300.0f, -780.0f, 560.0f))) blockID = 2;
	if (originalLocation == (FVector(2300.0f, -920.0f, 560.0f))) blockID = 3;
	if (originalLocation == (FVector(2300.0f, -1060.0f, 560.0f))) blockID = 4;
	if (originalLocation == (FVector(2300.0f, -1200.0f, 560.0f))) blockID = 5;
	if (originalLocation == (FVector(2300.0f, -1340.0f, 560.0f))) blockID = 6;
	if (originalLocation == (FVector(2300.0f, -1480.0f, 560.0f))) blockID = 7;
	if (originalLocation == (FVector(2300.0f, -1620.0f, 560.0f))) blockID = 8;
	if (originalLocation == (FVector(2300.0f, -640.0f, 420.0f))) blockID = 9;
	if (originalLocation == (FVector(2300.0f, -780.0f, 420.0f))) blockID = 10;
	if (originalLocation == (FVector(2300.0f, -920.0f, 420.0f))) blockID = 11;
	if (originalLocation == (FVector(2300.0f, -1060.0f, 420.0f))) blockID = 12;
	if (originalLocation == (FVector(2300.0f, -1200.0f, 420.0f))) blockID = 13;
	if (originalLocation == (FVector(2300.0f, -1340.0f, 420.0f))) blockID = 14;
	if (originalLocation == (FVector(2300.0f, -1480.0f, 420.0f))) blockID = 15;
	if (originalLocation == (FVector(2300.0f, -1620.0f, 420.0f))) blockID = 16;
	if (originalLocation == (FVector(2300.0f, -640.0f, 280.0f))) blockID = 17;
	if (originalLocation == (FVector(2300.0f, -780.0f, 280.0f))) blockID = 18;
	if (originalLocation == (FVector(2300.0f, -920.0f, 280.0f))) blockID = 19;
	if (originalLocation == (FVector(2300.0f, -1060.0f, 280.0f))) blockID = 20;
	if (originalLocation == (FVector(2300.0f, -1200.0f, 280.0f))) blockID = 21;
	if (originalLocation == (FVector(2300.0f, -1340.0f, 280.0f))) blockID = 22;
	if (originalLocation == (FVector(2300.0f, -1480.0f, 280.0f))) blockID = 23;
	if (originalLocation == (FVector(2300.0f, -1620.0f, 280.0f))) blockID = 24;
	if (originalLocation == (FVector(2300.0f, -640.0f, 140.0f))) blockID = 25;
	if (originalLocation == (FVector(2300.0f, -780.0f, 140.0f))) blockID = 26;
	if (originalLocation == (FVector(2300.0f, -920.0f, 140.0f))) blockID = 27;
	if (originalLocation == (FVector(2300.0f, -1060.0f, 140.0f))) blockID = 28;
	if (originalLocation == (FVector(2300.0f, -1200.0f, 140.0f))) blockID = 29;
	if (originalLocation == (FVector(2300.0f, -1340.0f, 140.0f))) blockID = 30;
	if (originalLocation == (FVector(2300.0f, -1480.0f, 140.0f))) blockID = 31;
	if (originalLocation == (FVector(2300.0f, -1620.0f, 140.0f))) blockID = 32;
}

// Called every frame
void ASlidingHints4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (::puzzle4CurrentHint == 1) ::puzzle4HintCode = 11;
	if (::puzzle4CurrentHint == 2) ::puzzle4HintCode = 21;
	if (::puzzle4CurrentHint == 3) ::puzzle4HintCode = 31;
	if (::puzzle4CurrentHint == 4) ::puzzle4HintCode = 12;
	if (::puzzle4CurrentHint == 5) ::puzzle4HintCode = 22;
	if (::puzzle4CurrentHint == 6) ::puzzle4HintCode = 32;
	if (::puzzle4CurrentHint == 7) ::puzzle4HintCode = 13;
	if (::puzzle4CurrentHint == 8) ::puzzle4HintCode = 23;
	if (::puzzle4CurrentHint >= 9) ::puzzle4HintCode = 33;

	if (::puzzle4Complete == true) {
		SetActorLocation(originalLocation);
		BoxVisual->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

	if (::puzzle3Complete == true && ::puzzle4Complete == false) {

		puzzle4LocalEnter = ::puzzle4EnterTemp;

		puzzle4LocalTimerScore = ::puzzle4TimerScore;
		puzzle4LocalActiveTimerScore = ::puzzle4ActiveTimeScore;
		puzzle4LocalMovesScore = ::puzzle4MovesScore;
		puzzle4LocalHintsScore = (10 - ((::puzzle4HintsTaken / ::puzzle4NumOfHints) * 10));
		puzzle1LocalFinalScore = ::puzzle1FinalScore;
		puzzle2LocalFinalScore = ::puzzle2FinalScore;
		puzzle3LocalFinalScore = ::puzzle3FinalScore;
		
		//Outputs
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle4RunningTime: %d"), ::puzzle4RunningTime));
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle4TimerScore: %d"), ::puzzle4TimerScore));
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle4ActiveTime: %d"), ::puzzle4ActiveTime));
		GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle4ActiveTimeScore: %d"), ::puzzle4ActiveTimeScore));
		GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle4Moves: %d"), ::puzzle4Moves));
		GEngine->AddOnScreenDebugMessage(6, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle4MovesScore: %d"), ::puzzle4MovesScore));
		GEngine->AddOnScreenDebugMessage(7, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle4HintsTaken: %d"), ::puzzle4HintsTaken));
		GEngine->AddOnScreenDebugMessage(8, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle4RunningScore: %d"), ::puzzle4RunningScore));
		GEngine->AddOnScreenDebugMessage(9, 5.f, FColor::Yellow, FString::Printf(TEXT("Puzzle 4 Progress: %f/100"), ::puzzle4Progress));
		GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("VIsible Hint: %d"), ::puzzle4HintCode));

		//Output the solution
		GEngine->AddOnScreenDebugMessage(12, 5.f, FColor::Blue, FString::Printf(TEXT("Solution: DDRUULDRRDLLURRULDDRULUL")));

		//Give a score based on the completion time of puzzle 4
		if (::puzzle4RunningTime < 60) ::puzzle4TimerScore = 10;
		else if (::puzzle4RunningTime < 120) ::puzzle4TimerScore = 8;
		else if (::puzzle4RunningTime < 180) ::puzzle4TimerScore = 6;
		else if (::puzzle4RunningTime < 240) ::puzzle4TimerScore = 4;
		else ::puzzle4TimerScore = 2;

		//Give a score based on the active time of puzzle 4
		if (::puzzle4ActiveTime < 30) ::puzzle4ActiveTimeScore = 10;
		else if (::puzzle4ActiveTime < 60) ::puzzle4ActiveTimeScore = 8;
		else if (::puzzle4ActiveTime < 90) ::puzzle4ActiveTimeScore = 6;
		else if (::puzzle4ActiveTime < 120) ::puzzle4ActiveTimeScore = 4;
		else ::puzzle4ActiveTimeScore = 2;

		//Give a score based on the number of moves of puzzle 4
		if (::puzzle4Moves < 30) ::puzzle4MovesScore = 10;
		else if (::puzzle4Moves < 40) ::puzzle4MovesScore = 8;
		else if (::puzzle4Moves < 50) ::puzzle4MovesScore = 6;
		else if (::puzzle4Moves < 60) ::puzzle4MovesScore = 4;
		else ::puzzle4MovesScore = 2;

		//Calculate the player's running score
		::puzzle4RunningScore = ((10 - ((::puzzle4HintsTaken / ::puzzle4NumOfHints) * 10)) + ::puzzle4TimerScore + ::puzzle4MovesScore + ::puzzle4ActiveTimeScore + ::puzzle1FinalScore + ::puzzle2FinalScore + ::puzzle3FinalScore);

		//Calculate the player's final score
		::puzzle4FinalScore = (::puzzle4RunningScore / 7);

		//Display the first hint
		if (::puzzle4FirstHint == false && ::puzzle4HintID >= 1) {
			::puzzle4VisibleHint = 1;
			if (blockID == 1) {
				::puzzle4MoveValue = 1;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			::puzzle4FirstHint = true;
		}

		//Get the player's location
		playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		//Display the correct hint to the player
		::puzzle4VisibleHint = ::puzzle4HintID;

		//Track what moves the player is making to determine hints
		::puzzle4MoveValue = ::puzzle4VisibleHint;

		//Reset input check
		::puzzle4HintCheck2 = false;

		//Display the hints to the player
		if (::puzzle4EnterTemp == true) {
			if (::puzzle4MoveValue == 1 && blockID == 1) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;

			}
			else if (::puzzle4MoveValue == 2 && blockID == 2) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 3 && blockID == 3) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 4 && blockID == 4) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 5 && blockID == 5) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 6 && blockID == 6) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 7 && blockID == 7) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 8 && blockID == 8) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 9 && blockID == 9) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 10 && blockID == 10) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 11 && blockID == 11) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 12 && blockID == 12) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 13 && blockID == 13) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 14 && blockID == 14) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 15 && blockID == 15) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 16 && blockID == 16) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 17 && blockID == 17) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 18 && blockID == 18) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 19 && blockID == 19) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 20 && blockID == 20) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 21 && blockID == 21) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 22 && blockID == 22) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 23 && blockID == 23) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 24 && blockID == 24) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 25 && blockID == 25) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 26 && blockID == 26) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 27 && blockID == 27) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 28 && blockID == 28) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 29 && blockID == 29) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 30 && blockID == 30) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 31 && blockID == 31) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else if (::puzzle4MoveValue == 32 && blockID == 32) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle4MoveValue != puzzle4CurrentHint) if (::puzzle4MoveValue != puzzle4CurrentHint) ::puzzle4EnterTemp = false;
				if (::puzzle4HintsTaken < ::puzzle4MoveValue) ::puzzle4HintsTaken = ::puzzle4MoveValue;
				::puzzle4CurrentHint = ::puzzle4MoveValue;
			}
			else {
				SetActorLocation(originalLocation);
				BoxVisual->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}
	}
}

//When the enter button is pressed
void ASlidingHints4::EnterReleased() {
	if (::puzzle3Complete == true && ::puzzle4Complete == false) {
		::puzzle4EnterTemp = true;
	}
}