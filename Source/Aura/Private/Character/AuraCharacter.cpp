// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

class AAuraHUD;

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0, 400.0, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info on Server
	GEngine->AddOnScreenDebugMessage(
		-1,                        // 键值 -1 表示自动分配，不会覆盖已有消息
		5.0f,                      // 显示时长（秒）
		FColor::Green,             // 颜色
		TEXT("AAuraCharacter::PossessedBy")        // 显示文本
	);
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info on Client
	GEngine->AddOnScreenDebugMessage(
		-1,                        // 键值 -1 表示自动分配，不会覆盖已有消息
		5.0f,                      // 显示时长（秒）
		FColor::Green,             // 颜色
		TEXT("AAuraCharacter::OnRep_PlayerState")  // 显示文本
	);
	InitAbilityActorInfo();
}

inline void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	AttributeSet = AuraPlayerState->GetAttributeSet();

	// Initialize HUD
	if(const auto PlayerController = GetController<APlayerController>())
	{
		if (const auto HUD = PlayerController->GetHUD<AAuraHUD>())
		{
			HUD->InitOverlay(PlayerController,
				AuraPlayerState,
				AuraPlayerState->GetAbilitySystemComponent(),
				AuraPlayerState->GetAttributeSet());
		}
	}
}