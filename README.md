# SSEEE Workshop — Arduino Projects

Three small Arduino sketches built during the SSEEE workshop. Each project lives in its own folder with a single `main.ino` file.

```
sseee-workshop/
├── traffic-light/
│   └── main.ino
├── mini-auto-door/
│   └── main.ino
└── game/
    └── main.ino
```
---

## 1. Traffic Light — `traffic-light/main.ino`

A minimal three-LED traffic light that flashes red → yellow → green in a loop.

### Wiring

| LED    | Arduino pin |
|--------|-------------|
| Red    | 1           |
| Yellow | 3           |
| Green  | 5           |

Each LED needs a current-limiting resistor (220 Ω–330 Ω) to GND.

### Behavior

- Loops over all three LEDs.
- Each LED turns on for **700 ms**, then off, then the next one.

---

## 2. Mini Auto Door — `mini-auto-door/main.ino`

An ultrasonic sensor detects how close an object is, and a servo opens a "door" when something comes near.

### Wiring

| HC-SR04 | Arduino |
|---------|---------|
| VCC     | 5V      |
| GND     | GND     |
| TRIG    | 7       |
| ECHO    | 8       |

| Servo   | Arduino |
|---------|---------|
| Signal  | 9       |
| VCC     | 5V      |
| GND     | GND     |

If the servo jitters or the board resets, power the servo from a **separate 5 V supply** and connect the grounds together.

### Behavior

- Reads the HC-SR04 every 150 ms.
- Converts echo time to centimeters: `cm = duration * 0.0343 / 2`.
- If an object is **≤ 20 cm** away → servo moves to **90°** ("Found target").
- Otherwise → servo returns to **0°** ("No target").
- Serial Monitor (9600 baud) prints distance and state.

### Tuning

- Change the trigger distance in `if (cm <= 20)`.
- Change the open angle in `doorServo.write(90)`.

---

## 3. Memory Game — `game/main.ino`

A Simon-style memory game. The board flashes a random sequence of 5 LEDs; the player must reproduce it using a potentiometer to select an LED and a button to commit.

### Wiring

| Component        | Arduino pin | Notes |
|------------------|-------------|-------|
| LEDs (×5)        | 2, 3, 4, 5, 6 | Each with a 220 Ω–330 Ω resistor to GND |
| Potentiometer    | A5          | Wiper to A5, ends to 5V and GND |
| Push button      | 8           | Between pin 8 and GND (internal pull-up used) |
| Seed pin         | A0          | **Leave unconnected** — floating noise seeds the RNG |

### How a round works

1. The game generates a random sequence of 5 LEDs.
2. The sequence is flashed one LED at a time (400 ms on, 200 ms off).
3. A longer 800 ms dark pause signals the player's turn.
4. The player turns the pot; the corresponding LED lights up.
5. Pressing the button commits that LED — it blinks out to confirm.
6. **One wrong entry ends the round.**

- **All 5 correct** → win animation (LEDs fill and drain).
- **Any wrong** → lose animation (all LEDs blink twice).

### Pot → LED mapping

| Pot value | LED index |
|-----------|-----------|
| 0–99      | 0 |
| 100–199   | 1 |
| 200–299   | 2 |
| 300–399   | 3 |
| 400–499   | 4 |
| 500–599   | 0 |
| …         | … |
| 900–1023  | 4 |

The value is clamped to `0…999` first, so the top of the pot range maps cleanly to index 4.

### Serial Monitor

Open at **9600 baud** to see round announcements, each step's chosen LED, and the win/lose result.

---

## Getting started

1. Install the `Arduino IDE`.
2. Open the `.ino` file for the project you want (see the naming note above).
3. Select your board and port under **Tools**.
4. Click **Upload**.
5. Open **Serial Monitor** at **9600 baud** for `mini-auto-door` and `game`.

## Requirements

- Arduino Uno, Nano, or compatible (5 V logic).
- `Servo` library — bundled with the Arduino IDE, no separate install needed.