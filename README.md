# It-Takes-Two-Copy
It Takes Two 일부 기능 모작

# Unreal Engine 5 Split Screen System
https://eovywjr1.tistory.com/459

네트워크 멀티플레이어 환경(리슨 서버)에서 **It Takes Two** 스타일의 분할 화면을 구현하는 시스템입니다.

## 개요

2대의 PC가 네트워크로 연결된 상태에서 각 플레이어가 자신의 화면(왼쪽)과 상대방의 화면(오른쪽)을 동시에 볼 수 있습니다.
```
[PC 1]                              [PC 2]
┌───────────┬───────────┐          ┌───────────┬───────────┐
│  Player1  │  Player2  │          │  Player1  │  Player2  │
│  (로컬)   │  (원격)   │          │  (원격)   │  (로컬)   │
└───────────┴───────────┘          └───────────┴───────────┘
```

## 특징

- **네트워크 분할 화면**: 로컬 멀티플레이어가 아닌 네트워크 환경에서 작동
- **SceneCapture 기반**: 원격 플레이어의 시점을 실시간 캡처
- **카메라 동기화**: RPC를 통한 카메라 위치/회전 동기화
- **부드러운 보간**: 네트워크 지연으로 인한 떨림 방지

## 요구 사항

- Unreal Engine 5.5+
- C++ 프로젝트

## 설치

### 1. 파일 복사
```
Source/Spectator
├── MSSpectatorComponent.h
├── MSSpectatorComponent.cpp
├── MSGameViewportClient.h
└── MSGameViewportClient.cpp
```

### 2. DefaultEngine.ini 설정
```ini
[/Script/Engine.Engine]
GameViewportClientClassName=/Script/YourProject.MSGameViewportClient
```

### 3. 캐릭터에 MSSpectatorComponent 컴포넌트 추가

## 아키텍처

### UMSSpectatorComponent

플레이어 캐릭터에 부착되는 컴포넌트로 다음을 담당합니다

| 역할 | 로컬 플레이어 | 원격 플레이어 |
|------|--------------|--------------|
| SceneCapture | 카메라에 부착 | 독립적으로 존재 |
| RPC | 카메라 정보 전송 | 카메라 정보 수신 |
| 보간 | - | 부드러운 카메라 이동 |

### UMSGameViewportClient

뷰포트 렌더링을 담당합니다

- 왼쪽 절반: 로컬 플레이어의 RenderTarget
- 오른쪽 절반: 원격 플레이어의 RenderTarget

## 성능 최적화

## 제한 사항

- 2인 플레이어 전용 (3인 이상 확장 필요)
- SceneCapture로 인한 추가 렌더링 비용 발생
- 네트워크 지연에 따른 약간의 시점 차이 존재
