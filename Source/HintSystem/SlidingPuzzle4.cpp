// Fill out your copyright notice in the Description page of Project Settings.

#include "SlidingPuzzle4.h"
#include "SlidingPuzzle3.h"
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
extern bool debugPuzzle4;

//Whether the player is by the puzzle
extern int puzzle4SlidingPuzzleRange;

//Whether the previous puzzle is complete
extern bool puzzle3Complete;

//Total number of moves
int puzzle4Moves = 0;

//Whether Puzzle is complete 
bool puzzle4Complete = false;

//The value of the open puzzle slot
int puzzle4EmptySlot = 9;

//Regulate input
bool puzzle4InputCheck = false;

//Check if the player is on a predetermined Path
bool puzzle4OnPath = false;
bool puzzle4BeenOnPath = false;

//The player's running time
int puzzle4RunningTime = 0;
bool puzzle4TempTick2 = false;

//The player's active time
int puzzle4ActiveTime = 0;
int puzzle4Countdown = 5;
bool puzzle4TempTick = false;
bool puzzle4CountdownCheck = true;

//Each block's location
int puzzle4ID1Loc = 7;
int puzzle4ID2Loc = 5;
int puzzle4ID3Loc = 8;
int puzzle4ID4Loc = 2;
int puzzle4ID5Loc = 1;
int puzzle4ID6Loc = 4;
int puzzle4ID7Loc = 6;
int puzzle4ID8Loc = 3;

//Track puzzle progress
float puzzle4Progress = 0.0f;
bool puzzle4ID1LocCheck = false;
bool puzzle4ID2LocCheck = false;
bool puzzle4ID3LocCheck = false;
bool puzzle4ID4LocCheck = false;
bool puzzle4ID5LocCheck = false;
bool puzzle4ID6LocCheck = false;
bool puzzle4ID7LocCheck = false;
bool puzzle4ID8LocCheck = false;

//Path states
int puzzle4PathStep24[] = { 1, 2, 3, 4, 5, 6, 7, 0, 8 };
int puzzle4PathStep23[] = { 1, 2, 3, 4, 0, 6, 7, 5, 8 };
int puzzle4PathStep22[] = { 1, 2, 3, 0, 4, 6, 7, 5, 8 };
int puzzle4PathStep21[] = { 0, 2, 3, 1, 4, 6, 7, 5, 8 };
int puzzle4PathStep20[] = { 2, 0, 3, 1, 4, 6, 7, 5, 8 };
int puzzle4PathStep19[] = { 2, 4, 3, 1, 0, 6, 7, 5, 8 };
int puzzle4PathStep18[] = { 2, 4, 3, 1, 5, 6, 7, 0, 8 };
int puzzle4PathStep17[] = { 2, 4, 3, 1, 5, 6, 0, 7, 8 };
int puzzle4PathStep16[] = { 2, 4, 3, 0, 5, 6, 1, 7, 8 };
int puzzle4PathStep15[] = { 2, 4, 3, 5, 0, 6, 1, 7, 8 };
int puzzle4PathStep14[] = { 2, 4, 3, 5, 6, 0, 1, 7, 8 };
int puzzle4PathStep13[] = { 2, 4, 0, 5, 6, 3, 1, 7, 8 };
int puzzle4PathStep12[] = { 2, 0, 4, 5, 6, 3, 1, 7, 8 };
int puzzle4PathStep11[] = { 0, 2, 4, 5, 6, 3, 1, 7, 8 };
int puzzle4PathStep10[] = { 5, 2, 4, 0, 6, 3, 1, 7, 8 };
int puzzle4PathStep9[] = { 5, 2, 4, 6, 0, 3, 1, 7, 8 };
int puzzle4PathStep8[] = { 5, 2, 4, 6, 3, 0, 1, 7, 8 };
int puzzle4PathStep7[] = { 5, 2, 4, 6, 3, 8, 1, 7, 0 };
int puzzle4PathStep6[] = { 5, 2, 4, 6, 3, 8, 1, 0, 7 };
int puzzle4PathStep5[] = { 5, 2, 4, 6, 0, 8, 1, 3, 7 };
int puzzle4PathStep4[] = { 5, 0, 4, 6, 2, 8, 1, 3, 7 };
int puzzle4PathStep3[] = { 5, 4, 0, 6, 2, 8, 1, 3, 7 };
int puzzle4PathStep2[] = { 5, 4, 8, 6, 2, 0, 1, 3, 7 };
int puzzle4PathStep1[] = { 5, 4, 8, 6, 2, 7, 1, 3, 0 };
int puzzle4PathStep;

//Check if slots are filled
bool puzzle4Slot1Filled = false;
bool puzzle4Slot2Filled = false;
bool puzzle4Slot3Filled = false;
bool puzzle4Slot4Filled = false;
bool puzzle4Slot5Filled = false;
bool puzzle4Slot6Filled = false;
bool puzzle4Slot7Filled = false;
bool puzzle4Slot8Filled = false;
bool puzzle4Slot9Filled = false;

