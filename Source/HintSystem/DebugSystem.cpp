// Fill out your copyright notice in the Description page of Project Settings.

#include "DebugSystem.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine.h"
#include "TimerManager.h"
#include "iostream"
#include "Components/InputComponent.h"

extern int puzzle1FinalScore;
extern int puzzle2FinalScore;
extern int puzzle3FinalScore;
extern int puzzle4FinalScore;
extern int puzzle5FinalScore;

bool debugPuzzle1 = false;
bool debugPuzzle2 = false;
bool debugPuzzle3 = false;
bool debugPuzzle4 = false;

extern bool puzzle1Complete;
extern bool puzzle2Complete;
extern bool puzzle3Complete;
extern bool puzzle4Complete;
extern bool puzzle5Complete;

// Sets default values
ADebugSystem::ADebugSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	::debugPuzzle1 = false;
	::debugPuzzle2 = false;
	::debugPuzzle3 = false;
	::debugPuzzle4 = false;
}

// Called when the game starts or when spawned
void ADebugSystem::BeginPlay()
{
	Super::BeginPlay();

	//Enable keyboard input to control the puzzle
	PlayerController = GetWorld()->GetFirstPlayerController();
	EnableInput(PlayerController);
	InputComponent->bBlockInput = false;

	// Respond when keys are pressed
	FInputActionBinding& Complete1 = InputComponent->BindAction("Complete1", IE_Released, this, &ADebugSystem::Complete1);
	FInputActionBinding& Complete2 = InputComponent->BindAction("Complete2", IE_Released, this, &ADebugSystem::Complete2);
	FInputActionBinding& Complete3 = InputComponent->BindAction("Complete3", IE_Released, this, &ADebugSystem::Complete3);
	FInputActionBinding& Complete4 = InputComponent->BindAction("Complete4", IE_Released, this, &ADebugSystem::Complete4);
}

// Called every frame
void ADebugSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	puzzle1LocalComplete = ::puzzle1Complete;
	puzzle2LocalComplete = ::puzzle2Complete;
	puzzle3LocalComplete = ::puzzle3Complete;
	puzzle4LocalComplete = ::puzzle4Complete;
	puzzle5LocalComplete = ::puzzle5Complete;
}

//Complete sliding puzzle 1
void ADebugSystem::Complete1()
{
	::debugPuzzle1 = true;
}

//Complete sliding puzzle 2
void ADebugSystem::Complete2()
{
	if(::debugPuzzle1 == true) ::debugPuzzle2 = true;
}
//Complete sliding puzzle 3
void ADebugSystem::Complete3()
{
	if (::debugPuzzle2 == true) ::debugPuzzle3 = true;
}
//Complete sliding puzzle 4
void ADebugSystem::Complete4()
{
	if (::debugPuzzle3 == true) ::debugPuzzle4 = true;
}
