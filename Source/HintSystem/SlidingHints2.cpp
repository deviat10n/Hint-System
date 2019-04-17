// Fill out your copyright notice in the Description page of Project Settings.

#include "SlidingHints2.h"
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

//The progress through puzzle 2
extern float puzzle2Progress;

//The active time for puzzle 2
extern int puzzle2ActiveTime;

//The Hint ID for puzzle 2
extern int puzzle2HintID;

//The total timer for puzzle 2
extern int puzzle2RunningTime;

//The total number of moves for puzzle 2
extern int puzzle2Moves;

//The scores from previous puzzles
extern int puzzle1FinalScore;

//Whether puzzle 2 is complete
extern bool puzzle2Complete;

//Whether the previous puzzle is complete
extern bool puzzle1Complete;

//The current hint for puzzle 2
int puzzle2CurrentHint;

//What the current visible hint is for puzzle 2
int puzzle2VisibleHint = 1;

//What move the player is on in puzzle 2
int puzzle2MoveValue = 0;

//The number of hints taken in puzzle 2
int puzzle2HintsTaken = 0;

//The total number of hints in puzzle 2
int puzzle2NumOfHints = 32;

//The score given based on completion time of puzzle 2
int puzzle2TimerScore = 0;

//The score given based on the number of moves in puzzle 2
int puzzle2MovesScore = 0;

//The score given based on the active time of puzzle 2
int puzzle2ActiveTimeScore = 0;

//The running total of scores for puzzle 2
int puzzle2RunningScore = 0;

//The final total score for puzzle 2
int puzzle2FinalScore = 0;

//The visible hint code
int puzzle2HintCode = 0;

//Regulate the input of the enter key to take hints
bool puzzle2EnterTemp = false;

//Whether the first hint has been determined yet
bool puzzle2FirstHint = false;

//Input check for giving hints
bool puzzle2HintCheck2 = false;

// Sets default values
ASlidingHints2::ASlidingHints2()
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
	::puzzle2MoveValue = 0;
	::puzzle2RunningScore = 0;
	::puzzle2HintsTaken = 0;
	::puzzle2TimerScore = 0;
	::puzzle2FinalScore = 0;
	::puzzle2MovesScore = 0;
	::puzzle2ActiveTimeScore = 0;
	::puzzle2VisibleHint = 1;
	::puzzle2NumOfHints = 32;
	::puzzle2EnterTemp = false;
	::puzzle2HintCheck2 = false;
	::puzzle2FirstHint = false;
	::puzzle2CurrentHint = 0;
	::puzzle2HintCode = 0;
}

// Called when the game starts or when spawned
void ASlidingHints2::BeginPlay()
{
	Super::BeginPlay();

	//Don't block inputs
	bBlockInput = false;

	//Enable keyboard input to control the puzzle
	PlayerController = GetWorld()->GetFirstPlayerController();
	EnableInput(PlayerController);
	InputComponent->bBlockInput = false;

	// Respond when keys are pressed
	FInputActionBinding& EnterBinding = InputComponent->BindAction("Enter", IE_Released, this, &ASlidingHints2::EnterReleased);

	//Don't consume inputs
	EnterBinding.bConsumeInput = false;

	//Give each block an ID
	originalLocation = this->GetActorLocation();
	if (originalLocation == (FVector(1960.0f, -640.0f, 560.0f))) blockID = 1;
	if (originalLocation == (FVector(1960.0f, -780.0f, 560.0f))) blockID = 2;
	if (originalLocation == (FVector(1960.0f, -920.0f, 560.0f))) blockID = 3;
	if (originalLocation == (FVector(1960.0f, -1060.0f, 560.0f))) blockID = 4;
	if (originalLocation == (FVector(1960.0f, -1200.0f, 560.0f))) blockID = 5;
	if (originalLocation == (FVector(1960.0f, -1340.0f, 560.0f))) blockID = 6;
	if (originalLocation == (FVector(1960.0f, -1480.0f, 560.0f))) blockID = 7;
	if (originalLocation == (FVector(1960.0f, -1620.0f, 560.0f))) blockID = 8;
	if (originalLocation == (FVector(1960.0f, -640.0f, 420.0f))) blockID = 9;
	if (originalLocation == (FVector(1960.0f, -780.0f, 420.0f))) blockID = 10;
	if (originalLocation == (FVector(1960.0f, -920.0f, 420.0f))) blockID = 11;
	if (originalLocation == (FVector(1960.0f, -1060.0f, 420.0f))) blockID = 12;
	if (originalLocation == (FVector(1960.0f, -1200.0f, 420.0f))) blockID = 13;
	if (originalLocation == (FVector(1960.0f, -1340.0f, 420.0f))) blockID = 14;
	if (originalLocation == (FVector(1960.0f, -1480.0f, 420.0f))) blockID = 15;
	if (originalLocation == (FVector(1960.0f, -1620.0f, 420.0f))) blockID = 16;
	if (originalLocation == (FVector(1960.0f, -640.0f, 280.0f))) blockID = 17;
	if (originalLocation == (FVector(1960.0f, -780.0f, 280.0f))) blockID = 18;
	if (originalLocation == (FVector(1960.0f, -920.0f, 280.0f))) blockID = 19;
	if (originalLocation == (FVector(1960.0f, -1060.0f, 280.0f))) blockID = 20;
	if (originalLocation == (FVector(1960.0f, -1200.0f, 280.0f))) blockID = 21;
	if (originalLocation == (FVector(1960.0f, -1340.0f, 280.0f))) blockID = 22;
	if (originalLocation == (FVector(1960.0f, -1480.0f, 280.0f))) blockID = 23;
	if (originalLocation == (FVector(1960.0f, -1620.0f, 280.0f))) blockID = 24;
	if (originalLocation == (FVector(1960.0f, -640.0f, 140.0f))) blockID = 25;
	if (originalLocation == (FVector(1960.0f, -780.0f, 140.0f))) blockID = 26;
	if (originalLocation == (FVector(1960.0f, -920.0f, 140.0f))) blockID = 27;
	if (originalLocation == (FVector(1960.0f, -1060.0f, 140.0f))) blockID = 28;
	if (originalLocation == (FVector(1960.0f, -1200.0f, 140.0f))) blockID = 29;
	if (originalLocation == (FVector(1960.0f, -1340.0f, 140.0f))) blockID = 30;
	if (originalLocation == (FVector(1960.0f, -1480.0f, 140.0f))) blockID = 31;
	if (originalLocation == (FVector(1960.0f, -1620.0f, 140.0f))) blockID = 32;
}

