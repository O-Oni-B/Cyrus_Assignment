#include "ScoreWidget.h"
#include "Components/TextBlock.h"

// Constructor for ScoreWidget
UScoreWidget::UScoreWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Initialize the current score to 0
    CurrentScore = 0;
}

// Called to update the score on the widget
void UScoreWidget::UpdateScore(int32 NewScore)
{
    // Set the new score
    CurrentScore = NewScore;

    // Update the displayed score in the TextBlock
    if (ScoreText)
    {
        // Convert the score to a string and set it in the TextBlock
        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), CurrentScore)));
    }
}
