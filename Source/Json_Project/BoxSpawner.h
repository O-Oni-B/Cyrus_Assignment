#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "HttpModule.h"
#include "BoxActor.h"  // Include the BoxActor class for spawning
#include "Templates/SharedPointer.h"
#include "BoxSpawner.generated.h"

// Struct to store box properties for spawning
USTRUCT(BlueprintType)
struct FBoxSpawnerTypeData
{
    GENERATED_BODY()

    // Box properties (Color, Health, Score)
    UPROPERTY(BlueprintReadWrite, Category = "Box")
    TArray<int32> Color;  // Color (RGBA)

    UPROPERTY(BlueprintReadWrite, Category = "Box")
    int32 Health;

    UPROPERTY(BlueprintReadWrite, Category = "Box")
    int32 Score;

    FBoxSpawnerTypeData() : Health(0), Score(0) {}
};

UCLASS()
class JSON_PROJECT_API ABoxSpawner : public AActor
{
    GENERATED_BODY()

public:
    // Constructor to initialize default values
    ABoxSpawner();

    // Function to initiate downloading JSON from the URL and spawn boxes
    UFUNCTION(BlueprintCallable, Category = "BoxSpawner")
    void DownloadAndSpawnObjectsFromJSON();

    // Reference to the BoxActor class (for spawning boxes)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    TSubclassOf<ABoxActor> BoxActorClass;

    // Map to store different box types with unique names
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    TMap<FString, FBoxSpawnerTypeData> BoxTypes;

    // Function to spawn objects based on parsed JSON data
    void SpawnObjectsFromJsonData(TSharedPtr<FJsonObject> JsonObject);

    // Callback function for handling downloaded JSON
    void OnJsonDownloaded(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
  
    // Cached JSON data for box spawning
    TSharedPtr<FJsonObject> JsonData;

    // Mesh to use for spawning boxes (can be set in editor)
    UStaticMesh* CubeMesh;

    // Helper function for logging debug messages
    void DebugLog(const FString& Message);
};
