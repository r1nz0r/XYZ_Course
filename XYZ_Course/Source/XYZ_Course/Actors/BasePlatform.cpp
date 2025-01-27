// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlatform.h"
#include "PlatformInvocator.h"

// Sets default values
ABasePlatform::ABasePlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* DefaultPlatformRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Platform Root"));
	RootComponent = DefaultPlatformRoot;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	PlatformMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABasePlatform::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = PlatformMesh->GetRelativeLocation();

	if (IsValid(TimelineCurve))
	{		
		PlatformTimeline.AddInterpFloat(TimelineCurve,FOnTimelineFloatStatic::CreateUObject(this, &ABasePlatform::UpdatePlatformTimeline));
		PlatformTimeline.SetTimelineFinishedFunc(FOnTimelineEventStatic::CreateUObject(this, &ABasePlatform::OnTimelineFinished));
	}

	if (IsValid(PlatformInvocator))
	{
		PlatformInvocator->OnPlatformInvocatorActivated.AddDynamic(this, &ABasePlatform::OnPlatformInvoked);
	}
}

// Called every frame
void ABasePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlatformTimeline.TickTimeline(DeltaTime);
}

void ABasePlatform::UpdatePlatformTimeline(float Alpha)
{
	const FVector PlatformTargetLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);
	PlatformMesh->SetRelativeLocation(PlatformTargetLocation);
}

bool ABasePlatform::IsPlatformTimelinePlaybackPositionAtEnd() const
{
	return PlatformTimeline.GetPlaybackPosition() == PlatformTimeline.GetTimelineLength();
}

void ABasePlatform::PlayPlatformTimeline()
{
	if (PlatformTimeline.IsPlaying())
	{
		return;
	}
	
	if (IsPlatformTimelinePlaybackPositionAtEnd())
	{
		PlatformTimeline.Reverse();
	}
	else
	{
		PlatformTimeline.Play();
	}
}

void ABasePlatform::OnPlatformInvoked()
{
	PlayPlatformTimeline();
}

void ABasePlatform::OnTimelineFinished()
{
	if (PlatformBehavior == EPlatformBehavior::OnDemand)
	{
		return;
	}
	
	PlayPlatformTimeline();
}
