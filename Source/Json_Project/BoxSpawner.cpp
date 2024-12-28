#include "BoxSpawner.h"
#include "Json.h"
#include "BoxActor.h"
#include "Kismet/GameplayStatics.h"

// Constructor
ABoxSpawner::ABoxSpawner()
{
    PrimaryActorTick.bCanEverTick = true;

    // Pre-load the Cube mesh in the constructor (safe to do here)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshObj(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    if (CubeMeshObj.Succeeded())
    {
        CubeMesh = CubeMeshObj.Object;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Cube mesh failed to load"));
    }
}

// Called when the game starts or when spawned
void ABoxSpawner::BeginPlay()
{
    Super::BeginPlay();

    DebugLog(TEXT("Beginning to download and spawn objects from JSON."));
    DownloadAndSpawnObjectsFromJSON();
}

// Empty Tick function - can be removed if not used
void ABoxSpawner::Tick(float DeltaTime)
{
}

// Debugging log function
void ABoxSpawner::DebugLog(const FString& Message)
{
    UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
}

// Download and parse JSON data from the URL
void ABoxSpawner::DownloadAndSpawnObjectsFromJSON()
{
    FHttpModule* HttpModule = &FHttpModule::Get();
    TSharedRef<IHttpRequest> HttpRequest = HttpModule->CreateRequest();

    HttpRequest->OnProcessRequestComplete().BindUObject(this, &ABoxSpawner::OnJsonDownloaded);
    HttpRequest->SetURL(TEXT("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json"));
    HttpRequest->SetVerb(TEXT("GET"));

    DebugLog(TEXT("Sending HTTP request to: https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json"));
    HttpRequest->ProcessRequest();
}

// Callback function for when the JSON data is downloaded
void ABoxSpawner::OnJsonDownloaded(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        FString ResponseString = Response->GetContentAsString();
        DebugLog(TEXT("Received successful response: " + ResponseString));

        // Parse the JSON content
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseString);

        if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
        {
            DebugLog(TEXT("Successfully deserialized JSON."));

            // Parse the types section and populate the BoxTypes map
            TArray<TSharedPtr<FJsonValue>> TypesArray = JsonObject->GetArrayField(TEXT("types"));
            for (const TSharedPtr<FJsonValue>& Value : TypesArray)
            {
                TSharedPtr<FJsonObject> TypeData = Value->AsObject();
                FString BoxTypeName = TypeData->GetStringField(TEXT("name"));
                TArray<TSharedPtr<FJsonValue>> ColorArray = TypeData->GetArrayField(TEXT("color"));

                TArray<int32> ColorComponents = {
                    static_cast<int32>(ColorArray[0]->AsNumber()),
                    static_cast<int32>(ColorArray[1]->AsNumber()),
                    static_cast<int32>(ColorArray[2]->AsNumber()),
                    ColorArray.Num() > 3 ? static_cast<int32>(ColorArray[3]->AsNumber()) : 255
                };

                int32 Health = static_cast<int32>(TypeData->GetNumberField(TEXT("health")));
                int32 Score = static_cast<int32>(TypeData->GetNumberField(TEXT("score")));

                FBoxSpawnerTypeData BoxData;
                BoxData.Color = ColorComponents;
                BoxData.Health = Health;
                BoxData.Score = Score;

                BoxTypes.Add(BoxTypeName, BoxData);
            }

            // Process the objects section and spawn the boxes
            SpawnObjectsFromJsonData(JsonObject);
        }
        else
        {
            DebugLog(TEXT("Failed to deserialize JSON."));
        }
    }
    else
    {
        DebugLog(TEXT("Failed to download JSON."));
    }
}

// Spawn objects based on the parsed JSON data
void ABoxSpawner::SpawnObjectsFromJsonData(TSharedPtr<FJsonObject> JsonObject)
{
    const TArray<TSharedPtr<FJsonValue>> ObjectsArray = JsonObject->GetArrayField(TEXT("objects"));

    UE_LOG(LogTemp, Log, TEXT("Found %d objects to spawn."), ObjectsArray.Num());

    for (const TSharedPtr<FJsonValue>& Value : ObjectsArray)
    {
        TSharedPtr<FJsonObject> ObjectData = Value->AsObject();
        FString BoxTypeName = ObjectData->GetStringField(TEXT("type"));
        UE_LOG(LogTemp, Log, TEXT("Processing box type: %s"), *BoxTypeName);

        if (BoxTypes.Contains(BoxTypeName))
        {
            FBoxSpawnerTypeData BoxData = BoxTypes[BoxTypeName];

            TArray<TSharedPtr<FJsonValue>> LocationArray = ObjectData->GetObjectField(TEXT("transform"))->GetArrayField(TEXT("location"));
            TArray<TSharedPtr<FJsonValue>> RotationArray = ObjectData->GetObjectField(TEXT("transform"))->GetArrayField(TEXT("rotation"));
            TArray<TSharedPtr<FJsonValue>> ScaleArray = ObjectData->GetObjectField(TEXT("transform"))->GetArrayField(TEXT("scale"));

            if (LocationArray.Num() == 3 && RotationArray.Num() == 3 && ScaleArray.Num() == 3)
            {
                FVector SpawnLocation(LocationArray[0]->AsNumber(), LocationArray[1]->AsNumber(), LocationArray[2]->AsNumber());
                FRotator SpawnRotation(RotationArray[0]->AsNumber(), RotationArray[1]->AsNumber(), RotationArray[2]->AsNumber());
                FVector SpawnScale(ScaleArray[0]->AsNumber(), ScaleArray[1]->AsNumber(), ScaleArray[2]->AsNumber());

                FTransform SpawnTransform(SpawnRotation, SpawnLocation, SpawnScale);

                // Create a new FBoxTypeData object and copy data
                FBoxTypeData ConvertedBoxData;
                ConvertedBoxData.Color = BoxData.Color;
                ConvertedBoxData.Health = BoxData.Health;
                ConvertedBoxData.Score = BoxData.Score;

                // Use the BoxActorClass to spawn the correct derived class
                if (BoxActorClass)
                {
                    // Spawn the box actor using BoxActorClass
                    ABoxActor* SpawnedBox = GetWorld()->SpawnActor<ABoxActor>(BoxActorClass, SpawnTransform);
                    if (SpawnedBox)
                    {
                        SpawnedBox->SetBoxType(ConvertedBoxData);  // Pass the converted FBoxTypeData
                    }
                    else
                    {
                        DebugLog(TEXT("Failed to spawn BoxActor"));
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("BoxActorClass is not set or invalid"));
                }
            }
            else
            {
                DebugLog(TEXT("Transform data is missing or invalid for object: " + BoxTypeName));
            }
        }
        else
        {
            DebugLog(TEXT("Box type not found in BoxTypes: " + BoxTypeName));
        }
    }
}
