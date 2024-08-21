// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/NStateMachineComponent.h"

#include "StateMachine/NStateBase.h"


// Sets default values for this component's properties
UNStateMachineComponent::UNStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UNStateMachineComponent::Init()
{
	StateClassMap.Add(ENState::OnGround, UNStateOnGround::StaticClass());
	StateClassMap.Add(ENState::Idle, UNStateIdle::StaticClass());
	StateClassMap.Add(ENState::Run, UNStateRun::StaticClass());
	
	StateClassMap.Add(ENState::OnAir, UNStateOnAir::StaticClass());
	StateClassMap.Add(ENState::Jump, UNStateJump::StaticClass());
}


// Called when the game starts
void UNStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();
	Init();
	SetState(ENState::Idle);
}


// Called every frame
void UNStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CurState)
	{
		CurState->OnUpdate();

		// HFSM 형태로 구현. Parent로부터 Transition을 체크한다.
		ENState searchState = CurState->MyState;
		ENState resultState = ENState::None;
		while(searchState != ENState::None)
		{
			if(UNStateBase* state = GetState(searchState))
			{
				ENState transitionResult = state->CheckTransition();
				if(transitionResult != ENState::None)
				{
					resultState = transitionResult;
				}
				
				searchState = state->Parent;
			}
			else
			{
				// Parent Root까지 도달하기 전 문제가 생길 경우 while문 종료
				break;
			}
		}

		if(resultState != ENState::None)
		{
			SetState(resultState);
		}	
	}
}

void UNStateMachineComponent::SetState(ENState NewState)
{
	if(!StateClassMap.Contains(NewState))
	{
		return;
	}
	
	if(CurState)
	{
		PrevState = CurState;
		PrevState->OnLeave();
	}

	CurState = GetState(NewState);
	if(CurState)
	{
		CurState->Init();
		CurState->OnEnter();		
	}
}

UNStateBase* UNStateMachineComponent::GetState(ENState State)
{
	if(!StateClassMap.Contains(State))
	{
		return nullptr;
	}
	
	if(StatePool.Contains(State))
	{
		return *StatePool.Find(State);
	}
	
	const TSubclassOf<UNStateBase>* nextState = StateClassMap.Find(State);
	UNStateBase* newState = NewObject<UNStateBase>(this, *nextState);
	newState->MyState = State;
	newState->Init();
	
	StatePool.Add(State, newState);

	return newState;
}

FNStateDebugData UNStateMachineComponent::GenerateDebugDataRecursive(ENState state)
{
	FNStateDebugData debugData;
	
	UNStateBase* stateBase = GetState(state);
	if(stateBase)
	{
		debugData.MyState = state;
		for(ENState child : stateBase->Children)
		{
			FNStateDebugData childData = GenerateDebugDataRecursive(child);
			debugData.Children.Add(childData);
		}
	}

	return debugData; 
}

// *** This method must call after UNStateMachineComponent::Init() ***
TArray<FNStateDebugData> UNStateMachineComponent::GenerateDebugData()
{
	TArray<FNStateDebugData> outData;
	
	TArray<ENState> rootChildStates;
	for(auto stateClassMapPair : StateClassMap)
	{
		UNStateBase* stateBase = GetState(stateClassMapPair.Key);
		if(stateBase && stateBase->Parent == ENState::None)
		{
			rootChildStates.Add(stateClassMapPair.Key);
		}
	}

	for(ENState rootChildState : rootChildStates)
	{
		FNStateDebugData rootChildStateData = GenerateDebugDataRecursive(rootChildState);
		outData.Add(rootChildStateData);	
	}

	return outData;
}

