// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FantasyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "HUD/FantasyHUD.h"
#include "HUD/FantasyOverlay.h"
#include "Component/AttributeComponent.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Characters/FantasyPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Characters/FantasyPlayerController.h"
#include "AbilitySystem/FantasyAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "NiagaraComponent.h"

AFantasyCharacter::AFantasyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

	CharacterClass = ECharacterClass::Elementalist;
}

void AFantasyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Atribute && FantasyOverlay)
	{
		Atribute->RegenStamina(DeltaTime);
		FantasyOverlay->SetStaminaBarPercent(Atribute->GetStaminaPercent());
	}
}

void AFantasyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFantasyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFantasyCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AFantasyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFantasyCharacter::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Equip"), IE_Pressed, this, &AFantasyCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AFantasyCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Dodge"), IE_Pressed, this, &AFantasyCharacter::Dodge);
}

void AFantasyCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	if(Atribute && Atribute->GetHealthPercent() > 0.f)
		ActionState = EActionState::EAS_HitReaction;
}

float AFantasyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();

	return DamageAmount;
}

void AFantasyCharacter::Jump()
{
	if (IsUnoccupied())
		Super::Jump();
}

void AFantasyCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AFantasyCharacter::AddSouls(ASoul* Soul)
{
	if (Atribute && FantasyOverlay)
	{
		Atribute->AddSouls(Soul->GetSouls());
		FantasyOverlay->SetSouls(Atribute->GetSouls());
	}
}

void AFantasyCharacter::AddGold(ATreasure* Gold)
{
	if (Atribute && FantasyOverlay)
	{
		Atribute->AddGold(Gold->GetGold());
		FantasyOverlay->SetGold(Atribute->GetGold());
	}
}

void AFantasyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AFantasyCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void AFantasyCharacter::AddToXP_Implementation(int32 InXP)
{
	AFantasyPlayerState* FantasyPlayerState = GetPlayerState<AFantasyPlayerState>();
	check(FantasyPlayerState);

	FantasyPlayerState->AddToXP(InXP);
}

int32 AFantasyCharacter::GetXP_Implementation() const
{
	const AFantasyPlayerState* FantasyPlayerState = GetPlayerState<AFantasyPlayerState>();
	check(FantasyPlayerState);
	return FantasyPlayerState->GetXP();
}

int32 AFantasyCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const AFantasyPlayerState* FantasyPlayerState = GetPlayerState<AFantasyPlayerState>();
	check(FantasyPlayerState);
	return FantasyPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AFantasyCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const AFantasyPlayerState* FantasyPlayerState = GetPlayerState<AFantasyPlayerState>();
	check(FantasyPlayerState);
	return FantasyPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 AFantasyCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const AFantasyPlayerState* FantasyPlayerState = GetPlayerState<AFantasyPlayerState>();
	check(FantasyPlayerState);
	return FantasyPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void AFantasyCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel) 
{
	AFantasyPlayerState* FantasyPlayerState = GetPlayerState<AFantasyPlayerState>();
	check(FantasyPlayerState);
	FantasyPlayerState->AddToLevel(InPlayerLevel);

	if (UFantasyAbilitySystemComponent* FantasyASC = Cast<UFantasyAbilitySystemComponent>(GetAbilitySystemComponent()))
		FantasyASC->UpdateAbilityStatuses(FantasyPlayerState->GetPlayerLevel());
}

void AFantasyCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints) const
{
	AFantasyPlayerState* FantasyPlayerState = GetPlayerState<AFantasyPlayerState>();
	check(FantasyPlayerState);
	FantasyPlayerState->AddToSpellPoints(InSpellPoints);
}

void AFantasyCharacter::AddToAttrbiutePoints_Implementation(int32 InAttributePoints) const
{
	AFantasyPlayerState* FantasyPlayerState = GetPlayerState<AFantasyPlayerState>();
	check(FantasyPlayerState);
	FantasyPlayerState->AddToAttributePoints(InAttributePoints);
}

void AFantasyCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

int32 AFantasyCharacter::GetAttributePoints_Implementation() const
{
	AFantasyPlayerState* FantasyPlayerState = GetPlayerState<AFantasyPlayerState>();
	check(FantasyPlayerState);
	return FantasyPlayerState->GetAttributePoints();
}

