// Fill out your copyright notice in the Description page of Project Settings.

#include "SlidingPuzzle3.h"
#include "SlidingPuzzle2.h"
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
extern bool debugPuzzle3;

//Whether the player is by the puzzle
extern int puzzle3SlidingPuzzleRange;

//Whether the previous puzzle is complete
extern bool puzzle2Complete;

//Total number of moves
int puzzle3Moves = 0;

//Whether Puzzle is complete 
bool puzzle3Complete = false;

//The value of the open puzzle slot
int puzzle3EmptySlot = 9;

//Regulate input
bool puzzle3InputCheck = false;

//Check if the player is on a predetermined Path
bool puzzle3OnPath = false;
bool puzzle3BeenOnPath = false;

//The player's running time
int puzzle3RunningTime = 0;
bool puzzle3TempTick2 = false;

//The player's active time
int puzzle3ActiveTime = 0;
int puzzle3Countdown = 5;
bool puzzle3TempTick = false;
bool puzzle3CountdownCheck = true;

//Each block's location
int puzzle3ID1Loc = 7;
int puzzle3ID2Loc = 5;
int puzzle3ID3Loc = 8;
int puzzle3ID4Loc = 2;
int puzzle3ID5Loc = 1;
int puzzle3ID6Loc = 4;
int puzzle3ID7Loc = 6;
int puzzle3ID8Loc = 3;

//Track puzzle progress
float puzzle3Progress = 0.0f;
bool puzzle3ID1LocCheck = false;
bool puzzle3ID2LocCheck = false;
bool puzzle3ID3LocCheck = false;
bool puzzle3ID4LocCheck = false;
bool puzzle3ID5LocCheck = false;
bool puzzle3ID6LocCheck = false;
bool puzzle3ID7LocCheck = false;
bool puzzle3ID8LocCheck = false;

//Path states
int puzzle3PathStep24[] = { 1, 2, 3, 4, 5, 6, 7, 0, 8 };
int puzzle3PathStep23[] = { 1, 2, 3, 4, 0, 6, 7, 5, 8 };
int puzzle3PathStep22[] = { 1, 2, 3, 0, 4, 6, 7, 5, 8 };
int puzzle3PathStep21[] = { 0, 2, 3, 1, 4, 6, 7, 5, 8 };
int puzzle3PathStep20[] = { 2, 0, 3, 1, 4, 6, 7, 5, 8 };
int puzzle3PathStep19[] = { 2, 4, 3, 1, 0, 6, 7, 5, 8 };
int puzzle3PathStep18[] = { 2, 4, 3, 1, 5, 6, 7, 0, 8 };
int puzzle3PathStep17[] = { 2, 4, 3, 1, 5, 6, 0, 7, 8 };
int puzzle3PathStep16[] = { 2, 4, 3, 0, 5, 6, 1, 7, 8 };
int puzzle3PathStep15[] = { 2, 4, 3, 5, 0, 6, 1, 7, 8 };
int puzzle3PathStep14[] = { 2, 4, 3, 5, 6, 0, 1, 7, 8 };
int puzzle3PathStep13[] = { 2, 4, 0, 5, 6, 3, 1, 7, 8 };
int puzzle3PathStep12[] = { 2, 0, 4, 5, 6, 3, 1, 7, 8 };
int puzzle3PathStep11[] = { 0, 2, 4, 5, 6, 3, 1, 7, 8 };
int puzzle3PathStep10[] = { 5, 2, 4, 0, 6, 3, 1, 7, 8 };
int puzzle3PathStep9[] = { 5, 2, 4, 6, 0, 3, 1, 7, 8 };
int puzzle3PathStep8[] = { 5, 2, 4, 6, 3, 0, 1, 7, 8 };
int puzzle3PathStep7[] = { 5, 2, 4, 6, 3, 8, 1, 7, 0 };
int puzzle3PathStep6[] = { 5, 2, 4, 6, 3, 8, 1, 0, 7 };
int puzzle3PathStep5[] = { 5, 2, 4, 6, 0, 8, 1, 3, 7 };
int puzzle3PathStep4[] = { 5, 0, 4, 6, 2, 8, 1, 3, 7 };
int puzzle3PathStep3[] = { 5, 4, 0, 6, 2, 8, 1, 3, 7 };
int puzzle3PathStep2[] = { 5, 4, 8, 6, 2, 0, 1, 3, 7 };
int puzzle3PathStep1[] = { 5, 4, 8, 6, 2, 7, 1, 3, 0 };
int puzzle3PathStep;

//Check if slots are filled
bool puzzle3Slot1Filled = false;
bool puzzle3Slot2Filled = false;
bool puzzle3Slot3Filled = false;
bool puzzle3Slot4Filled = false;
bool puzzle3Slot5Filled = false;
bool puzzle3Slot6Filled = false;
bool puzzle3Slot7Filled = false;
bool puzzle3Slot8Filled = false;
bool puzzle3Slot9Filled = false;

