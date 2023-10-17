// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/MobSpawner.h"
#include "Enemy/Enemy.h"
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
AMobSpawner::AMobSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMobSpawner::SetEnemySpawnedCount(int value)
{
	if (value < 0)
		value = 0;
	if (value > MaxEnemySpawnedCount)
		value = MaxEnemySpawnedCount;

	EnemySpawnedCount = value;
}

int AMobSpawner::GetEnemySpawnedCount()
{
	return EnemySpawnedCount;
}

void AMobSpawner::ResetSpawnTimer()
{
	ClearSpawnTimer();
	StartSpawnTimer();
}

void AMobSpawner::StartSpawnTimer()
{
	const float Delay = FMath::RandRange(SpawnEnemyTimerMin, SpawnEnemyTimerMax);
	GetWorldTimerManager().SetTimer(SpawnEnemyTimer, this, &AMobSpawner::SpawnEnemyMob, Delay);
}

void AMobSpawner::ClearSpawnTimer()
{
	GetWorldTimerManager().ClearTimer(SpawnEnemyTimer);
}

// Called when the game starts or when spawned
void AMobSpawner::BeginPlay()
{
	Super::BeginPlay();

	StartSpawnTimer();
}

void AMobSpawner::SpawnEnemyMob()
{
	if (EnemySpawnedCount >= MaxEnemySpawnedCount) return;

	UWorld* World = GetWorld();

	if (World && EnemyMob)
	{
		const FVector Location = CalculateSpawnLocation();
		AEnemy* SpawnedEnemy = World->SpawnActor<AEnemy>(EnemyMob, Location, GetActorRotation());
		if (SpawnedEnemy)
		{
			EnemySpawnedCount++;
			SpawnedEnemy->SetOwner(this);

			if (PatrolTarget && PatrolTargets.Num() > 0)
			{
				SpawnedEnemy->SetPatrolTarget(PatrolTarget);
				SpawnedEnemy->SetPatrolTargets(PatrolTargets);
				SpawnedEnemy->EnemyState = EEnemyState::EES_Chasing;
			}
		}
	}
}

FVector AMobSpawner::CalculateSpawnLocation()
{
	const float newX = FMath::RandRange(-MobRadiusSpawn, MobRadiusSpawn);
	const float newY = FMath::RandRange(-MobRadiusSpawn, MobRadiusSpawn);

	const FVector Start = GetActorLocation() + FVector(newX, newY, 2000.f);
	const FVector End = Start - FVector(0.f, 0.f, 4000.f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingleForObjects(this, Start, End, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	const float newZ = HitResult.ImpactPoint.Z + ZTranlsation;

	return FVector(GetActorLocation().X + newX, GetActorLocation().Y + newY, newZ);
}

// Called every frame
void AMobSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

