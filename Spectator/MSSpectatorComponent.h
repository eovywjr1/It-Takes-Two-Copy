// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MSSpectatorComponent.generated.h"

class UCameraComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTISPLITVIEW_API UMSSpectatorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UFUNCTION(Server, Unreliable)
	void Server_UpdateCameraTransform(const FVector& Location, const FRotator& Rotation);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_BroadcastCameraTransform(const FVector& Location, const FRotator& Rotation);

	bool IsLocallyControlled() const;

	UCameraComponent* GetOwnerCamera() const;

	void InitializeSceneCapture();

	UMSSpectatorComponent();
	virtual void BeginPlay() override final;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override final;

	UPROPERTY(EditDefaultsOnly, Category = "Spectator")
	FIntPoint CaptureResolution = FIntPoint(960, 540);

	UPROPERTY(EditDefaultsOnly, Category = "Spectator")
	float InterpolationSpeed = 15.0f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneCaptureComponent2D> SceneCapture;

	UPROPERTY()
	TObjectPtr<UTextureRenderTarget2D> RenderTarget;

	UPROPERTY()
	TObjectPtr<APawn> SpectatorPawn;

	UPROPERTY()
	TObjectPtr<APlayerController> SpectatorPC;

	TOptional<FVector> TargetLocation;
	TOptional<FRotator> TargetRotation;
};
