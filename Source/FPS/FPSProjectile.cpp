// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime/Engine/Classes/Engine/TriggerVolume.h>
#include <FPS/FPSCharacter.h>

AFPSProjectile::AFPSProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);		// set up a notification for when this component hits something blocking
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSProjectile::OnBeginOverlap);
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}
void AFPSProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		ATriggerVolume* volume = Cast<ATriggerVolume>(OtherActor);
		if (volume != nullptr) {
			//volume->GetActorLocation();
			FVector vec = SweepResult.ImpactPoint;
			float dist = FVector::Distance(vec, volume->GetActorLocation());
			AFPSCharacter* charactor = Cast<AFPSCharacter>(GetWorld()->GetFirstPlayerController()
				->GetPawn());
			UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(dist));
			if (charactor != nullptr)
			{
				float norm = 200 * sqrt(pow(volume->GetActorScale().X, 2) + pow(volume->GetActorScale().Z, 2));
				UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(norm));
				charactor->addScore(10 - dist / norm *10);
			}
		}

	}
}
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		Destroy();
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_pEmitterSystem, this->GetTransform());
}