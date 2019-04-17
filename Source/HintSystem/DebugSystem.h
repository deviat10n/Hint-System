// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugSystem.generated.h"

UCLASS()
class HINTSYSTEM_API ADebugSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADebugSystem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Complete)
		bool puzzle1LocalComplete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Complete)
		bool puzzle2LocalComplete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Complete)
		bool puzzle3LocalComplete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Complete)
		bool puzzle4LocalComplete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Complete)
		bool puzzle5LocalComplete;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//The Player's controller
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere)
		USceneComponent* OurVisibleComponent;

	//Debug complete puzzles
	void Complete1();
	void Complete2();
	void Complete3();
	void Complete4();
	
};
