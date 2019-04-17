// Fill out your copyright notice in the Description page of Project Settings.

#include "SlidingPuzzle.h"
#include "DebugSystem.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine.h"
#include "TimerManager.h"
#include "iostream"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/InputComponent.h"

//Debug complete puzzle
extern bool debugPuzzle1;

//Whether the player is by the puzzle
extern int puzzle1SlidingPuzzleRange;

//Whether puzzle is complete 
bool puzzle1Complete = false;

//Total number of moves
int puzzle1Moves = 0;

//The value of the open puzzle slot
int puzzle1EmptySlot = 9;

//Regulate input
bool puzzle1InputCheck = false;

//Check if the player is on a predetermined path
bool puzzle1OnPath = false;
bool puzzle1BeenOnPath = false;

//The player's running time
int puzzle1RunningTime = 0;
bool puzzle1TempTick2 = false;

//The player's active time
int puzzle1ActiveTime = 0;
int puzzle1Countdown = 5;
bool puzzle1TempTick = false;
bool puzzle1CountdownCheck = true;

//Each block's location
int puzzle1ID1Loc = 7;
int puzzle1ID2Loc = 5;
int puzzle1ID3Loc = 8;
int puzzle1ID4Loc = 2;
int puzzle1ID5Loc = 1;
int puzzle1ID6Loc = 4;
int puzzle1ID7Loc = 6;
int puzzle1ID8Loc = 3;

//Track puzzle progress
float puzzle1Progress = 0.0f;
bool puzzle1ID1LocCheck = false;
bool puzzle1ID2LocCheck = false;
bool puzzle1ID3LocCheck = false;
bool puzzle1ID4LocCheck = false;
bool puzzle1ID5LocCheck = false;
bool puzzle1ID6LocCheck = false;
bool puzzle1ID7LocCheck = false;
bool puzzle1ID8LocCheck = false;

//Path states
int puzzle1PathStep24[] = { 1, 2, 3, 4, 5, 6, 7, 0, 8 };
int puzzle1PathStep23[] = { 1, 2, 3, 4, 0, 6, 7, 5, 8 };
int puzzle1PathStep22[] = { 1, 2, 3, 0, 4, 6, 7, 5, 8 };
int puzzle1PathStep21[] = { 0, 2, 3, 1, 4, 6, 7, 5, 8 };
int puzzle1PathStep20[] = { 2, 0, 3, 1, 4, 6, 7, 5, 8 };
int puzzle1PathStep19[] = { 2, 4, 3, 1, 0, 6, 7, 5, 8 };
int puzzle1PathStep18[] = { 2, 4, 3, 1, 5, 6, 7, 0, 8 };
int puzzle1PathStep17[] = { 2, 4, 3, 1, 5, 6, 0, 7, 8 };
int puzzle1PathStep16[] = { 2, 4, 3, 0, 5, 6, 1, 7, 8 };
int puzzle1PathStep15[] = { 2, 4, 3, 5, 0, 6, 1, 7, 8 };
int puzzle1PathStep14[] = { 2, 4, 3, 5, 6, 0, 1, 7, 8 };
int puzzle1PathStep13[] = { 2, 4, 0, 5, 6, 3, 1, 7, 8 };
int puzzle1PathStep12[] = { 2, 0, 4, 5, 6, 3, 1, 7, 8 };
int puzzle1PathStep11[] = { 0, 2, 4, 5, 6, 3, 1, 7, 8 };
int puzzle1PathStep10[] = { 5, 2, 4, 0, 6, 3, 1, 7, 8 };
int puzzle1PathStep9[] = { 5, 2, 4, 6, 0, 3, 1, 7, 8 };
int puzzle1PathStep8[] = { 5, 2, 4, 6, 3, 0, 1, 7, 8 };
int puzzle1PathStep7[] = { 5, 2, 4, 6, 3, 8, 1, 7, 0 };
int puzzle1PathStep6[] = { 5, 2, 4, 6, 3, 8, 1, 0, 7 };
int puzzle1PathStep5[] = { 5, 2, 4, 6, 0, 8, 1, 3, 7 };
int puzzle1PathStep4[] = { 5, 0, 4, 6, 2, 8, 1, 3, 7 };
int puzzle1PathStep3[] = { 5, 4, 0, 6, 2, 8, 1, 3, 7 };
int puzzle1PathStep2[] = { 5, 4, 8, 6, 2, 0, 1, 3, 7 };
int puzzle1PathStep1[] = { 5, 4, 8, 6, 2, 7, 1, 3, 0 };
int puzzle1PathStep;

//Check if slots are filled
bool puzzle1Slot1Filled = false;
bool puzzle1Slot2Filled = false;
bool puzzle1Slot3Filled = false;
bool puzzle1Slot4Filled = false;
bool puzzle1Slot5Filled = false;
bool puzzle1Slot6Filled = false;
bool puzzle1Slot7Filled = false;
bool puzzle1Slot8Filled = false;
bool puzzle1Slot9Filled = false;

