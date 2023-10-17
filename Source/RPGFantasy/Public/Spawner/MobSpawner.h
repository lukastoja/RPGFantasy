// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MobSpawner.generated.h"

UCLASS()
class RPGFANTASY_API AMobSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AMobSpawner();

	void SetEnemySpawnedCount(int value);
	int GetEnemySpawnedCount();
	void ResetSpawnTimer();
	void StartSpawnTimer();
	void ClearSpawnTimer();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Mob Spawner")
		TSubclassOf <class AEnemy> EnemyMob;

	UPROPERTY(EditAnywhere, Category = "Mob Spawner")
		int EnemySpawnedCount = 0;
	
	UPROPERTY(EditAnywhere, Category = "Mob Spawner")
		int MaxEnemySpawnedCount = 10;

	UPROPERTY(EditAnywhere, Category = "Mob Spawner")
		int MobRadiusSpawn = 50;

	//Current patrol target
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

	FTimerHandle SpawnEnemyTimer;

	UPROPERTY(EditAnywhere, Category = "Mob Spawner")
		float SpawnEnemyTimerMin = 5;

	UPROPERTY(EditAnywhere, Category = "Mob Spawner")
		float SpawnEnemyTimerMax = 10;

	UPROPERTY(EditAnywhere, Category = "Mob Spawner")
		float ZTranlsation = 20;

	void SpawnEnemyMob();
	FVector CalculateSpawnLocation();

public:	
	virtual void Tick(float DeltaTime) override;

};