//Current State
int puzzle4CurrentArray[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Sets default values
ASlidingPuzzle4::ASlidingPuzzle4()
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
	puzzle4Slot1.X = (-2010.0f);
	puzzle4Slot1.Y = (-1210.0f);
	puzzle4Slot1.Z = (550.0f);
	puzzle4Slot2.X = (-2010.0f);
	puzzle4Slot2.Y = (-1330.0f);
	puzzle4Slot2.Z = (550.0f);
	puzzle4Slot3.X = (-2010.0f);
	puzzle4Slot3.Y = (-1450.0f);
	puzzle4Slot3.Z = (550.0f);
	puzzle4Slot4.X = (-2010.0f);
	puzzle4Slot4.Y = (-1210.0f);
	puzzle4Slot4.Z = (430.0f);
	puzzle4Slot5.X = (-2010.0f);
	puzzle4Slot5.Y = (-1330.0f);
	puzzle4Slot5.Z = (430.0f);
	puzzle4Slot6.X = (-2010.0f);
	puzzle4Slot6.Y = (-1450.0f);
	puzzle4Slot6.Z = (430.0f);
	puzzle4Slot7.X = (-2010.0f);
	puzzle4Slot7.Y = (-1210.0f);
	puzzle4Slot7.Z = (310.0f);
	puzzle4Slot8.X = (-2010.0f);
	puzzle4Slot8.Y = (-1330.0f);
	puzzle4Slot8.Z = (310.0f);
	puzzle4Slot9.X = (-2010.0f);
	puzzle4Slot9.Y = (-1450.0f);
	puzzle4Slot9.Z = (310.0f);

	//Default global values
	::puzzle4RunningTime = 0;
	::puzzle4ActiveTime = 0;
	::puzzle4Moves = 0;
	::puzzle4PathStep = 0;
	::puzzle4ID1Loc = 7;
	::puzzle4ID2Loc = 5;
	::puzzle4ID3Loc = 8;
	::puzzle4ID4Loc = 2;
	::puzzle4ID5Loc = 1;
	::puzzle4ID6Loc = 4;
	::puzzle4ID7Loc = 6;
	::puzzle4ID8Loc = 3;
	::puzzle4Countdown = 5;
	::puzzle4EmptySlot = 9;
	::puzzle4Progress = 0.0f;
	::puzzle4InputCheck = false;
	::puzzle4OnPath = false;
	::puzzle4BeenOnPath = false;
	::puzzle4ID1LocCheck = false;
	::puzzle4ID2LocCheck = false;
	::puzzle4ID3LocCheck = false;
	::puzzle4ID4LocCheck = false;
	::puzzle4ID5LocCheck = false;
	::puzzle4ID6LocCheck = false;
	::puzzle4ID7LocCheck = false;
	::puzzle4ID8LocCheck = false;
	::puzzle4Slot1Filled = false;
	::puzzle4Slot2Filled = false;
	::puzzle4Slot3Filled = false;
	::puzzle4Slot4Filled = false;
	::puzzle4Slot5Filled = false;
	::puzzle4Slot6Filled = false;
	::puzzle4Slot7Filled = false;
	::puzzle4Slot8Filled = false;
	::puzzle4Slot9Filled = false;
	::puzzle4Complete = false;
	::puzzle4TempTick = false;
	::puzzle4TempTick2 = false;
	::puzzle4CountdownCheck = true;

	//Update the blocks locations
	puzzle4ActorLoc = this->GetActorLocation();
}

// Called when the game starts or when spawned
void ASlidingPuzzle4::BeginPlay()
{
	Super::BeginPlay();

	//Start the timer
	UpdateTimerDisplay();
	GetWorldTimerManager().SetTimer(TimerTimerHandle, this, &ASlidingPuzzle4::AdvanceTimer, 1.0f, true);

	//Get the block's location
	puzzle4ActorLoc = this->GetActorLocation();

	//Give each block an ID
	if ((puzzle4ActorLoc.Y == puzzle4Slot7.Y) && (puzzle4ActorLoc.Z == puzzle4Slot7.Z)) puzzle4ID = 1;
	if ((puzzle4ActorLoc.Y == puzzle4Slot5.Y) && (puzzle4ActorLoc.Z == puzzle4Slot5.Z)) puzzle4ID = 2;
	if ((puzzle4ActorLoc.Y == puzzle4Slot8.Y) && (puzzle4ActorLoc.Z == puzzle4Slot8.Z)) puzzle4ID = 3;
	if ((puzzle4ActorLoc.Y == puzzle4Slot2.Y) && (puzzle4ActorLoc.Z == puzzle4Slot2.Z)) puzzle4ID = 4;
	if ((puzzle4ActorLoc.Y == puzzle4Slot1.Y) && (puzzle4ActorLoc.Z == puzzle4Slot1.Z)) puzzle4ID = 5;
	if ((puzzle4ActorLoc.Y == puzzle4Slot4.Y) && (puzzle4ActorLoc.Z == puzzle4Slot4.Z)) puzzle4ID = 6;
	if ((puzzle4ActorLoc.Y == puzzle4Slot6.Y) && (puzzle4ActorLoc.Z == puzzle4Slot6.Z)) puzzle4ID = 7;
	if ((puzzle4ActorLoc.Y == puzzle4Slot3.Y) && (puzzle4ActorLoc.Z == puzzle4Slot3.Z)) puzzle4ID = 8;
	if ((puzzle4ActorLoc.Y == puzzle4Slot9.Y) && (puzzle4ActorLoc.Z == puzzle4Slot9.Z)) puzzle4ID = 9;

	//Don't block inputs
	bBlockInput = false;

	//Enable keyboard input to control the puzzle
	PlayerController = GetWorld()->GetFirstPlayerController();
	EnableInput(PlayerController);
	InputComponent->bBlockInput = false;

	// Respond when keys are pressed
	FInputActionBinding& UpBinding = InputComponent->BindAction("SlideUp", IE_Released, this, &ASlidingPuzzle4::UpReleased);
	FInputActionBinding& DownBinding = InputComponent->BindAction("SlideDown", IE_Released, this, &ASlidingPuzzle4::DownReleased);
	FInputActionBinding& LeftBinding = InputComponent->BindAction("SlideLeft", IE_Released, this, &ASlidingPuzzle4::LeftReleased);
	FInputActionBinding& RightBinding = InputComponent->BindAction("SlideRight", IE_Released, this, &ASlidingPuzzle4::RightReleased);

	//Don't consume inputs
	UpBinding.bConsumeInput = false;
	DownBinding.bConsumeInput = false;
	LeftBinding.bConsumeInput = false;
	RightBinding.bConsumeInput = false;
}

