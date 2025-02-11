// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "BasePlatform.generated.h"

UENUM()
enum class EPlatformBehavior : uint8
{
	OnDemand = 0,
	Loop
};

UCLASS()
class XYZ_COURSE_API ABasePlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (MakeEditWidget))
	FVector EndLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	FVector StartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* TimelineCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPlatformBehavior PlatformBehavior = EPlatformBehavior::OnDemand;
	
	UPROPERTY(EditInstanceOnly,BlueprintReadOnly)
	class APlatformInvocator* PlatformInvocator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin="0"))
	float ReturnDelay = 2.0f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION(BlueprintCallable)
	void OnPlatformInvoked();
	void SetPlatformReturnTimer();

	UFUNCTION()
	void OnTimelineFinished();
	
	void HandlePlatformBehavior();
	void PlayPlatformTimeline();
	void ClearPlatformReturnTimer();
	void UpdatePlatformTimeline(float Alpha);
	bool IsPlatformTimelinePlaybackPositionAtEnd() const;

	FTimeline PlatformTimeline;
	FTimerHandle PlatformReturnTimerHandler;
};
