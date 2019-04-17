// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Platforms.generated.h"

UCLASS()
class HINTSYSTEM_API APlatforms : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlatforms();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//The button locations
	FVector resetSlot1;
	FVector controlSlot1;
	FVector resetSlot2;
	FVector controlSlot2;
	FVector resetSlot3;
	FVector controlSlot3;
	FVector resetSlot4;
	FVector controlSlot4;
	FVector resetSlot5;
	FVector controlSlot5;

	// The actors location
	FVector platformActorLoc;

	//The ID for each platform
	int platformID;

private:
	// A static mesh component. The visual representation of our actor
	class UStaticMeshComponent* BoxVisual;
	
};
