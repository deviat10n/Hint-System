// Fill out your copyright notice in the Description page of Project Settings.

#include "Platforms.h"
#include "SlidingPuzzle.h"
#include "SlidingPuzzle2.h"
#include "SlidingPuzzle3.h"
#include "SlidingPuzzle4.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine.h"
#include "TimerManager.h"
#include "iostream"
#include "Components/InputComponent.h"

extern bool puzzle1Complete;
extern bool puzzle2Complete;
extern bool puzzle3Complete;
extern bool puzzle4Complete;

// Sets default values
APlatforms::APlatforms()
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

	//Set all the button locations
	resetSlot1.X = (-1710.0f);
	resetSlot1.Y = (1040.0f);
	resetSlot1.Z = (80.0f);
	controlSlot1.X = (-1320.0f);
	controlSlot1.Y = (690.0f);
	controlSlot1.Z = (80.0f);
	resetSlot2.X = (-1710.0f);
	resetSlot2.Y = (370.0f);
	resetSlot2.Z = (80.0f);
	controlSlot2.X = (-1320.0f);
	controlSlot2.Y = (20.0f);
	controlSlot2.Z = (80.0f);
	resetSlot3.X = (-1710.0f);
	resetSlot3.Y = (-300.0f);
	resetSlot3.Z = (80.0f);
	controlSlot3.X = (-1320.0f);
	controlSlot3.Y = (-650.0f);
	controlSlot3.Z = (80.0f);
	resetSlot4.X = (-1710.0f);
	resetSlot4.Y = (-970.0f);
	resetSlot4.Z = (80.0f);
	controlSlot4.X = (-1320.0f);
	controlSlot4.Y = (-1320.0f);
	controlSlot4.Z = (80.0f);
	resetSlot5.X = (-1710.0f);
	resetSlot5.Y = (-1640.0f);
	resetSlot5.Z = (80.0f);
	controlSlot5.X = (-1320.0f);
	controlSlot5.Y = (-1990.0f);
	controlSlot5.Z = (80.0f);
}

// Called when the game starts or when spawned
void APlatforms::BeginPlay()
{
	Super::BeginPlay();

	platformActorLoc = this->GetActorLocation();
	
	if (platformActorLoc == resetSlot1) platformID = 1;
	if (platformActorLoc == controlSlot1) platformID = 2;
}

// Called every frame
void APlatforms::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (::puzzle4Complete == true) {
		if (platformID == 1) {
			this->SetActorLocation(resetSlot5);
		}
		if (platformID == 2) {
			this->SetActorLocation(controlSlot5);
		}
	}
	else if (::puzzle3Complete == true) {
		if (platformID == 1) {
			this->SetActorLocation(resetSlot4);
		}
		if (platformID == 2) {
			this->SetActorLocation(controlSlot4);
		}
	}
	else if (::puzzle2Complete == true) {
		if (platformID == 1) {
			this->SetActorLocation(resetSlot3);
		}
		if (platformID == 2) {
			this->SetActorLocation(controlSlot3);
		}
	}
	else if (::puzzle1Complete == true) {
		if (platformID == 1) {
			this->SetActorLocation(resetSlot2);
		}
		if (platformID == 2) {
			this->SetActorLocation(controlSlot2);
		}
	}
	else {
		if (platformID == 1) {
			this->SetActorLocation(resetSlot1);
		}
		if (platformID == 2) {
			this->SetActorLocation(controlSlot1);
		}
	}
}


