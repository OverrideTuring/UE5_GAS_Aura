// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth((50.f));
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always)
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	} else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);
}


void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth)
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth)
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana)
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana)
}

void UAuraAttributeSet::SetEffectProperties(const struct FGameplayEffectModCallbackData& Data,
	FEffectProperties& EffectProperties) const
{
	// SourceEffectProperties
	const FGameplayEffectContextHandle& EffectContextHandle = Data.EffectSpec.GetEffectContext();
	EffectProperties.EffectContextHandle = EffectContextHandle;
	UAbilitySystemComponent* SourceASC = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	EffectProperties.SourceAbilitySystemComponent = SourceASC;
	if (IsValid(SourceASC))
	{
		AActor* SourceAvatarActor = SourceASC->GetAvatarActor();
		EffectProperties.SourceAvatarActor = SourceAvatarActor;
		if (APawn* SourceAvatarPawn = Cast<APawn>(SourceAvatarActor))
		{
			AController* SourceAvatarController = SourceAvatarPawn->GetController();
			ACharacter* SourceAvatarCharacter = Cast<ACharacter>(SourceAvatarPawn);
			EffectProperties.SourceController = SourceAvatarController;
			EffectProperties.SourceCharacter = SourceAvatarCharacter;
		}
	}

	// TargetEffectProperties
	UAbilitySystemComponent* TargetASC = &Data.Target;
	EffectProperties.TargetAbilitySystemComponent = TargetASC;
	if (IsValid(TargetASC))
	{
		AActor* TargetAvatarActor = TargetASC->GetAvatarActor();
		EffectProperties.TargetAvatarActor = TargetAvatarActor;
		if (APawn* TargetAvatarPawn = Cast<APawn>(TargetAvatarActor))
		{
			AController* TargetAvatarController = TargetAvatarPawn->GetController();
			ACharacter* TargetAvatarCharacter = Cast<ACharacter>(TargetAvatarPawn);
			EffectProperties.TargetController = TargetAvatarController;
			EffectProperties.TargetCharacter = TargetAvatarCharacter;
		}
	}
}