//Current State
int puzzle3CurrentArray[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Sets default values
ASlidingPuzzle3::ASlidingPuzzle3()
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
	puzzle3Slot1.X = (-2010.0f);
	puzzle3Slot1.Y = (-540.0f);
	puzzle3Slot1.Z = (550.0f);
	puzzle3Slot2.X = (-2010.0f);
	puzzle3Slot2.Y = (-660.0f);
	puzzle3Slot2.Z = (550.0f);
	puzzle3Slot3.X = (-2010.0f);
	puzzle3Slot3.Y = (-780.0f);
	puzzle3Slot3.Z = (550.0f);
	puzzle3Slot4.X = (-2010.0f);
	puzzle3Slot4.Y = (-540.0f);
	puzzle3Slot4.Z = (430.0f);
	puzzle3Slot5.X = (-2010.0f);
	puzzle3Slot5.Y = (-660.0f);
	puzzle3Slot5.Z = (430.0f);
	puzzle3Slot6.X = (-2010.0f);
	puzzle3Slot6.Y = (-780.0f);
	puzzle3Slot6.Z = (430.0f);
	puzzle3Slot7.X = (-2010.0f);
	puzzle3Slot7.Y = (-540.0f);
	puzzle3Slot7.Z = (310.0f);
	puzzle3Slot8.X = (-2010.0f);
	puzzle3Slot8.Y = (-660.0f);
	puzzle3Slot8.Z = (310.0f);
	puzzle3Slot9.X = (-2010.0f);
	puzzle3Slot9.Y = (-780.0f);
	puzzle3Slot9.Z = (310.0f);

	//Default global values
	::puzzle3RunningTime = 0;
	::puzzle3ActiveTime = 0;
	::puzzle3Moves = 0;
	::puzzle3PathStep = 0;
	::puzzle3ID1Loc = 7;
	::puzzle3ID2Loc = 5;
	::puzzle3ID3Loc = 8;
	::puzzle3ID4Loc = 2;
	::puzzle3ID5Loc = 1;
	::puzzle3ID6Loc = 4;
	::puzzle3ID7Loc = 6;
	::puzzle3ID8Loc = 3;
	::puzzle3Countdown = 5;
	::puzzle3EmptySlot = 9;
	::puzzle3Progress = 0.0f;
	::puzzle3InputCheck = false;
	::puzzle3OnPath = false;
	::puzzle3BeenOnPath = false;
	::puzzle3ID1LocCheck = false;
	::puzzle3ID2LocCheck = false;
	::puzzle3ID3LocCheck = false;
	::puzzle3ID4LocCheck = false;
	::puzzle3ID5LocCheck = false;
	::puzzle3ID6LocCheck = false;
	::puzzle3ID7LocCheck = false;
	::puzzle3ID8LocCheck = false;
	::puzzle3Slot1Filled = false;
	::puzzle3Slot2Filled = false;
	::puzzle3Slot3Filled = false;
	::puzzle3Slot4Filled = false;
	::puzzle3Slot5Filled = false;
	::puzzle3Slot6Filled = false;
	::puzzle3Slot7Filled = false;
	::puzzle3Slot8Filled = false;
	::puzzle3Slot9Filled = false;
	::puzzle3Complete = false;
	::puzzle3TempTick = false;
	::puzzle3TempTick2 = false;
	::puzzle3CountdownCheck = true;

	//Update the blocks locations
	puzzle3ActorLoc = this->GetActorLocation();
}

// Called when the game starts or when spawned
void ASlidingPuzzle3::BeginPlay()
{
	Super::BeginPlay();

	//Start the timer
	UpdateTimerDisplay();
	GetWorldTimerManager().SetTimer(TimerTimerHandle, this, &ASlidingPuzzle3::AdvanceTimer, 1.0f, true);

	//Get the block's location
	puzzle3ActorLoc = this->GetActorLocation();

	//Give each block an ID
	if ((puzzle3ActorLoc.Y == puzzle3Slot7.Y) && (puzzle3ActorLoc.Z == puzzle3Slot7.Z)) puzzle3ID = 1;
	if ((puzzle3ActorLoc.Y == puzzle3Slot5.Y) && (puzzle3ActorLoc.Z == puzzle3Slot5.Z)) puzzle3ID = 2;
	if ((puzzle3ActorLoc.Y == puzzle3Slot8.Y) && (puzzle3ActorLoc.Z == puzzle3Slot8.Z)) puzzle3ID = 3;
	if ((puzzle3ActorLoc.Y == puzzle3Slot2.Y) && (puzzle3ActorLoc.Z == puzzle3Slot2.Z)) puzzle3ID = 4;
	if ((puzzle3ActorLoc.Y == puzzle3Slot1.Y) && (puzzle3ActorLoc.Z == puzzle3Slot1.Z)) puzzle3ID = 5;
	if ((puzzle3ActorLoc.Y == puzzle3Slot4.Y) && (puzzle3ActorLoc.Z == puzzle3Slot4.Z)) puzzle3ID = 6;
	if ((puzzle3ActorLoc.Y == puzzle3Slot6.Y) && (puzzle3ActorLoc.Z == puzzle3Slot6.Z)) puzzle3ID = 7;
	if ((puzzle3ActorLoc.Y == puzzle3Slot3.Y) && (puzzle3ActorLoc.Z == puzzle3Slot3.Z)) puzzle3ID = 8;
	if ((puzzle3ActorLoc.Y == puzzle3Slot9.Y) && (puzzle3ActorLoc.Z == puzzle3Slot9.Z)) puzzle3ID = 9;

	//Don't block inputs
	bBlockInput = false;

	//Enable keyboard input to control the puzzle
	PlayerController = GetWorld()->GetFirstPlayerController();
	EnableInput(PlayerController);
	InputComponent->bBlockInput = false;

	// Respond when keys are pressed
	FInputActionBinding& UpBinding = InputComponent->BindAction("SlideUp", IE_Released, this, &ASlidingPuzzle3::UpReleased);
	FInputActionBinding& DownBinding = InputComponent->BindAction("SlideDown", IE_Released, this, &ASlidingPuzzle3::DownReleased);
	FInputActionBinding& LeftBinding = InputComponent->BindAction("SlideLeft", IE_Released, this, &ASlidingPuzzle3::LeftReleased);
	FInputActionBinding& RightBinding = InputComponent->BindAction("SlideRight", IE_Released, this, &ASlidingPuzzle3::RightReleased);

	//Don't consume inputs
	UpBinding.bConsumeInput = false;
	DownBinding.bConsumeInput = false;
	LeftBinding.bConsumeInput = false;
	RightBinding.bConsumeInput = false;
}

