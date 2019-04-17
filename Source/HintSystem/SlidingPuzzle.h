// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "SlidingPuzzle.generated.h"

UCLASS()
class HINTSYSTEM_API ASlidingPuzzle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlidingPuzzle();

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
	ASlidingPuzzle* puzzle1Block1;
	ASlidingPuzzle* puzzle1Block2;
	ASlidingPuzzle* puzzle1Block3;
	ASlidingPuzzle* puzzle1Block4;
	ASlidingPuzzle* puzzle1Block5;
	ASlidingPuzzle* puzzle1Block6;
	ASlidingPuzzle* puzzle1Block7;
	ASlidingPuzzle* puzzle1Block8;

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
	FVector puzzle1Slot1;
	FVector puzzle1Slot2;
	FVector puzzle1Slot3;
	FVector puzzle1Slot4;
	FVector puzzle1Slot5;
	FVector puzzle1Slot6;
	FVector puzzle1Slot7;
	FVector puzzle1Slot8;
	FVector puzzle1Slot9;

	// The actors location
	FVector puzzle1ActorLoc;

	//The ID for each block
	int puzzle1ID;

	//The player's location
	FVector playerLoc;
};
