// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Json_Project : ModuleRules
{
    public Json_Project(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Add the dependencies for the project
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "Http",          // HTTP module for making requests
            "Json",          // Module for handling JSON data
            "JsonUtilities"  // Module for advanced JSON operations (like serializing/deserializing)
        });
    }
}
