#include "BoxActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ABoxActor::ABoxActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create and set the Static Mesh Component as the root
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Default health and score
    Health = 100;
    Score = 0;
    DamageCauser = nullptr;  // Initialize to nullptr
}

void ABoxActor::BeginPlay()
{
    Super::BeginPlay();
    // Initialization code if needed
}

void ABoxActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Per-frame logic can be added here if required
}

void ABoxActor::SetBoxType(const FBoxTypeData& InBoxData)
{
    // Update BoxData and apply health/score values
    BoxData = InBoxData;
    Health = InBoxData.Health;
    Score = InBoxData.Score;

    // Set the box color if a valid material instance exists
    if (MeshComponent)
    {
        UMaterialInstanceDynamic* DynMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
        if (DynMaterial && InBoxData.Color.Num() == 4)
        {
            // Convert RGBA to normalized values and apply color
            FLinearColor BoxColor(InBoxData.Color[0] / 255.0f, InBoxData.Color[1] / 255.0f, InBoxData.Color[2] / 255.0f, InBoxData.Color[3] / 255.0f);
            DynMaterial->SetVectorParameterValue(TEXT("BaseColor"), BoxColor);
        }
    }
}

void ABoxActor::ApplyDamage(int32 DamageAmount, AActor* Causer)
{
    // Apply damage to the box's health
    Health -= DamageAmount;
    DamageCauser = Causer;  // Set the actor responsible for the damage

    // Check if health is zero or less, trigger death logic
    if (Health <= 0)
    {
        // Trigger OnDeath event, passing the score and damage causer
        if (DamageCauser)
        {
            OnDeath.Broadcast(Score, DamageCauser);
        }

        // Destroy the actor once it's "dead"
        Destroy();
    }
}