// Called every frame
void ASlidingPuzzle4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (::puzzle3Complete == true && ::puzzle4Complete == false) {

		//Puts the puzzle on screen if previous is solved
		puzzle4ActorLoc.X = -2010.0f;

		//Update block's location if moved
		SetActorLocation(puzzle4ActorLoc);

		//Reset input check
		::puzzle4InputCheck = false;
		::puzzle4TempTick = false;
		::puzzle4TempTick2 = false;

		//Debug complete puzzle
		if (::debugPuzzle4 == true) {
			if (puzzle4ID == 1) puzzle4ActorLoc = puzzle4Slot1;
			if (puzzle4ID == 2) puzzle4ActorLoc = puzzle4Slot2;
			if (puzzle4ID == 3) puzzle4ActorLoc = puzzle4Slot3;
			if (puzzle4ID == 4) puzzle4ActorLoc = puzzle4Slot4;
			if (puzzle4ID == 5) puzzle4ActorLoc = puzzle4Slot5;
			if (puzzle4ID == 6) puzzle4ActorLoc = puzzle4Slot6;
			if (puzzle4ID == 7) puzzle4ActorLoc = puzzle4Slot7;
			if (puzzle4ID == 8) puzzle4ActorLoc = puzzle4Slot8;
			puzzle4Complete = true;
		}

		//Determine the location of each block
		if (puzzle4ID == 1 && puzzle4ActorLoc == puzzle4Slot1) {
			::puzzle4ID1Loc = 1;
			::puzzle4Slot1Filled = true;
		}
		if (puzzle4ID == 1 && puzzle4ActorLoc == puzzle4Slot2) {
			::puzzle4ID1Loc = 2;
			::puzzle4Slot2Filled = true;
		}
		if (puzzle4ID == 1 && puzzle4ActorLoc == puzzle4Slot3) {
			::puzzle4ID1Loc = 3;
			::puzzle4Slot3Filled = true;
		}
		if (puzzle4ID == 1 && puzzle4ActorLoc == puzzle4Slot4) {
			::puzzle4ID1Loc = 4;
			::puzzle4Slot4Filled = true;
		}
		if (puzzle4ID == 1 && puzzle4ActorLoc == puzzle4Slot5) {
			::puzzle4ID1Loc = 5;
			::puzzle4Slot5Filled = true;
		}
		if (puzzle4ID == 1 && puzzle4ActorLoc == puzzle4Slot6) {
			::puzzle4ID1Loc = 6;
			::puzzle4Slot6Filled = true;
		}
		if (puzzle4ID == 1 && puzzle4ActorLoc == puzzle4Slot7) {
			::puzzle4ID1Loc = 7;
			::puzzle4Slot7Filled = true;
		}
		if (puzzle4ID == 1 && puzzle4ActorLoc == puzzle4Slot8) {
			::puzzle4ID1Loc = 8;
			::puzzle4Slot8Filled = true;
		}
		if (puzzle4ID == 1 && puzzle4ActorLoc == puzzle4Slot9) {
			::puzzle4ID1Loc = 9;
			::puzzle4Slot9Filled = true;
		}
		if (puzzle4ID == 2 && puzzle4ActorLoc == puzzle4Slot1) {
			::puzzle4ID2Loc = 1;
			::puzzle4Slot1Filled = true;
		}
		if (puzzle4ID == 2 && puzzle4ActorLoc == puzzle4Slot2) {
			::puzzle4ID2Loc = 2;
			::puzzle4Slot2Filled = true;
		}
		if (puzzle4ID == 2 && puzzle4ActorLoc == puzzle4Slot3) {
			::puzzle4ID2Loc = 3;
			::puzzle4Slot3Filled = true;
		}
		if (puzzle4ID == 2 && puzzle4ActorLoc == puzzle4Slot4) {
			::puzzle4ID2Loc = 4;
			::puzzle4Slot4Filled = true;
		}
		if (puzzle4ID == 2 && puzzle4ActorLoc == puzzle4Slot5) {
			::puzzle4ID2Loc = 5;
			::puzzle4Slot5Filled = true;
		}
		if (puzzle4ID == 2 && puzzle4ActorLoc == puzzle4Slot6) {
			::puzzle4ID2Loc = 6;
			::puzzle4Slot6Filled = true;
		}
		if (puzzle4ID == 2 && puzzle4ActorLoc == puzzle4Slot7) {
			::puzzle4ID2Loc = 7;
			::puzzle4Slot7Filled = true;
		}
		if (puzzle4ID == 2 && puzzle4ActorLoc == puzzle4Slot8) {
			::puzzle4ID2Loc = 8;
			::puzzle4Slot8Filled = true;
		}
		if (puzzle4ID == 2 && puzzle4ActorLoc == puzzle4Slot9) {
			::puzzle4ID2Loc = 9;
			::puzzle4Slot9Filled = true;
		}
		if (puzzle4ID == 3 && puzzle4ActorLoc == puzzle4Slot1) {
			::puzzle4ID3Loc = 1;
			::puzzle4Slot1Filled = true;
		}
		if (puzzle4ID == 3 && puzzle4ActorLoc == puzzle4Slot2) {
			::puzzle4ID3Loc = 2;
			::puzzle4Slot2Filled = true;
		}
		if (puzzle4ID == 3 && puzzle4ActorLoc == puzzle4Slot3) {
			::puzzle4ID3Loc = 3;
			::puzzle4Slot3Filled = true;
		}
		if (puzzle4ID == 3 && puzzle4ActorLoc == puzzle4Slot4) {
			::puzzle4ID3Loc = 4;
			::puzzle4Slot4Filled = true;
		}
		if (puzzle4ID == 3 && puzzle4ActorLoc == puzzle4Slot5) {
			::puzzle4ID3Loc = 5;
			::puzzle4Slot5Filled = true;
		}
		if (puzzle4ID == 3 && puzzle4ActorLoc == puzzle4Slot6) {
			::puzzle4ID3Loc = 6;
			::puzzle4Slot6Filled = true;
		}
		if (puzzle4ID == 3 && puzzle4ActorLoc == puzzle4Slot7) {
			::puzzle4ID3Loc = 7;
			::puzzle4Slot7Filled = true;
		}
		if (puzzle4ID == 3 && puzzle4ActorLoc == puzzle4Slot8) {
			::puzzle4ID3Loc = 8;
			::puzzle4Slot8Filled = true;
		}
		if (puzzle4ID == 3 && puzzle4ActorLoc == puzzle4Slot9) {
			::puzzle4ID3Loc = 9;
			::puzzle4Slot9Filled = true;
		}
		if (puzzle4ID == 4 && puzzle4ActorLoc == puzzle4Slot1) {
			::puzzle4ID4Loc = 1;
			::puzzle4Slot1Filled = true;
		}
		if (puzzle4ID == 4 && puzzle4ActorLoc == puzzle4Slot2) {
			::puzzle4ID4Loc = 2;
			::puzzle4Slot2Filled = true;
		}
		if (puzzle4ID == 4 && puzzle4ActorLoc == puzzle4Slot3) {
			::puzzle4ID4Loc = 3;
			::puzzle4Slot3Filled = true;
		}
		if (puzzle4ID == 4 && puzzle4ActorLoc == puzzle4Slot4) {
			::puzzle4ID4Loc = 4;
			::puzzle4Slot4Filled = true;
		}
		if (puzzle4ID == 4 && puzzle4ActorLoc == puzzle4Slot5) {
			::puzzle4ID4Loc = 5;
			::puzzle4Slot5Filled = true;
		}
		if (puzzle4ID == 4 && puzzle4ActorLoc == puzzle4Slot6) {
			::puzzle4ID4Loc = 6;
			::puzzle4Slot6Filled = true;
		}
		if (puzzle4ID == 4 && puzzle4ActorLoc == puzzle4Slot7) {
			::puzzle4ID4Loc = 7;
			::puzzle4Slot7Filled = true;
		}
		if (puzzle4ID == 4 && puzzle4ActorLoc == puzzle4Slot8) {
			::puzzle4ID4Loc = 8;
			::puzzle4Slot8Filled = true;
		}
		if (puzzle4ID == 4 && puzzle4ActorLoc == puzzle4Slot9) {
			::puzzle4ID4Loc = 9;
			::puzzle4Slot9Filled = true;
		}
		if (puzzle4ID == 5 && puzzle4ActorLoc == puzzle4Slot1) {
			::puzzle4ID5Loc = 1;
			::puzzle4Slot1Filled = true;
		}
		if (puzzle4ID == 5 && puzzle4ActorLoc == puzzle4Slot2) {
			::puzzle4ID5Loc = 2;
			::puzzle4Slot2Filled = true;
		}
		if (puzzle4ID == 5 && puzzle4ActorLoc == puzzle4Slot3) {
			::puzzle4ID5Loc = 3;
			::puzzle4Slot3Filled = true;
		}
		if (puzzle4ID == 5 && puzzle4ActorLoc == puzzle4Slot4) {
			::puzzle4ID5Loc = 4;
			::puzzle4Slot4Filled = true;
		}
		if (puzzle4ID == 5 && puzzle4ActorLoc == puzzle4Slot5) {
			::puzzle4ID5Loc = 5;
			::puzzle4Slot5Filled = true;
		}
		if (puzzle4ID == 5 && puzzle4ActorLoc == puzzle4Slot6) {
			::puzzle4ID5Loc = 6;
			::puzzle4Slot6Filled = true;
		}
		if (puzzle4ID == 5 && puzzle4ActorLoc == puzzle4Slot7) {
			::puzzle4ID5Loc = 7;
			::puzzle4Slot7Filled = true;
		}
		if (puzzle4ID == 5 && puzzle4ActorLoc == puzzle4Slot8) {
			::puzzle4ID5Loc = 8;
			::puzzle4Slot8Filled = true;
		}
		if (puzzle4ID == 5 && puzzle4ActorLoc == puzzle4Slot9) {
			::puzzle4ID5Loc = 9;
			::puzzle4Slot9Filled = true;
		}
		if (puzzle4ID == 6 && puzzle4ActorLoc == puzzle4Slot1) {
			::puzzle4ID6Loc = 1;
			::puzzle4Slot1Filled = true;
		}
		if (puzzle4ID == 6 && puzzle4ActorLoc == puzzle4Slot2) {
			::puzzle4ID6Loc = 2;
			::puzzle4Slot2Filled = true;
		}
		if (puzzle4ID == 6 && puzzle4ActorLoc == puzzle4Slot3) {
			::puzzle4ID6Loc = 3;
			::puzzle4Slot3Filled = true;
		}
		if (puzzle4ID == 6 && puzzle4ActorLoc == puzzle4Slot4) {
			::puzzle4ID6Loc = 4;
			::puzzle4Slot4Filled = true;
		}
		if (puzzle4ID == 6 && puzzle4ActorLoc == puzzle4Slot5) {
			::puzzle4ID6Loc = 5;
			::puzzle4Slot5Filled = true;
		}
		if (puzzle4ID == 6 && puzzle4ActorLoc == puzzle4Slot6) {
			::puzzle4ID6Loc = 6;
			::puzzle4Slot6Filled = true;
		}
		if (puzzle4ID == 6 && puzzle4ActorLoc == puzzle4Slot7) {
			::puzzle4ID6Loc = 7;
			::puzzle4Slot7Filled = true;
		}
		if (puzzle4ID == 6 && puzzle4ActorLoc == puzzle4Slot8) {
			::puzzle4ID6Loc = 8;
			::puzzle4Slot8Filled = true;
		}
		if (puzzle4ID == 6 && puzzle4ActorLoc == puzzle4Slot9) {
			::puzzle4ID6Loc = 9;
			::puzzle4Slot9Filled = true;
		}
		if (puzzle4ID == 7 && puzzle4ActorLoc == puzzle4Slot1) {
			::puzzle4ID7Loc = 1;
			::puzzle4Slot1Filled = true;
		}
		if (puzzle4ID == 7 && puzzle4ActorLoc == puzzle4Slot2) {
			::puzzle4ID7Loc = 2;
			::puzzle4Slot2Filled = true;
		}
		if (puzzle4ID == 7 && puzzle4ActorLoc == puzzle4Slot3) {
			::puzzle4ID7Loc = 3;
			::puzzle4Slot3Filled = true;
		}
		if (puzzle4ID == 7 && puzzle4ActorLoc == puzzle4Slot4) {
			::puzzle4ID7Loc = 4;
			::puzzle4Slot4Filled = true;
		}
		if (puzzle4ID == 7 && puzzle4ActorLoc == puzzle4Slot5) {
			::puzzle4ID7Loc = 5;
			::puzzle4Slot5Filled = true;
		}
		if (puzzle4ID == 7 && puzzle4ActorLoc == puzzle4Slot6) {
			::puzzle4ID7Loc = 6;
			::puzzle4Slot6Filled = true;
		}
		if (puzzle4ID == 7 && puzzle4ActorLoc == puzzle4Slot7) {
			::puzzle4ID7Loc = 7;
			::puzzle4Slot7Filled = true;
		}
		if (puzzle4ID == 7 && puzzle4ActorLoc == puzzle4Slot8) {
			::puzzle4ID7Loc = 8;
			::puzzle4Slot8Filled = true;
		}
		if (puzzle4ID == 7 && puzzle4ActorLoc == puzzle4Slot9) {
			::puzzle4ID7Loc = 9;
			::puzzle4Slot9Filled = true;
		}
		if (puzzle4ID == 8 && puzzle4ActorLoc == puzzle4Slot1) {
			::puzzle4ID8Loc = 1;
			::puzzle4Slot1Filled = true;
		}
		if (puzzle4ID == 8 && puzzle4ActorLoc == puzzle4Slot2) {
			::puzzle4ID8Loc = 2;
			::puzzle4Slot2Filled = true;
		}
		if (puzzle4ID == 8 && puzzle4ActorLoc == puzzle4Slot3) {
			::puzzle4ID8Loc = 3;
			::puzzle4Slot3Filled = true;
		}
		if (puzzle4ID == 8 && puzzle4ActorLoc == puzzle4Slot4) {
			::puzzle4ID8Loc = 4;
			::puzzle4Slot4Filled = true;
		}
		if (puzzle4ID == 8 && puzzle4ActorLoc == puzzle4Slot5) {
			::puzzle4ID8Loc = 5;
			::puzzle4Slot5Filled = true;
		}
		if (puzzle4ID == 8 && puzzle4ActorLoc == puzzle4Slot6) {
			::puzzle4ID8Loc = 6;
			::puzzle4Slot6Filled = true;
		}
		if (puzzle4ID == 8 && puzzle4ActorLoc == puzzle4Slot7) {
			::puzzle4ID8Loc = 7;
			::puzzle4Slot7Filled = true;
		}
		if (puzzle4ID == 8 && puzzle4ActorLoc == puzzle4Slot8) {
			::puzzle4ID8Loc = 8;
			::puzzle4Slot8Filled = true;
		}
		if (puzzle4ID == 8 && puzzle4ActorLoc == puzzle4Slot9) {
			::puzzle4ID8Loc = 9;
			::puzzle4Slot9Filled = true;
		}

		//Fill the empty slot as 0
		if (::puzzle4Slot1Filled == false) ::puzzle4CurrentArray[0] = 0;
		if (::puzzle4Slot2Filled == false) ::puzzle4CurrentArray[1] = 0;
		if (::puzzle4Slot3Filled == false) ::puzzle4CurrentArray[2] = 0;
		if (::puzzle4Slot4Filled == false) ::puzzle4CurrentArray[3] = 0;
		if (::puzzle4Slot5Filled == false) ::puzzle4CurrentArray[4] = 0;
		if (::puzzle4Slot6Filled == false) ::puzzle4CurrentArray[5] = 0;
		if (::puzzle4Slot7Filled == false) ::puzzle4CurrentArray[6] = 0;
		if (::puzzle4Slot8Filled == false) ::puzzle4CurrentArray[7] = 0;
		if (::puzzle4Slot9Filled == false) ::puzzle4CurrentArray[8] = 0;

		//Reset the filled values to update
		::puzzle4Slot1Filled = false;
		::puzzle4Slot2Filled = false;
		::puzzle4Slot3Filled = false;
		::puzzle4Slot4Filled = false;
		::puzzle4Slot5Filled = false;
		::puzzle4Slot6Filled = false;
		::puzzle4Slot7Filled = false;
		::puzzle4Slot8Filled = false;
		::puzzle4Slot9Filled = false;

		//Update progress
		if (::puzzle4ID1Loc == 7 && ::puzzle4ID1LocCheck == false) {
			::puzzle4Progress = ::puzzle4Progress + 12.5f;
			puzzle4ID1LocCheck = true;
		}
		if (::puzzle4ID2Loc == 5 && ::puzzle4ID2LocCheck == false) {
			::puzzle4Progress = ::puzzle4Progress + 12.5f;
			puzzle4ID2LocCheck = true;
		}
		if (::puzzle4ID3Loc == 8 && ::puzzle4ID3LocCheck == false) {
			::puzzle4Progress = ::puzzle4Progress + 12.5f;
			puzzle4ID3LocCheck = true;
		}
		if (::puzzle4ID4Loc == 2 && ::puzzle4ID4LocCheck == false) {
			::puzzle4Progress = ::puzzle4Progress + 12.5f;
			puzzle4ID4LocCheck = true;
		}
		if (::puzzle4ID5Loc == 1 && ::puzzle4ID5LocCheck == false) {
			::puzzle4Progress = ::puzzle4Progress + 12.5f;
			puzzle4ID5LocCheck = true;
		}
		if (::puzzle4ID6Loc == 4 && ::puzzle4ID6LocCheck == false) {
			::puzzle4Progress = ::puzzle4Progress + 12.5f;
			puzzle4ID6LocCheck = true;
		}
		if (::puzzle4ID7Loc == 6 && ::puzzle4ID7LocCheck == false) {
			::puzzle4Progress = ::puzzle4Progress + 12.5f;
			puzzle4ID7LocCheck = true;
		}
		if (::puzzle4ID8Loc == 3 && ::puzzle4ID8LocCheck == false) {
			::puzzle4Progress = ::puzzle4Progress + 12.5f;
			puzzle4ID8LocCheck = true;
		}
		if (::puzzle4ID1Loc != 7 && puzzle4ID1LocCheck == true) {
			::puzzle4Progress = ::puzzle4Progress - 12.5f;
			puzzle4ID1LocCheck = false;
		}
		if (::puzzle4ID2Loc != 5 && ::puzzle4ID2LocCheck == true) {
			::puzzle4Progress = ::puzzle4Progress - 12.5f;
			puzzle4ID2LocCheck = false;
		}
		if (::puzzle4ID3Loc != 8 && ::puzzle4ID3LocCheck == true) {
			::puzzle4Progress = ::puzzle4Progress - 12.5f;
			puzzle4ID3LocCheck = false;
		}
		if (::puzzle4ID4Loc != 2 && ::puzzle4ID4LocCheck == true) {
			::puzzle4Progress = ::puzzle4Progress - 12.5f;
			puzzle4ID4LocCheck = false;
		}
		if (::puzzle4ID5Loc != 1 && ::puzzle4ID5LocCheck == true) {
			::puzzle4Progress = ::puzzle4Progress - 12.5f;
			puzzle4ID5LocCheck = false;
		}
		if (::puzzle4ID6Loc != 4 && ::puzzle4ID6LocCheck == true) {
			::puzzle4Progress = ::puzzle4Progress - 12.5f;
			puzzle4ID6LocCheck = false;
		}
		if (::puzzle4ID7Loc != 6 && ::puzzle4ID7LocCheck == true) {
			::puzzle4Progress = ::puzzle4Progress - 12.5f;
			puzzle4ID7LocCheck = false;
		}
		if (::puzzle4ID8Loc != 3 && ::puzzle4ID8LocCheck == true) {
			::puzzle4Progress = ::puzzle4Progress - 12.5f;
			puzzle4ID8LocCheck = false;
		}

		//Ensure you cannot get negative progress
		if (::puzzle4Progress < 0) ::puzzle4Progress = 0;

		//Fill the array with block locations
		::puzzle4CurrentArray[puzzle4ID1Loc - 1] = 1;
		::puzzle4CurrentArray[puzzle4ID2Loc - 1] = 2;
		::puzzle4CurrentArray[puzzle4ID3Loc - 1] = 3;
		::puzzle4CurrentArray[puzzle4ID4Loc - 1] = 4;
		::puzzle4CurrentArray[puzzle4ID5Loc - 1] = 5;
		::puzzle4CurrentArray[puzzle4ID6Loc - 1] = 6;
		::puzzle4CurrentArray[puzzle4ID7Loc - 1] = 7;
		::puzzle4CurrentArray[puzzle4ID8Loc - 1] = 8;

		//Check if the player is following the predetermined puzzle4Path
		if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep1[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep1[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep1[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep1[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep1[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep1[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep1[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep1[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep1[8])) {
			::puzzle4PathStep = 1;
			::puzzle4Progress = (100.0f / 24.0f) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep2[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep2[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep2[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep2[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep2[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep2[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep2[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep2[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep2[8])) {
			::puzzle4PathStep = 2;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep3[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep3[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep3[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep3[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep3[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep3[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep3[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep3[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep3[8])) {
			::puzzle4PathStep = 3;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep4[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep4[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep4[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep4[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep4[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep4[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep4[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep4[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep4[8])) {
			::puzzle4PathStep = 4;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep5[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep5[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep5[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep5[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep5[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep5[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep5[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep5[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep5[8])) {
			::puzzle4PathStep = 5;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep6[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep6[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep6[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep6[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep6[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep6[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep6[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep6[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep6[8])) {
			::puzzle4PathStep = 6;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep7[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep7[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep7[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep7[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep7[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep7[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep7[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep7[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep7[8])) {
			::puzzle4PathStep = 7;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep8[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep8[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep8[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep8[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep8[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep8[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep8[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep8[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep8[8])) {
			::puzzle4PathStep = 8;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep9[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep9[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep9[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep9[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep9[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep9[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep9[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep9[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep9[8])) {
			::puzzle4PathStep = 9;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep10[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep10[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep10[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep10[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep10[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep10[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep10[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep10[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep10[8])) {
			::puzzle4PathStep = 10;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep11[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep11[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep11[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep11[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep11[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep11[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep11[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep11[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep11[8])) {
			::puzzle4PathStep = 11;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep12[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep12[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep12[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep12[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep12[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep12[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep12[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep12[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep12[8])) {
			::puzzle4PathStep = 12;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep13[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep13[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep13[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep13[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep13[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep13[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep13[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep13[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep13[8])) {
			::puzzle4PathStep = 13;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep14[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep14[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep14[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep14[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep14[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep14[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep14[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep14[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep14[8])) {
			::puzzle4PathStep = 14;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep15[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep15[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep15[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep15[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep15[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep15[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep15[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep15[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep15[8])) {
			::puzzle4PathStep = 15;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep16[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep16[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep16[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep16[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep16[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep16[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep16[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep16[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep16[8])) {
			::puzzle4PathStep = 16;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep17[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep17[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep17[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep17[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep17[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep17[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep17[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep17[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep17[8])) {
			::puzzle4PathStep = 17;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep18[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep18[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep18[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep18[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep18[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep18[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep18[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep18[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep18[8])) {
			::puzzle4PathStep = 18;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep19[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep19[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep19[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep19[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep19[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep19[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep19[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep19[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep19[8])) {
			::puzzle4PathStep = 19;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep20[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep20[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep20[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep20[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep20[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep20[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep20[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep20[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep20[8])) {
			::puzzle4PathStep = 20;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep21[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep21[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep21[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep21[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep21[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep21[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep21[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep21[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep21[8])) {
			::puzzle4PathStep = 21;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep22[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep22[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep22[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep22[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep22[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep22[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep22[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep22[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep22[8])) {
			::puzzle4PathStep = 22;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep23[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep23[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep23[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep23[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep23[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep23[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep23[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep23[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep23[8])) {
			::puzzle4PathStep = 23;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if ((::puzzle4CurrentArray[0] == ::puzzle4PathStep24[0]) && (::puzzle4CurrentArray[0] == ::puzzle4PathStep24[0]) && (::puzzle4CurrentArray[2] == ::puzzle4PathStep24[2]) && (::puzzle4CurrentArray[3] == ::puzzle4PathStep24[3]) && (::puzzle4CurrentArray[4] == ::puzzle4PathStep24[4]) && (::puzzle4CurrentArray[5] == ::puzzle4PathStep24[5]) && (::puzzle4CurrentArray[6] == ::puzzle4PathStep24[6]) && (::puzzle4CurrentArray[7] == ::puzzle4PathStep24[7]) && (::puzzle4CurrentArray[8] == ::puzzle4PathStep24[8])) {
			::puzzle4PathStep = 24;
			::puzzle4Progress = (100 / 24) * puzzle4PathStep;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
		}
		else if (::puzzle4ID1Loc == 1 && ::puzzle4ID2Loc == 2 && ::puzzle4ID3Loc == 3 && ::puzzle4ID4Loc == 4 && ::puzzle4ID5Loc == 5 && ::puzzle4ID6Loc == 6 && ::puzzle4ID7Loc == 7 && ::puzzle4ID8Loc == 8) {
			::puzzle4Progress = 100.0f;
			puzzle4OnPath = true;
			puzzle4BeenOnPath = true;
			::puzzle4Complete = true;
		}
		else if (puzzle4BeenOnPath == true) {
			::puzzle4PathStep = 0;
			::puzzle4Progress = 0;
			puzzle4OnPath = false;
			puzzle4BeenOnPath = false;
		}

		//Get the player's location
		playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		//Reset the puzzle
		if (playerLoc.X < -1620.0f && playerLoc.X > -1800.0f && playerLoc.Y > -1060.0f && playerLoc.Y < -880.0f) {
			if (puzzle4ID == 1) puzzle4ActorLoc = puzzle4Slot7;
			if (puzzle4ID == 2) puzzle4ActorLoc = puzzle4Slot5;
			if (puzzle4ID == 3) puzzle4ActorLoc = puzzle4Slot8;
			if (puzzle4ID == 4) puzzle4ActorLoc = puzzle4Slot2;
			if (puzzle4ID == 5) puzzle4ActorLoc = puzzle4Slot1;
			if (puzzle4ID == 6) puzzle4ActorLoc = puzzle4Slot4;
			if (puzzle4ID == 7) puzzle4ActorLoc = puzzle4Slot6;
			if (puzzle4ID == 8) puzzle4ActorLoc = puzzle4Slot3;
			if (puzzle4ID == 9) puzzle4ActorLoc = puzzle4Slot9;

			::puzzle4EmptySlot = 9;
		}
	}
}

//When Up is pressed
void ASlidingPuzzle4::UpReleased()
{
	::puzzle4Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -1460) && (playerLoc.Y < -1180) && ::puzzle4Complete == false && ::puzzle3Complete == true) {

		//Check if the block can be moved
		if (::puzzle4EmptySlot == 6) {
			if (puzzle4ActorLoc == puzzle4Slot9 && ::puzzle4InputCheck == false) {
				puzzle4ActorLoc = puzzle4Slot6;
				::puzzle4EmptySlot = 9;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 5 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot8) {
				puzzle4ActorLoc = puzzle4Slot5;
				::puzzle4EmptySlot = 8;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 4 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot7) {
				puzzle4ActorLoc = puzzle4Slot4;
				::puzzle4EmptySlot = 7;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 3 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot6) {
				puzzle4ActorLoc = puzzle4Slot3;
				::puzzle4EmptySlot = 6;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 2 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot5) {
				puzzle4ActorLoc = puzzle4Slot2;
				::puzzle4EmptySlot = 5;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 1 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot4) {
				puzzle4ActorLoc = puzzle4Slot1;
				::puzzle4EmptySlot = 4;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
	}
}

//When Down is pressed
void ASlidingPuzzle4::DownReleased()
{
	::puzzle4Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -1460) && (playerLoc.Y < -1180) && ::puzzle4Complete == false && ::puzzle3Complete == true) {

		//Check if the block can be moved
		if (::puzzle4EmptySlot == 4 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot1) {
				puzzle4ActorLoc = puzzle4Slot4;
				::puzzle4EmptySlot = 1;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 5 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot2) {
				puzzle4ActorLoc = puzzle4Slot5;
				::puzzle4EmptySlot = 2;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 6 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot3) {
				puzzle4ActorLoc = puzzle4Slot6;
				::puzzle4EmptySlot = 3;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 7 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot4) {
				puzzle4ActorLoc = puzzle4Slot7;
				::puzzle4EmptySlot = 4;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 8 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot5) {
				puzzle4ActorLoc = puzzle4Slot8;
				::puzzle4EmptySlot = 5;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 9 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot6) {
				puzzle4ActorLoc = puzzle4Slot9;
				::puzzle4EmptySlot = 6;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
	}
}

//When Left is pressed
void ASlidingPuzzle4::LeftReleased()
{
	::puzzle4Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -1460) && (playerLoc.Y < -1180) && ::puzzle4Complete == false && ::puzzle3Complete == true) {

		//Check if the block can be moved
		if (::puzzle4EmptySlot == 2 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot3) {
				puzzle4ActorLoc = puzzle4Slot2;
				::puzzle4EmptySlot = 3;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 5 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot6) {
				puzzle4ActorLoc = puzzle4Slot5;
				::puzzle4EmptySlot = 6;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 8 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot9) {
				puzzle4ActorLoc = puzzle4Slot8;
				::puzzle4EmptySlot = 9;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 1 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot2) {
				puzzle4ActorLoc = puzzle4Slot1;
				::puzzle4EmptySlot = 2;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 4 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot5) {
				puzzle4ActorLoc = puzzle4Slot4;
				::puzzle4EmptySlot = 5;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 7 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot8) {
				puzzle4ActorLoc = puzzle4Slot7;
				::puzzle4EmptySlot = 8;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
	}
}

//When Right is pressed
void ASlidingPuzzle4::RightReleased()
{
	::puzzle4Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -1460) && (playerLoc.Y < -1180) && ::puzzle4Complete == false && ::puzzle3Complete == true) {

		//Check if the block can be moved
		if (::puzzle4EmptySlot == 2 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot1) {
				puzzle4ActorLoc = puzzle4Slot2;
				::puzzle4EmptySlot = 1;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 5 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot4) {
				puzzle4ActorLoc = puzzle4Slot5;
				::puzzle4EmptySlot = 4;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 8 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot7) {
				puzzle4ActorLoc = puzzle4Slot8;
				::puzzle4EmptySlot = 7;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 3 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot2) {
				puzzle4ActorLoc = puzzle4Slot3;
				::puzzle4EmptySlot = 2;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 6 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot5) {
				puzzle4ActorLoc = puzzle4Slot6;
				::puzzle4EmptySlot = 5;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
		if (::puzzle4EmptySlot == 9 && ::puzzle4InputCheck == false) {
			if (puzzle4ActorLoc == puzzle4Slot8) {
				puzzle4ActorLoc = puzzle4Slot9;
				::puzzle4EmptySlot = 8;
				::puzzle4InputCheck = true;
				::puzzle4Moves++;
			}
		}
	}
}

//Update the timer display
void ASlidingPuzzle4::UpdateTimerDisplay() {
	TimerText->SetText(FString::FromInt(FMath::Max(::puzzle4ActiveTime, 0)));
}

//Make the timers tick
void ASlidingPuzzle4::AdvanceTimer() {
	if (::puzzle4TempTick2 == false && ::puzzle4SlidingPuzzleRange == 1 && ::puzzle4Complete == false && ::puzzle3Complete == true) {
		::puzzle4TempTick2 = true;
		::puzzle4RunningTime++;
	}
	if (::puzzle4TempTick == false && ::puzzle4CountdownCheck == true && ::puzzle4SlidingPuzzleRange == 1 && ::puzzle4Complete == false && ::puzzle3Complete == true) {
		::puzzle4TempTick = true;
		::puzzle4ActiveTime++;
		::puzzle4Countdown--;
	}
	UpdateTimerDisplay();
	if (::puzzle4Countdown < 1) ::puzzle4CountdownCheck = false;
	else ::puzzle4CountdownCheck = true;
}

//When the timer hits 0
void ASlidingPuzzle4::TimerHasFinished_Implementation() {
}