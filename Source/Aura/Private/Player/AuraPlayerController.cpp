// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Player/AuraPlayerState.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CurseTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		FRotator Rotation = GetControlRotation();
		FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();
		ControlledPawn->AddMovementInput(ForwardVector, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightVector, InputAxisVector.X);
	}
}

void AAuraPlayerController::CurseTrace()
{
	FHitResult CursorHitResult;
	GetHitResultUnderCursor(ECC_Pawn, true, CursorHitResult);
	if (!CursorHitResult.bBlockingHit) return;

	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(CursorHitResult.GetActor());
	
	if (CurrentActor && CurrentActor != LastActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		CurrentActor->HighlightActor();
	} else if (CurrentActor == nullptr && LastActor)
	{
		LastActor->UnHighlightActor();
	}
}
