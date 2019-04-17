// Fill out your copyright notice in the Description page of Project Settings.

#include "SlidingPuzzle5.h"
#include "SlidingPuzzle4.h"
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

//Whether the previous puzzle is complete
extern bool puzzle4Complete;

//Whether the player is by the puzzle
extern int puzzle5SlidingPuzzleRange;

//Total number of moves
int puzzle5Moves = 0;

//Whether Puzzle is complete 
bool puzzle5Complete = false;

//The value of the open puzzle slot
int puzzle5EmptySlot = 9;

//Regulate input
bool puzzle5InputCheck = false;

//Check if the player is on a predetermined Path
bool puzzle5OnPath = false;
bool puzzle5BeenOnPath = false;

//The player's running time
int puzzle5RunningTime = 0;
bool puzzle5TempTick2 = false;

//The player's active time
int puzzle5ActiveTime = 0;
int puzzle5Countdown = 5;
bool puzzle5TempTick = false;
bool puzzle5CountdownCheck = true;

//Each block's location
int puzzle5ID1Loc = 7;
int puzzle5ID2Loc = 5;
int puzzle5ID3Loc = 8;
int puzzle5ID4Loc = 2;
int puzzle5ID5Loc = 1;
int puzzle5ID6Loc = 4;
int puzzle5ID7Loc = 6;
int puzzle5ID8Loc = 3;

//Track puzzle progress
float puzzle5Progress = 0.0f;
bool puzzle5ID1LocCheck = false;
bool puzzle5ID2LocCheck = false;
bool puzzle5ID3LocCheck = false;
bool puzzle5ID4LocCheck = false;
bool puzzle5ID5LocCheck = false;
bool puzzle5ID6LocCheck = false;
bool puzzle5ID7LocCheck = false;
bool puzzle5ID8LocCheck = false;

//Path states
int puzzle5PathStep24[] = { 1, 2, 3, 4, 5, 6, 7, 0, 8 };
int puzzle5PathStep23[] = { 1, 2, 3, 4, 0, 6, 7, 5, 8 };
int puzzle5PathStep22[] = { 1, 2, 3, 0, 4, 6, 7, 5, 8 };
int puzzle5PathStep21[] = { 0, 2, 3, 1, 4, 6, 7, 5, 8 };
int puzzle5PathStep20[] = { 2, 0, 3, 1, 4, 6, 7, 5, 8 };
int puzzle5PathStep19[] = { 2, 4, 3, 1, 0, 6, 7, 5, 8 };
int puzzle5PathStep18[] = { 2, 4, 3, 1, 5, 6, 7, 0, 8 };
int puzzle5PathStep17[] = { 2, 4, 3, 1, 5, 6, 0, 7, 8 };
int puzzle5PathStep16[] = { 2, 4, 3, 0, 5, 6, 1, 7, 8 };
int puzzle5PathStep15[] = { 2, 4, 3, 5, 0, 6, 1, 7, 8 };
int puzzle5PathStep14[] = { 2, 4, 3, 5, 6, 0, 1, 7, 8 };
int puzzle5PathStep13[] = { 2, 4, 0, 5, 6, 3, 1, 7, 8 };
int puzzle5PathStep12[] = { 2, 0, 4, 5, 6, 3, 1, 7, 8 };
int puzzle5PathStep11[] = { 0, 2, 4, 5, 6, 3, 1, 7, 8 };
int puzzle5PathStep10[] = { 5, 2, 4, 0, 6, 3, 1, 7, 8 };
int puzzle5PathStep9[] = { 5, 2, 4, 6, 0, 3, 1, 7, 8 };
int puzzle5PathStep8[] = { 5, 2, 4, 6, 3, 0, 1, 7, 8 };
int puzzle5PathStep7[] = { 5, 2, 4, 6, 3, 8, 1, 7, 0 };
int puzzle5PathStep6[] = { 5, 2, 4, 6, 3, 8, 1, 0, 7 };
int puzzle5PathStep5[] = { 5, 2, 4, 6, 0, 8, 1, 3, 7 };
int puzzle5PathStep4[] = { 5, 0, 4, 6, 2, 8, 1, 3, 7 };
int puzzle5PathStep3[] = { 5, 4, 0, 6, 2, 8, 1, 3, 7 };
int puzzle5PathStep2[] = { 5, 4, 8, 6, 2, 0, 1, 3, 7 };
int puzzle5PathStep1[] = { 5, 4, 8, 6, 2, 7, 1, 3, 0 };
int puzzle5PathStep;

//Check if slots are filled
bool puzzle5Slot1Filled = false;
bool puzzle5Slot2Filled = false;
bool puzzle5Slot3Filled = false;
bool puzzle5Slot4Filled = false;
bool puzzle5Slot5Filled = false;
bool puzzle5Slot6Filled = false;
bool puzzle5Slot7Filled = false;
bool puzzle5Slot8Filled = false;
bool puzzle5Slot9Filled = false;

