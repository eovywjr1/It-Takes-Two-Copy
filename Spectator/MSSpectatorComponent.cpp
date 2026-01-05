// Fill out your copyright notice in the Description page of Project Settings.


#include "MSSpectatorComponent.h"

#include "MSGameViewportClient.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"


bool UMSSpectatorComponent::IsLocallyControlled() const
{
	return SpectatorPC && SpectatorPC->IsLocalPlayerController();
}

UCameraComponent* UMSSpectatorComponent::GetOwnerCamera() const
{
	return SpectatorPawn->FindComponentByClass<UCameraComponent>();
}

void UMSSpectatorComponent::InitializeSceneCapture()
{
	SceneCapture = NewObject<USceneCaptureComponent2D>(SpectatorPawn);
	SceneCapture->RegisterComponent();
	
	RenderTarget = NewObject<UTextureRenderTarget2D>();
	RenderTarget->InitAutoFormat(CaptureResolution.X, CaptureResolution.Y);
	RenderTarget->UpdateResourceImmediate();

	SceneCapture->TextureTarget = RenderTarget;
	SceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	SceneCapture->bCaptureEveryFrame = true;
	SceneCapture->bCaptureOnMovement = false;
}

UMSSpectatorComponent::UMSSpectatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UMSSpectatorComponent::BeginPlay()
{
	Super::BeginPlay();

	SpectatorPawn = GetOwner<APawn>();
	check(SpectatorPawn);

	SpectatorPC = Cast<APlayerController>(SpectatorPawn->GetController());
	
	InitializeSceneCapture();
	
	if (IsLocallyControlled())
	{
		SceneCapture->AttachToComponent(GetOwnerCamera(), FAttachmentTransformRules::SnapToTargetIncludingScale);

		if (UMSGameViewportClient* ViewportClient = Cast<UMSGameViewportClient>(GetWorld()->GetGameViewport()))
		{
			ViewportClient->SetLocalRenderTarget(RenderTarget);
		}
	}
	else
	{
		SceneCapture->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		
		if (UMSGameViewportClient* ViewportClient = Cast<UMSGameViewportClient>(GetWorld()->GetGameViewport()))
		{
			ViewportClient->SetRemoteRenderTarget(RenderTarget);
		}
	}
}

void UMSSpectatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsLocallyControlled())
	{
		FVector CamLoc;
		FRotator CamRot;
		SpectatorPC->GetPlayerViewPoint(CamLoc, CamRot);
		Server_UpdateCameraTransform(CamLoc, CamRot);
	}
	else if (SceneCapture && TargetLocation.IsSet() && TargetRotation.IsSet())
	{
		const FVector CurrentLoc = SceneCapture->GetComponentLocation();
		const FRotator CurrentRot = SceneCapture->GetComponentRotation();
		const FVector NewLoc = FMath::VInterpTo(CurrentLoc, TargetLocation.GetValue(), DeltaTime, InterpolationSpeed);
		const FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRotation.GetValue(), DeltaTime, InterpolationSpeed);
		SceneCapture->SetWorldLocationAndRotation(NewLoc, NewRot);
	}
}

void UMSSpectatorComponent::Server_UpdateCameraTransform_Implementation(const FVector& Location, const FRotator& Rotation)
{
	Multicast_BroadcastCameraTransform(Location, Rotation);
}

void UMSSpectatorComponent::Multicast_BroadcastCameraTransform_Implementation(const FVector& Location, const FRotator& Rotation)
{
	if (!IsLocallyControlled())
	{
		TargetLocation = Location;
		TargetRotation = Rotation;
	}
}
