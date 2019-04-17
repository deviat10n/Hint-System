// Fill out your copyright notice in the Description page of Project Settings.

#include "SlidingPuzzle2.h"
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
extern bool debugPuzzle2;

//Whether the player is by the puzzle
extern int puzzle2SlidingPuzzleRange;

//Whether the previous puzzle is complete
extern bool puzzle1Complete;

//Whether puzzle is complete 
bool puzzle2Complete = false;

//Total number of moves
int puzzle2Moves = 0;

//The value of the open puzzle slot
int puzzle2EmptySlot = 9;

//Regulate input
bool puzzle2InputCheck = false;

//Check if the player is on a predetermined path
bool puzzle2OnPath = false;
bool puzzle2BeenOnPath = false;

//The player's running time
int puzzle2RunningTime = 0;
bool puzzle2TempTick2 = false;

//The player's active time
int puzzle2ActiveTime = 0;
int puzzle2Countdown = 5;
bool puzzle2TempTick = false;
bool puzzle2CountdownCheck = true;

//Each block's location
int puzzle2ID1Loc = 7;
int puzzle2ID2Loc = 5;
int puzzle2ID3Loc = 8;
int puzzle2ID4Loc = 2;
int puzzle2ID5Loc = 1;
int puzzle2ID6Loc = 4;
int puzzle2ID7Loc = 6;
int puzzle2ID8Loc = 3;

//Track puzzle progress
float puzzle2Progress = 0.0f;
bool puzzle2ID1LocCheck = false;
bool puzzle2ID2LocCheck = false;
bool puzzle2ID3LocCheck = false;
bool puzzle2ID4LocCheck = false;
bool puzzle2ID5LocCheck = false;
bool puzzle2ID6LocCheck = false;
bool puzzle2ID7LocCheck = false;
bool puzzle2ID8LocCheck = false;

//Path states
int puzzle2PathStep24[] = { 1, 2, 3, 4, 5, 6, 7, 0, 8 };
int puzzle2PathStep23[] = { 1, 2, 3, 4, 0, 6, 7, 5, 8 };
int puzzle2PathStep22[] = { 1, 2, 3, 0, 4, 6, 7, 5, 8 };
int puzzle2PathStep21[] = { 0, 2, 3, 1, 4, 6, 7, 5, 8 };
int puzzle2PathStep20[] = { 2, 0, 3, 1, 4, 6, 7, 5, 8 };
int puzzle2PathStep19[] = { 2, 4, 3, 1, 0, 6, 7, 5, 8 };
int puzzle2PathStep18[] = { 2, 4, 3, 1, 5, 6, 7, 0, 8 };
int puzzle2PathStep17[] = { 2, 4, 3, 1, 5, 6, 0, 7, 8 };
int puzzle2PathStep16[] = { 2, 4, 3, 0, 5, 6, 1, 7, 8 };
int puzzle2PathStep15[] = { 2, 4, 3, 5, 0, 6, 1, 7, 8 };
int puzzle2PathStep14[] = { 2, 4, 3, 5, 6, 0, 1, 7, 8 };
int puzzle2PathStep13[] = { 2, 4, 0, 5, 6, 3, 1, 7, 8 };
int puzzle2PathStep12[] = { 2, 0, 4, 5, 6, 3, 1, 7, 8 };
int puzzle2PathStep11[] = { 0, 2, 4, 5, 6, 3, 1, 7, 8 };
int puzzle2PathStep10[] = { 5, 2, 4, 0, 6, 3, 1, 7, 8 };
int puzzle2PathStep9[] = { 5, 2, 4, 6, 0, 3, 1, 7, 8 };
int puzzle2PathStep8[] = { 5, 2, 4, 6, 3, 0, 1, 7, 8 };
int puzzle2PathStep7[] = { 5, 2, 4, 6, 3, 8, 1, 7, 0 };
int puzzle2PathStep6[] = { 5, 2, 4, 6, 3, 8, 1, 0, 7 };
int puzzle2PathStep5[] = { 5, 2, 4, 6, 0, 8, 1, 3, 7 };
int puzzle2PathStep4[] = { 5, 0, 4, 6, 2, 8, 1, 3, 7 };
int puzzle2PathStep3[] = { 5, 4, 0, 6, 2, 8, 1, 3, 7 };
int puzzle2PathStep2[] = { 5, 4, 8, 6, 2, 0, 1, 3, 7 };
int puzzle2PathStep1[] = { 5, 4, 8, 6, 2, 7, 1, 3, 0 };
int puzzle2PathStep;

//Check if slots are filled
bool puzzle2Slot1Filled = false;
bool puzzle2Slot2Filled = false;
bool puzzle2Slot3Filled = false;
bool puzzle2Slot4Filled = false;
bool puzzle2Slot5Filled = false;
bool puzzle2Slot6Filled = false;
bool puzzle2Slot7Filled = false;
bool puzzle2Slot8Filled = false;
bool puzzle2Slot9Filled = false;