//Current State
int puzzle1CurrentArray[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Sets default values
ASlidingPuzzle::ASlidingPuzzle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialise a timer
	TimerText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TimerNumber"));
	TimerText->SetHorizontalAlignment(EHTA_Center);
	TimerText->SetWorldSize(150.0f);
	RootComponent = TimerText;

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

	//Set all the slot locations
	puzzle1Slot1.X = (-2010.0f);
	puzzle1Slot1.Y = (800.0f);
	puzzle1Slot1.Z = (550.0f);
	puzzle1Slot2.X = (-2010.0f);
	puzzle1Slot2.Y = (680.0f);
	puzzle1Slot2.Z = (550.0f);
	puzzle1Slot3.X = (-2010.0f);
	puzzle1Slot3.Y = (560.0f);
	puzzle1Slot3.Z = (550.0f);
	puzzle1Slot4.X = (-2010.0f);
	puzzle1Slot4.Y = (800.0f);
	puzzle1Slot4.Z = (430.0f);
	puzzle1Slot5.X = (-2010.0f);
	puzzle1Slot5.Y = (680.0f);
	puzzle1Slot5.Z = (430.0f);
	puzzle1Slot6.X = (-2010.0f);
	puzzle1Slot6.Y = (560.0f);
	puzzle1Slot6.Z = (430.0f);
	puzzle1Slot7.X = (-2010.0f);
	puzzle1Slot7.Y = (800.0f);
	puzzle1Slot7.Z = (310.0f);
	puzzle1Slot8.X = (-2010.0f);
	puzzle1Slot8.Y = (680.0f);
	puzzle1Slot8.Z = (310.0f);
	puzzle1Slot9.X = (-2010.0f);
	puzzle1Slot9.Y = (560.0f);
	puzzle1Slot9.Z = (310.0f);

	//Default global values
	::puzzle1RunningTime = 0;
	::puzzle1ActiveTime = 0;
	::puzzle1Moves = 0;
	::puzzle1PathStep = 0;
	::puzzle1ID1Loc = 7;
	::puzzle1ID2Loc = 5;
	::puzzle1ID3Loc = 8;
	::puzzle1ID4Loc = 2;
	::puzzle1ID5Loc = 1;
	::puzzle1ID6Loc = 4;
	::puzzle1ID7Loc = 6;
	::puzzle1ID8Loc = 3;
	::puzzle1Countdown = 5;
	::puzzle1EmptySlot = 9;
	::puzzle1Progress = 0.0f;	
	::puzzle1InputCheck = false;
	::puzzle1OnPath = false;
	::puzzle1BeenOnPath = false;	
	::puzzle1ID1LocCheck = false;
	::puzzle1ID2LocCheck = false;
	::puzzle1ID3LocCheck = false;
	::puzzle1ID4LocCheck = false;
	::puzzle1ID5LocCheck = false;
	::puzzle1ID6LocCheck = false;
	::puzzle1ID7LocCheck = false;
	::puzzle1ID8LocCheck = false;
	::puzzle1Slot1Filled = false;
	::puzzle1Slot2Filled = false;
	::puzzle1Slot3Filled = false;
	::puzzle1Slot4Filled = false;
	::puzzle1Slot5Filled = false;
	::puzzle1Slot6Filled = false;
	::puzzle1Slot7Filled = false;
	::puzzle1Slot8Filled = false;
	::puzzle1Slot9Filled = false;
	::puzzle1Complete = false;
	::puzzle1TempTick = false;
	::puzzle1TempTick2 = false;
	::puzzle1CountdownCheck = true;

	//Update the block's locations
	puzzle1ActorLoc = this->GetActorLocation();
}

// Called when the game starts or when spawned
void ASlidingPuzzle::BeginPlay()
{
	Super::BeginPlay();

	//Start the timer
	UpdateTimerDisplay();
	GetWorldTimerManager().SetTimer(TimerTimerHandle, this, &ASlidingPuzzle::AdvanceTimer, 1.0f, true);

	//Get the block's location
	puzzle1ActorLoc = this->GetActorLocation();

	//Give each block an ID
	if ((puzzle1ActorLoc.Y == puzzle1Slot7.Y) && (puzzle1ActorLoc.Z == puzzle1Slot7.Z)) puzzle1ID = 1;
	if ((puzzle1ActorLoc.Y == puzzle1Slot5.Y) && (puzzle1ActorLoc.Z == puzzle1Slot5.Z)) puzzle1ID = 2;
	if ((puzzle1ActorLoc.Y == puzzle1Slot8.Y) && (puzzle1ActorLoc.Z == puzzle1Slot8.Z)) puzzle1ID = 3;
	if ((puzzle1ActorLoc.Y == puzzle1Slot2.Y) && (puzzle1ActorLoc.Z == puzzle1Slot2.Z)) puzzle1ID = 4;
	if ((puzzle1ActorLoc.Y == puzzle1Slot1.Y) && (puzzle1ActorLoc.Z == puzzle1Slot1.Z)) puzzle1ID = 5;
	if ((puzzle1ActorLoc.Y == puzzle1Slot4.Y) && (puzzle1ActorLoc.Z == puzzle1Slot4.Z)) puzzle1ID = 6;
	if ((puzzle1ActorLoc.Y == puzzle1Slot6.Y) && (puzzle1ActorLoc.Z == puzzle1Slot6.Z)) puzzle1ID = 7;
	if ((puzzle1ActorLoc.Y == puzzle1Slot3.Y) && (puzzle1ActorLoc.Z == puzzle1Slot3.Z)) puzzle1ID = 8;
	if ((puzzle1ActorLoc.Y == puzzle1Slot9.Y) && (puzzle1ActorLoc.Z == puzzle1Slot9.Z)) puzzle1ID = 9;

	//Don't block inputs
	bBlockInput = false;

	//Enable keyboard input to control the puzzle
	PlayerController = GetWorld()->GetFirstPlayerController();
	EnableInput(PlayerController);
	InputComponent->bBlockInput = false;

	// Respond when keys are pressed
	FInputActionBinding& UpBinding = InputComponent->BindAction("SlideUp", IE_Released, this, &ASlidingPuzzle::UpReleased);
	FInputActionBinding& DownBinding = InputComponent->BindAction("SlideDown", IE_Released, this, &ASlidingPuzzle::DownReleased);
	FInputActionBinding& LeftBinding = InputComponent->BindAction("SlideLeft", IE_Released, this, &ASlidingPuzzle::LeftReleased);
	FInputActionBinding& RightBinding = InputComponent->BindAction("SlideRight", IE_Released, this, &ASlidingPuzzle::RightReleased);
	
	//Don't consume inputs
	UpBinding.bConsumeInput = false;
	DownBinding.bConsumeInput = false;
	LeftBinding.bConsumeInput = false;
	RightBinding.bConsumeInput = false;
}

