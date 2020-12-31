// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class UCapsuleComponent;
UCLASS()
class TOONTANKS_API AProjectileBase : public AActor
{
	GENERATED_BODY()


private:
	// COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		TSubclassOf<UDamageType> DamageType;

	// VARIABLES
	// The movement speeds might need to be public to allow the movement componenet to look at these!
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		float LifeTime = 3000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
		float InitialProjectileMovementSpeed = 1300;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
		float MaxProjectileMovementSpeed = 1300;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		float Damage = 50;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		float GravityEffect = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
		float ParticleLifeTime = 1;


	FTimerHandle ParticleLifeTimeHandler;

	// PARTICLE STUFF
	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystemComponent* ParticleTrail;
	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* HitParticle;
	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* HitParticleVariantOne;
	UPROPERTY(EditAnywhere, Category = "Effects")
		FVector HitParticleScale = FVector(1, 1, 1);
	UPROPERTY(EditAnywhere, Category = "Effects")
		FRotator HitParticleVariantOneRotation = FRotator(0, 0, 0);
	UPROPERTY(EditAnywhere, Category = "Effects")
		FVector HitParticleVariantOneScale = FVector(1, 1, 1);
	UPROPERTY(EditAnywhere, Category = "Effects")
		FVector HitParticleVariantOneLocationOffset = FVector(0, 0, 0);
	UPROPERTY(EditAnywhere, Category = "Effects")
		USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = "Effects")
		USoundBase* LaunchSound;
	UPROPERTY(EditAnywhere, Category = "Effects")
		UAudioComponent* FlyWhistleSound;

	UPROPERTY(EditAnywhere, Category = "Effects" )
		TSubclassOf<UMatineeCameraShake> HitShake;

	// FUNCTIONS
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void HandleProjectileDestruction();

public:
	// Sets default values for this actor's properties
	AProjectileBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
