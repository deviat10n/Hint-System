// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlidingHints5.generated.h"

UCLASS()
class HINTSYSTEM_API ASlidingHints5 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlidingHints5();
	// Direction vector
	UPROPERTY(EditAnywhere)
		FVector movementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scores)
		int32 puzzle5LocalTimerScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scores)
		int32 puzzle5LocalActiveTimerScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scores)
		int32 puzzle5LocalMovesScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scores)
		int32 puzzle5LocalHintsScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scores)
		int32 puzzle1LocalFinalScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scores)
		int32 puzzle2LocalFinalScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scores)
		int32 puzzle3LocalFinalScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Scores)
		int32 puzzle4LocalFinalScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hints)
		bool puzzle5LocalEnter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		USceneComponent* OurVisibleComponent;

	//The Player's controller
	APlayerController* PlayerController;

	//Movement controls
	void EnterReleased();

	//Each hint's original location
	FVector originalLocation;

	//The ID of each hint
	int blockID;

private:
	// A static mesh component. The visual representation of our actor
	class UStaticMeshComponent* BoxVisual;

	//The player's location
	FVector playerLoc;
};