// Called every frame
void ASlidingPuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Update block's location if moved
	SetActorLocation(puzzle1ActorLoc);

	//Reset input check
	::puzzle1InputCheck = false;
	::puzzle1TempTick = false;
	::puzzle1TempTick2 = false;

	//Debug complete puzzle
	if (::debugPuzzle1 == true) {
		if (puzzle1ID == 1) puzzle1ActorLoc = puzzle1Slot1;
		if (puzzle1ID == 2) puzzle1ActorLoc = puzzle1Slot2;
		if (puzzle1ID == 3) puzzle1ActorLoc = puzzle1Slot3;
		if (puzzle1ID == 4) puzzle1ActorLoc = puzzle1Slot4;
		if (puzzle1ID == 5) puzzle1ActorLoc = puzzle1Slot5;
		if (puzzle1ID == 6) puzzle1ActorLoc = puzzle1Slot6;
		if (puzzle1ID == 7) puzzle1ActorLoc = puzzle1Slot7;
		if (puzzle1ID == 8) puzzle1ActorLoc = puzzle1Slot8;
		puzzle1Complete = true;
	}

	//Determine the location of each block
	if (puzzle1ID == 1 && puzzle1ActorLoc == puzzle1Slot1) {
		::puzzle1ID1Loc = 1;
		::puzzle1Slot1Filled = true;
	}
	if (puzzle1ID == 1 && puzzle1ActorLoc == puzzle1Slot2) {
		::puzzle1ID1Loc = 2;
		::puzzle1Slot2Filled = true;
	}
	if (puzzle1ID == 1 && puzzle1ActorLoc == puzzle1Slot3) {
		::puzzle1ID1Loc = 3;
		::puzzle1Slot3Filled = true;
	}
	if (puzzle1ID == 1 && puzzle1ActorLoc == puzzle1Slot4) {
		::puzzle1ID1Loc = 4;
		::puzzle1Slot4Filled = true;
	}
	if (puzzle1ID == 1 && puzzle1ActorLoc == puzzle1Slot5) {
		::puzzle1ID1Loc = 5;
		::puzzle1Slot5Filled = true;
	}
	if (puzzle1ID == 1 && puzzle1ActorLoc == puzzle1Slot6) {
		::puzzle1ID1Loc = 6;
		::puzzle1Slot6Filled = true;
	}
	if (puzzle1ID == 1 && puzzle1ActorLoc == puzzle1Slot7) {
		::puzzle1ID1Loc = 7;
		::puzzle1Slot7Filled = true;
	}
	if (puzzle1ID == 1 && puzzle1ActorLoc == puzzle1Slot8) {
		::puzzle1ID1Loc = 8;
		::puzzle1Slot8Filled = true;
	}
	if (puzzle1ID == 1 && puzzle1ActorLoc == puzzle1Slot9) {
		::puzzle1ID1Loc = 9;
		::puzzle1Slot9Filled = true;
	}
	if (puzzle1ID == 2 && puzzle1ActorLoc == puzzle1Slot1) {
		::puzzle1ID2Loc = 1;
		::puzzle1Slot1Filled = true;
	}
	if (puzzle1ID == 2 && puzzle1ActorLoc == puzzle1Slot2) {
		::puzzle1ID2Loc = 2;
		::puzzle1Slot2Filled = true;
	}
	if (puzzle1ID == 2 && puzzle1ActorLoc == puzzle1Slot3) {
		::puzzle1ID2Loc = 3;
		::puzzle1Slot3Filled = true;
	}
	if (puzzle1ID == 2 && puzzle1ActorLoc == puzzle1Slot4) {
		::puzzle1ID2Loc = 4;
		::puzzle1Slot4Filled = true;
	}
	if (puzzle1ID == 2 && puzzle1ActorLoc == puzzle1Slot5) {
		::puzzle1ID2Loc = 5;
		::puzzle1Slot5Filled = true;
	}
	if (puzzle1ID == 2 && puzzle1ActorLoc == puzzle1Slot6) {
		::puzzle1ID2Loc = 6;
		::puzzle1Slot6Filled = true;
	}
	if (puzzle1ID == 2 && puzzle1ActorLoc == puzzle1Slot7) {
		::puzzle1ID2Loc = 7;
		::puzzle1Slot7Filled = true;
	}
	if (puzzle1ID == 2 && puzzle1ActorLoc == puzzle1Slot8) {
		::puzzle1ID2Loc = 8;
		::puzzle1Slot8Filled = true;
	}
	if (puzzle1ID == 2 && puzzle1ActorLoc == puzzle1Slot9) {
		::puzzle1ID2Loc = 9;
		::puzzle1Slot9Filled = true;
	}
	if (puzzle1ID == 3 && puzzle1ActorLoc == puzzle1Slot1) {
		::puzzle1ID3Loc = 1;
		::puzzle1Slot1Filled = true;
	}
	if (puzzle1ID == 3 && puzzle1ActorLoc == puzzle1Slot2) {
		::puzzle1ID3Loc = 2;
		::puzzle1Slot2Filled = true;
	}
	if (puzzle1ID == 3 && puzzle1ActorLoc == puzzle1Slot3) {
		::puzzle1ID3Loc = 3;
		::puzzle1Slot3Filled = true;
	}
	if (puzzle1ID == 3 && puzzle1ActorLoc == puzzle1Slot4) {
		::puzzle1ID3Loc = 4;
		::puzzle1Slot4Filled = true;
	}
	if (puzzle1ID == 3 && puzzle1ActorLoc == puzzle1Slot5) {
		::puzzle1ID3Loc = 5;
		::puzzle1Slot5Filled = true;
	}
	if (puzzle1ID == 3 && puzzle1ActorLoc == puzzle1Slot6) {
		::puzzle1ID3Loc = 6;
		::puzzle1Slot6Filled = true;
	}
	if (puzzle1ID == 3 && puzzle1ActorLoc == puzzle1Slot7) {
		::puzzle1ID3Loc = 7;
		::puzzle1Slot7Filled = true;
	}
	if (puzzle1ID == 3 && puzzle1ActorLoc == puzzle1Slot8) {
		::puzzle1ID3Loc = 8;
		::puzzle1Slot8Filled = true;
	}
	if (puzzle1ID == 3 && puzzle1ActorLoc == puzzle1Slot9) {
		::puzzle1ID3Loc = 9;
		::puzzle1Slot9Filled = true;
	}
	if (puzzle1ID == 4 && puzzle1ActorLoc == puzzle1Slot1) {
		::puzzle1ID4Loc = 1;
		::puzzle1Slot1Filled = true;
	}
	if (puzzle1ID == 4 && puzzle1ActorLoc == puzzle1Slot2) {
		::puzzle1ID4Loc = 2;
		::puzzle1Slot2Filled = true;
	}
	if (puzzle1ID == 4 && puzzle1ActorLoc == puzzle1Slot3) {
		::puzzle1ID4Loc = 3;
		::puzzle1Slot3Filled = true;
	}
	if (puzzle1ID == 4 && puzzle1ActorLoc == puzzle1Slot4) {
		::puzzle1ID4Loc = 4;
		::puzzle1Slot4Filled = true;
	}
	if (puzzle1ID == 4 && puzzle1ActorLoc == puzzle1Slot5) {
		::puzzle1ID4Loc = 5;
		::puzzle1Slot5Filled = true;
	}
	if (puzzle1ID == 4 && puzzle1ActorLoc == puzzle1Slot6) {
		::puzzle1ID4Loc = 6;
		::puzzle1Slot6Filled = true;
	}
	if (puzzle1ID == 4 && puzzle1ActorLoc == puzzle1Slot7) {
		::puzzle1ID4Loc = 7;
		::puzzle1Slot7Filled = true;
	}
	if (puzzle1ID == 4 && puzzle1ActorLoc == puzzle1Slot8) {
		::puzzle1ID4Loc = 8;
		::puzzle1Slot8Filled = true;
	}
	if (puzzle1ID == 4 && puzzle1ActorLoc == puzzle1Slot9) {
		::puzzle1ID4Loc = 9;
		::puzzle1Slot9Filled = true;
	}
	if (puzzle1ID == 5 && puzzle1ActorLoc == puzzle1Slot1) {
		::puzzle1ID5Loc = 1;
		::puzzle1Slot1Filled = true;
	}
	if (puzzle1ID == 5 && puzzle1ActorLoc == puzzle1Slot2) {
		::puzzle1ID5Loc = 2;
		::puzzle1Slot2Filled = true;
	}
	if (puzzle1ID == 5 && puzzle1ActorLoc == puzzle1Slot3) {
		::puzzle1ID5Loc = 3;
		::puzzle1Slot3Filled = true;
	}
	if (puzzle1ID == 5 && puzzle1ActorLoc == puzzle1Slot4) {
		::puzzle1ID5Loc = 4;
		::puzzle1Slot4Filled = true;
	}
	if (puzzle1ID == 5 && puzzle1ActorLoc == puzzle1Slot5) {
		::puzzle1ID5Loc = 5;
		::puzzle1Slot5Filled = true;
	}
	if (puzzle1ID == 5 && puzzle1ActorLoc == puzzle1Slot6) {
		::puzzle1ID5Loc = 6;
		::puzzle1Slot6Filled = true;
	}
	if (puzzle1ID == 5 && puzzle1ActorLoc == puzzle1Slot7) {
		::puzzle1ID5Loc = 7;
		::puzzle1Slot7Filled = true;
	}
	if (puzzle1ID == 5 && puzzle1ActorLoc == puzzle1Slot8) {
		::puzzle1ID5Loc = 8;
		::puzzle1Slot8Filled = true;
	}
	if (puzzle1ID == 5 && puzzle1ActorLoc == puzzle1Slot9) {
		::puzzle1ID5Loc = 9;
		::puzzle1Slot9Filled = true;
	}
	if (puzzle1ID == 6 && puzzle1ActorLoc == puzzle1Slot1) {
		::puzzle1ID6Loc = 1;
		::puzzle1Slot1Filled = true;
	}
	if (puzzle1ID == 6 && puzzle1ActorLoc == puzzle1Slot2) {
		::puzzle1ID6Loc = 2;
		::puzzle1Slot2Filled = true;
	}
	if (puzzle1ID == 6 && puzzle1ActorLoc == puzzle1Slot3) {
		::puzzle1ID6Loc = 3;
		::puzzle1Slot3Filled = true;
	}
	if (puzzle1ID == 6 && puzzle1ActorLoc == puzzle1Slot4) {
		::puzzle1ID6Loc = 4;
		::puzzle1Slot4Filled = true;
	}
	if (puzzle1ID == 6 && puzzle1ActorLoc == puzzle1Slot5) {
		::puzzle1ID6Loc = 5;
		::puzzle1Slot5Filled = true;
	}
	if (puzzle1ID == 6 && puzzle1ActorLoc == puzzle1Slot6) {
		::puzzle1ID6Loc = 6;
		::puzzle1Slot6Filled = true;
	}
	if (puzzle1ID == 6 && puzzle1ActorLoc == puzzle1Slot7) {
		::puzzle1ID6Loc = 7;
		::puzzle1Slot7Filled = true;
	}
	if (puzzle1ID == 6 && puzzle1ActorLoc == puzzle1Slot8) {
		::puzzle1ID6Loc = 8;
		::puzzle1Slot8Filled = true;
	}
	if (puzzle1ID == 6 && puzzle1ActorLoc == puzzle1Slot9) {
		::puzzle1ID6Loc = 9;
		::puzzle1Slot9Filled = true;
	}
	if (puzzle1ID == 7 && puzzle1ActorLoc == puzzle1Slot1) {
		::puzzle1ID7Loc = 1;
		::puzzle1Slot1Filled = true;
	}
	if (puzzle1ID == 7 && puzzle1ActorLoc == puzzle1Slot2) {
		::puzzle1ID7Loc = 2;
		::puzzle1Slot2Filled = true;
	}
	if (puzzle1ID == 7 && puzzle1ActorLoc == puzzle1Slot3) {
		::puzzle1ID7Loc = 3;
		::puzzle1Slot3Filled = true;
	}
	if (puzzle1ID == 7 && puzzle1ActorLoc == puzzle1Slot4) {
		::puzzle1ID7Loc = 4;
		::puzzle1Slot4Filled = true;
	}
	if (puzzle1ID == 7 && puzzle1ActorLoc == puzzle1Slot5) {
		::puzzle1ID7Loc = 5;
		::puzzle1Slot5Filled = true;
	}
	if (puzzle1ID == 7 && puzzle1ActorLoc == puzzle1Slot6) {
		::puzzle1ID7Loc = 6;
		::puzzle1Slot6Filled = true;
	}
	if (puzzle1ID == 7 && puzzle1ActorLoc == puzzle1Slot7) {
		::puzzle1ID7Loc = 7;
		::puzzle1Slot7Filled = true;
	}
	if (puzzle1ID == 7 && puzzle1ActorLoc == puzzle1Slot8) {
		::puzzle1ID7Loc = 8;
		::puzzle1Slot8Filled = true;
	}
	if (puzzle1ID == 7 && puzzle1ActorLoc == puzzle1Slot9) {
		::puzzle1ID7Loc = 9;
		::puzzle1Slot9Filled = true;
	}
	if (puzzle1ID == 8 && puzzle1ActorLoc == puzzle1Slot1) {
		::puzzle1ID8Loc = 1;
		::puzzle1Slot1Filled = true;
	}
	if (puzzle1ID == 8 && puzzle1ActorLoc == puzzle1Slot2) {
		::puzzle1ID8Loc = 2;
		::puzzle1Slot2Filled = true;
	}
	if (puzzle1ID == 8 && puzzle1ActorLoc == puzzle1Slot3) {
		::puzzle1ID8Loc = 3;
		::puzzle1Slot3Filled = true;
	}
	if (puzzle1ID == 8 && puzzle1ActorLoc == puzzle1Slot4) {
		::puzzle1ID8Loc = 4;
		::puzzle1Slot4Filled = true;
	}
	if (puzzle1ID == 8 && puzzle1ActorLoc == puzzle1Slot5) {
		::puzzle1ID8Loc = 5;
		::puzzle1Slot5Filled = true;
	}
	if (puzzle1ID == 8 && puzzle1ActorLoc == puzzle1Slot6) {
		::puzzle1ID8Loc = 6;
		::puzzle1Slot6Filled = true;
	}
	if (puzzle1ID == 8 && puzzle1ActorLoc == puzzle1Slot7) {
		::puzzle1ID8Loc = 7;
		::puzzle1Slot7Filled = true;
	}
	if (puzzle1ID == 8 && puzzle1ActorLoc == puzzle1Slot8) {
		::puzzle1ID8Loc = 8;
		::puzzle1Slot8Filled = true;
	}
	if (puzzle1ID == 8 && puzzle1ActorLoc == puzzle1Slot9) {
		::puzzle1ID8Loc = 9;
		::puzzle1Slot9Filled = true;
	}

	//Fill the empty slot as 0
	if (::puzzle1Slot1Filled == false) ::puzzle1CurrentArray[0] = 0;
	if (::puzzle1Slot2Filled == false) ::puzzle1CurrentArray[1] = 0;
	if (::puzzle1Slot3Filled == false) ::puzzle1CurrentArray[2] = 0;
	if (::puzzle1Slot4Filled == false) ::puzzle1CurrentArray[3] = 0;
	if (::puzzle1Slot5Filled == false) ::puzzle1CurrentArray[4] = 0;
	if (::puzzle1Slot6Filled == false) ::puzzle1CurrentArray[5] = 0;
	if (::puzzle1Slot7Filled == false) ::puzzle1CurrentArray[6] = 0;
	if (::puzzle1Slot8Filled == false) ::puzzle1CurrentArray[7] = 0;
	if (::puzzle1Slot9Filled == false) ::puzzle1CurrentArray[8] = 0;

	//Reset the filled values to update
	::puzzle1Slot1Filled = false;
	::puzzle1Slot2Filled = false;
	::puzzle1Slot3Filled = false;
	::puzzle1Slot4Filled = false;
	::puzzle1Slot5Filled = false;
	::puzzle1Slot6Filled = false;
	::puzzle1Slot7Filled = false;
	::puzzle1Slot8Filled = false;
	::puzzle1Slot9Filled = false;

	//Update progress
	if (::puzzle1ID1Loc == 7 && ::puzzle1ID1LocCheck == false) {
		::puzzle1Progress = ::puzzle1Progress + 12.5f;
		puzzle1ID1LocCheck = true;
	}
	if (::puzzle1ID2Loc == 5 && ::puzzle1ID2LocCheck == false) {
		::puzzle1Progress = ::puzzle1Progress + 12.5f;
		puzzle1ID2LocCheck = true;
	}
	if (::puzzle1ID3Loc == 8 && ::puzzle1ID3LocCheck == false) {
		::puzzle1Progress = ::puzzle1Progress + 12.5f;
		puzzle1ID3LocCheck = true;
	}
	if (::puzzle1ID4Loc == 2 && ::puzzle1ID4LocCheck == false) {
		::puzzle1Progress = ::puzzle1Progress + 12.5f;
		puzzle1ID4LocCheck = true;
	}
	if (::puzzle1ID5Loc == 1 && ::puzzle1ID5LocCheck == false) {
		::puzzle1Progress = ::puzzle1Progress + 12.5f;
		puzzle1ID5LocCheck = true;
	}
	if (::puzzle1ID6Loc == 4 && ::puzzle1ID6LocCheck == false) {
		::puzzle1Progress = ::puzzle1Progress + 12.5f;
		puzzle1ID6LocCheck = true;
	}
	if (::puzzle1ID7Loc == 6 && ::puzzle1ID7LocCheck == false) {
		::puzzle1Progress = ::puzzle1Progress + 12.5f;
		puzzle1ID7LocCheck = true;
	}
	if (::puzzle1ID8Loc == 3 && ::puzzle1ID8LocCheck == false) {
		::puzzle1Progress = ::puzzle1Progress + 12.5f;
		puzzle1ID8LocCheck = true;
	}
	if (::puzzle1ID1Loc != 7 && puzzle1ID1LocCheck == true) {
		::puzzle1Progress = ::puzzle1Progress - 12.5f;
		puzzle1ID1LocCheck = false;
	}
	if (::puzzle1ID2Loc != 5 && ::puzzle1ID2LocCheck == true) {
		::puzzle1Progress = ::puzzle1Progress - 12.5f;
		puzzle1ID2LocCheck = false;
	}
	if (::puzzle1ID3Loc != 8 && ::puzzle1ID3LocCheck == true) {
		::puzzle1Progress = ::puzzle1Progress - 12.5f;
		puzzle1ID3LocCheck = false;
	}
	if (::puzzle1ID4Loc != 2 && ::puzzle1ID4LocCheck == true) {
		::puzzle1Progress = ::puzzle1Progress - 12.5f;
		puzzle1ID4LocCheck = false;
	}
	if (::puzzle1ID5Loc != 1 && ::puzzle1ID5LocCheck == true) {
		::puzzle1Progress = ::puzzle1Progress - 12.5f;
		puzzle1ID5LocCheck = false;
	}
	if (::puzzle1ID6Loc != 4 && ::puzzle1ID6LocCheck == true) {
		::puzzle1Progress = ::puzzle1Progress - 12.5f;
		puzzle1ID6LocCheck = false;
	}
	if (::puzzle1ID7Loc != 6 && ::puzzle1ID7LocCheck == true) {
		::puzzle1Progress = ::puzzle1Progress - 12.5f;
		puzzle1ID7LocCheck = false;
	}
	if (::puzzle1ID8Loc != 3 && ::puzzle1ID8LocCheck == true) {
		::puzzle1Progress = ::puzzle1Progress - 12.5f;
		puzzle1ID8LocCheck = false;
	}

	//Ensure you cannot get negative progress
	if (::puzzle1Progress < 0) ::puzzle1Progress = 0;

	//Fill the array with block locations
	::puzzle1CurrentArray[puzzle1ID1Loc - 1] = 1;
	::puzzle1CurrentArray[puzzle1ID2Loc - 1] = 2;
	::puzzle1CurrentArray[puzzle1ID3Loc - 1] = 3;
	::puzzle1CurrentArray[puzzle1ID4Loc - 1] = 4;
	::puzzle1CurrentArray[puzzle1ID5Loc - 1] = 5;
	::puzzle1CurrentArray[puzzle1ID6Loc - 1] = 6;
	::puzzle1CurrentArray[puzzle1ID7Loc - 1] = 7;
	::puzzle1CurrentArray[puzzle1ID8Loc - 1] = 8;

	//Check if the player is following the predetermined puzzle1Path
	if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep1[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep1[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep1[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep1[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep1[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep1[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep1[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep1[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep1[8])) {
		::puzzle1PathStep = 1;
		::puzzle1Progress = (100.0f / 24.0f) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep2[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep2[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep2[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep2[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep2[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep2[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep2[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep2[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep2[8])) {
		::puzzle1PathStep = 2;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep3[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep3[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep3[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep3[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep3[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep3[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep3[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep3[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep3[8])) {
		::puzzle1PathStep = 3;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep4[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep4[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep4[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep4[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep4[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep4[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep4[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep4[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep4[8])) {
		::puzzle1PathStep = 4;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep5[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep5[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep5[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep5[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep5[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep5[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep5[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep5[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep5[8])) {
		::puzzle1PathStep = 5;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep6[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep6[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep6[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep6[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep6[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep6[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep6[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep6[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep6[8])) {
		::puzzle1PathStep = 6;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep7[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep7[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep7[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep7[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep7[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep7[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep7[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep7[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep7[8])) {
		::puzzle1PathStep = 7;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep8[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep8[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep8[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep8[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep8[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep8[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep8[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep8[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep8[8])) {
		::puzzle1PathStep = 8;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep9[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep9[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep9[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep9[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep9[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep9[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep9[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep9[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep9[8])) {
		::puzzle1PathStep = 9;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep10[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep10[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep10[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep10[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep10[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep10[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep10[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep10[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep10[8])) {
		::puzzle1PathStep = 10;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep11[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep11[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep11[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep11[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep11[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep11[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep11[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep11[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep11[8])) {
		::puzzle1PathStep = 11;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep12[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep12[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep12[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep12[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep12[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep12[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep12[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep12[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep12[8])) {
		::puzzle1PathStep = 12;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep13[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep13[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep13[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep13[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep13[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep13[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep13[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep13[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep13[8])) {
		::puzzle1PathStep = 13;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep14[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep14[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep14[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep14[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep14[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep14[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep14[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep14[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep14[8])) {
		::puzzle1PathStep = 14;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep15[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep15[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep15[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep15[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep15[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep15[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep15[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep15[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep15[8])) {
		::puzzle1PathStep = 15;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep16[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep16[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep16[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep16[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep16[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep16[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep16[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep16[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep16[8])) {
		::puzzle1PathStep = 16;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep17[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep17[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep17[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep17[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep17[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep17[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep17[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep17[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep17[8])) {
		::puzzle1PathStep = 17;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep18[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep18[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep18[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep18[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep18[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep18[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep18[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep18[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep18[8])) {
		::puzzle1PathStep = 18;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep19[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep19[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep19[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep19[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep19[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep19[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep19[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep19[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep19[8])) {
		::puzzle1PathStep = 19;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep20[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep20[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep20[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep20[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep20[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep20[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep20[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep20[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep20[8])) {
		::puzzle1PathStep = 20;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep21[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep21[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep21[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep21[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep21[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep21[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep21[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep21[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep21[8])) {
		::puzzle1PathStep = 21;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep22[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep22[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep22[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep22[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep22[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep22[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep22[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep22[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep22[8])) {
		::puzzle1PathStep = 22;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep23[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep23[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep23[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep23[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep23[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep23[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep23[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep23[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep23[8])) {
		::puzzle1PathStep = 23;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if ((::puzzle1CurrentArray[0] == ::puzzle1PathStep24[0]) && (::puzzle1CurrentArray[0] == ::puzzle1PathStep24[0]) && (::puzzle1CurrentArray[2] == ::puzzle1PathStep24[2]) && (::puzzle1CurrentArray[3] == ::puzzle1PathStep24[3]) && (::puzzle1CurrentArray[4] == ::puzzle1PathStep24[4]) && (::puzzle1CurrentArray[5] == ::puzzle1PathStep24[5]) && (::puzzle1CurrentArray[6] == ::puzzle1PathStep24[6]) && (::puzzle1CurrentArray[7] == ::puzzle1PathStep24[7]) && (::puzzle1CurrentArray[8] == ::puzzle1PathStep24[8])) {
		::puzzle1PathStep = 24;
		::puzzle1Progress = (100 / 24) * puzzle1PathStep;
		puzzle1OnPath = true;
		puzzle1BeenOnPath = true;
	}
	else if (::puzzle1ID1Loc == 1 && ::puzzle1ID2Loc == 2 && ::puzzle1ID3Loc == 3 && ::puzzle1ID4Loc == 4 && ::puzzle1ID5Loc == 5 && ::puzzle1ID6Loc == 6 && ::puzzle1ID7Loc == 7 && ::puzzle1ID8Loc == 8) {
	::puzzle1Progress = 100.0f;
	puzzle1OnPath = true;
	puzzle1BeenOnPath = true;
	::puzzle1Complete = true;
	}
	else if (puzzle1BeenOnPath == true) {
		::puzzle1PathStep = 0;
		::puzzle1Progress = 0;
		puzzle1OnPath = false;
		puzzle1BeenOnPath = false;
	}

	//Get the player's location
	playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	//Reset the puzzle
	if (playerLoc.X < -1620.0f && playerLoc.X > -1800.0f && playerLoc.Y > 950.0f && playerLoc.Y < 1130.0f) {
		if (puzzle1ID == 1) puzzle1ActorLoc = puzzle1Slot7;
		if (puzzle1ID == 2) puzzle1ActorLoc = puzzle1Slot5;
		if (puzzle1ID == 3) puzzle1ActorLoc = puzzle1Slot8;
		if (puzzle1ID == 4) puzzle1ActorLoc = puzzle1Slot2;
		if (puzzle1ID == 5) puzzle1ActorLoc = puzzle1Slot1;
		if (puzzle1ID == 6) puzzle1ActorLoc = puzzle1Slot4;
		if (puzzle1ID == 7) puzzle1ActorLoc = puzzle1Slot6;
		if (puzzle1ID == 8) puzzle1ActorLoc = puzzle1Slot3;
		if (puzzle1ID == 9) puzzle1ActorLoc = puzzle1Slot9;

		::puzzle1EmptySlot = 9;
	}
}

//When Up is pressed
void ASlidingPuzzle::UpReleased()
{
	::puzzle1Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > 550) && (playerLoc.Y < 830) && ::puzzle1Complete == false) {
	
		//Check if the block can be moved
		if (::puzzle1EmptySlot == 6) {
			if (puzzle1ActorLoc == puzzle1Slot9 && ::puzzle1InputCheck == false) {
				puzzle1ActorLoc = puzzle1Slot6;
				::puzzle1EmptySlot = 9;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 5 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot8) {
				puzzle1ActorLoc = puzzle1Slot5;
				::puzzle1EmptySlot = 8;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 4 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot7) {
				puzzle1ActorLoc = puzzle1Slot4;
				::puzzle1EmptySlot = 7;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 3 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot6) {
				puzzle1ActorLoc = puzzle1Slot3;
				::puzzle1EmptySlot = 6;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 2 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot5) {
				puzzle1ActorLoc = puzzle1Slot2;
				::puzzle1EmptySlot = 5;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 1 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot4) {
				puzzle1ActorLoc = puzzle1Slot1;
				::puzzle1EmptySlot = 4;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
	}
}

//When Down is pressed
void ASlidingPuzzle::DownReleased()
{
	::puzzle1Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > 550) && (playerLoc.Y < 830) && ::puzzle1Complete == false) {

		//Check if the block can be moved
		if (::puzzle1EmptySlot == 4 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot1) {
				puzzle1ActorLoc = puzzle1Slot4;
				::puzzle1EmptySlot = 1;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 5 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot2) {
				puzzle1ActorLoc = puzzle1Slot5;
				::puzzle1EmptySlot = 2;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 6 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot3) {
				puzzle1ActorLoc = puzzle1Slot6;
				::puzzle1EmptySlot = 3;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 7 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot4) {
				puzzle1ActorLoc = puzzle1Slot7;
				::puzzle1EmptySlot = 4;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 8 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot5) {
				puzzle1ActorLoc = puzzle1Slot8;
				::puzzle1EmptySlot = 5;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 9 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot6) {
				puzzle1ActorLoc = puzzle1Slot9;
				::puzzle1EmptySlot = 6;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
	}
}

//When Left is pressed
void ASlidingPuzzle::LeftReleased()
{
	::puzzle1Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > 550) && (playerLoc.Y < 830) && ::puzzle1Complete == false) {

		//Check if the block can be moved
		if (::puzzle1EmptySlot == 2 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot3) {
				puzzle1ActorLoc = puzzle1Slot2;
				::puzzle1EmptySlot = 3;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 5 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot6) {
				puzzle1ActorLoc = puzzle1Slot5;
				::puzzle1EmptySlot = 6;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 8 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot9) {
				puzzle1ActorLoc = puzzle1Slot8;
				::puzzle1EmptySlot = 9;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 1 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot2) {
				puzzle1ActorLoc = puzzle1Slot1;
				::puzzle1EmptySlot = 2;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 4 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot5) {
				puzzle1ActorLoc = puzzle1Slot4;
				::puzzle1EmptySlot = 5;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 7 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot8) {
				puzzle1ActorLoc = puzzle1Slot7;
				::puzzle1EmptySlot = 8;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
	}
}

//When Right is pressed
void ASlidingPuzzle::RightReleased()
{
	::puzzle1Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > 550) && (playerLoc.Y < 830) && ::puzzle1Complete == false) {

		//Check if the block can be moved
		if (::puzzle1EmptySlot == 2 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot1) {
				puzzle1ActorLoc = puzzle1Slot2;
				::puzzle1EmptySlot = 1;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 5 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot4) {
				puzzle1ActorLoc = puzzle1Slot5;
				::puzzle1EmptySlot = 4;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 8 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot7) {
				puzzle1ActorLoc = puzzle1Slot8;
				::puzzle1EmptySlot = 7;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 3 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot2) {
				puzzle1ActorLoc = puzzle1Slot3;
				::puzzle1EmptySlot = 2;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 6 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot5) {
				puzzle1ActorLoc = puzzle1Slot6;
				::puzzle1EmptySlot = 5;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
		if (::puzzle1EmptySlot == 9 && ::puzzle1InputCheck == false) {
			if (puzzle1ActorLoc == puzzle1Slot8) {
				puzzle1ActorLoc = puzzle1Slot9;
				::puzzle1EmptySlot = 8;
				::puzzle1InputCheck = true;
				::puzzle1Moves++;
			}
		}
	}
}

//Update the timer display
void ASlidingPuzzle::UpdateTimerDisplay() {
	TimerText->SetText(FString::FromInt(FMath::Max(::puzzle1ActiveTime, 0)));
}

//Make the timers tick
void ASlidingPuzzle::AdvanceTimer() {
	if (::puzzle1TempTick2 == false && ::puzzle1SlidingPuzzleRange == 1 && ::puzzle1Complete == false) {
		::puzzle1TempTick2 = true;
		::puzzle1RunningTime++;
	}
	if (::puzzle1TempTick == false && ::puzzle1CountdownCheck == true && ::puzzle1SlidingPuzzleRange == 1 && ::puzzle1Complete == false) {
		::puzzle1TempTick = true;
		::puzzle1ActiveTime++;
		::puzzle1Countdown--;
	}
	UpdateTimerDisplay();
	if (::puzzle1Countdown < 1) ::puzzle1CountdownCheck = false;
	else ::puzzle1CountdownCheck = true;
}

//When the timer hits 0
void ASlidingPuzzle::TimerHasFinished_Implementation() {
}