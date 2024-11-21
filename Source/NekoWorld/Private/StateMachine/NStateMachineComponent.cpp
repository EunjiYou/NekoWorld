// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/NStateMachineComponent.h"

#include "Common/CommonEnum.h"
#include "StateMachine/NStateBase.h"
#include "StateMachine/NStateOnAir.h"
#include "StateMachine/NStateOnGround.h"
#include "StateMachine/NStateOnWall.h"
#include "SubSystem/NInputSubsystem.h"


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
	RegisterState();

	if(auto world = GetWorld())
	{
		auto inputSubSystem = world->GetGameInstance()->GetSubsystem<UNInputSubsystem>();
		if(inputSubSystem)
		{
			inputSubSystem->EventInputAction.AddUObject(this, &UNStateMachineComponent::OnInputAction);
		}
	}

	OnStateChange.Clear();
}

void UNStateMachineComponent::RegisterState()
{
	StateClassMap.Add(ENState::OnGround, UNStateOnGround::StaticClass());
	StateClassMap.Add(ENState::Idle, UNStateIdle::StaticClass());
	StateClassMap.Add(ENState::WalkRun, UNStateWalkRun::StaticClass());
	StateClassMap.Add(ENState::Walk, UNStateWalk::StaticClass());
	StateClassMap.Add(ENState::Run, UNStateRun::StaticClass());
	StateClassMap.Add(ENState::Dash, UNStateDash::StaticClass());
	StateClassMap.Add(ENState::DashStart, UNStateDashStart::StaticClass());
	StateClassMap.Add(ENState::DashEnd, UNStateDashEnd::StaticClass());
	StateClassMap.Add(ENState::Sprint, UNStateSprint::StaticClass());
	StateClassMap.Add(ENState::Sliding, UNStateSliding::StaticClass());

	StateClassMap.Add(ENState::OnWall, UNStateOnWall::StaticClass());
	StateClassMap.Add(ENState::ClimbStart, UNStateClimbStart::StaticClass());
	StateClassMap.Add(ENState::ClimbMove, UNStateClimbMove::StaticClass());
	StateClassMap.Add(ENState::ClimbJump, UNStateClimbJump::StaticClass());
	StateClassMap.Add(ENState::ClimbSprint, UNStateClimbSprint::StaticClass());
	StateClassMap.Add(ENState::ClimbEnd, UNStateClimbEnd::StaticClass());
	
	StateClassMap.Add(ENState::OnAir, UNStateOnAir::StaticClass());
	StateClassMap.Add(ENState::Jump, UNStateJump::StaticClass());
	StateClassMap.Add(ENState::Falling, UNStateFalling::StaticClass());
	StateClassMap.Add(ENState::Gliding, UNStateGliding::StaticClass());
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

	if(CurStateObj)
	{
		CurStateObj->OnUpdate(DeltaTime);

		// HFSM 형태로 구현. Parent로부터 Transition을 체크한다.
		ENState searchState = GetCurState();
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
		
		if(resultState != ENState::None && resultState != GetCurState())
		{
			SetState(resultState);
		}
		else
		{
			// Transition Check를 한 번 진행한 후에는 데이터 리셋
			CurStateObj->ReceivedCancelAction = ENActionInputType::None;
		}
	}
}

void UNStateMachineComponent::SetState(ENState NewState)
{
	if(!StateClassMap.Contains(NewState))
	{
		return;
	}

	auto newStateObj = GetState(NewState);
	if(!newStateObj)
	{
		return;
	}

	// 현재 State에 대한 종료 처리
	if(CurStateObj)
	{
		auto newStateParents = GetAllParentState(NewState);
		if(!newStateParents.Contains(GetCurState()))
		{
			CurStateObj->OnLeave();
		}
		
		// CurStateObj Parent OnLeave 처리
		// 한 그룹에 있는 여러 State의 OnLeave() 처리가 반복되는 관계로 현재 State의 부모에서 OnLeave()를 처리하는게 깔끔해 보이므로 들어간 작업
		{
			auto curStateParents = GetAllParentState(GetCurState());
			// 부모가 일치하지 않을 경우 빨리 끊기 위해 최상위 부모부터 체크
			for(int i = curStateParents.Num() - 1; i >= 0; i--)
			{
				if(newStateParents.Contains(curStateParents[i]))
				{
					break;
				}

				if(auto parentStateObj = GetState(curStateParents[i]))
				{
					parentStateObj->OnLeave();
				}
			}
		}

		PrevStateObj = CurStateObj;
	}

	// 새로운 State를 CurState로 변경
	//newStateObj->Init();
	newStateObj->OnEnter();
	CurStateObj = newStateObj;

	OnStateChange.Broadcast(PrevStateObj? PrevStateObj->MyState : ENState::None, CurStateObj? CurStateObj->MyState : ENState::None);
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

void UNStateMachineComponent::OnInputAction(const ENActionInputType actionInputType)
{
	//if(CurStateObj && CurStateObj->HasCancelActionInput)
	{
		if(IsValid(CurStateObj) && (CurStateObj->CancelActionInputs & static_cast<uint32>(actionInputType)))
		{
			CurStateObj->ReceivedCancelAction = actionInputType;
		}
	}
}

bool UNStateMachineComponent::IsChildStateOf(ENState targetState, ENState parentState, bool recursiveCheck)
{
	auto targetStateObj = GetState(targetState);
	if(!IsValid(targetStateObj))
	{
		return false;
	}
	
	if(!recursiveCheck)
	{
		return targetStateObj->Parent == parentState; 
	}

	auto targetStateParents = GetAllParentState(targetState);
	return targetStateParents.Contains(parentState);
}

TArray<ENState> UNStateMachineComponent::GetAllParentState(ENState targetState)
{
	TArray<ENState> states;
	auto stateObj = GetState(targetState);
	if(!stateObj)
	{
		return states;
	}
	
	ENState searchState = stateObj->Parent;
	while(searchState != ENState::None)
	{
		states.AddUnique(searchState);

		stateObj = GetState(searchState);
		if(!stateObj)
		{
			break;
		}
		searchState = stateObj->Parent;
	}
	
	return states;
}

#if WITH_EDITOR
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
#endif

