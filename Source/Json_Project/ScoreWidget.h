#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

/**
 * A UserWidget that displays the player's score in the Json_Project game.
 */
UCLASS()
class JSON_PROJECT_API UScoreWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Constructor for ScoreWidget
    UScoreWidget(const FObjectInitializer& ObjectInitializer);

    // Function to update the score text
    UFUNCTION(BlueprintCallable, Category = "Score")
    void UpdateScore(int32 NewScore);

    // Getter function to access the current score from Blueprint
    UFUNCTION(BlueprintCallable, Category = "Score")
    int32 GetCurrentScore() const { return CurrentScore; }

protected:
    // Text block to display the score
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ScoreText;

private:
    // Player's score, exposed to Blueprint
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Score", meta = (AllowPrivateAccess = "true"))
    int32 CurrentScore;
};
