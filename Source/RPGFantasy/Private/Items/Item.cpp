// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "RPGFantasy/DebugMacros.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(0.f, 0.f, 320.f));
	SetActorRotation(FRotator(0.f, 45.f, 0.f));
	DRAW_SPHERE(GetActorLocation());
	DRAW_VECTOR(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

