// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Component/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/FantasyAbilitySystemComponent.h"
#include "FantasyGameplayTags.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Net/UnrealNetwork.h>
#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Atribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>(TEXT("BurnDebuffComponent"));
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FFantasyGameplayTags::Get().Debuff_Burn;

	StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>(TEXT("StunDebuffComponent"));
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = FFantasyGameplayTags::Get().Debuff_Stun;

	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachPoint");
	EffectAttachComponent->SetupAttachment(GetRootComponent());

	HaloOfProtectionNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("haloOfProtection");
	HaloOfProtectionNiagaraComponent->SetupAttachment(EffectAttachComponent);

	LifeSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("LifeSiphon");
	LifeSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);

	ManaSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("ManaSiphon");
	ManaSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, bIsStunned);
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ABaseCharacter::Die(const FVector& DeathImpulse)
{
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
	MulticastHandleDeath(DeathImpulse);
}

void ABaseCharacter::CalculateHitReactMontage(const FVector& ImpactPoint)
{
	HitReactSectionName = FName("FromBack");

	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector test = GetActorLocation();
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward| |ToHit| * cos (theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);

	//convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// if cross product points down, theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
		Theta *= -1.f;

	if (Theta < 45.f && Theta >= -45.f)
		HitReactSectionName = FName("FromFront");
	else if (Theta < -45.f && Theta >= -135.f)
		HitReactSectionName = FName("FromLeft");
	else if (Theta >= 45.f && Theta < 135.f)
		HitReactSectionName = FName("FromRight");
}

FName ABaseCharacter::GetHitReactSectionName_Implementation()
{
	return HitReactSectionName;
}

void ABaseCharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	bDead = true;
	OnDeath.Broadcast(this);
	StunDebuffComponent->Deactivate();
	BurnDebuffComponent->Deactivate();
	//GetMesh()->AddImpulse(DeathImpulse); ako zelimo dodati impuls na monstere da odlete ca kada umru
}

void ABaseCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

void ABaseCharacter::OnRep_Stunned()
{

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
		CombatTarget = nullptr;
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);

	if (Pose < EDeathPose::EDP_MAX)
		DeathPose = Pose;

	return Selection;
}

void ABaseCharacter::PlayDodgeMontage()
{
	PlayMontageSection(DodgeMontage, FName("Default"));
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) AnimInstance->Montage_Stop(0.25f, AttackMontage);
}

void ABaseCharacter::InitAbilityActorInfo()
{
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (CombatTarget == nullptr) return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget) return CombatTarget->GetActorLocation();

	return FVector();
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	//PlayHitReactMontage(Section);
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles && GetWorld())
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Atribute)
	{
		Atribute->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 Selection = FMath::RandRange(0, SectionNames.Num() - 1);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

void ABaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ABaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ABaseCharacter::AddCharacterAbilities()
{
	UFantasyAbilitySystemComponent* FantasyASC = CastChecked<UFantasyAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	FantasyASC->AddCharacterAbilities(StartupAbilities);
	FantasyASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

bool ABaseCharacter::IsAlive()
{
	return Atribute && Atribute->IsAlive();
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::DodgeEnd()
{
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{ //stari hitter funkcija
	if (IsAlive() && Hitter) DirectionalHitReact(Hitter->GetActorLocation());
	//else Die(); lukao provijeriti

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

bool ABaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

AActor* ABaseCharacter::GetAvatar_Implementation()
{
	return this;
}

FVector ABaseCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FFantasyGameplayTags& GameplayTags = FFantasyGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon))
		return GetMesh()->GetSocketLocation(WeaponTipSocketName);

	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
		return GetMesh()->GetSocketLocation(LeftHandTipSocketName);

	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
		return GetMesh()->GetSocketLocation(RightHandTipSocketName);

	return FVector();
}

FRotator ABaseCharacter::GetActorRotation_Interface()
{
	return GetActorRotation();
}

TArray<FTaggedMontage> ABaseCharacter::GetAttackMontage_Implementation()
{
	return AttackMontages;
}

UParticleSystem* ABaseCharacter::GetBloodEffect_Implementation()
{
	return HitParticles;
}

FTaggedMontage ABaseCharacter::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
			return TaggedMontage;
	}

	return FTaggedMontage();
}

ECharacterClass ABaseCharacter::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered& ABaseCharacter::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered;
}

FOnDeath ABaseCharacter::GetOnDeathDelegate()
{
	return OnDeath;
}

USkeletalMeshComponent* ABaseCharacter::GetMeshComponent_Implementation()
{
	return GetMesh();
}

FOnDamageSignature& ABaseCharacter::GetOnDamageSignature()
{
	return OnDamageDelegate;
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamageDelegate.Broadcast(DamageTaken);
	return DamageTaken;
}

