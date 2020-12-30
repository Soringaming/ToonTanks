// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"

ATankGameModeBase::ATankGameModeBase()
{

    PrimaryActorTick.bCanEverTick = true;

    GameMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("Game Music"));
    RootComponent = GameMusic;

}

void ATankGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GameMusic)
    {
        PlayMusic();
    }
}

void ATankGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
    // Check what type of actor died. If Turret, tally. If Player -> go to lose condition.
    if (DeadActor == PlayerTank)
    {
        PlayerTank->HandleDestruction();
        HandleGameOver(false);

        if (PlayerControllerRef)
        {
            PlayerControllerRef->SetPlayerEnabledState(false);
        }
    }
    else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
    {
        DestroyedTurret->HandleDestruction();
        if (--TargetTurrets <= 0)
        {
            HandleGameOver(true);
        }
    }
}

void ATankGameModeBase::HandleGameStart()
{
    TargetTurrets = GetTargetTurretCount();
    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

    GameStart();
    if (GameMusic)
    {
        PlayMusic();
    }
    if (PlayerControllerRef)
    {
        PlayerControllerRef->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableHandle;
        FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject
        (
            PlayerControllerRef,
            &APlayerControllerBase::SetPlayerEnabledState,
            true
        );
        GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
    }
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
    if (PlayerWon)
    {
        FadeMusic(FadeMusicSpeed, FadeMusicWin);
    }
    else
    {
        FadeMusic(FadeMusicSpeed, FadeMusicLoss);
    }
    GameOver(PlayerWon);
}

void ATankGameModeBase::PlayMusic()
{
    if (GameMusic && !GameMusic->IsPlaying())
    {
        GameMusic->Play();
    }
}

void ATankGameModeBase::FadeMusic(float FadeSpeed, float FadeTo)
{
    if (GameMusic)
    {
        GameMusic->FadeOut(FadeSpeed, FadeTo);
    }
}

void ATankGameModeBase::SetupGameMusic(float GameVolumeMultiplier) 
{
    GameVolumeMultiplier = InitialVolumeMultiplier;
    if(GameMusic)
    {
        GameMusic->SetVolumeMultiplier(GameVolumeMultiplier);
        GameMusic->SetUISound(true);
    }
}

int32 ATankGameModeBase::GetTargetTurretCount()
{
    TArray<AActor*> TurretActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), TurretActors);
    return TurretActors.Num();
}
