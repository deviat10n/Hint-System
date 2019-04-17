// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "SlidingPuzzle4.generated.h"

UCLASS()
class HINTSYSTEM_API ASlidingPuzzle4 : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlidingPuzzle4();

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
	ASlidingPuzzle4* puzzle4Block1;
	ASlidingPuzzle4* puzzle4Block2;
	ASlidingPuzzle4* puzzle4Block3;
	ASlidingPuzzle4* puzzle4Block4;
	ASlidingPuzzle4* puzzle4Block5;
	ASlidingPuzzle4* puzzle4Block6;
	ASlidingPuzzle4* puzzle4Block7;
	ASlidingPuzzle4* puzzle4Block8;

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
	FVector puzzle4Slot1;
	FVector puzzle4Slot2;
	FVector puzzle4Slot3;
	FVector puzzle4Slot4;
	FVector puzzle4Slot5;
	FVector puzzle4Slot6;
	FVector puzzle4Slot7;
	FVector puzzle4Slot8;
	FVector puzzle4Slot9;

	// The actors location
	FVector puzzle4ActorLoc;

	//The ID for each block
	int puzzle4ID;

	//The player's location
	FVector playerLoc;
};