//Current State
int puzzle2CurrentArray[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// Sets default values
ASlidingPuzzle2::ASlidingPuzzle2()
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
	puzzle2Slot1.X = (-2010.0f);
	puzzle2Slot1.Y = (130.0f);
	puzzle2Slot1.Z = (550.0f);
	puzzle2Slot2.X = (-2010.0f);
	puzzle2Slot2.Y = (10.0f);
	puzzle2Slot2.Z = (550.0f);
	puzzle2Slot3.X = (-2010.0f);
	puzzle2Slot3.Y = (-110.0f);
	puzzle2Slot3.Z = (550.0f);
	puzzle2Slot4.X = (-2010.0f);
	puzzle2Slot4.Y = (130.0f);
	puzzle2Slot4.Z = (430.0f);
	puzzle2Slot5.X = (-2010.0f);
	puzzle2Slot5.Y = (10.0f);
	puzzle2Slot5.Z = (430.0f);
	puzzle2Slot6.X = (-2010.0f);
	puzzle2Slot6.Y = (-110.0f);
	puzzle2Slot6.Z = (430.0f);
	puzzle2Slot7.X = (-2010.0f);
	puzzle2Slot7.Y = (130.0f);
	puzzle2Slot7.Z = (310.0f);
	puzzle2Slot8.X = (-2010.0f);
	puzzle2Slot8.Y = (10.0f);
	puzzle2Slot8.Z = (310.0f);
	puzzle2Slot9.X = (-2010.0f);
	puzzle2Slot9.Y = (-110.0f);
	puzzle2Slot9.Z = (310.0f);

	//Default global values
	::puzzle2RunningTime = 0;
	::puzzle2ActiveTime = 0;
	::puzzle2Moves = 0;
	::puzzle2PathStep = 0;
	::puzzle2ID1Loc = 7;
	::puzzle2ID2Loc = 5;
	::puzzle2ID3Loc = 8;
	::puzzle2ID4Loc = 2;
	::puzzle2ID5Loc = 1;
	::puzzle2ID6Loc = 4;
	::puzzle2ID7Loc = 6;
	::puzzle2ID8Loc = 3;
	::puzzle2Countdown = 5;
	::puzzle2EmptySlot = 9;
	::puzzle2Progress = 0.0f;
	::puzzle2InputCheck = false;
	::puzzle2OnPath = false;
	::puzzle2BeenOnPath = false;
	::puzzle2ID1LocCheck = false;
	::puzzle2ID2LocCheck = false;
	::puzzle2ID3LocCheck = false;
	::puzzle2ID4LocCheck = false;
	::puzzle2ID5LocCheck = false;
	::puzzle2ID6LocCheck = false;
	::puzzle2ID7LocCheck = false;
	::puzzle2ID8LocCheck = false;
	::puzzle2Slot1Filled = false;
	::puzzle2Slot2Filled = false;
	::puzzle2Slot3Filled = false;
	::puzzle2Slot4Filled = false;
	::puzzle2Slot5Filled = false;
	::puzzle2Slot6Filled = false;
	::puzzle2Slot7Filled = false;
	::puzzle2Slot8Filled = false;
	::puzzle2Slot9Filled = false;
	::puzzle2Complete = false;
	::puzzle2TempTick = false;
	::puzzle2TempTick2 = false;
	::puzzle2CountdownCheck = true;

	//Update the block's locations
	puzzle2ActorLoc = this->GetActorLocation();
}

// Called when the game starts or when spawned
void ASlidingPuzzle2::BeginPlay()
{
	Super::BeginPlay();

	//Start the timer
	UpdateTimerDisplay();
	GetWorldTimerManager().SetTimer(TimerTimerHandle, this, &ASlidingPuzzle2::AdvanceTimer, 1.0f, true);

	//Get the block's location
	puzzle2ActorLoc = this->GetActorLocation();

	//Give each block an ID
	if ((puzzle2ActorLoc.Y == puzzle2Slot7.Y) && (puzzle2ActorLoc.Z == puzzle2Slot7.Z)) puzzle2ID = 1;
	if ((puzzle2ActorLoc.Y == puzzle2Slot5.Y) && (puzzle2ActorLoc.Z == puzzle2Slot5.Z)) puzzle2ID = 2;
	if ((puzzle2ActorLoc.Y == puzzle2Slot8.Y) && (puzzle2ActorLoc.Z == puzzle2Slot8.Z)) puzzle2ID = 3;
	if ((puzzle2ActorLoc.Y == puzzle2Slot2.Y) && (puzzle2ActorLoc.Z == puzzle2Slot2.Z)) puzzle2ID = 4;
	if ((puzzle2ActorLoc.Y == puzzle2Slot1.Y) && (puzzle2ActorLoc.Z == puzzle2Slot1.Z)) puzzle2ID = 5;
	if ((puzzle2ActorLoc.Y == puzzle2Slot4.Y) && (puzzle2ActorLoc.Z == puzzle2Slot4.Z)) puzzle2ID = 6;
	if ((puzzle2ActorLoc.Y == puzzle2Slot6.Y) && (puzzle2ActorLoc.Z == puzzle2Slot6.Z)) puzzle2ID = 7;
	if ((puzzle2ActorLoc.Y == puzzle2Slot3.Y) && (puzzle2ActorLoc.Z == puzzle2Slot3.Z)) puzzle2ID = 8;
	if ((puzzle2ActorLoc.Y == puzzle2Slot9.Y) && (puzzle2ActorLoc.Z == puzzle2Slot9.Z)) puzzle2ID = 9;

	//Don't block inputs
	bBlockInput = false;

	//Enable keyboard input to control the puzzle
	PlayerController = GetWorld()->GetFirstPlayerController();
	EnableInput(PlayerController);
	InputComponent->bBlockInput = false;

	// Respond when keys are pressed
	FInputActionBinding& UpBinding = InputComponent->BindAction("SlideUp", IE_Released, this, &ASlidingPuzzle2::UpReleased);
	FInputActionBinding& DownBinding = InputComponent->BindAction("SlideDown", IE_Released, this, &ASlidingPuzzle2::DownReleased);
	FInputActionBinding& LeftBinding = InputComponent->BindAction("SlideLeft", IE_Released, this, &ASlidingPuzzle2::LeftReleased);
	FInputActionBinding& RightBinding = InputComponent->BindAction("SlideRight", IE_Released, this, &ASlidingPuzzle2::RightReleased);

	//Don't consume inputs
	UpBinding.bConsumeInput = false;
	DownBinding.bConsumeInput = false;
	LeftBinding.bConsumeInput = false;
	RightBinding.bConsumeInput = false;
}

