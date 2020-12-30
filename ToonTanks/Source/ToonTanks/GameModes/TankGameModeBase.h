// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "TankGameModeBase.generated.h"

class APawnTurret;
class APawnTank;
class APlayerControllerBase;
class UCapsuleComponent;
UCLASS()
class TOONTANKS_API ATankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	APawnTank* PlayerTank;
	int32 TargetTurrets = 0;
	APlayerControllerBase* PlayerControllerRef;
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, Category = "Game Music")
		UAudioComponent* GameMusic;

	int32 GetTargetTurretCount();
	void HandleGameStart();
	void HandleGameOver(bool PlayerWon);
	void PlayMusic();
	void FadeMusic(float FadeSpeed, float FadeTo);
	void SetupGameMusic(float GameVolumeMultiplier);

public:

	ATankGameModeBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActorDied(AActor* DeadActor);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Loop")
		float StartDelay = 3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Music")
		float FadeMusicLoss = 0.5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Music")
		float FadeMusicWin = 0.5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Music")
		float FadeMusicSpeed = 0.5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Music")
		float InitialVolumeMultiplier = 0.5;

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
		void GameStart();
	UFUNCTION(BlueprintImplementableEvent)
		void GameOver(bool PlayerWon);

};