// Called every frame
void ASlidingPuzzle3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (::puzzle2Complete == true && ::puzzle3Complete == false) {

		//Puts the puzzle on screen if previous is solved
		puzzle3ActorLoc.X = -2010.0f;

		//Update block's location if moved
		SetActorLocation(puzzle3ActorLoc);

		//Reset input check
		::puzzle3InputCheck = false;
		::puzzle3TempTick = false;
		::puzzle3TempTick2 = false;

		//Debug complete puzzle
		if (::debugPuzzle3 == true) {
			if (puzzle3ID == 1) puzzle3ActorLoc = puzzle3Slot1;
			if (puzzle3ID == 2) puzzle3ActorLoc = puzzle3Slot2;
			if (puzzle3ID == 3) puzzle3ActorLoc = puzzle3Slot3;
			if (puzzle3ID == 4) puzzle3ActorLoc = puzzle3Slot4;
			if (puzzle3ID == 5) puzzle3ActorLoc = puzzle3Slot5;
			if (puzzle3ID == 6) puzzle3ActorLoc = puzzle3Slot6;
			if (puzzle3ID == 7) puzzle3ActorLoc = puzzle3Slot7;
			if (puzzle3ID == 8) puzzle3ActorLoc = puzzle3Slot8;
			puzzle3Complete = true;
		}

		//Determine the location of each block
		if (puzzle3ID == 1 && puzzle3ActorLoc == puzzle3Slot1) {
			::puzzle3ID1Loc = 1;
			::puzzle3Slot1Filled = true;
		}
		if (puzzle3ID == 1 && puzzle3ActorLoc == puzzle3Slot2) {
			::puzzle3ID1Loc = 2;
			::puzzle3Slot2Filled = true;
		}
		if (puzzle3ID == 1 && puzzle3ActorLoc == puzzle3Slot3) {
			::puzzle3ID1Loc = 3;
			::puzzle3Slot3Filled = true;
		}
		if (puzzle3ID == 1 && puzzle3ActorLoc == puzzle3Slot4) {
			::puzzle3ID1Loc = 4;
			::puzzle3Slot4Filled = true;
		}
		if (puzzle3ID == 1 && puzzle3ActorLoc == puzzle3Slot5) {
			::puzzle3ID1Loc = 5;
			::puzzle3Slot5Filled = true;
		}
		if (puzzle3ID == 1 && puzzle3ActorLoc == puzzle3Slot6) {
			::puzzle3ID1Loc = 6;
			::puzzle3Slot6Filled = true;
		}
		if (puzzle3ID == 1 && puzzle3ActorLoc == puzzle3Slot7) {
			::puzzle3ID1Loc = 7;
			::puzzle3Slot7Filled = true;
		}
		if (puzzle3ID == 1 && puzzle3ActorLoc == puzzle3Slot8) {
			::puzzle3ID1Loc = 8;
			::puzzle3Slot8Filled = true;
		}
		if (puzzle3ID == 1 && puzzle3ActorLoc == puzzle3Slot9) {
			::puzzle3ID1Loc = 9;
			::puzzle3Slot9Filled = true;
		}
		if (puzzle3ID == 2 && puzzle3ActorLoc == puzzle3Slot1) {
			::puzzle3ID2Loc = 1;
			::puzzle3Slot1Filled = true;
		}
		if (puzzle3ID == 2 && puzzle3ActorLoc == puzzle3Slot2) {
			::puzzle3ID2Loc = 2;
			::puzzle3Slot2Filled = true;
		}
		if (puzzle3ID == 2 && puzzle3ActorLoc == puzzle3Slot3) {
			::puzzle3ID2Loc = 3;
			::puzzle3Slot3Filled = true;
		}
		if (puzzle3ID == 2 && puzzle3ActorLoc == puzzle3Slot4) {
			::puzzle3ID2Loc = 4;
			::puzzle3Slot4Filled = true;
		}
		if (puzzle3ID == 2 && puzzle3ActorLoc == puzzle3Slot5) {
			::puzzle3ID2Loc = 5;
			::puzzle3Slot5Filled = true;
		}
		if (puzzle3ID == 2 && puzzle3ActorLoc == puzzle3Slot6) {
			::puzzle3ID2Loc = 6;
			::puzzle3Slot6Filled = true;
		}
		if (puzzle3ID == 2 && puzzle3ActorLoc == puzzle3Slot7) {
			::puzzle3ID2Loc = 7;
			::puzzle3Slot7Filled = true;
		}
		if (puzzle3ID == 2 && puzzle3ActorLoc == puzzle3Slot8) {
			::puzzle3ID2Loc = 8;
			::puzzle3Slot8Filled = true;
		}
		if (puzzle3ID == 2 && puzzle3ActorLoc == puzzle3Slot9) {
			::puzzle3ID2Loc = 9;
			::puzzle3Slot9Filled = true;
		}
		if (puzzle3ID == 3 && puzzle3ActorLoc == puzzle3Slot1) {
			::puzzle3ID3Loc = 1;
			::puzzle3Slot1Filled = true;
		}
		if (puzzle3ID == 3 && puzzle3ActorLoc == puzzle3Slot2) {
			::puzzle3ID3Loc = 2;
			::puzzle3Slot2Filled = true;
		}
		if (puzzle3ID == 3 && puzzle3ActorLoc == puzzle3Slot3) {
			::puzzle3ID3Loc = 3;
			::puzzle3Slot3Filled = true;
		}
		if (puzzle3ID == 3 && puzzle3ActorLoc == puzzle3Slot4) {
			::puzzle3ID3Loc = 4;
			::puzzle3Slot4Filled = true;
		}
		if (puzzle3ID == 3 && puzzle3ActorLoc == puzzle3Slot5) {
			::puzzle3ID3Loc = 5;
			::puzzle3Slot5Filled = true;
		}
		if (puzzle3ID == 3 && puzzle3ActorLoc == puzzle3Slot6) {
			::puzzle3ID3Loc = 6;
			::puzzle3Slot6Filled = true;
		}
		if (puzzle3ID == 3 && puzzle3ActorLoc == puzzle3Slot7) {
			::puzzle3ID3Loc = 7;
			::puzzle3Slot7Filled = true;
		}
		if (puzzle3ID == 3 && puzzle3ActorLoc == puzzle3Slot8) {
			::puzzle3ID3Loc = 8;
			::puzzle3Slot8Filled = true;
		}
		if (puzzle3ID == 3 && puzzle3ActorLoc == puzzle3Slot9) {
			::puzzle3ID3Loc = 9;
			::puzzle3Slot9Filled = true;
		}
		if (puzzle3ID == 4 && puzzle3ActorLoc == puzzle3Slot1) {
			::puzzle3ID4Loc = 1;
			::puzzle3Slot1Filled = true;
		}
		if (puzzle3ID == 4 && puzzle3ActorLoc == puzzle3Slot2) {
			::puzzle3ID4Loc = 2;
			::puzzle3Slot2Filled = true;
		}
		if (puzzle3ID == 4 && puzzle3ActorLoc == puzzle3Slot3) {
			::puzzle3ID4Loc = 3;
			::puzzle3Slot3Filled = true;
		}
		if (puzzle3ID == 4 && puzzle3ActorLoc == puzzle3Slot4) {
			::puzzle3ID4Loc = 4;
			::puzzle3Slot4Filled = true;
		}
		if (puzzle3ID == 4 && puzzle3ActorLoc == puzzle3Slot5) {
			::puzzle3ID4Loc = 5;
			::puzzle3Slot5Filled = true;
		}
		if (puzzle3ID == 4 && puzzle3ActorLoc == puzzle3Slot6) {
			::puzzle3ID4Loc = 6;
			::puzzle3Slot6Filled = true;
		}
		if (puzzle3ID == 4 && puzzle3ActorLoc == puzzle3Slot7) {
			::puzzle3ID4Loc = 7;
			::puzzle3Slot7Filled = true;
		}
		if (puzzle3ID == 4 && puzzle3ActorLoc == puzzle3Slot8) {
			::puzzle3ID4Loc = 8;
			::puzzle3Slot8Filled = true;
		}
		if (puzzle3ID == 4 && puzzle3ActorLoc == puzzle3Slot9) {
			::puzzle3ID4Loc = 9;
			::puzzle3Slot9Filled = true;
		}
		if (puzzle3ID == 5 && puzzle3ActorLoc == puzzle3Slot1) {
			::puzzle3ID5Loc = 1;
			::puzzle3Slot1Filled = true;
		}
		if (puzzle3ID == 5 && puzzle3ActorLoc == puzzle3Slot2) {
			::puzzle3ID5Loc = 2;
			::puzzle3Slot2Filled = true;
		}
		if (puzzle3ID == 5 && puzzle3ActorLoc == puzzle3Slot3) {
			::puzzle3ID5Loc = 3;
			::puzzle3Slot3Filled = true;
		}
		if (puzzle3ID == 5 && puzzle3ActorLoc == puzzle3Slot4) {
			::puzzle3ID5Loc = 4;
			::puzzle3Slot4Filled = true;
		}
		if (puzzle3ID == 5 && puzzle3ActorLoc == puzzle3Slot5) {
			::puzzle3ID5Loc = 5;
			::puzzle3Slot5Filled = true;
		}
		if (puzzle3ID == 5 && puzzle3ActorLoc == puzzle3Slot6) {
			::puzzle3ID5Loc = 6;
			::puzzle3Slot6Filled = true;
		}
		if (puzzle3ID == 5 && puzzle3ActorLoc == puzzle3Slot7) {
			::puzzle3ID5Loc = 7;
			::puzzle3Slot7Filled = true;
		}
		if (puzzle3ID == 5 && puzzle3ActorLoc == puzzle3Slot8) {
			::puzzle3ID5Loc = 8;
			::puzzle3Slot8Filled = true;
		}
		if (puzzle3ID == 5 && puzzle3ActorLoc == puzzle3Slot9) {
			::puzzle3ID5Loc = 9;
			::puzzle3Slot9Filled = true;
		}
		if (puzzle3ID == 6 && puzzle3ActorLoc == puzzle3Slot1) {
			::puzzle3ID6Loc = 1;
			::puzzle3Slot1Filled = true;
		}
		if (puzzle3ID == 6 && puzzle3ActorLoc == puzzle3Slot2) {
			::puzzle3ID6Loc = 2;
			::puzzle3Slot2Filled = true;
		}
		if (puzzle3ID == 6 && puzzle3ActorLoc == puzzle3Slot3) {
			::puzzle3ID6Loc = 3;
			::puzzle3Slot3Filled = true;
		}
		if (puzzle3ID == 6 && puzzle3ActorLoc == puzzle3Slot4) {
			::puzzle3ID6Loc = 4;
			::puzzle3Slot4Filled = true;
		}
		if (puzzle3ID == 6 && puzzle3ActorLoc == puzzle3Slot5) {
			::puzzle3ID6Loc = 5;
			::puzzle3Slot5Filled = true;
		}
		if (puzzle3ID == 6 && puzzle3ActorLoc == puzzle3Slot6) {
			::puzzle3ID6Loc = 6;
			::puzzle3Slot6Filled = true;
		}
		if (puzzle3ID == 6 && puzzle3ActorLoc == puzzle3Slot7) {
			::puzzle3ID6Loc = 7;
			::puzzle3Slot7Filled = true;
		}
		if (puzzle3ID == 6 && puzzle3ActorLoc == puzzle3Slot8) {
			::puzzle3ID6Loc = 8;
			::puzzle3Slot8Filled = true;
		}
		if (puzzle3ID == 6 && puzzle3ActorLoc == puzzle3Slot9) {
			::puzzle3ID6Loc = 9;
			::puzzle3Slot9Filled = true;
		}
		if (puzzle3ID == 7 && puzzle3ActorLoc == puzzle3Slot1) {
			::puzzle3ID7Loc = 1;
			::puzzle3Slot1Filled = true;
		}
		if (puzzle3ID == 7 && puzzle3ActorLoc == puzzle3Slot2) {
			::puzzle3ID7Loc = 2;
			::puzzle3Slot2Filled = true;
		}
		if (puzzle3ID == 7 && puzzle3ActorLoc == puzzle3Slot3) {
			::puzzle3ID7Loc = 3;
			::puzzle3Slot3Filled = true;
		}
		if (puzzle3ID == 7 && puzzle3ActorLoc == puzzle3Slot4) {
			::puzzle3ID7Loc = 4;
			::puzzle3Slot4Filled = true;
		}
		if (puzzle3ID == 7 && puzzle3ActorLoc == puzzle3Slot5) {
			::puzzle3ID7Loc = 5;
			::puzzle3Slot5Filled = true;
		}
		if (puzzle3ID == 7 && puzzle3ActorLoc == puzzle3Slot6) {
			::puzzle3ID7Loc = 6;
			::puzzle3Slot6Filled = true;
		}
		if (puzzle3ID == 7 && puzzle3ActorLoc == puzzle3Slot7) {
			::puzzle3ID7Loc = 7;
			::puzzle3Slot7Filled = true;
		}
		if (puzzle3ID == 7 && puzzle3ActorLoc == puzzle3Slot8) {
			::puzzle3ID7Loc = 8;
			::puzzle3Slot8Filled = true;
		}
		if (puzzle3ID == 7 && puzzle3ActorLoc == puzzle3Slot9) {
			::puzzle3ID7Loc = 9;
			::puzzle3Slot9Filled = true;
		}
		if (puzzle3ID == 8 && puzzle3ActorLoc == puzzle3Slot1) {
			::puzzle3ID8Loc = 1;
			::puzzle3Slot1Filled = true;
		}
		if (puzzle3ID == 8 && puzzle3ActorLoc == puzzle3Slot2) {
			::puzzle3ID8Loc = 2;
			::puzzle3Slot2Filled = true;
		}
		if (puzzle3ID == 8 && puzzle3ActorLoc == puzzle3Slot3) {
			::puzzle3ID8Loc = 3;
			::puzzle3Slot3Filled = true;
		}
		if (puzzle3ID == 8 && puzzle3ActorLoc == puzzle3Slot4) {
			::puzzle3ID8Loc = 4;
			::puzzle3Slot4Filled = true;
		}
		if (puzzle3ID == 8 && puzzle3ActorLoc == puzzle3Slot5) {
			::puzzle3ID8Loc = 5;
			::puzzle3Slot5Filled = true;
		}
		if (puzzle3ID == 8 && puzzle3ActorLoc == puzzle3Slot6) {
			::puzzle3ID8Loc = 6;
			::puzzle3Slot6Filled = true;
		}
		if (puzzle3ID == 8 && puzzle3ActorLoc == puzzle3Slot7) {
			::puzzle3ID8Loc = 7;
			::puzzle3Slot7Filled = true;
		}
		if (puzzle3ID == 8 && puzzle3ActorLoc == puzzle3Slot8) {
			::puzzle3ID8Loc = 8;
			::puzzle3Slot8Filled = true;
		}
		if (puzzle3ID == 8 && puzzle3ActorLoc == puzzle3Slot9) {
			::puzzle3ID8Loc = 9;
			::puzzle3Slot9Filled = true;
		}

		//Fill the empty slot as 0
		if (::puzzle3Slot1Filled == false) ::puzzle3CurrentArray[0] = 0;
		if (::puzzle3Slot2Filled == false) ::puzzle3CurrentArray[1] = 0;
		if (::puzzle3Slot3Filled == false) ::puzzle3CurrentArray[2] = 0;
		if (::puzzle3Slot4Filled == false) ::puzzle3CurrentArray[3] = 0;
		if (::puzzle3Slot5Filled == false) ::puzzle3CurrentArray[4] = 0;
		if (::puzzle3Slot6Filled == false) ::puzzle3CurrentArray[5] = 0;
		if (::puzzle3Slot7Filled == false) ::puzzle3CurrentArray[6] = 0;
		if (::puzzle3Slot8Filled == false) ::puzzle3CurrentArray[7] = 0;
		if (::puzzle3Slot9Filled == false) ::puzzle3CurrentArray[8] = 0;

		//Reset the filled values to update
		::puzzle3Slot1Filled = false;
		::puzzle3Slot2Filled = false;
		::puzzle3Slot3Filled = false;
		::puzzle3Slot4Filled = false;
		::puzzle3Slot5Filled = false;
		::puzzle3Slot6Filled = false;
		::puzzle3Slot7Filled = false;
		::puzzle3Slot8Filled = false;
		::puzzle3Slot9Filled = false;

		//Update progress
		if (::puzzle3ID1Loc == 7 && ::puzzle3ID1LocCheck == false) {
			::puzzle3Progress = ::puzzle3Progress + 12.5f;
			puzzle3ID1LocCheck = true;
		}
		if (::puzzle3ID2Loc == 5 && ::puzzle3ID2LocCheck == false) {
			::puzzle3Progress = ::puzzle3Progress + 12.5f;
			puzzle3ID2LocCheck = true;
		}
		if (::puzzle3ID3Loc == 8 && ::puzzle3ID3LocCheck == false) {
			::puzzle3Progress = ::puzzle3Progress + 12.5f;
			puzzle3ID3LocCheck = true;
		}
		if (::puzzle3ID4Loc == 2 && ::puzzle3ID4LocCheck == false) {
			::puzzle3Progress = ::puzzle3Progress + 12.5f;
			puzzle3ID4LocCheck = true;
		}
		if (::puzzle3ID5Loc == 1 && ::puzzle3ID5LocCheck == false) {
			::puzzle3Progress = ::puzzle3Progress + 12.5f;
			puzzle3ID5LocCheck = true;
		}
		if (::puzzle3ID6Loc == 4 && ::puzzle3ID6LocCheck == false) {
			::puzzle3Progress = ::puzzle3Progress + 12.5f;
			puzzle3ID6LocCheck = true;
		}
		if (::puzzle3ID7Loc == 6 && ::puzzle3ID7LocCheck == false) {
			::puzzle3Progress = ::puzzle3Progress + 12.5f;
			puzzle3ID7LocCheck = true;
		}
		if (::puzzle3ID8Loc == 3 && ::puzzle3ID8LocCheck == false) {
			::puzzle3Progress = ::puzzle3Progress + 12.5f;
			puzzle3ID8LocCheck = true;
		}
		if (::puzzle3ID1Loc != 7 && puzzle3ID1LocCheck == true) {
			::puzzle3Progress = ::puzzle3Progress - 12.5f;
			puzzle3ID1LocCheck = false;
		}
		if (::puzzle3ID2Loc != 5 && ::puzzle3ID2LocCheck == true) {
			::puzzle3Progress = ::puzzle3Progress - 12.5f;
			puzzle3ID2LocCheck = false;
		}
		if (::puzzle3ID3Loc != 8 && ::puzzle3ID3LocCheck == true) {
			::puzzle3Progress = ::puzzle3Progress - 12.5f;
			puzzle3ID3LocCheck = false;
		}
		if (::puzzle3ID4Loc != 2 && ::puzzle3ID4LocCheck == true) {
			::puzzle3Progress = ::puzzle3Progress - 12.5f;
			puzzle3ID4LocCheck = false;
		}
		if (::puzzle3ID5Loc != 1 && ::puzzle3ID5LocCheck == true) {
			::puzzle3Progress = ::puzzle3Progress - 12.5f;
			puzzle3ID5LocCheck = false;
		}
		if (::puzzle3ID6Loc != 4 && ::puzzle3ID6LocCheck == true) {
			::puzzle3Progress = ::puzzle3Progress - 12.5f;
			puzzle3ID6LocCheck = false;
		}
		if (::puzzle3ID7Loc != 6 && ::puzzle3ID7LocCheck == true) {
			::puzzle3Progress = ::puzzle3Progress - 12.5f;
			puzzle3ID7LocCheck = false;
		}
		if (::puzzle3ID8Loc != 3 && ::puzzle3ID8LocCheck == true) {
			::puzzle3Progress = ::puzzle3Progress - 12.5f;
			puzzle3ID8LocCheck = false;
		}

		//Ensure you cannot get negative progress
		if (::puzzle3Progress < 0) ::puzzle3Progress = 0;

		//Fill the array with block locations
		::puzzle3CurrentArray[puzzle3ID1Loc - 1] = 1;
		::puzzle3CurrentArray[puzzle3ID2Loc - 1] = 2;
		::puzzle3CurrentArray[puzzle3ID3Loc - 1] = 3;
		::puzzle3CurrentArray[puzzle3ID4Loc - 1] = 4;
		::puzzle3CurrentArray[puzzle3ID5Loc - 1] = 5;
		::puzzle3CurrentArray[puzzle3ID6Loc - 1] = 6;
		::puzzle3CurrentArray[puzzle3ID7Loc - 1] = 7;
		::puzzle3CurrentArray[puzzle3ID8Loc - 1] = 8;

		//Check if the player is following the predetermined puzzle3Path
		if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep1[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep1[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep1[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep1[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep1[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep1[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep1[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep1[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep1[8])) {
			::puzzle3PathStep = 1;
			::puzzle3Progress = (100.0f / 24.0f) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep2[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep2[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep2[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep2[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep2[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep2[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep2[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep2[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep2[8])) {
			::puzzle3PathStep = 2;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep3[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep3[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep3[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep3[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep3[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep3[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep3[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep3[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep3[8])) {
			::puzzle3PathStep = 3;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep4[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep4[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep4[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep4[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep4[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep4[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep4[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep4[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep4[8])) {
			::puzzle3PathStep = 4;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep5[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep5[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep5[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep5[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep5[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep5[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep5[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep5[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep5[8])) {
			::puzzle3PathStep = 5;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep6[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep6[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep6[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep6[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep6[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep6[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep6[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep6[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep6[8])) {
			::puzzle3PathStep = 6;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep7[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep7[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep7[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep7[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep7[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep7[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep7[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep7[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep7[8])) {
			::puzzle3PathStep = 7;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep8[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep8[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep8[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep8[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep8[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep8[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep8[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep8[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep8[8])) {
			::puzzle3PathStep = 8;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep9[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep9[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep9[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep9[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep9[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep9[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep9[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep9[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep9[8])) {
			::puzzle3PathStep = 9;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep10[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep10[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep10[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep10[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep10[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep10[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep10[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep10[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep10[8])) {
			::puzzle3PathStep = 10;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep11[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep11[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep11[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep11[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep11[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep11[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep11[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep11[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep11[8])) {
			::puzzle3PathStep = 11;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep12[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep12[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep12[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep12[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep12[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep12[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep12[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep12[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep12[8])) {
			::puzzle3PathStep = 12;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep13[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep13[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep13[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep13[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep13[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep13[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep13[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep13[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep13[8])) {
			::puzzle3PathStep = 13;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep14[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep14[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep14[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep14[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep14[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep14[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep14[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep14[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep14[8])) {
			::puzzle3PathStep = 14;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep15[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep15[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep15[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep15[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep15[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep15[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep15[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep15[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep15[8])) {
			::puzzle3PathStep = 15;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep16[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep16[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep16[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep16[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep16[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep16[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep16[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep16[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep16[8])) {
			::puzzle3PathStep = 16;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep17[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep17[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep17[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep17[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep17[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep17[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep17[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep17[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep17[8])) {
			::puzzle3PathStep = 17;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep18[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep18[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep18[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep18[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep18[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep18[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep18[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep18[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep18[8])) {
			::puzzle3PathStep = 18;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep19[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep19[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep19[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep19[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep19[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep19[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep19[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep19[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep19[8])) {
			::puzzle3PathStep = 19;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep20[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep20[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep20[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep20[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep20[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep20[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep20[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep20[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep20[8])) {
			::puzzle3PathStep = 20;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep21[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep21[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep21[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep21[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep21[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep21[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep21[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep21[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep21[8])) {
			::puzzle3PathStep = 21;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep22[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep22[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep22[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep22[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep22[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep22[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep22[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep22[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep22[8])) {
			::puzzle3PathStep = 22;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep23[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep23[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep23[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep23[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep23[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep23[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep23[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep23[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep23[8])) {
			::puzzle3PathStep = 23;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if ((::puzzle3CurrentArray[0] == ::puzzle3PathStep24[0]) && (::puzzle3CurrentArray[0] == ::puzzle3PathStep24[0]) && (::puzzle3CurrentArray[2] == ::puzzle3PathStep24[2]) && (::puzzle3CurrentArray[3] == ::puzzle3PathStep24[3]) && (::puzzle3CurrentArray[4] == ::puzzle3PathStep24[4]) && (::puzzle3CurrentArray[5] == ::puzzle3PathStep24[5]) && (::puzzle3CurrentArray[6] == ::puzzle3PathStep24[6]) && (::puzzle3CurrentArray[7] == ::puzzle3PathStep24[7]) && (::puzzle3CurrentArray[8] == ::puzzle3PathStep24[8])) {
			::puzzle3PathStep = 24;
			::puzzle3Progress = (100 / 24) * puzzle3PathStep;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
		}
		else if (::puzzle3ID1Loc == 1 && ::puzzle3ID2Loc == 2 && ::puzzle3ID3Loc == 3 && ::puzzle3ID4Loc == 4 && ::puzzle3ID5Loc == 5 && ::puzzle3ID6Loc == 6 && ::puzzle3ID7Loc == 7 && ::puzzle3ID8Loc == 8) {
			::puzzle3Progress = 100.0f;
			puzzle3OnPath = true;
			puzzle3BeenOnPath = true;
			::puzzle3Complete = true;
		}
		else if (puzzle3BeenOnPath == true) {
			::puzzle3PathStep = 0;
			::puzzle3Progress = 0;
			puzzle3OnPath = false;
			puzzle3BeenOnPath = false;
		}

		//Get the player's location
		playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		//Reset the puzzle
		if (playerLoc.X < -1620.0f && playerLoc.X > -1800.0f && playerLoc.Y > -390.0f && playerLoc.Y < -210.0f) {
			if (puzzle3ID == 1) puzzle3ActorLoc = puzzle3Slot7;
			if (puzzle3ID == 2) puzzle3ActorLoc = puzzle3Slot5;
			if (puzzle3ID == 3) puzzle3ActorLoc = puzzle3Slot8;
			if (puzzle3ID == 4) puzzle3ActorLoc = puzzle3Slot2;
			if (puzzle3ID == 5) puzzle3ActorLoc = puzzle3Slot1;
			if (puzzle3ID == 6) puzzle3ActorLoc = puzzle3Slot4;
			if (puzzle3ID == 7) puzzle3ActorLoc = puzzle3Slot6;
			if (puzzle3ID == 8) puzzle3ActorLoc = puzzle3Slot3;
			if (puzzle3ID == 9) puzzle3ActorLoc = puzzle3Slot9;

			::puzzle3EmptySlot = 9;
		}
	}
}

//When Up is pressed
void ASlidingPuzzle3::UpReleased()
{
	::puzzle3Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -790) && (playerLoc.Y < -510) && ::puzzle3Complete == false && ::puzzle2Complete == true) {

		//Check if the block can be moved
		if (::puzzle3EmptySlot == 6) {
			if (puzzle3ActorLoc == puzzle3Slot9 && ::puzzle3InputCheck == false) {
				puzzle3ActorLoc = puzzle3Slot6;
				::puzzle3EmptySlot = 9;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 5 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot8) {
				puzzle3ActorLoc = puzzle3Slot5;
				::puzzle3EmptySlot = 8;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 4 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot7) {
				puzzle3ActorLoc = puzzle3Slot4;
				::puzzle3EmptySlot = 7;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 3 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot6) {
				puzzle3ActorLoc = puzzle3Slot3;
				::puzzle3EmptySlot = 6;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 2 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot5) {
				puzzle3ActorLoc = puzzle3Slot2;
				::puzzle3EmptySlot = 5;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 1 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot4) {
				puzzle3ActorLoc = puzzle3Slot1;
				::puzzle3EmptySlot = 4;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
	}
}

//When Down is pressed
void ASlidingPuzzle3::DownReleased()
{
	::puzzle3Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -790) && (playerLoc.Y < -510) && ::puzzle3Complete == false && ::puzzle2Complete == true) {

		//Check if the block can be moved
		if (::puzzle3EmptySlot == 4 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot1) {
				puzzle3ActorLoc = puzzle3Slot4;
				::puzzle3EmptySlot = 1;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 5 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot2) {
				puzzle3ActorLoc = puzzle3Slot5;
				::puzzle3EmptySlot = 2;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 6 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot3) {
				puzzle3ActorLoc = puzzle3Slot6;
				::puzzle3EmptySlot = 3;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 7 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot4) {
				puzzle3ActorLoc = puzzle3Slot7;
				::puzzle3EmptySlot = 4;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 8 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot5) {
				puzzle3ActorLoc = puzzle3Slot8;
				::puzzle3EmptySlot = 5;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 9 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot6) {
				puzzle3ActorLoc = puzzle3Slot9;
				::puzzle3EmptySlot = 6;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
	}
}

//When Left is pressed
void ASlidingPuzzle3::LeftReleased()
{
	::puzzle3Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -790) && (playerLoc.Y < -510) && ::puzzle3Complete == false && ::puzzle2Complete == true) {

		//Check if the block can be moved
		if (::puzzle3EmptySlot == 2 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot3) {
				puzzle3ActorLoc = puzzle3Slot2;
				::puzzle3EmptySlot = 3;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 5 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot6) {
				puzzle3ActorLoc = puzzle3Slot5;
				::puzzle3EmptySlot = 6;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 8 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot9) {
				puzzle3ActorLoc = puzzle3Slot8;
				::puzzle3EmptySlot = 9;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 1 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot2) {
				puzzle3ActorLoc = puzzle3Slot1;
				::puzzle3EmptySlot = 2;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 4 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot5) {
				puzzle3ActorLoc = puzzle3Slot4;
				::puzzle3EmptySlot = 5;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 7 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot8) {
				puzzle3ActorLoc = puzzle3Slot7;
				::puzzle3EmptySlot = 8;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
	}
}

//When Right is pressed
void ASlidingPuzzle3::RightReleased()
{
	::puzzle3Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -790) && (playerLoc.Y < -510) && ::puzzle3Complete == false && ::puzzle2Complete == true) {

		//Check if the block can be moved
		if (::puzzle3EmptySlot == 2 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot1) {
				puzzle3ActorLoc = puzzle3Slot2;
				::puzzle3EmptySlot = 1;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 5 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot4) {
				puzzle3ActorLoc = puzzle3Slot5;
				::puzzle3EmptySlot = 4;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 8 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot7) {
				puzzle3ActorLoc = puzzle3Slot8;
				::puzzle3EmptySlot = 7;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 3 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot2) {
				puzzle3ActorLoc = puzzle3Slot3;
				::puzzle3EmptySlot = 2;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 6 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot5) {
				puzzle3ActorLoc = puzzle3Slot6;
				::puzzle3EmptySlot = 5;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
		if (::puzzle3EmptySlot == 9 && ::puzzle3InputCheck == false) {
			if (puzzle3ActorLoc == puzzle3Slot8) {
				puzzle3ActorLoc = puzzle3Slot9;
				::puzzle3EmptySlot = 8;
				::puzzle3InputCheck = true;
				::puzzle3Moves++;
			}
		}
	}
}

//Update the timer display
void ASlidingPuzzle3::UpdateTimerDisplay() {
	TimerText->SetText(FString::FromInt(FMath::Max(::puzzle3ActiveTime, 0)));
}

//Make the timers tick
void ASlidingPuzzle3::AdvanceTimer() {
	if (::puzzle3TempTick2 == false && ::puzzle3SlidingPuzzleRange == 1 && ::puzzle3Complete == false && ::puzzle2Complete == true) {
		::puzzle3TempTick2 = true;
		::puzzle3RunningTime++;
	}
	if (::puzzle3TempTick == false && ::puzzle3CountdownCheck == true && ::puzzle3SlidingPuzzleRange == 1 && ::puzzle3Complete == false && ::puzzle2Complete == true) {
		::puzzle3TempTick = true;
		::puzzle3ActiveTime++;
		::puzzle3Countdown--;
	}
	UpdateTimerDisplay();
	if (::puzzle3Countdown < 1) ::puzzle3CountdownCheck = false;
	else ::puzzle3CountdownCheck = true;
}

//When the timer hits 0
void ASlidingPuzzle3::TimerHasFinished_Implementation() {
}