int32 AFantasyCharacter::GetSpellPoints_Implementation() const
{
	AFantasyPlayerState* FantasyPlayerState = GetPlayerState<AFantasyPlayerState>();
	check(FantasyPlayerState);
	return FantasyPlayerState->GetSpellPoints();
}

void AFantasyCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = ViewCamera->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 AFantasyCharacter::GetPlayerLevel_Implementation()
{
	const AFantasyPlayerState* FantasyPlayerState = GetPlayerState<AFantasyPlayerState>();
	check(FantasyPlayerState);

	return FantasyPlayerState->GetPlayerLevel();
}

void AFantasyCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

void AFantasyCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("EngageableTarget"));
	Tags.Add(FName("Player"));

	InitializeFantasyOverlay();
}

void AFantasyCharacter::MoveForward(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	
	if (Controller != nullptr && Value != 0.f)
	{
		//find out which way is forward
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFantasyCharacter::MoveRight(float Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if (Controller != nullptr && Value != 0.f)
	{
		//find out which way is right
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AFantasyCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AFantasyCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AFantasyCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon) Equip(OverlappingWeapon);
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

void AFantasyCharacter::Attack()
{
	Super::Attack();

	if (CanAttack())
	{
		ActionState = EActionState::EAS_Attacking;
		PlayAttackMontage();
	}

}

void AFantasyCharacter::Dodge()
{
	if (IsOccupied() || !HasEnoughStamina()) return;

	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	if (Atribute && FantasyOverlay)
	{
		Atribute->UseStamina(Atribute->GetDodgeCost());
		FantasyOverlay->SetStaminaBarPercent(Atribute->GetStaminaPercent());
	}
}

void AFantasyCharacter::Equip(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void AFantasyCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AFantasyCharacter::DodgeEnd()
{
	Super::DodgeEnd();
	ActionState = EActionState::EAS_Unoccupied;
}

bool AFantasyCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool AFantasyCharacter::CanDisarm()
{
	return CharacterState != ECharacterState::ECS_Unequipped &&
		ActionState == EActionState::EAS_Unoccupied;
}

bool AFantasyCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void AFantasyCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AFantasyCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AFantasyCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

/*void AFantasyCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	ActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}*/

bool AFantasyCharacter::HasEnoughStamina()
{
	return Atribute && Atribute->GetStamina() > Atribute->GetDodgeCost();
}

bool AFantasyCharacter::IsOccupied()
{
	return ActionState != EActionState::EAS_Unoccupied;
}

void AFantasyCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AFantasyCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void AFantasyCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AFantasyCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool AFantasyCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void AFantasyCharacter::InitAbilityActorInfo()
{
	AFantasyPlayerState* FantasyPlayerState = GetPlayerState<AFantasyPlayerState>();
	check(FantasyPlayerState);
	FantasyPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(FantasyPlayerState, this);
	Cast<UFantasyAbilitySystemComponent>(FantasyPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = FantasyPlayerState->GetAbilitySystemComponent();
	AttributeSet = FantasyPlayerState->GetAttributeSet();

	OnASCRegistered.Broadcast(AbilitySystemComponent);

	if (AFantasyPlayerController* FantasyPlayerController = Cast<AFantasyPlayerController>(GetController()))
		if (AFantasyHUD* FantasyHUD = Cast<AFantasyHUD>(FantasyPlayerController->GetHUD()))
			FantasyHUD->InitOverlay(FantasyPlayerController, FantasyPlayerState, AbilitySystemComponent, AttributeSet);

	InitializeDefaultAttributes();
}

void AFantasyCharacter::InitializeFantasyOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		AFantasyHUD* FantasyHUD = Cast<AFantasyHUD>(PlayerController->GetHUD());
		if (FantasyHUD)
		{
			FantasyOverlay = FantasyHUD->GetFantasyOverlay();
			if (FantasyOverlay && Atribute)
			{
				FantasyOverlay->SetHealthBarPercent(Atribute->GetHealthPercent());
				FantasyOverlay->SetStaminaBarPercent(1.f);
				FantasyOverlay->SetGold(0);
				FantasyOverlay->SetSouls(0);
			}
		}
	}
}

void AFantasyCharacter::SetHUDHealth()
{
	if (FantasyOverlay && Atribute)
		FantasyOverlay->SetHealthBarPercent(Atribute->GetHealthPercent());
}
