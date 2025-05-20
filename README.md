# MSP430 FSM-Based Embedded System

This project implements a **multi-mode embedded system** on the MSP430G2553 microcontroller. The system is interrupt-driven and uses a **Finite State Machine (FSM)** to switch between three operational modes: frequency counter, stopwatch, and PWM-controlled buzzer.

---

## 🚀 Features

### ✅ Mode 1: Frequency Counter (PB0)
- Measures input signal frequency using **Timer_A1 Input Capture**.
- Displays frequency in Hz on **LCD** in the range of 50Hz–20kHz.
- LCD updates only the value digits to reduce flicker.

### ✅ Mode 2: Stopwatch (PB1)
- Implements a stopwatch using **Timer_A0 in Up/Down mode**.
- Activated by switch **SW0** and displays MM:SS format.
- Time resets after 59:59, display updates dynamically.

### ✅ Mode 3: PWM Buzzer (PB2)
- Samples analog voltage from **A3 input (potentiometer)** using ADC10.
- Translates ADC value to output frequency (1kHz–2.5kHz).
- PWM signal generated on **P2.2** to control a buzzer.

---

## 🧭 State Machine (FSM)

The system starts in `State 0` (idle), and transitions are based on pushbutton inputs:

- PB0 → State 1 (Frequency Counter)
- PB1 → State 2 (Stopwatch)
- PB2 → State 3 (PWM Buzzer)

From States 1–3, the following transitions apply:

- PB0 → Frequency Counter  
- PB1 → Stopwatch  
- PB2 → PWM Buzzer  

## 📦 File Structure

| File        | Purpose |
|-------------|---------|
| `main.c`    | FSM logic and main loop |
| `api.c/h`   | High-level mode implementations |
| `halGPIO.c/h` | Drivers: timers, LCD, ADC, interrupts |
| `bsp.c/h`   | Board support and hardware init |
| `app.h`     | FSM state enums |

---

## 🔧 Hardware Requirements

- MSP430G2553 LaunchPad
- LCD (4-bit mode)
- Pushbuttons (PB0–PB2)
- Slide switch (SW0)
- Potentiometer on A3 (P1.3)
- Buzzer on P2.2
- Signal Generator (P2.4 for frequency input)
