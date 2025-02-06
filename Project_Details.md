# Arduino Reaction Game - Full Documentation

## Description
This project is a reaction game using an Arduino Uno, Bluetooth control, and adjustable speed. The game challenges players to press a button when the correct LED color appears. The difficulty can be adjusted using a potentiometer, and the LED colors change dynamically based on ambient temperature.

## Components
- **Arduino Uno** – Controls the entire system.
- **Bluetooth Module HC-05** – Allows wireless communication with a mobile app.
- **RGB LED + Additional LEDs** – Displays colors for the reaction game.
- **Temperature Sensor** – Adjusts LED colors based on room temperature.
- **Potentiometer** – Adjusts game difficulty by changing LED speed.
- **Power Supply or Battery** – Powers the circuit.
- **Wires & Breadboard** – Used for connections.

## Features
- **Reaction-based gameplay** – Players must react quickly to a color change.
- **Wireless Bluetooth control** – Eliminates the need for physical buttons.
- **Temperature-based LED colors** – LED colors shift between warm and cool tones depending on the room temperature.
- **Adjustable difficulty** – The potentiometer controls the speed of color changes.

## How It Works

1. **Game Start**  
   - The player connects to the system via Bluetooth using the "Arduino Bluetooth Control" app.  
   - The game starts when the player sends the `"START"` command from the mobile app.

2. **LED Color Display**  
   - A color is randomly selected and displayed on the LED.  
   - If the temperature is **below 30°C**, the system displays cool colors (**green or cyan**).  
   - If the temperature is **above 30°C**, the system displays warm colors (**red, yellow, or magenta**).  

3. **Player Reaction**  
   - The player must press the corresponding color button on the mobile app.  
   - If the correct button is pressed, the **score increases**.  
   - If the wrong button is pressed (or no response is given within the time limit), the **score decreases**.  

4. **Game Progression**  
   - The difficulty is controlled by a **potentiometer**:  
     - **Turning it up** increases the speed of the color changes (harder game).  
     - **Turning it down** slows the color changes (easier game).  

5. **Winning and Losing Conditions**  
   - The player **wins** if they reach **10 correct answers**.  
   - The player **loses** if their score drops to **-10**.  
   - When the game ends, all LEDs turn off, and the player can restart the game.

## Code
The full Arduino code is available in `reaction_game.ino`.

## Mobile App
- The game is controlled using **Arduino Bluetooth Control** (available on Android).
- The app sends `"START"` to begin the game and allows players to press virtual buttons corresponding to the LED colors.
