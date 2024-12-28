#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "BoxActor.generated.h"

// Struct to define box properties
USTRUCT(BlueprintType)
struct FBoxTypeData
{
    GENERATED_BODY()

    // RGBA Color array for the box
    UPROPERTY(BlueprintReadWrite, Category = "Box")
    TArray<int32> Color;

    // Box's Health
    UPROPERTY(BlueprintReadWrite, Category = "Box")
    int32 Health;

    // Box's Score
    UPROPERTY(BlueprintReadWrite, Category = "Box")
    int32 Score;

    FBoxTypeData() : Health(0), Score(0) {}
};

// BoxActor class definition
UCLASS()
class JSON_PROJECT_API ABoxActor : public AActor
{
    GENERATED_BODY()

public:
    ABoxActor();

    // Sets the box type data (Color, Health, Score)
    UFUNCTION(BlueprintCallable, Category = "Box")
    void SetBoxType(const FBoxTypeData& InBoxData);

    // Applies damage to the box
    UFUNCTION(BlueprintCallable, Category = "Box")
    void ApplyDamage(int32 DamageAmount, AActor* DamageCauser);

protected:
    // Mesh component for the box
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Box")
    UStaticMeshComponent* MeshComponent;

    virtual void BeginPlay() override;

    // Health and score values for the box
    UPROPERTY(BlueprintReadWrite, Category = "Box")
    int32 Health;

    UPROPERTY(BlueprintReadWrite, Category = "Box")
    int32 Score;

    // Holds the box data (color, health, score)
    UPROPERTY(BlueprintReadWrite, Category = "Box")
    FBoxTypeData BoxData;

    // Pointer to the actor that caused the damage
    UPROPERTY(BlueprintReadWrite, Category = "Box")
    AActor* DamageCauser;

    // Delegate for the box death event
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoxDeath, int32, Score, AActor*, DamageCauser);

public:
    virtual void Tick(float DeltaTime) override;

    // Event broadcasted when the box dies
    UPROPERTY(BlueprintAssignable, Category = "Box")
    FOnBoxDeath OnDeath;
};
