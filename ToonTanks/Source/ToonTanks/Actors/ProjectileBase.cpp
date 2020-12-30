// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;
	ProjectileMovement->ProjectileGravityScale = GravityEffect;
	InitialLifeSpan = 3.0f;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = ProjectileMesh;

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovement->UpdateComponentVelocity();

}


void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Try to get a reference to the owning class
	AActor* MyOwner = GetOwner();
	// If for some reason we can't get a valid reference, return as we need to check against the owner.
	if (!MyOwner)
	{
		return;
	}
	// If the other actor ISN'T self OR Owner AND exists, then apply damage.
	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation(), GetActorRotation(), HitParticleScale);
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticleVariantOne, GetActorLocation() + HitParticleVariantOneLocationOffset, GetActorRotation() - HitParticleVariantOneRotation, HitParticleVariantOneScale);

		GetWorld()->GetTimerManager().SetTimer(ParticleLifeTimeHandler, this, &AProjectileBase::HandleProjectileDestruction, ParticleLifeTime);
		ProjectileMesh->SetHiddenInGame(true);
		ParticleTrail->DeactivateSystem();

	}

}

void AProjectileBase::HandleProjectileDestruction()
{
	Destroy();
}
