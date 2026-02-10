# Contents

## Entry point

* [Main](main.md) — Główny punkt startowy systemu, inicjalizacja modułów oraz uruchomienie pętli sterowania.
* [Configuration](Configuration.md) — Struktura konfiguracji systemu, parametry sprzętowe i runtime wykorzystywane przez moduły.
* [DroneData](DroneData.md) — Sterowanie dronem

---

## Interfaces (Abstractions)

* [ISensor](ISensor.md) — Bazowy interfejs dla komponentów dostarczających dane pomiarowe.
* [IActuator](IActuator.md) — Abstrakcja urządzeń wykonawczych generujących fizyczną reakcję systemu.
* [IMixer](IMixer.md) — Interfejs mapujący sygnały sterujące na wyjścia aktuatorów.
* [ICommunicationInterface](ICommunicationInterface.md) — Kontrakt implementacyjny dla warstw komunikacyjnych.

---

## Communication Modules

* [CommunicationModule](CommunicationModule.md) — Warstwa zarządzająca komunikacją i wyborem backendu transmisji.
* [CommunicationSerialModule](CommunicationSerialModule.md) — Implementacja komunikacji przewodowej UART/Serial.
* [CommunicationWiFiUDPModule](CommunicationWiFiUDPModule.md) — Transport pakietów przez UDP w sieci WiFi.
* [CommunicationESPNowModule](CommunicationESPNowModule.md) — Niskolatencyjna komunikacja peer-to-peer przez ESP-NOW.

---

## Sensors
* [SensorsData](SensorsData.md) — Agregowane dane pomiarowe z wszystkich sensorów w ujednoliconej formie.
* [SensorsModule](SensorsModule.md) — Moduł orkiestrujący inicjalizację, odczyt i agregację danych z sensorów.
* [AdxlSensor](AdxlSensor.md) — Obsługa akcelerometru ADXL do pomiaru przyspieszeń liniowych.
* [MpuSensor](MpuSensor.md) — Integracja IMU (akcelerometr + żyroskop) do estymacji orientacji.
* [DHT11Sensor](DHT11Sensor.md) — Pomiar temperatury i wilgotności otoczenia.
* [HCSR04Sensor](HCSR04Sensor.md) — Ultrasoniczny czujnik odległości do pomiaru dystansu/przeszkód.
* [BatteryVoltageDividerSensor](BatteryVoltageDividerSensor.md) — Monitorowanie napięcia baterii przez dzielnik napięcia.

---

## Actuators

* [DCMotor](DCMotor.md) — Sterowanie klasycznym silnikiem DC (PWM / kierunek).
* [ServoMotor](ServoMotor.md) — Kontrola serwomechanizmów pozycjonujących.
* [ESCActuator](ESCActuator.md) — Sterowanie regulatorami ESC dla silników bezszczotkowych.

---

## Mixers

* [AirBoatMixer](AirBoatMixer.md) — Mapowanie sterowania na napęd platformy typu airboat.
* [BicopterMixer](BicopterMixer.md) — Algorytm mieszania sygnałów dla konfiguracji bicopter.

---