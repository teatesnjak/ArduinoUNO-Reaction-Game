#include <SoftwareSerial.h>
#include <Arduino.h>

// Serial configuration for Bluetooth
SoftwareSerial Bluetooth(10, 11); // RX, TX

// LED Pins
const int redLed = 2;
const int yellowLed = 12;
const int greenLed = 4;
const int magentaR = 5; // Magenta uses Red and Blue (RGB)
const int magentaB = 6;
const int cyanG = 7;    // Cyan uses Green and Blue (RGB)
const int cyanB = 8;

// Temperature sensor and potentiometer pins
const int tempSensorPin = A0;
const int difficultyPotPin = A1;

// Game variables
int score = 1;
int targetScore = 10; // Score needed to win the game
int gameOverScore = -10; // Score needed to lose the game
bool gameActive = false; // Indicates if the game is active
String currentColor = ""; // Current target color
unsigned long lastColorChangeTime = 0; // Time of the last color change
unsigned long colorDuration = 5000; // Duration of each color in milliseconds (adjustable via potentiometer)
unsigned long colorPauseTime = 1000; // 1-second pause between colors
bool waitingForResponse = false; // Indicates if the game is waiting for the user's response

// Helper functions
void clearLEDs();
void lightUpLED(String color);
void generateRandomColor();
void checkTemperatureAndSetColors();
void adjustDifficulty();

// Function to start the game
void startGame() {
    score = 1;
    gameActive = true;
    adjustDifficulty(); // Adjust difficulty before starting
    Bluetooth.println("Game started! Press the corresponding color button.");

    delay(colorPauseTime);  // Pause for 1 second before starting
    generateRandomColor(); // Generate the first random color
    lastColorChangeTime = millis(); // Start the timer
    waitingForResponse = true;
}

// Function to adjust difficulty using the potentiometer
void adjustDifficulty() {
    int potValue = analogRead(difficultyPotPin);
    colorDuration = map(potValue, 0, 1023, 3000, 7000); // Map the value to a range of 3 to 7 seconds
    Bluetooth.println("Difficulty set: Color duration is " + String(colorDuration / 1000.0) + " seconds.");
}

// Function to generate a random color based on temperature
void generateRandomColor() {
    checkTemperatureAndSetColors();
    lightUpLED(currentColor);
    lastColorChangeTime = millis(); // Store the time when the color was displayed
    waitingForResponse = true;
}

// Function to check if the user's response is correct
void checkUserResponse(String incomingData) {
    if (!waitingForResponse) return; // Ignore input if not waiting for a response

    if (incomingData == currentColor) {
        score++; // Correct response
        Bluetooth.println("Correct! Score: " + String(score));
    } else {
        score--; // Incorrect response
        Bluetooth.println("Incorrect! Score: " + String(score));
    }

    waitingForResponse = false; // Stop waiting for response

    // Check if the game is over
    if (score >= targetScore) {
        Bluetooth.println("You win! Final Score: " + String(score));
        gameActive = false;
        clearLEDs();
    } else if (score <= gameOverScore) {
        Bluetooth.println("You lost the game! Final Score: " + String(score));
        gameActive = false;
        clearLEDs();
    } else {
        // Pause and generate a new color if the game continues
        delay(colorPauseTime);
        clearLEDs();
        delay(colorPauseTime);
        generateRandomColor();
    }
}

// Function to handle game timing
void handleGameTiming() {
    unsigned long currentMillis = millis();

    if (waitingForResponse && currentMillis - lastColorChangeTime >= colorDuration) {
        // Time's up for the current color, penalize the player
        score--;
        Bluetooth.println("Time's up! Incorrect! Score: " + String(score));

        waitingForResponse = false; // Stop waiting for response

        // Check if the game is over
        if (score <= gameOverScore) {
            Bluetooth.println("You lost the game! Final Score: " + String(score));
            gameActive = false;
            clearLEDs();
        } else {
            // Pause and generate a new color if the game continues
            delay(colorPauseTime);
            clearLEDs();
            delay(colorPauseTime);
            generateRandomColor();
        }
    }
}

void setup() {
    // Initialize serial communication for Bluetooth
    Bluetooth.begin(9600);

    // Set LED pins as outputs
    pinMode(redLed, OUTPUT);
    pinMode(yellowLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(magentaR, OUTPUT);
    pinMode(magentaB, OUTPUT);
    pinMode(cyanG, OUTPUT);
    pinMode(cyanB, OUTPUT);

    // Turn off all LEDs at the beginning
    clearLEDs();
    Bluetooth.println("Game Initialized. Set difficulty and press 'START' to begin!");
}

void loop() {
    // Check if data is available from Bluetooth
    if (Bluetooth.available()) {
        String incomingData = Bluetooth.readString();
        incomingData.trim(); // Remove unnecessary spaces

        // If the "START" button is pressed, start the game
        if (incomingData == "START" && !gameActive) {
            startGame();
        }

        // If the game is active, check the user response
        if (gameActive) {
            checkUserResponse(incomingData);
        }
    }

    // Handle the game timing and LED durations
    if (gameActive) {
        handleGameTiming();
    }
}

// Function to turn off all LEDs
void clearLEDs() {
    digitalWrite(redLed, LOW);
    digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, LOW);
    analogWrite(magentaR, LOW);
    analogWrite(magentaB, LOW);
    analogWrite(cyanG, LOW);
    analogWrite(cyanB, LOW);
}

// Function to light up a specific LED
void lightUpLED(String color) {
    clearLEDs(); // Turn off all LEDs before turning on the new one

    if (color == "RED") {
        digitalWrite(redLed, HIGH); // Turn on the red LED
    } else if (color == "YELLOW") {
        digitalWrite(yellowLed, HIGH); // Turn on the yellow LED
    } else if (color == "GREEN") {
        digitalWrite(greenLed, HIGH); // Turn on the green LED
    } else if (color == "MAGENTA") {
        analogWrite(magentaR, 255); // Turn on the magenta LED (RGB)
        analogWrite(magentaB, 200);
    } else if (color == "CYAN") {
        analogWrite(cyanG, 150); // Turn on the cyan LED (RGB)
        analogWrite(cyanB, 250);
    }
}

// Function to check the temperature and adjust colors
void checkTemperatureAndSetColors() {
    int tempReading = analogRead(tempSensorPin);
    float voltage = tempReading * (5.0 / 1023.0);
    float temperatureC = voltage * 100.0;

    if (temperatureC < 30.0) {
        // Cold: Cool tones (Green, Cyan)
        Bluetooth.println("Temperature: " + String(temperatureC) + "°C - Cool tones active.");
        currentColor = (random(2) == 0) ? "GREEN" : "CYAN";
    } else {
        // Hot: Warm tones (Red, Yellow, Magenta)
        Bluetooth.println("Temperature: " + String(temperatureC) + "°C - Warm tones active.");
        int randomWarmColor = random(3);
        if (randomWarmColor == 0) currentColor = "RED";
        else if (randomWarmColor == 1) currentColor = "YELLOW";
        else currentColor = "MAGENTA";
    }
}