//Current State
int puzzle5CurrentArray[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Sets default values
ASlidingPuzzle5::ASlidingPuzzle5()
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
	puzzle5Slot1.X = (-2010.0f);
	puzzle5Slot1.Y = (-1880.0f);
	puzzle5Slot1.Z = (550.0f);
	puzzle5Slot2.X = (-2010.0f);
	puzzle5Slot2.Y = (-2000.0f);
	puzzle5Slot2.Z = (550.0f);
	puzzle5Slot3.X = (-2010.0f);
	puzzle5Slot3.Y = (-2120.0f);
	puzzle5Slot3.Z = (550.0f);
	puzzle5Slot4.X = (-2010.0f);
	puzzle5Slot4.Y = (-1880.0f);
	puzzle5Slot4.Z = (430.0f);
	puzzle5Slot5.X = (-2010.0f);
	puzzle5Slot5.Y = (-2000.0f);
	puzzle5Slot5.Z = (430.0f);
	puzzle5Slot6.X = (-2010.0f);
	puzzle5Slot6.Y = (-2120.0f);
	puzzle5Slot6.Z = (430.0f);
	puzzle5Slot7.X = (-2010.0f);
	puzzle5Slot7.Y = (-1880.0f);
	puzzle5Slot7.Z = (310.0f);
	puzzle5Slot8.X = (-2010.0f);
	puzzle5Slot8.Y = (-2000.0f);
	puzzle5Slot8.Z = (310.0f);
	puzzle5Slot9.X = (-2010.0f);
	puzzle5Slot9.Y = (-2120.0f);
	puzzle5Slot9.Z = (310.0f);

	//Default global values
	::puzzle5RunningTime = 0;
	::puzzle5ActiveTime = 0;
	::puzzle5Moves = 0;
	::puzzle5PathStep = 0;
	::puzzle5ID1Loc = 7;
	::puzzle5ID2Loc = 5;
	::puzzle5ID3Loc = 8;
	::puzzle5ID4Loc = 2;
	::puzzle5ID5Loc = 1;
	::puzzle5ID6Loc = 4;
	::puzzle5ID7Loc = 6;
	::puzzle5ID8Loc = 3;
	::puzzle5Countdown = 5;
	::puzzle5EmptySlot = 9;
	::puzzle5Progress = 0.0f;
	::puzzle5InputCheck = false;
	::puzzle5OnPath = false;
	::puzzle5BeenOnPath = false;
	::puzzle5ID1LocCheck = false;
	::puzzle5ID2LocCheck = false;
	::puzzle5ID3LocCheck = false;
	::puzzle5ID4LocCheck = false;
	::puzzle5ID5LocCheck = false;
	::puzzle5ID6LocCheck = false;
	::puzzle5ID7LocCheck = false;
	::puzzle5ID8LocCheck = false;
	::puzzle5Slot1Filled = false;
	::puzzle5Slot2Filled = false;
	::puzzle5Slot3Filled = false;
	::puzzle5Slot4Filled = false;
	::puzzle5Slot5Filled = false;
	::puzzle5Slot6Filled = false;
	::puzzle5Slot7Filled = false;
	::puzzle5Slot8Filled = false;
	::puzzle5Slot9Filled = false;
	::puzzle5Complete = false;
	::puzzle5TempTick = false;
	::puzzle5TempTick2 = false;
	::puzzle5CountdownCheck = true;

	//Update the blocks locations
	puzzle5ActorLoc = this->GetActorLocation();
}

// Called when the game starts or when spawned
void ASlidingPuzzle5::BeginPlay()
{
	Super::BeginPlay();

	//Start the timer
	UpdateTimerDisplay();
	GetWorldTimerManager().SetTimer(TimerTimerHandle, this, &ASlidingPuzzle5::AdvanceTimer, 1.0f, true);

	//Get the block's location
	puzzle5ActorLoc = this->GetActorLocation();

	//Give each block an ID
	if ((puzzle5ActorLoc.Y == puzzle5Slot7.Y) && (puzzle5ActorLoc.Z == puzzle5Slot7.Z)) puzzle5ID = 1;
	if ((puzzle5ActorLoc.Y == puzzle5Slot5.Y) && (puzzle5ActorLoc.Z == puzzle5Slot5.Z)) puzzle5ID = 2;
	if ((puzzle5ActorLoc.Y == puzzle5Slot8.Y) && (puzzle5ActorLoc.Z == puzzle5Slot8.Z)) puzzle5ID = 3;
	if ((puzzle5ActorLoc.Y == puzzle5Slot2.Y) && (puzzle5ActorLoc.Z == puzzle5Slot2.Z)) puzzle5ID = 4;
	if ((puzzle5ActorLoc.Y == puzzle5Slot1.Y) && (puzzle5ActorLoc.Z == puzzle5Slot1.Z)) puzzle5ID = 5;
	if ((puzzle5ActorLoc.Y == puzzle5Slot4.Y) && (puzzle5ActorLoc.Z == puzzle5Slot4.Z)) puzzle5ID = 6;
	if ((puzzle5ActorLoc.Y == puzzle5Slot6.Y) && (puzzle5ActorLoc.Z == puzzle5Slot6.Z)) puzzle5ID = 7;
	if ((puzzle5ActorLoc.Y == puzzle5Slot3.Y) && (puzzle5ActorLoc.Z == puzzle5Slot3.Z)) puzzle5ID = 8;
	if ((puzzle5ActorLoc.Y == puzzle5Slot9.Y) && (puzzle5ActorLoc.Z == puzzle5Slot9.Z)) puzzle5ID = 9;

	//Don't block inputs
	bBlockInput = false;

	//Enable keyboard input to control the puzzle
	PlayerController = GetWorld()->GetFirstPlayerController();
	EnableInput(PlayerController);
	InputComponent->bBlockInput = false;

	// Respond when keys are pressed
	FInputActionBinding& UpBinding = InputComponent->BindAction("SlideUp", IE_Released, this, &ASlidingPuzzle5::UpReleased);
	FInputActionBinding& DownBinding = InputComponent->BindAction("SlideDown", IE_Released, this, &ASlidingPuzzle5::DownReleased);
	FInputActionBinding& LeftBinding = InputComponent->BindAction("SlideLeft", IE_Released, this, &ASlidingPuzzle5::LeftReleased);
	FInputActionBinding& RightBinding = InputComponent->BindAction("SlideRight", IE_Released, this, &ASlidingPuzzle5::RightReleased);

	//Don't consume inputs
	UpBinding.bConsumeInput = false;
	DownBinding.bConsumeInput = false;
	LeftBinding.bConsumeInput = false;
	RightBinding.bConsumeInput = false;
}

