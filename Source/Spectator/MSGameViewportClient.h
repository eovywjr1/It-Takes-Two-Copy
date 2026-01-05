// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "MSGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class MULTISPLITVIEW_API UMSGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	virtual void Draw(FViewport* InViewport, FCanvas* Canvas) override;

	FORCEINLINE void SetLocalRenderTarget(UTextureRenderTarget2D* RenderTarget) { LocalRenderTarget = RenderTarget; }
	FORCEINLINE void SetRemoteRenderTarget(UTextureRenderTarget2D* RenderTarget) { RemoteRenderTarget = RenderTarget; }
	
private:
	UPROPERTY()
	UTextureRenderTarget2D* LocalRenderTarget;

	UPROPERTY()
	UTextureRenderTarget2D* RemoteRenderTarget;
};
