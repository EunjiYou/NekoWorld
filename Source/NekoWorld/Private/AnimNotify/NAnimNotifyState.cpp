// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/NAnimNotifyState.h"

#include "NCharacter.h"

void UNAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(!OwnerCharacter.IsValid() && MeshComp && MeshComp->GetOwner())
	{
		OwnerCharacter = Cast<ANCharacter>(MeshComp->GetOwner());	
	}
}

void UNAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