// Called every frame
void ASlidingPuzzle2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (::puzzle1Complete == true && ::puzzle2Complete == false) {

		//Puts the puzzle on screen if previous is solved
		puzzle2ActorLoc.X = -2010.0f;

		//Update block's location if moved
		SetActorLocation(puzzle2ActorLoc);

		//Reset input check
		::puzzle2InputCheck = false;
		::puzzle2TempTick = false;
		::puzzle2TempTick2 = false;

		//Debug complete puzzle
		if (::debugPuzzle2 == true) {
			if (puzzle2ID == 1) puzzle2ActorLoc = puzzle2Slot1;
			if (puzzle2ID == 2) puzzle2ActorLoc = puzzle2Slot2;
			if (puzzle2ID == 3) puzzle2ActorLoc = puzzle2Slot3;
			if (puzzle2ID == 4) puzzle2ActorLoc = puzzle2Slot4;
			if (puzzle2ID == 5) puzzle2ActorLoc = puzzle2Slot5;
			if (puzzle2ID == 6) puzzle2ActorLoc = puzzle2Slot6;
			if (puzzle2ID == 7) puzzle2ActorLoc = puzzle2Slot7;
			if (puzzle2ID == 8) puzzle2ActorLoc = puzzle2Slot8;
			puzzle2Complete = true;
		}

		//Determine the location of each block
		if (puzzle2ID == 1 && puzzle2ActorLoc == puzzle2Slot1) {
			::puzzle2ID1Loc = 1;
			::puzzle2Slot1Filled = true;
		}
		if (puzzle2ID == 1 && puzzle2ActorLoc == puzzle2Slot2) {
			::puzzle2ID1Loc = 2;
			::puzzle2Slot2Filled = true;
		}
		if (puzzle2ID == 1 && puzzle2ActorLoc == puzzle2Slot3) {
			::puzzle2ID1Loc = 3;
			::puzzle2Slot3Filled = true;
		}
		if (puzzle2ID == 1 && puzzle2ActorLoc == puzzle2Slot4) {
			::puzzle2ID1Loc = 4;
			::puzzle2Slot4Filled = true;
		}
		if (puzzle2ID == 1 && puzzle2ActorLoc == puzzle2Slot5) {
			::puzzle2ID1Loc = 5;
			::puzzle2Slot5Filled = true;
		}
		if (puzzle2ID == 1 && puzzle2ActorLoc == puzzle2Slot6) {
			::puzzle2ID1Loc = 6;
			::puzzle2Slot6Filled = true;
		}
		if (puzzle2ID == 1 && puzzle2ActorLoc == puzzle2Slot7) {
			::puzzle2ID1Loc = 7;
			::puzzle2Slot7Filled = true;
		}
		if (puzzle2ID == 1 && puzzle2ActorLoc == puzzle2Slot8) {
			::puzzle2ID1Loc = 8;
			::puzzle2Slot8Filled = true;
		}
		if (puzzle2ID == 1 && puzzle2ActorLoc == puzzle2Slot9) {
			::puzzle2ID1Loc = 9;
			::puzzle2Slot9Filled = true;
		}
		if (puzzle2ID == 2 && puzzle2ActorLoc == puzzle2Slot1) {
			::puzzle2ID2Loc = 1;
			::puzzle2Slot1Filled = true;
		}
		if (puzzle2ID == 2 && puzzle2ActorLoc == puzzle2Slot2) {
			::puzzle2ID2Loc = 2;
			::puzzle2Slot2Filled = true;
		}
		if (puzzle2ID == 2 && puzzle2ActorLoc == puzzle2Slot3) {
			::puzzle2ID2Loc = 3;
			::puzzle2Slot3Filled = true;
		}
		if (puzzle2ID == 2 && puzzle2ActorLoc == puzzle2Slot4) {
			::puzzle2ID2Loc = 4;
			::puzzle2Slot4Filled = true;
		}
		if (puzzle2ID == 2 && puzzle2ActorLoc == puzzle2Slot5) {
			::puzzle2ID2Loc = 5;
			::puzzle2Slot5Filled = true;
		}
		if (puzzle2ID == 2 && puzzle2ActorLoc == puzzle2Slot6) {
			::puzzle2ID2Loc = 6;
			::puzzle2Slot6Filled = true;
		}
		if (puzzle2ID == 2 && puzzle2ActorLoc == puzzle2Slot7) {
			::puzzle2ID2Loc = 7;
			::puzzle2Slot7Filled = true;
		}
		if (puzzle2ID == 2 && puzzle2ActorLoc == puzzle2Slot8) {
			::puzzle2ID2Loc = 8;
			::puzzle2Slot8Filled = true;
		}
		if (puzzle2ID == 2 && puzzle2ActorLoc == puzzle2Slot9) {
			::puzzle2ID2Loc = 9;
			::puzzle2Slot9Filled = true;
		}
		if (puzzle2ID == 3 && puzzle2ActorLoc == puzzle2Slot1) {
			::puzzle2ID3Loc = 1;
			::puzzle2Slot1Filled = true;
		}
		if (puzzle2ID == 3 && puzzle2ActorLoc == puzzle2Slot2) {
			::puzzle2ID3Loc = 2;
			::puzzle2Slot2Filled = true;
		}
		if (puzzle2ID == 3 && puzzle2ActorLoc == puzzle2Slot3) {
			::puzzle2ID3Loc = 3;
			::puzzle2Slot3Filled = true;
		}
		if (puzzle2ID == 3 && puzzle2ActorLoc == puzzle2Slot4) {
			::puzzle2ID3Loc = 4;
			::puzzle2Slot4Filled = true;
		}
		if (puzzle2ID == 3 && puzzle2ActorLoc == puzzle2Slot5) {
			::puzzle2ID3Loc = 5;
			::puzzle2Slot5Filled = true;
		}
		if (puzzle2ID == 3 && puzzle2ActorLoc == puzzle2Slot6) {
			::puzzle2ID3Loc = 6;
			::puzzle2Slot6Filled = true;
		}
		if (puzzle2ID == 3 && puzzle2ActorLoc == puzzle2Slot7) {
			::puzzle2ID3Loc = 7;
			::puzzle2Slot7Filled = true;
		}
		if (puzzle2ID == 3 && puzzle2ActorLoc == puzzle2Slot8) {
			::puzzle2ID3Loc = 8;
			::puzzle2Slot8Filled = true;
		}
		if (puzzle2ID == 3 && puzzle2ActorLoc == puzzle2Slot9) {
			::puzzle2ID3Loc = 9;
			::puzzle2Slot9Filled = true;
		}
		if (puzzle2ID == 4 && puzzle2ActorLoc == puzzle2Slot1) {
			::puzzle2ID4Loc = 1;
			::puzzle2Slot1Filled = true;
		}
		if (puzzle2ID == 4 && puzzle2ActorLoc == puzzle2Slot2) {
			::puzzle2ID4Loc = 2;
			::puzzle2Slot2Filled = true;
		}
		if (puzzle2ID == 4 && puzzle2ActorLoc == puzzle2Slot3) {
			::puzzle2ID4Loc = 3;
			::puzzle2Slot3Filled = true;
		}
		if (puzzle2ID == 4 && puzzle2ActorLoc == puzzle2Slot4) {
			::puzzle2ID4Loc = 4;
			::puzzle2Slot4Filled = true;
		}
		if (puzzle2ID == 4 && puzzle2ActorLoc == puzzle2Slot5) {
			::puzzle2ID4Loc = 5;
			::puzzle2Slot5Filled = true;
		}
		if (puzzle2ID == 4 && puzzle2ActorLoc == puzzle2Slot6) {
			::puzzle2ID4Loc = 6;
			::puzzle2Slot6Filled = true;
		}
		if (puzzle2ID == 4 && puzzle2ActorLoc == puzzle2Slot7) {
			::puzzle2ID4Loc = 7;
			::puzzle2Slot7Filled = true;
		}
		if (puzzle2ID == 4 && puzzle2ActorLoc == puzzle2Slot8) {
			::puzzle2ID4Loc = 8;
			::puzzle2Slot8Filled = true;
		}
		if (puzzle2ID == 4 && puzzle2ActorLoc == puzzle2Slot9) {
			::puzzle2ID4Loc = 9;
			::puzzle2Slot9Filled = true;
		}
		if (puzzle2ID == 5 && puzzle2ActorLoc == puzzle2Slot1) {
			::puzzle2ID5Loc = 1;
			::puzzle2Slot1Filled = true;
		}
		if (puzzle2ID == 5 && puzzle2ActorLoc == puzzle2Slot2) {
			::puzzle2ID5Loc = 2;
			::puzzle2Slot2Filled = true;
		}
		if (puzzle2ID == 5 && puzzle2ActorLoc == puzzle2Slot3) {
			::puzzle2ID5Loc = 3;
			::puzzle2Slot3Filled = true;
		}
		if (puzzle2ID == 5 && puzzle2ActorLoc == puzzle2Slot4) {
			::puzzle2ID5Loc = 4;
			::puzzle2Slot4Filled = true;
		}
		if (puzzle2ID == 5 && puzzle2ActorLoc == puzzle2Slot5) {
			::puzzle2ID5Loc = 5;
			::puzzle2Slot5Filled = true;
		}
		if (puzzle2ID == 5 && puzzle2ActorLoc == puzzle2Slot6) {
			::puzzle2ID5Loc = 6;
			::puzzle2Slot6Filled = true;
		}
		if (puzzle2ID == 5 && puzzle2ActorLoc == puzzle2Slot7) {
			::puzzle2ID5Loc = 7;
			::puzzle2Slot7Filled = true;
		}
		if (puzzle2ID == 5 && puzzle2ActorLoc == puzzle2Slot8) {
			::puzzle2ID5Loc = 8;
			::puzzle2Slot8Filled = true;
		}
		if (puzzle2ID == 5 && puzzle2ActorLoc == puzzle2Slot9) {
			::puzzle2ID5Loc = 9;
			::puzzle2Slot9Filled = true;
		}
		if (puzzle2ID == 6 && puzzle2ActorLoc == puzzle2Slot1) {
			::puzzle2ID6Loc = 1;
			::puzzle2Slot1Filled = true;
		}
		if (puzzle2ID == 6 && puzzle2ActorLoc == puzzle2Slot2) {
			::puzzle2ID6Loc = 2;
			::puzzle2Slot2Filled = true;
		}
		if (puzzle2ID == 6 && puzzle2ActorLoc == puzzle2Slot3) {
			::puzzle2ID6Loc = 3;
			::puzzle2Slot3Filled = true;
		}
		if (puzzle2ID == 6 && puzzle2ActorLoc == puzzle2Slot4) {
			::puzzle2ID6Loc = 4;
			::puzzle2Slot4Filled = true;
		}
		if (puzzle2ID == 6 && puzzle2ActorLoc == puzzle2Slot5) {
			::puzzle2ID6Loc = 5;
			::puzzle2Slot5Filled = true;
		}
		if (puzzle2ID == 6 && puzzle2ActorLoc == puzzle2Slot6) {
			::puzzle2ID6Loc = 6;
			::puzzle2Slot6Filled = true;
		}
		if (puzzle2ID == 6 && puzzle2ActorLoc == puzzle2Slot7) {
			::puzzle2ID6Loc = 7;
			::puzzle2Slot7Filled = true;
		}
		if (puzzle2ID == 6 && puzzle2ActorLoc == puzzle2Slot8) {
			::puzzle2ID6Loc = 8;
			::puzzle2Slot8Filled = true;
		}
		if (puzzle2ID == 6 && puzzle2ActorLoc == puzzle2Slot9) {
			::puzzle2ID6Loc = 9;
			::puzzle2Slot9Filled = true;
		}
		if (puzzle2ID == 7 && puzzle2ActorLoc == puzzle2Slot1) {
			::puzzle2ID7Loc = 1;
			::puzzle2Slot1Filled = true;
		}
		if (puzzle2ID == 7 && puzzle2ActorLoc == puzzle2Slot2) {
			::puzzle2ID7Loc = 2;
			::puzzle2Slot2Filled = true;
		}
		if (puzzle2ID == 7 && puzzle2ActorLoc == puzzle2Slot3) {
			::puzzle2ID7Loc = 3;
			::puzzle2Slot3Filled = true;
		}
		if (puzzle2ID == 7 && puzzle2ActorLoc == puzzle2Slot4) {
			::puzzle2ID7Loc = 4;
			::puzzle2Slot4Filled = true;
		}
		if (puzzle2ID == 7 && puzzle2ActorLoc == puzzle2Slot5) {
			::puzzle2ID7Loc = 5;
			::puzzle2Slot5Filled = true;
		}
		if (puzzle2ID == 7 && puzzle2ActorLoc == puzzle2Slot6) {
			::puzzle2ID7Loc = 6;
			::puzzle2Slot6Filled = true;
		}
		if (puzzle2ID == 7 && puzzle2ActorLoc == puzzle2Slot7) {
			::puzzle2ID7Loc = 7;
			::puzzle2Slot7Filled = true;
		}
		if (puzzle2ID == 7 && puzzle2ActorLoc == puzzle2Slot8) {
			::puzzle2ID7Loc = 8;
			::puzzle2Slot8Filled = true;
		}
		if (puzzle2ID == 7 && puzzle2ActorLoc == puzzle2Slot9) {
			::puzzle2ID7Loc = 9;
			::puzzle2Slot9Filled = true;
		}
		if (puzzle2ID == 8 && puzzle2ActorLoc == puzzle2Slot1) {
			::puzzle2ID8Loc = 1;
			::puzzle2Slot1Filled = true;
		}
		if (puzzle2ID == 8 && puzzle2ActorLoc == puzzle2Slot2) {
			::puzzle2ID8Loc = 2;
			::puzzle2Slot2Filled = true;
		}
		if (puzzle2ID == 8 && puzzle2ActorLoc == puzzle2Slot3) {
			::puzzle2ID8Loc = 3;
			::puzzle2Slot3Filled = true;
		}
		if (puzzle2ID == 8 && puzzle2ActorLoc == puzzle2Slot4) {
			::puzzle2ID8Loc = 4;
			::puzzle2Slot4Filled = true;
		}
		if (puzzle2ID == 8 && puzzle2ActorLoc == puzzle2Slot5) {
			::puzzle2ID8Loc = 5;
			::puzzle2Slot5Filled = true;
		}
		if (puzzle2ID == 8 && puzzle2ActorLoc == puzzle2Slot6) {
			::puzzle2ID8Loc = 6;
			::puzzle2Slot6Filled = true;
		}
		if (puzzle2ID == 8 && puzzle2ActorLoc == puzzle2Slot7) {
			::puzzle2ID8Loc = 7;
			::puzzle2Slot7Filled = true;
		}
		if (puzzle2ID == 8 && puzzle2ActorLoc == puzzle2Slot8) {
			::puzzle2ID8Loc = 8;
			::puzzle2Slot8Filled = true;
		}
		if (puzzle2ID == 8 && puzzle2ActorLoc == puzzle2Slot9) {
			::puzzle2ID8Loc = 9;
			::puzzle2Slot9Filled = true;
		}

		//Fill the empty slot as 0
		if (::puzzle2Slot1Filled == false) ::puzzle2CurrentArray[0] = 0;
		if (::puzzle2Slot2Filled == false) ::puzzle2CurrentArray[1] = 0;
		if (::puzzle2Slot3Filled == false) ::puzzle2CurrentArray[2] = 0;
		if (::puzzle2Slot4Filled == false) ::puzzle2CurrentArray[3] = 0;
		if (::puzzle2Slot5Filled == false) ::puzzle2CurrentArray[4] = 0;
		if (::puzzle2Slot6Filled == false) ::puzzle2CurrentArray[5] = 0;
		if (::puzzle2Slot7Filled == false) ::puzzle2CurrentArray[6] = 0;
		if (::puzzle2Slot8Filled == false) ::puzzle2CurrentArray[7] = 0;
		if (::puzzle2Slot9Filled == false) ::puzzle2CurrentArray[8] = 0;

		//Reset the filled values to update
		::puzzle2Slot1Filled = false;
		::puzzle2Slot2Filled = false;
		::puzzle2Slot3Filled = false;
		::puzzle2Slot4Filled = false;
		::puzzle2Slot5Filled = false;
		::puzzle2Slot6Filled = false;
		::puzzle2Slot7Filled = false;
		::puzzle2Slot8Filled = false;
		::puzzle2Slot9Filled = false;

		//Update progress
		if (::puzzle2ID1Loc == 7 && ::puzzle2ID1LocCheck == false) {
			::puzzle2Progress = ::puzzle2Progress + 12.5f;
			puzzle2ID1LocCheck = true;
		}
		if (::puzzle2ID2Loc == 5 && ::puzzle2ID2LocCheck == false) {
			::puzzle2Progress = ::puzzle2Progress + 12.5f;
			puzzle2ID2LocCheck = true;
		}
		if (::puzzle2ID3Loc == 8 && ::puzzle2ID3LocCheck == false) {
			::puzzle2Progress = ::puzzle2Progress + 12.5f;
			puzzle2ID3LocCheck = true;
		}
		if (::puzzle2ID4Loc == 2 && ::puzzle2ID4LocCheck == false) {
			::puzzle2Progress = ::puzzle2Progress + 12.5f;
			puzzle2ID4LocCheck = true;
		}
		if (::puzzle2ID5Loc == 1 && ::puzzle2ID5LocCheck == false) {
			::puzzle2Progress = ::puzzle2Progress + 12.5f;
			puzzle2ID5LocCheck = true;
		}
		if (::puzzle2ID6Loc == 4 && ::puzzle2ID6LocCheck == false) {
			::puzzle2Progress = ::puzzle2Progress + 12.5f;
			puzzle2ID6LocCheck = true;
		}
		if (::puzzle2ID7Loc == 6 && ::puzzle2ID7LocCheck == false) {
			::puzzle2Progress = ::puzzle2Progress + 12.5f;
			puzzle2ID7LocCheck = true;
		}
		if (::puzzle2ID8Loc == 3 && ::puzzle2ID8LocCheck == false) {
			::puzzle2Progress = ::puzzle2Progress + 12.5f;
			puzzle2ID8LocCheck = true;
		}
		if (::puzzle2ID1Loc != 7 && puzzle2ID1LocCheck == true) {
			::puzzle2Progress = ::puzzle2Progress - 12.5f;
			puzzle2ID1LocCheck = false;
		}
		if (::puzzle2ID2Loc != 5 && ::puzzle2ID2LocCheck == true) {
			::puzzle2Progress = ::puzzle2Progress - 12.5f;
			puzzle2ID2LocCheck = false;
		}
		if (::puzzle2ID3Loc != 8 && ::puzzle2ID3LocCheck == true) {
			::puzzle2Progress = ::puzzle2Progress - 12.5f;
			puzzle2ID3LocCheck = false;
		}
		if (::puzzle2ID4Loc != 2 && ::puzzle2ID4LocCheck == true) {
			::puzzle2Progress = ::puzzle2Progress - 12.5f;
			puzzle2ID4LocCheck = false;
		}
		if (::puzzle2ID5Loc != 1 && ::puzzle2ID5LocCheck == true) {
			::puzzle2Progress = ::puzzle2Progress - 12.5f;
			puzzle2ID5LocCheck = false;
		}
		if (::puzzle2ID6Loc != 4 && ::puzzle2ID6LocCheck == true) {
			::puzzle2Progress = ::puzzle2Progress - 12.5f;
			puzzle2ID6LocCheck = false;
		}
		if (::puzzle2ID7Loc != 6 && ::puzzle2ID7LocCheck == true) {
			::puzzle2Progress = ::puzzle2Progress - 12.5f;
			puzzle2ID7LocCheck = false;
		}
		if (::puzzle2ID8Loc != 3 && ::puzzle2ID8LocCheck == true) {
			::puzzle2Progress = ::puzzle2Progress - 12.5f;
			puzzle2ID8LocCheck = false;
		}

		//Ensure you cannot get negative progress
		if (::puzzle2Progress < 0) ::puzzle2Progress = 0;

		//Fill the array with block locations
		::puzzle2CurrentArray[puzzle2ID1Loc - 1] = 1;
		::puzzle2CurrentArray[puzzle2ID2Loc - 1] = 2;
		::puzzle2CurrentArray[puzzle2ID3Loc - 1] = 3;
		::puzzle2CurrentArray[puzzle2ID4Loc - 1] = 4;
		::puzzle2CurrentArray[puzzle2ID5Loc - 1] = 5;
		::puzzle2CurrentArray[puzzle2ID6Loc - 1] = 6;
		::puzzle2CurrentArray[puzzle2ID7Loc - 1] = 7;
		::puzzle2CurrentArray[puzzle2ID8Loc - 1] = 8;

		//Check if the player is following the predetermined puzzle2Path
		if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep1[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep1[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep1[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep1[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep1[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep1[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep1[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep1[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep1[8])) {
			::puzzle2PathStep = 1;
			::puzzle2Progress = (100.0f / 24.0f) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep2[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep2[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep2[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep2[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep2[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep2[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep2[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep2[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep2[8])) {
			::puzzle2PathStep = 2;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep3[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep3[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep3[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep3[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep3[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep3[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep3[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep3[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep3[8])) {
			::puzzle2PathStep = 3;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep4[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep4[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep4[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep4[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep4[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep4[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep4[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep4[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep4[8])) {
			::puzzle2PathStep = 4;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep5[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep5[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep5[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep5[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep5[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep5[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep5[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep5[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep5[8])) {
			::puzzle2PathStep = 5;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep6[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep6[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep6[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep6[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep6[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep6[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep6[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep6[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep6[8])) {
			::puzzle2PathStep = 6;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep7[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep7[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep7[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep7[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep7[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep7[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep7[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep7[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep7[8])) {
			::puzzle2PathStep = 7;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep8[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep8[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep8[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep8[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep8[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep8[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep8[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep8[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep8[8])) {
			::puzzle2PathStep = 8;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep9[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep9[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep9[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep9[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep9[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep9[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep9[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep9[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep9[8])) {
			::puzzle2PathStep = 9;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep10[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep10[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep10[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep10[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep10[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep10[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep10[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep10[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep10[8])) {
			::puzzle2PathStep = 10;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep11[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep11[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep11[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep11[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep11[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep11[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep11[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep11[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep11[8])) {
			::puzzle2PathStep = 11;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep12[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep12[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep12[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep12[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep12[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep12[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep12[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep12[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep12[8])) {
			::puzzle2PathStep = 12;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep13[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep13[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep13[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep13[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep13[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep13[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep13[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep13[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep13[8])) {
			::puzzle2PathStep = 13;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep14[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep14[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep14[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep14[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep14[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep14[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep14[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep14[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep14[8])) {
			::puzzle2PathStep = 14;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep15[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep15[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep15[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep15[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep15[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep15[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep15[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep15[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep15[8])) {
			::puzzle2PathStep = 15;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep16[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep16[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep16[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep16[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep16[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep16[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep16[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep16[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep16[8])) {
			::puzzle2PathStep = 16;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep17[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep17[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep17[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep17[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep17[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep17[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep17[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep17[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep17[8])) {
			::puzzle2PathStep = 17;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep18[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep18[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep18[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep18[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep18[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep18[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep18[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep18[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep18[8])) {
			::puzzle2PathStep = 18;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep19[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep19[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep19[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep19[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep19[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep19[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep19[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep19[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep19[8])) {
			::puzzle2PathStep = 19;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep20[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep20[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep20[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep20[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep20[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep20[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep20[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep20[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep20[8])) {
			::puzzle2PathStep = 20;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep21[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep21[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep21[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep21[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep21[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep21[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep21[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep21[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep21[8])) {
			::puzzle2PathStep = 21;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep22[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep22[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep22[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep22[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep22[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep22[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep22[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep22[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep22[8])) {
			::puzzle2PathStep = 22;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep23[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep23[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep23[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep23[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep23[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep23[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep23[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep23[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep23[8])) {
			::puzzle2PathStep = 23;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if ((::puzzle2CurrentArray[0] == ::puzzle2PathStep24[0]) && (::puzzle2CurrentArray[0] == ::puzzle2PathStep24[0]) && (::puzzle2CurrentArray[2] == ::puzzle2PathStep24[2]) && (::puzzle2CurrentArray[3] == ::puzzle2PathStep24[3]) && (::puzzle2CurrentArray[4] == ::puzzle2PathStep24[4]) && (::puzzle2CurrentArray[5] == ::puzzle2PathStep24[5]) && (::puzzle2CurrentArray[6] == ::puzzle2PathStep24[6]) && (::puzzle2CurrentArray[7] == ::puzzle2PathStep24[7]) && (::puzzle2CurrentArray[8] == ::puzzle2PathStep24[8])) {
			::puzzle2PathStep = 24;
			::puzzle2Progress = (100 / 24) * puzzle2PathStep;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
		}
		else if (::puzzle2ID1Loc == 1 && ::puzzle2ID2Loc == 2 && ::puzzle2ID3Loc == 3 && ::puzzle2ID4Loc == 4 && ::puzzle2ID5Loc == 5 && ::puzzle2ID6Loc == 6 && ::puzzle2ID7Loc == 7 && ::puzzle2ID8Loc == 8) {
			::puzzle2Progress = 100.0f;
			puzzle2OnPath = true;
			puzzle2BeenOnPath = true;
			::puzzle2Complete = true;
		}
		else if (puzzle2BeenOnPath == true) {
			::puzzle2PathStep = 0;
			::puzzle2Progress = 0;
			puzzle2OnPath = false;
			puzzle2BeenOnPath = false;
		}

		//Get the player's location
		playerLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		//Reset the puzzle
		if (playerLoc.X < -1620.0f && playerLoc.X > -1800.0f && playerLoc.Y > 280.0f && playerLoc.Y < 460.0f) {
			if (puzzle2ID == 1) puzzle2ActorLoc = puzzle2Slot7;
			if (puzzle2ID == 2) puzzle2ActorLoc = puzzle2Slot5;
			if (puzzle2ID == 3) puzzle2ActorLoc = puzzle2Slot8;
			if (puzzle2ID == 4) puzzle2ActorLoc = puzzle2Slot2;
			if (puzzle2ID == 5) puzzle2ActorLoc = puzzle2Slot1;
			if (puzzle2ID == 6) puzzle2ActorLoc = puzzle2Slot4;
			if (puzzle2ID == 7) puzzle2ActorLoc = puzzle2Slot6;
			if (puzzle2ID == 8) puzzle2ActorLoc = puzzle2Slot3;
			if (puzzle2ID == 9) puzzle2ActorLoc = puzzle2Slot9;

			::puzzle2EmptySlot = 9;
		}
	}
}

//When Up is pressed
void ASlidingPuzzle2::UpReleased()
{
	::puzzle2Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -120) && (playerLoc.Y < 160) && ::puzzle2Complete == false && ::puzzle1Complete == true) {

		//Check if the block can be moved
		if (::puzzle2EmptySlot == 6) {
			if (puzzle2ActorLoc == puzzle2Slot9 && ::puzzle2InputCheck == false) {
				puzzle2ActorLoc = puzzle2Slot6;
				::puzzle2EmptySlot = 9;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 5 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot8) {
				puzzle2ActorLoc = puzzle2Slot5;
				::puzzle2EmptySlot = 8;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 4 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot7) {
				puzzle2ActorLoc = puzzle2Slot4;
				::puzzle2EmptySlot = 7;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 3 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot6) {
				puzzle2ActorLoc = puzzle2Slot3;
				::puzzle2EmptySlot = 6;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 2 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot5) {
				puzzle2ActorLoc = puzzle2Slot2;
				::puzzle2EmptySlot = 5;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 1 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot4) {
				puzzle2ActorLoc = puzzle2Slot1;
				::puzzle2EmptySlot = 4;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
	}
}

//When Down is pressed
void ASlidingPuzzle2::DownReleased()
{
	::puzzle2Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -120) && (playerLoc.Y < 160) && ::puzzle2Complete == false && ::puzzle1Complete == true) {

		//Check if the block can be moved
		if (::puzzle2EmptySlot == 4 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot1) {
				puzzle2ActorLoc = puzzle2Slot4;
				::puzzle2EmptySlot = 1;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 5 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot2) {
				puzzle2ActorLoc = puzzle2Slot5;
				::puzzle2EmptySlot = 2;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 6 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot3) {
				puzzle2ActorLoc = puzzle2Slot6;
				::puzzle2EmptySlot = 3;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 7 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot4) {
				puzzle2ActorLoc = puzzle2Slot7;
				::puzzle2EmptySlot = 4;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 8 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot5) {
				puzzle2ActorLoc = puzzle2Slot8;
				::puzzle2EmptySlot = 5;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 9 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot6) {
				puzzle2ActorLoc = puzzle2Slot9;
				::puzzle2EmptySlot = 6;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
	}
}

//When Left is pressed
void ASlidingPuzzle2::LeftReleased()
{
	::puzzle2Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -120) && (playerLoc.Y < 160) && ::puzzle2Complete == false && ::puzzle1Complete == true) {

		//Check if the block can be moved
		if (::puzzle2EmptySlot == 2 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot3) {
				puzzle2ActorLoc = puzzle2Slot2;
				::puzzle2EmptySlot = 3;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 5 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot6) {
				puzzle2ActorLoc = puzzle2Slot5;
				::puzzle2EmptySlot = 6;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 8 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot9) {
				puzzle2ActorLoc = puzzle2Slot8;
				::puzzle2EmptySlot = 9;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 1 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot2) {
				puzzle2ActorLoc = puzzle2Slot1;
				::puzzle2EmptySlot = 2;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 4 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot5) {
				puzzle2ActorLoc = puzzle2Slot4;
				::puzzle2EmptySlot = 5;
				::puzzle2InputCheck = true;
			}
		}
		if (::puzzle2EmptySlot == 7 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot8) {
				puzzle2ActorLoc = puzzle2Slot7;
				::puzzle2EmptySlot = 8;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
	}
}

//When Right is pressed
void ASlidingPuzzle2::RightReleased()
{
	::puzzle2Countdown = 5;
	if ((playerLoc.X > -1460) && (playerLoc.X < -1180) && (playerLoc.Y > -120) && (playerLoc.Y < 160) && ::puzzle2Complete == false && ::puzzle1Complete == true) {

		//Check if the block can be moved
		if (::puzzle2EmptySlot == 2 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot1) {
				puzzle2ActorLoc = puzzle2Slot2;
				::puzzle2EmptySlot = 1;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 5 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot4) {
				puzzle2ActorLoc = puzzle2Slot5;
				::puzzle2EmptySlot = 4;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 8 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot7) {
				puzzle2ActorLoc = puzzle2Slot8;
				::puzzle2EmptySlot = 7;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 3 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot2) {
				puzzle2ActorLoc = puzzle2Slot3;
				::puzzle2EmptySlot = 2;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 6 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot5) {
				puzzle2ActorLoc = puzzle2Slot6;
				::puzzle2EmptySlot = 5;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
		if (::puzzle2EmptySlot == 9 && ::puzzle2InputCheck == false) {
			if (puzzle2ActorLoc == puzzle2Slot8) {
				puzzle2ActorLoc = puzzle2Slot9;
				::puzzle2EmptySlot = 8;
				::puzzle2InputCheck = true;
				::puzzle2Moves++;
			}
		}
	}
}

//Update the timer display
void ASlidingPuzzle2::UpdateTimerDisplay() {
	TimerText->SetText(FString::FromInt(FMath::Max(::puzzle2ActiveTime, 0)));
}

//Make the timers tick
void ASlidingPuzzle2::AdvanceTimer() {
	if (::puzzle2TempTick2 == false && ::puzzle2SlidingPuzzleRange == 1 && ::puzzle2Complete == false && ::puzzle1Complete == true) {
		::puzzle2TempTick2 = true;
		::puzzle2RunningTime++;
	}
	if (::puzzle2TempTick == false && ::puzzle2CountdownCheck == true && ::puzzle2SlidingPuzzleRange == 1 && ::puzzle1Complete == true && ::puzzle2Complete == false) {
		::puzzle2TempTick = true;
		::puzzle2ActiveTime++;
		::puzzle2Countdown--;
	}
	UpdateTimerDisplay();
	if (::puzzle2Countdown < 1) ::puzzle2CountdownCheck = false;
	else ::puzzle2CountdownCheck = true;
}

//When the timer hits 0
void ASlidingPuzzle2::TimerHasFinished_Implementation() {
}
