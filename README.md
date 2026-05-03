
# Smart Car Parking System 

An IoT-based smart parking system that enables real-time slot monitoring and predicts future parking availability using time-series forecasting.

---

## Overview

This system uses sensors and microcontrollers to detect parking slot occupancy, automate entry control, and send real-time data to a database. Historical data is analyzed using the ARIMA model to forecast future parking availability.

---

## Features

* Real-time slot detection using IR sensors
* Automated gate control (servo motor)
* Live slot display via LCD
* Fire alert integration
* IoT-based data transmission (ESP8266)
* Predictive analysis for parking availability

---

## Tech Stack

**Hardware**

* Arduino Uno
* IR Sensors
* Servo Motor
* LCD Display
* ESP8266

**Software**

* Embedded C (Arduino programming)
* Python (data analysis & forecasting)
* pandas, numpy (data processing)
* statsmodels (ARIMA model)
* Firebase / MySQL

---

## System Architecture

1. Sensors detect vehicle entry/exit
2. Arduino updates slot count
3. Servo controls gate based on availability
4. Data sent via ESP8266 to database
5. Python processes historical data
6. ARIMA model predicts future slots

---

## Data Format

```
timestamp | available_slots
```

---

## How It Works

* Entry detected → slot count decreases
* Exit detected → slot count increases
* Data stored in database with timestamps
* Python fetches and preprocesses data
* ARIMA model forecasts future availability

---

