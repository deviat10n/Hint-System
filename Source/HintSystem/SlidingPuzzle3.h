// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "SlidingPuzzle3.generated.h"

UCLASS()
class HINTSYSTEM_API ASlidingPuzzle3 : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlidingPuzzle3();

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
	ASlidingPuzzle3* puzzle3Block1;
	ASlidingPuzzle3* puzzle3Block2;
	ASlidingPuzzle3* puzzle3Block3;
	ASlidingPuzzle3* puzzle3Block4;
	ASlidingPuzzle3* puzzle3Block5;
	ASlidingPuzzle3* puzzle3Block6;
	ASlidingPuzzle3* puzzle3Block7;
	ASlidingPuzzle3* puzzle3Block8;

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
	FVector puzzle3Slot1;
	FVector puzzle3Slot2;
	FVector puzzle3Slot3;
	FVector puzzle3Slot4;
	FVector puzzle3Slot5;
	FVector puzzle3Slot6;
	FVector puzzle3Slot7;
	FVector puzzle3Slot8;
	FVector puzzle3Slot9;

	// The actors location
	FVector puzzle3ActorLoc;

	//The ID for each block
	int puzzle3ID;

	//The player's location
	FVector playerLoc;
};