// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiSplitView/Spectator/MSGameViewportClient.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "Engine/TextureRenderTarget2D.h"


void UMSGameViewportClient::Draw(FViewport* InViewport, FCanvas* Canvas)
{
	if (!LocalRenderTarget || !RemoteRenderTarget)
	{
		// 분할 화면 비활성화시 기본 렌더링
		Super::Draw(InViewport, Canvas);
		return;
	}

	Super::Draw(InViewport, Canvas);

	// Canvas 위에 RenderTarget 그리기
	if (Canvas)
	{
		// 뷰포트 크기
		const int32 ViewportWidth = InViewport->GetSizeXY().X;
		const int32 ViewportHeight = InViewport->GetSizeXY().Y;
		const int32 HalfWidth = ViewportWidth / 2;
		
		FCanvasTileItem LocalTile(
			FVector2D(0, 0),
			LocalRenderTarget->GetResource(),
			FVector2D(HalfWidth, ViewportHeight),
			FLinearColor::White
		);
		LocalTile.BlendMode = SE_BLEND_Opaque;
		Canvas->DrawItem(LocalTile);

		FCanvasTileItem RemoteTile(
			FVector2D(HalfWidth, 0),
			RemoteRenderTarget->GetResource(),
			FVector2D(HalfWidth, ViewportHeight),
			FLinearColor::White
		);
		RemoteTile.BlendMode = SE_BLEND_Opaque;
		Canvas->DrawItem(RemoteTile);
	}
}