// Called every frame
void ASlidingHints2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (::puzzle2CurrentHint == 1) ::puzzle2HintCode = 11;
	if (::puzzle2CurrentHint == 2) ::puzzle2HintCode = 21;
	if (::puzzle2CurrentHint == 3) ::puzzle2HintCode = 31;
	if (::puzzle2CurrentHint == 4) ::puzzle2HintCode = 12;
	if (::puzzle2CurrentHint == 5) ::puzzle2HintCode = 22;
	if (::puzzle2CurrentHint == 6) ::puzzle2HintCode = 32;
	if (::puzzle2CurrentHint == 7) ::puzzle2HintCode = 13;
	if (::puzzle2CurrentHint == 8) ::puzzle2HintCode = 23;
	if (::puzzle2CurrentHint >= 9) ::puzzle2HintCode = 33;

	if (::puzzle2Complete == true) {
		SetActorLocation(originalLocation);
		BoxVisual->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	}

	if (::puzzle1Complete == true && ::puzzle2Complete == false) {

		puzzle2LocalEnter = ::puzzle2EnterTemp;

		puzzle2LocalTimerScore = ::puzzle2TimerScore;
		puzzle2LocalActiveTimerScore = ::puzzle2ActiveTimeScore;
		puzzle2LocalMovesScore = ::puzzle2MovesScore;
		puzzle2LocalHintsScore = (10 - ((::puzzle2HintsTaken / ::puzzle2NumOfHints) * 10));
		puzzle1LocalFinalScore = ::puzzle1FinalScore;

		//Outputs
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle2RunningTime: %d"), ::puzzle2RunningTime));
		GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle2TimerScore: %d"), ::puzzle2TimerScore));
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle2ActiveTime: %d"), ::puzzle2ActiveTime));
		GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle2ActiveTimeScore: %d"), ::puzzle2ActiveTimeScore));
		GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle2Moves: %d"), ::puzzle2Moves));
		GEngine->AddOnScreenDebugMessage(6, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle2MovesScore: %d"), ::puzzle2MovesScore));
		GEngine->AddOnScreenDebugMessage(7, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle2HintsTaken: %d"), ::puzzle2HintsTaken));
		GEngine->AddOnScreenDebugMessage(8, 5.f, FColor::Yellow, FString::Printf(TEXT("puzzle2RunningScore: %d"), ::puzzle2RunningScore));
		GEngine->AddOnScreenDebugMessage(9, 5.f, FColor::Yellow, FString::Printf(TEXT("Puzzle 2 Progress: %f/100"), ::puzzle2Progress));
		GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("VIsible Hint: %d"), ::puzzle2HintCode));

		//Output the solution
		GEngine->AddOnScreenDebugMessage(12, 5.f, FColor::Blue, FString::Printf(TEXT("Solution: DDRUULDRRDLLURRULDDRULUL")));

		//Give a score based on the completion time of puzzle 2
		if (::puzzle2RunningTime < 60) ::puzzle2TimerScore = 10;
		else if (::puzzle2RunningTime < 120) ::puzzle2TimerScore = 8;
		else if (::puzzle2RunningTime < 180) ::puzzle2TimerScore = 6;
		else if (::puzzle2RunningTime < 240) ::puzzle2TimerScore = 4;
		else ::puzzle2TimerScore = 2;

		//Give a score based on the active time of puzzle 2
		if (::puzzle2ActiveTime < 30) ::puzzle2ActiveTimeScore = 10;
		else if (::puzzle2ActiveTime < 60) ::puzzle2ActiveTimeScore = 8;
		else if (::puzzle2ActiveTime < 90) ::puzzle2ActiveTimeScore = 6;
		else if (::puzzle2ActiveTime < 120) ::puzzle2ActiveTimeScore = 4;
		else ::puzzle2ActiveTimeScore = 2;

		//Give a score based on the number of moves of puzzle 2
		if (::puzzle2Moves < 30) ::puzzle2MovesScore = 10;
		else if (::puzzle2Moves < 40) ::puzzle2MovesScore = 8;
		else if (::puzzle2Moves < 50) ::puzzle2MovesScore = 6;
		else if (::puzzle2Moves < 60) ::puzzle2MovesScore = 4;
		else ::puzzle2MovesScore = 2;

		//Calculate the player's running score
		::puzzle2RunningScore = ((10 - ((::puzzle2HintsTaken / ::puzzle2NumOfHints) * 10)) + ::puzzle2TimerScore + ::puzzle2MovesScore + ::puzzle2ActiveTimeScore + ::puzzle1FinalScore);

		//Calculate the player's final score
		::puzzle2FinalScore = (::puzzle2RunningScore / 5);

		//Display the first hint
		if (::puzzle2FirstHint == false && ::puzzle2HintID >= 1) {
			::puzzle2VisibleHint = 1;
			if (blockID == 1) {
				::puzzle2MoveValue = 1;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			::puzzle2FirstHint = true;
		}

		//Get the player's location
		playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		//Display the correct hint to the player
		::puzzle2VisibleHint = ::puzzle2HintID;

		//Track what moves the player is making to determine hints
		::puzzle2MoveValue = ::puzzle2VisibleHint;

		//Reset input check
		::puzzle2HintCheck2 = false;

		//Display the hints to the player
		if (::puzzle2EnterTemp == true) {
			if (::puzzle2MoveValue == 1 && blockID == 1) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 2 && blockID == 2) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 3 && blockID == 3) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 4 && blockID == 4) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 5 && blockID == 5) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 6 && blockID == 6) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 7 && blockID == 7) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 8 && blockID == 8) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 9 && blockID == 9) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 10 && blockID == 10) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 11 && blockID == 11) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 12 && blockID == 12) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 13 && blockID == 13) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 14 && blockID == 14) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 15 && blockID == 15) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 16 && blockID == 16) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 17 && blockID == 17) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 18 && blockID == 18) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 19 && blockID == 19) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 20 && blockID == 20) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 21 && blockID == 21) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 22 && blockID == 22) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 23 && blockID == 23) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 24 && blockID == 24) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 25 && blockID == 25) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 26 && blockID == 26) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 27 && blockID == 27) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 28 && blockID == 28) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 29 && blockID == 29) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 30 && blockID == 30) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 31 && blockID == 31) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else if (::puzzle2MoveValue == 32 && blockID == 32) {
				SetActorLocation(FVector(1660.0f, 680.0f, 240.0f));
				BoxVisual->SetWorldScale3D(FVector(1.0f, 6.0f, 4.0f));
				if (::puzzle2MoveValue != puzzle2CurrentHint) if (::puzzle2MoveValue != puzzle2CurrentHint) ::puzzle2EnterTemp = false;
				if (::puzzle2HintsTaken < ::puzzle2MoveValue) ::puzzle2HintsTaken = ::puzzle2MoveValue;
				::puzzle2CurrentHint = ::puzzle2MoveValue;
			}
			else {
				SetActorLocation(originalLocation);
				BoxVisual->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
			}
		}	
	}
}

//When the enter button is pressed
void ASlidingHints2::EnterReleased() {
	if (::puzzle1Complete == true && ::puzzle2Complete == false) {
		::puzzle2EnterTemp = true;
	}
}