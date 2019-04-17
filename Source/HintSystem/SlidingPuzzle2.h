// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "SlidingPuzzle2.generated.h"

UCLASS()
class HINTSYSTEM_API ASlidingPuzzle2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlidingPuzzle2();

	// Direction vector
	UPROPERTY(EditAnywhere)
		FVector movementDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		USceneComponent* OurVisibleComponent;

	//Movement controls
	void UpReleased();
	void DownReleased();
	void LeftReleased();
	void RightReleased();

	//The boxes location
	FVector Location;

	//Blocks to spawn
	ASlidingPuzzle2* puzzle2Block1;
	ASlidingPuzzle2* puzzle2Block2;
	ASlidingPuzzle2* puzzle2Block3;
	ASlidingPuzzle2* puzzle2Block4;
	ASlidingPuzzle2* puzzle2Block5;
	ASlidingPuzzle2* puzzle2Block6;
	ASlidingPuzzle2* puzzle2Block7;
	ASlidingPuzzle2* puzzle2Block8;

	//The movement direction
	float moveDirection;

	//The Player's controller
	APlayerController* PlayerController;

	UTextRenderComponent* TimerText;

	void UpdateTimerDisplay();

	void AdvanceTimer();

	UFUNCTION(BlueprintNativeEvent)
		void TimerHasFinished();

	virtual void TimerHasFinished_Implementation();

	FTimerHandle TimerTimerHandle;

private:
	// A static mesh component. The visual representation of our actor
	class UStaticMeshComponent* BoxVisual;

	// A class to store the human player
	class APawn* Player;

	//The box slots
	FVector puzzle2Slot1;
	FVector puzzle2Slot2;
	FVector puzzle2Slot3;
	FVector puzzle2Slot4;
	FVector puzzle2Slot5;
	FVector puzzle2Slot6;
	FVector puzzle2Slot7;
	FVector puzzle2Slot8;
	FVector puzzle2Slot9;

	// The actors location
	FVector puzzle2ActorLoc;

	//The ID for each block
	int puzzle2ID;

	//The player's location
	FVector playerLoc;
};