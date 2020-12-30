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
	PrimaryActorTick.bCanEverTick = true;


	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = ProjectileMesh;

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	ParticleTrail->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->SetUpdatedComponent(ProjectileMesh);
	ProjectileMovement->InitialSpeed = InitialProjectileMovementSpeed;
	ProjectileMovement->MaxSpeed = MaxProjectileMovementSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = GravityEffect;
	InitialLifeSpan = LifeTime;

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ProjectileMesh && ProjectileMovement && ParticleTrail)
	{

		FVector LaunchDirection = ProjectileMesh->GetComponentRotation().Vector();
		const FVector NewVelocity = (LaunchDirection * ProjectileMovement->InitialSpeed);
		ProjectileMovement->Velocity = NewVelocity;
		ProjectileMovement->ComputeMoveDelta(NewVelocity, DeltaTime);
		ProjectileMovement->UpdateComponentVelocity();
	}
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
		ProjectileMesh->DestroyComponent();
		ParticleTrail->DeactivateSystem();

	}

}

void AProjectileBase::HandleProjectileDestruction()
{
	Destroy();
}

