# Cyrus_Assignment
 
Description
This Project demonstrates several Unreal Engine features implemented entirely in native C++ without external libraries. The project integrates a dynamic box spawning system, online JSON data fetching, score tracking, and UI updates, showcasing how Unreal's C++ API can be used to create robust game mechanics.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Features:
1. JSON Data Fetching
The project fetches a JSON file from an online source:
https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json

The JSON file contains definitions for multiple boxes with attributes:
Type: Reserved for future expansions.

Color: Used to set the material color of each box.
Health: The initial health value of each box.
Score: Points awarded upon destroying the box.

Unreal's FHttpModule and FJsonSerializer are used to fetch and parse the data.
Proper error handling ensures the game gracefully handles HTTP errors, JSON parse errors, or invalid data formats.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

2. Box Spawning Based on JSON Data
Each entry in the JSON file defines the attributes of a box that the BoxSpawner uses to spawn boxes dynamically.
Key Functionalities:
- Boxes spawn with a static mesh and dynamic material instance.
- The material color is determined by the Color attribute from the JSON data.
- Boxes are damageable and track their Health. Each hit reduces health by 1.
- A box is destroyed when its health reaches zero, triggering a score increment based on its Score attribute.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

3. Dynamic Box Features
Custom Appearance:
The BoxActor class supports dynamic customization through its child blueprint. This enables:

- Setting the mesh, material, and other properties dynamically.
- Adjusting material parameters like color using data from the JSON file.

Destructible Boxes:

-Each box tracks its health and responds to damage.
-On destruction, it awards the specified score and informs the player character to update the UI.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

4. Dynamic Score Display
A UScoreWidget class is implemented in C++ to display the current score.
The widget binds to the player's score and updates dynamically when boxes are destroyed.
The score UI layout and positioning are configured in Blueprint.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Implementation Logic

A. JSON Data Fetching
HTTP Request

Unreal's FHttpModule is used to perform GET requests to the provided JSON URL.
Data Parsing

The fetched JSON string is parsed using FJsonSerializer.
Valid entries are added to a structured array for spawning.
Error Handling

Handles HTTP errors, ensuring the game does not crash if the request fails.
Verifies the JSON schema to avoid processing invalid data.


B. Box Spawning
Spawner Logic

The BoxSpawner retrieves parsed JSON data and spawns boxes with attributes:
Mesh: Static mesh for visual representation.
Material: Dynamic material instances with customizable color.
Health: Set the initial health value for the box.
Score: Determines the points awarded upon destruction.

BoxActor Behavior

Each box is an instance of BoxActor.
Boxes handle:
Health Reduction: Reduces health on hit.
Destruction: Award score and update UI when health reaches zero.
Challenges Faced
JSON Parsing

Initial issues with handling malformed JSON entries were resolved by adding schema validation and default values for missing fields.
Dynamic Material Updates

Configuring material parameters dynamically required creating dynamic material instances at runtime for each box.
Error Handling

Ensuring robust error handling for network and parsing errors to prevent crashes or undefined behavior.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

How to Test the Feature
Run the Project

Open Json_Project in Unreal Engine, and click "Play" to start the game.
Observe Box Spawning

Boxes spawn dynamically with attributes derived from the JSON file.
Destroy Boxes

Interact with or shoot boxes to destroy them. Observe health reduction and destruction behavior.
Score Updates

The score displayed on the UI updates dynamically based on the Score value of each destroyed box.
Check Logs (Optional)

Use the output log to monitor network requests, JSON parsing results, and debug messages.