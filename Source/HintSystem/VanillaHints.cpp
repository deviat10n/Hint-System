// Fill out your copyright notice in the Description page of Project Settings.

#include "VanillaHints.h"
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

//The player's running time
int vanillaCountdown = 30;

// Sets default values
AVanillaHints::AVanillaHints()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	::vanillaCountdown = 30;
}

// Called when the game starts or when spawned
void AVanillaHints::BeginPlay()
{
	Super::BeginPlay();
	
	//Start the timer
	UpdateTimerDisplay();
	GetWorldTimerManager().SetTimer(TimerTimerHandle, this, &AVanillaHints::AdvanceTimer, 1.0f, true);
}

// Called every frame
void AVanillaHints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("VanillaCountdown: %d"), ::vanillaCountdown));
}

//Update the timer display
void AVanillaHints::UpdateTimerDisplay() {
}

//Make the timers tick
void AVanillaHints::AdvanceTimer() {
	::vanillaCountdown++;
}