// Called every frame
void ASlidingPuzzle5::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (::puzzle4Complete == true && ::puzzle5Complete == false) {

		//Puts the puzzle on screen if previous is solved
		puzzle5ActorLoc.X = -2010.0f;

		//Update block's location if moved
		SetActorLocation(puzzle5ActorLoc);

		//Reset input check
		::puzzle5InputCheck = false;
		::puzzle5TempTick = false;
		::puzzle5TempTick2 = false;


		//Determine the location of each block
		if (puzzle5ID == 1 && puzzle5ActorLoc == puzzle5Slot1) {
			::puzzle5ID1Loc = 1;
			::puzzle5Slot1Filled = true;
		}
		if (puzzle5ID == 1 && puzzle5ActorLoc == puzzle5Slot2) {
			::puzzle5ID1Loc = 2;
			::puzzle5Slot2Filled = true;
		}
		if (puzzle5ID == 1 && puzzle5ActorLoc == puzzle5Slot3) {
			::puzzle5ID1Loc = 3;
			::puzzle5Slot3Filled = true;
		}
		if (puzzle5ID == 1 && puzzle5ActorLoc == puzzle5Slot4) {
			::puzzle5ID1Loc = 4;
			::puzzle5Slot4Filled = true;
		}
		if (puzzle5ID == 1 && puzzle5ActorLoc == puzzle5Slot5) {
			::puzzle5ID1Loc = 5;
			::puzzle5Slot5Filled = true;
		}
		if (puzzle5ID == 1 && puzzle5ActorLoc == puzzle5Slot6) {
			::puzzle5ID1Loc = 6;
			::puzzle5Slot6Filled = true;
		}
		if (puzzle5ID == 1 && puzzle5ActorLoc == puzzle5Slot7) {
			::puzzle5ID1Loc = 7;
			::puzzle5Slot7Filled = true;
		}
		if (puzzle5ID == 1 && puzzle5ActorLoc == puzzle5Slot8) {
			::puzzle5ID1Loc = 8;
			::puzzle5Slot8Filled = true;
		}
		if (puzzle5ID == 1 && puzzle5ActorLoc == puzzle5Slot9) {
			::puzzle5ID1Loc = 9;
			::puzzle5Slot9Filled = true;
		}
		if (puzzle5ID == 2 && puzzle5ActorLoc == puzzle5Slot1) {
			::puzzle5ID2Loc = 1;
			::puzzle5Slot1Filled = true;
		}
		if (puzzle5ID == 2 && puzzle5ActorLoc == puzzle5Slot2) {
			::puzzle5ID2Loc = 2;
			::puzzle5Slot2Filled = true;
		}
		if (puzzle5ID == 2 && puzzle5ActorLoc == puzzle5Slot3) {
			::puzzle5ID2Loc = 3;
			::puzzle5Slot3Filled = true;
		}
		if (puzzle5ID == 2 && puzzle5ActorLoc == puzzle5Slot4) {
			::puzzle5ID2Loc = 4;
			::puzzle5Slot4Filled = true;
		}
		if (puzzle5ID == 2 && puzzle5ActorLoc == puzzle5Slot5) {
			::puzzle5ID2Loc = 5;
			::puzzle5Slot5Filled = true;
		}
		if (puzzle5ID == 2 && puzzle5ActorLoc == puzzle5Slot6) {
			::puzzle5ID2Loc = 6;
			::puzzle5Slot6Filled = true;
		}
		if (puzzle5ID == 2 && puzzle5ActorLoc == puzzle5Slot7) {
			::puzzle5ID2Loc = 7;
			::puzzle5Slot7Filled = true;
		}
		if (puzzle5ID == 2 && puzzle5ActorLoc == puzzle5Slot8) {
			::puzzle5ID2Loc = 8;
			::puzzle5Slot8Filled = true;
		}
		if (puzzle5ID == 2 && puzzle5ActorLoc == puzzle5Slot9) {
			::puzzle5ID2Loc = 9;
			::puzzle5Slot9Filled = true;
		}
		if (puzzle5ID == 3 && puzzle5ActorLoc == puzzle5Slot1) {
			::puzzle5ID3Loc = 1;
			::puzzle5Slot1Filled = true;
		}
		if (puzzle5ID == 3 && puzzle5ActorLoc == puzzle5Slot2) {
			::puzzle5ID3Loc = 2;
			::puzzle5Slot2Filled = true;
		}
		if (puzzle5ID == 3 && puzzle5ActorLoc == puzzle5Slot3) {
			::puzzle5ID3Loc = 3;
			::puzzle5Slot3Filled = true;
		}
		if (puzzle5ID == 3 && puzzle5ActorLoc == puzzle5Slot4) {
			::puzzle5ID3Loc = 4;
			::puzzle5Slot4Filled = true;
		}
		if (puzzle5ID == 3 && puzzle5ActorLoc == puzzle5Slot5) {
			::puzzle5ID3Loc = 5;
			::puzzle5Slot5Filled = true;
		}
		if (puzzle5ID == 3 && puzzle5ActorLoc == puzzle5Slot6) {
			::puzzle5ID3Loc = 6;
			::puzzle5Slot6Filled = true;
		}
		if (puzzle5ID == 3 && puzzle5ActorLoc == puzzle5Slot7) {
			::puzzle5ID3Loc = 7;
			::puzzle5Slot7Filled = true;
		}
		if (puzzle5ID == 3 && puzzle5ActorLoc == puzzle5Slot8) {
			::puzzle5ID3Loc = 8;
			::puzzle5Slot8Filled = true;
		}
		if (puzzle5ID == 3 && puzzle5ActorLoc == puzzle5Slot9) {
			::puzzle5ID3Loc = 9;
			::puzzle5Slot9Filled = true;
		}
		if (puzzle5ID == 4 && puzzle5ActorLoc == puzzle5Slot1) {
			::puzzle5ID4Loc = 1;
			::puzzle5Slot1Filled = true;
		}
		if (puzzle5ID == 4 && puzzle5ActorLoc == puzzle5Slot2) {
			::puzzle5ID4Loc = 2;
			::puzzle5Slot2Filled = true;
		}
		if (puzzle5ID == 4 && puzzle5ActorLoc == puzzle5Slot3) {
			::puzzle5ID4Loc = 3;
			::puzzle5Slot3Filled = true;
		}
		if (puzzle5ID == 4 && puzzle5ActorLoc == puzzle5Slot4) {
			::puzzle5ID4Loc = 4;
			::puzzle5Slot4Filled = true;
		}
		if (puzzle5ID == 4 && puzzle5ActorLoc == puzzle5Slot5) {
			::puzzle5ID4Loc = 5;
			::puzzle5Slot5Filled = true;
		}
		if (puzzle5ID == 4 && puzzle5ActorLoc == puzzle5Slot6) {
			::puzzle5ID4Loc = 6;
			::puzzle5Slot6Filled = true;
		}
		if (puzzle5ID == 4 && puzzle5ActorLoc == puzzle5Slot7) {
			::puzzle5ID4Loc = 7;
			::puzzle5Slot7Filled = true;
		}
		if (puzzle5ID == 4 && puzzle5ActorLoc == puzzle5Slot8) {
			::puzzle5ID4Loc = 8;
			::puzzle5Slot8Filled = true;
		}
		if (puzzle5ID == 4 && puzzle5ActorLoc == puzzle5Slot9) {
			::puzzle5ID4Loc = 9;
			::puzzle5Slot9Filled = true;
		}
		if (puzzle5ID == 5 && puzzle5ActorLoc == puzzle5Slot1) {
			::puzzle5ID5Loc = 1;
			::puzzle5Slot1Filled = true;
		}
		if (puzzle5ID == 5 && puzzle5ActorLoc == puzzle5Slot2) {
			::puzzle5ID5Loc = 2;
			::puzzle5Slot2Filled = true;
		}
		if (puzzle5ID == 5 && puzzle5ActorLoc == puzzle5Slot3) {
			::puzzle5ID5Loc = 3;
			::puzzle5Slot3Filled = true;
		}
		if (puzzle5ID == 5 && puzzle5ActorLoc == puzzle5Slot4) {
			::puzzle5ID5Loc = 4;
			::puzzle5Slot4Filled = true;
		}
		if (puzzle5ID == 5 && puzzle5ActorLoc == puzzle5Slot5) {
			::puzzle5ID5Loc = 5;
			::puzzle5Slot5Filled = true;
		}
		if (puzzle5ID == 5 && puzzle5ActorLoc == puzzle5Slot6) {
			::puzzle5ID5Loc = 6;
			::puzzle5Slot6Filled = true;
		}
		if (puzzle5ID == 5 && puzzle5ActorLoc == puzzle5Slot7) {
			::puzzle5ID5Loc = 7;
			::puzzle5Slot7Filled = true;
		}
		if (puzzle5ID == 5 && puzzle5ActorLoc == puzzle5Slot8) {
			::puzzle5ID5Loc = 8;
			::puzzle5Slot8Filled = true;
		}
		if (puzzle5ID == 5 && puzzle5ActorLoc == puzzle5Slot9) {
			::puzzle5ID5Loc = 9;
			::puzzle5Slot9Filled = true;
		}
		if (puzzle5ID == 6 && puzzle5ActorLoc == puzzle5Slot1) {
			::puzzle5ID6Loc = 1;
			::puzzle5Slot1Filled = true;
		}
		if (puzzle5ID == 6 && puzzle5ActorLoc == puzzle5Slot2) {
			::puzzle5ID6Loc = 2;
			::puzzle5Slot2Filled = true;
		}
		if (puzzle5ID == 6 && puzzle5ActorLoc == puzzle5Slot3) {
			::puzzle5ID6Loc = 3;
			::puzzle5Slot3Filled = true;
		}
		if (puzzle5ID == 6 && puzzle5ActorLoc == puzzle5Slot4) {
			::puzzle5ID6Loc = 4;
			::puzzle5Slot4Filled = true;
		}
		if (puzzle5ID == 6 && puzzle5ActorLoc == puzzle5Slot5) {
			::puzzle5ID6Loc = 5;
			::puzzle5Slot5Filled = true;
		}
		if (puzzle5ID == 6 && puzzle5ActorLoc == puzzle5Slot6) {
			::puzzle5ID6Loc = 6;
			::puzzle5Slot6Filled = true;
		}
		if (puzzle5ID == 6 && puzzle5ActorLoc == puzzle5Slot7) {
			::puzzle5ID6Loc = 7;
			::puzzle5Slot7Filled = true;
		}
		if (puzzle5ID == 6 && puzzle5ActorLoc == puzzle5Slot8) {
			::puzzle5ID6Loc = 8;
			::puzzle5Slot8Filled = true;
		}
		if (puzzle5ID == 6 && puzzle5ActorLoc == puzzle5Slot9) {
			::puzzle5ID6Loc = 9;
			::puzzle5Slot9Filled = true;
		}
		if (puzzle5ID == 7 && puzzle5ActorLoc == puzzle5Slot1) {
			::puzzle5ID7Loc = 1;
			::puzzle5Slot1Filled = true;
		}
		if (puzzle5ID == 7 && puzzle5ActorLoc == puzzle5Slot2) {
			::puzzle5ID7Loc = 2;
			::puzzle5Slot2Filled = true;
		}
		if (puzzle5ID == 7 && puzzle5ActorLoc == puzzle5Slot3) {
			::puzzle5ID7Loc = 3;
			::puzzle5Slot3Filled = true;
		}
		if (puzzle5ID == 7 && puzzle5ActorLoc == puzzle5Slot4) {
			::puzzle5ID7Loc = 4;
			::puzzle5Slot4Filled = true;
		}
		if (puzzle5ID == 7 && puzzle5ActorLoc == puzzle5Slot5) {
			::puzzle5ID7Loc = 5;
			::puzzle5Slot5Filled = true;
		}
		if (puzzle5ID == 7 && puzzle5ActorLoc == puzzle5Slot6) {
			::puzzle5ID7Loc = 6;
			::puzzle5Slot6Filled = true;
		}
		if (puzzle5ID == 7 && puzzle5ActorLoc == puzzle5Slot7) {
			::puzzle5ID7Loc = 7;
			::puzzle5Slot7Filled = true;
		}
		if (puzzle5ID == 7 && puzzle5ActorLoc == puzzle5Slot8) {
			::puzzle5ID7Loc = 8;
			::puzzle5Slot8Filled = true;
		}
		if (puzzle5ID == 7 && puzzle5ActorLoc == puzzle5Slot9) {
			::puzzle5ID7Loc = 9;
			::puzzle5Slot9Filled = true;
		}
		if (puzzle5ID == 8 && puzzle5ActorLoc == puzzle5Slot1) {
			::puzzle5ID8Loc = 1;
			::puzzle5Slot1Filled = true;
		}
		if (puzzle5ID == 8 && puzzle5ActorLoc == puzzle5Slot2) {
			::puzzle5ID8Loc = 2;
			::puzzle5Slot2Filled = true;
		}
		if (puzzle5ID == 8 && puzzle5ActorLoc == puzzle5Slot3) {
			::puzzle5ID8Loc = 3;
			::puzzle5Slot3Filled = true;
		}
		if (puzzle5ID == 8 && puzzle5ActorLoc == puzzle5Slot4) {
			::puzzle5ID8Loc = 4;
			::puzzle5Slot4Filled = true;
		}
		if (puzzle5ID == 8 && puzzle5ActorLoc == puzzle5Slot5) {
			::puzzle5ID8Loc = 5;
			::puzzle5Slot5Filled = true;
		}
		if (puzzle5ID == 8 && puzzle5ActorLoc == puzzle5Slot6) {
			::puzzle5ID8Loc = 6;
			::puzzle5Slot6Filled = true;
		}
		if (puzzle5ID == 8 && puzzle5ActorLoc == puzzle5Slot7) {
			::puzzle5ID8Loc = 7;
			::puzzle5Slot7Filled = true;
		}
		if (puzzle5ID == 8 && puzzle5ActorLoc == puzzle5Slot8) {
			::puzzle5ID8Loc = 8;
			::puzzle5Slot8Filled = true;
		}
		if (puzzle5ID == 8 && puzzle5ActorLoc == puzzle5Slot9) {
			::puzzle5ID8Loc = 9;
			::puzzle5Slot9Filled = true;
		}

		//Fill the empty slot as 0
		if (::puzzle5Slot1Filled == false) ::puzzle5CurrentArray[0] = 0;
		if (::puzzle5Slot2Filled == false) ::puzzle5CurrentArray[1] = 0;
		if (::puzzle5Slot3Filled == false) ::puzzle5CurrentArray[2] = 0;
		if (::puzzle5Slot4Filled == false) ::puzzle5CurrentArray[3] = 0;
		if (::puzzle5Slot5Filled == false) ::puzzle5CurrentArray[4] = 0;
		if (::puzzle5Slot6Filled == false) ::puzzle5CurrentArray[5] = 0;
		if (::puzzle5Slot7Filled == false) ::puzzle5CurrentArray[6] = 0;
		if (::puzzle5Slot8Filled == false) ::puzzle5CurrentArray[7] = 0;
		if (::puzzle5Slot9Filled == false) ::puzzle5CurrentArray[8] = 0;

		//Reset the filled values to update
		::puzzle5Slot1Filled = false;
		::puzzle5Slot2Filled = false;
		::puzzle5Slot3Filled = false;
		::puzzle5Slot4Filled = false;
		::puzzle5Slot5Filled = false;
		::puzzle5Slot6Filled = false;
		::puzzle5Slot7Filled = false;
		::puzzle5Slot8Filled = false;
		::puzzle5Slot9Filled = false;

		//Update progress
		if (::puzzle5ID1Loc == 7 && ::puzzle5ID1LocCheck == false) {
			::puzzle5Progress = ::puzzle5Progress + 12.5f;
			puzzle5ID1LocCheck = true;
		}
		if (::puzzle5ID2Loc == 5 && ::puzzle5ID2LocCheck == false) {
			::puzzle5Progress = ::puzzle5Progress + 12.5f;
			puzzle5ID2LocCheck = true;
		}
		if (::puzzle5ID3Loc == 8 && ::puzzle5ID3LocCheck == false) {
			::puzzle5Progress = ::puzzle5Progress + 12.5f;
			puzzle5ID3LocCheck = true;
		}
		if (::puzzle5ID4Loc == 2 && ::puzzle5ID4LocCheck == false) {
			::puzzle5Progress = ::puzzle5Progress + 12.5f;
			puzzle5ID4LocCheck = true;
		}
		if (::puzzle5ID5Loc == 1 && ::puzzle5ID5LocCheck == false) {
			::puzzle5Progress = ::puzzle5Progress + 12.5f;
			puzzle5ID5LocCheck = true;
		}
		if (::puzzle5ID6Loc == 4 && ::puzzle5ID6LocCheck == false) {
			::puzzle5Progress = ::puzzle5Progress + 12.5f;
			puzzle5ID6LocCheck = true;
		}
		if (::puzzle5ID7Loc == 6 && ::puzzle5ID7LocCheck == false) {
			::puzzle5Progress = ::puzzle5Progress + 12.5f;
			puzzle5ID7LocCheck = true;
		}
		if (::puzzle5ID8Loc == 3 && ::puzzle5ID8LocCheck == false) {
			::puzzle5Progress = ::puzzle5Progress + 12.5f;
			puzzle5ID8LocCheck = true;
		}
		if (::puzzle5ID1Loc != 7 && puzzle5ID1LocCheck == true) {
			::puzzle5Progress = ::puzzle5Progress - 12.5f;
			puzzle5ID1LocCheck = false;
		}
		if (::puzzle5ID2Loc != 5 && ::puzzle5ID2LocCheck == true) {
			::puzzle5Progress = ::puzzle5Progress - 12.5f;
			puzzle5ID2LocCheck = false;
		}
		if (::puzzle5ID3Loc != 8 && ::puzzle5ID3LocCheck == true) {
			::puzzle5Progress = ::puzzle5Progress - 12.5f;
			puzzle5ID3LocCheck = false;
		}
		if (::puzzle5ID4Loc != 2 && ::puzzle5ID4LocCheck == true) {
			::puzzle5Progress = ::puzzle5Progress - 12.5f;
			puzzle5ID4LocCheck = false;
		}
		if (::puzzle5ID5Loc != 1 && ::puzzle5ID5LocCheck == true) {
			::puzzle5Progress = ::puzzle5Progress - 12.5f;
			puzzle5ID5LocCheck = false;
		}
		if (::puzzle5ID6Loc != 4 && ::puzzle5ID6LocCheck == true) {
			::puzzle5Progress = ::puzzle5Progress - 12.5f;
			puzzle5ID6LocCheck = false;
		}
		if (::puzzle5ID7Loc != 6 && ::puzzle5ID7LocCheck == true) {
			::puzzle5Progress = ::puzzle5Progress - 12.5f;
			puzzle5ID7LocCheck = false;
		}
		if (::puzzle5ID8Loc != 3 && ::puzzle5ID8LocCheck == true) {
			::puzzle5Progress = ::puzzle5Progress - 12.5f;
			puzzle5ID8LocCheck = false;
		}
	}

	//Ensure you cannot get negative progress
	if (::puzzle5Progress < 0) ::puzzle5Progress = 0;

	//Fill the array with block locations
	::puzzle5CurrentArray[puzzle5ID1Loc - 1] = 1;
	::puzzle5CurrentArray[puzzle5ID2Loc - 1] = 2;
	::puzzle5CurrentArray[puzzle5ID3Loc - 1] = 3;
	::puzzle5CurrentArray[puzzle5ID4Loc - 1] = 4;
	::puzzle5CurrentArray[puzzle5ID5Loc - 1] = 5;
	::puzzle5CurrentArray[puzzle5ID6Loc - 1] = 6;
	::puzzle5CurrentArray[puzzle5ID7Loc - 1] = 7;
	::puzzle5CurrentArray[puzzle5ID8Loc - 1] = 8;

	//Check if the player is following the predetermined puzzle5Path
	if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep1[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep1[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep1[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep1[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep1[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep1[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep1[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep1[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep1[8])) {
		::puzzle5PathStep = 1;
		::puzzle5Progress = (100.0f / 24.0f) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep2[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep2[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep2[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep2[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep2[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep2[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep2[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep2[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep2[8])) {
		::puzzle5PathStep = 2;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep3[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep3[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep3[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep3[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep3[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep3[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep3[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep3[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep3[8])) {
		::puzzle5PathStep = 3;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep4[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep4[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep4[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep4[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep4[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep4[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep4[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep4[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep4[8])) {
		::puzzle5PathStep = 4;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep5[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep5[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep5[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep5[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep5[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep5[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep5[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep5[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep5[8])) {
		::puzzle5PathStep = 5;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep6[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep6[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep6[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep6[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep6[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep6[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep6[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep6[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep6[8])) {
		::puzzle5PathStep = 6;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep7[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep7[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep7[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep7[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep7[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep7[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep7[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep7[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep7[8])) {
		::puzzle5PathStep = 7;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep8[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep8[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep8[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep8[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep8[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep8[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep8[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep8[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep8[8])) {
		::puzzle5PathStep = 8;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep9[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep9[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep9[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep9[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep9[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep9[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep9[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep9[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep9[8])) {
		::puzzle5PathStep = 9;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep10[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep10[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep10[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep10[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep10[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep10[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep10[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep10[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep10[8])) {
		::puzzle5PathStep = 10;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep11[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep11[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep11[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep11[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep11[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep11[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep11[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep11[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep11[8])) {
		::puzzle5PathStep = 11;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep12[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep12[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep12[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep12[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep12[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep12[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep12[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep12[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep12[8])) {
		::puzzle5PathStep = 12;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep13[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep13[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep13[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep13[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep13[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep13[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep13[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep13[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep13[8])) {
		::puzzle5PathStep = 13;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep14[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep14[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep14[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep14[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep14[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep14[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep14[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep14[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep14[8])) {
		::puzzle5PathStep = 14;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep15[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep15[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep15[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep15[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep15[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep15[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep15[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep15[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep15[8])) {
		::puzzle5PathStep = 15;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep16[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep16[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep16[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep16[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep16[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep16[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep16[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep16[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep16[8])) {
		::puzzle5PathStep = 16;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep17[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep17[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep17[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep17[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep17[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep17[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep17[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep17[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep17[8])) {
		::puzzle5PathStep = 17;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep18[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep18[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep18[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep18[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep18[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep18[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep18[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep18[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep18[8])) {
		::puzzle5PathStep = 18;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep19[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep19[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep19[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep19[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep19[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep19[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep19[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep19[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep19[8])) {
		::puzzle5PathStep = 19;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep20[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep20[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep20[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep20[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep20[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep20[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep20[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep20[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep20[8])) {
		::puzzle5PathStep = 20;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep21[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep21[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep21[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep21[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep21[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep21[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep21[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep21[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep21[8])) {
		::puzzle5PathStep = 21;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep22[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep22[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep22[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep22[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep22[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep22[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep22[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep22[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep22[8])) {
		::puzzle5PathStep = 22;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep23[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep23[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep23[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep23[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep23[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep23[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep23[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep23[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep23[8])) {
		::puzzle5PathStep = 23;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if ((::puzzle5CurrentArray[0] == ::puzzle5PathStep24[0]) && (::puzzle5CurrentArray[0] == ::puzzle5PathStep24[0]) && (::puzzle5CurrentArray[2] == ::puzzle5PathStep24[2]) && (::puzzle5CurrentArray[3] == ::puzzle5PathStep24[3]) && (::puzzle5CurrentArray[4] == ::puzzle5PathStep24[4]) && (::puzzle5CurrentArray[5] == ::puzzle5PathStep24[5]) && (::puzzle5CurrentArray[6] == ::puzzle5PathStep24[6]) && (::puzzle5CurrentArray[7] == ::puzzle5PathStep24[7]) && (::puzzle5CurrentArray[8] == ::puzzle5PathStep24[8])) {
		::puzzle5PathStep = 24;
		::puzzle5Progress = (100 / 24) * puzzle5PathStep;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
	}
	else if (::puzzle5ID1Loc == 1 && ::puzzle5ID2Loc == 2 && ::puzzle5ID3Loc == 3 && ::puzzle5ID4Loc == 4 && ::puzzle5ID5Loc == 5 && ::puzzle5ID6Loc == 6 && ::puzzle5ID7Loc == 7 && ::puzzle5ID8Loc == 8) {
		::puzzle5Progress = 100.0f;
		puzzle5OnPath = true;
		puzzle5BeenOnPath = true;
		::puzzle5Complete = true;
	}
	else if (puzzle5BeenOnPath == true) {
		::puzzle5PathStep = 0;
		::puzzle5Progress = 0;
		puzzle5OnPath = false;
		puzzle5BeenOnPath = false;
	}

	//Get the player's location
	playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	//Reset the puzzle
	if (playerLoc.X < -1620.0f && playerLoc.X > -1800.0f && playerLoc.Y > -1730.0f && playerLoc.Y < -1550.0f) {
		if (puzzle5ID == 1) puzzle5ActorLoc = puzzle5Slot7;
		if (puzzle5ID == 2) puzzle5ActorLoc = puzzle5Slot5;
		if (puzzle5ID == 3) puzzle5ActorLoc = puzzle5Slot8;
		if (puzzle5ID == 4) puzzle5ActorLoc = puzzle5Slot2;
		if (puzzle5ID == 5) puzzle5ActorLoc = puzzle5Slot1;
		if (puzzle5ID == 6) puzzle5ActorLoc = puzzle5Slot4;
		if (puzzle5ID == 7) puzzle5ActorLoc = puzzle5Slot6;
		if (puzzle5ID == 8) puzzle5ActorLoc = puzzle5Slot3;
		if (puzzle5ID == 9) puzzle5ActorLoc = puzzle5Slot9;

		::puzzle5EmptySlot = 9;
	}
}

//When Up is pressed
void ASlidingPuzzle5::UpReleased()
{
	::puzzle5Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -2130) && (playerLoc.Y < -1850) && ::puzzle5Complete == false && ::puzzle4Complete == true) {

		//Check if the block can be moved
		if (::puzzle5EmptySlot == 6) {
			if (puzzle5ActorLoc == puzzle5Slot9 && ::puzzle5InputCheck == false) {
				puzzle5ActorLoc = puzzle5Slot6;
				::puzzle5EmptySlot = 9;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 5 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot8) {
				puzzle5ActorLoc = puzzle5Slot5;
				::puzzle5EmptySlot = 8;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 4 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot7) {
				puzzle5ActorLoc = puzzle5Slot4;
				::puzzle5EmptySlot = 7;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 3 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot6) {
				puzzle5ActorLoc = puzzle5Slot3;
				::puzzle5EmptySlot = 6;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 2 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot5) {
				puzzle5ActorLoc = puzzle5Slot2;
				::puzzle5EmptySlot = 5;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 1 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot4) {
				puzzle5ActorLoc = puzzle5Slot1;
				::puzzle5EmptySlot = 4;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
	}
}

//When Down is pressed
void ASlidingPuzzle5::DownReleased()
{
	::puzzle5Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -2130) && (playerLoc.Y < -1850) && ::puzzle5Complete == false && ::puzzle4Complete == true) {

		//Check if the block can be moved
		if (::puzzle5EmptySlot == 4 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot1) {
				puzzle5ActorLoc = puzzle5Slot4;
				::puzzle5EmptySlot = 1;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 5 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot2) {
				puzzle5ActorLoc = puzzle5Slot5;
				::puzzle5EmptySlot = 2;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 6 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot3) {
				puzzle5ActorLoc = puzzle5Slot6;
				::puzzle5EmptySlot = 3;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 7 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot4) {
				puzzle5ActorLoc = puzzle5Slot7;
				::puzzle5EmptySlot = 4;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 8 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot5) {
				puzzle5ActorLoc = puzzle5Slot8;
				::puzzle5EmptySlot = 5;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 9 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot6) {
				puzzle5ActorLoc = puzzle5Slot9;
				::puzzle5EmptySlot = 6;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
	}
}

//When Left is pressed
void ASlidingPuzzle5::LeftReleased()
{
	::puzzle5Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -2130) && (playerLoc.Y < -1850) && ::puzzle5Complete == false && ::puzzle4Complete == true) {

		//Check if the block can be moved
		if (::puzzle5EmptySlot == 2 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot3) {
				puzzle5ActorLoc = puzzle5Slot2;
				::puzzle5EmptySlot = 3;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 5 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot6) {
				puzzle5ActorLoc = puzzle5Slot5;
				::puzzle5EmptySlot = 6;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 8 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot9) {
				puzzle5ActorLoc = puzzle5Slot8;
				::puzzle5EmptySlot = 9;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 1 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot2) {
				puzzle5ActorLoc = puzzle5Slot1;
				::puzzle5EmptySlot = 2;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 4 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot5) {
				puzzle5ActorLoc = puzzle5Slot4;
				::puzzle5EmptySlot = 5;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 7 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot8) {
				puzzle5ActorLoc = puzzle5Slot7;
				::puzzle5EmptySlot = 8;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
	}
}

//When Right is pressed
void ASlidingPuzzle5::RightReleased()
{
	::puzzle5Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -2130) && (playerLoc.Y < -1850) && ::puzzle5Complete == false && ::puzzle4Complete == true) {

		//Check if the block can be moved
		if (::puzzle5EmptySlot == 2 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot1) {
				puzzle5ActorLoc = puzzle5Slot2;
				::puzzle5EmptySlot = 1;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 5 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot4) {
				puzzle5ActorLoc = puzzle5Slot5;
				::puzzle5EmptySlot = 4;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 8 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot7) {
				puzzle5ActorLoc = puzzle5Slot8;
				::puzzle5EmptySlot = 7;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 3 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot2) {
				puzzle5ActorLoc = puzzle5Slot3;
				::puzzle5EmptySlot = 2;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 6 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot5) {
				puzzle5ActorLoc = puzzle5Slot6;
				::puzzle5EmptySlot = 5;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
		if (::puzzle5EmptySlot == 9 && ::puzzle5InputCheck == false) {
			if (puzzle5ActorLoc == puzzle5Slot8) {
				puzzle5ActorLoc = puzzle5Slot9;
				::puzzle5EmptySlot = 8;
				::puzzle5InputCheck = true;
				::puzzle5Moves++;
			}
		}
	}
}

//Update the timer display
void ASlidingPuzzle5::UpdateTimerDisplay() {
	TimerText->SetText(FString::FromInt(FMath::Max(::puzzle5ActiveTime, 0)));
}

//Make the timers tick
void ASlidingPuzzle5::AdvanceTimer() {
	if (::puzzle5TempTick2 == false && ::puzzle5SlidingPuzzleRange == 1 && ::puzzle5Complete == false && ::puzzle4Complete == true) {
		::puzzle5TempTick2 = true;
		::puzzle5RunningTime++;
	}
	if (::puzzle5TempTick == false && ::puzzle5CountdownCheck == true && ::puzzle5SlidingPuzzleRange == 1 && ::puzzle5Complete == false && ::puzzle4Complete == true) {
		::puzzle5TempTick = true;
		::puzzle5ActiveTime++;
		::puzzle5Countdown--;
	}
	UpdateTimerDisplay();
	if (::puzzle5Countdown < 1) ::puzzle5CountdownCheck = false;
	else ::puzzle5CountdownCheck = true;
}

//When the timer hits 0
void ASlidingPuzzle5::TimerHasFinished_Implementation() {
}