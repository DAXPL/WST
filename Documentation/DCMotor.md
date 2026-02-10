# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Plik `PIDController.cs` zawiera implementację **sterownika PID (Proporcjonalno-Różniczkowo-Integralnego)**, który jest kluczowym elementem w systemach sterowania automatycznego. Sterownik PID stosowany jest do regulacji procesów fizycznych, takich jak temperatura, ciśnienie, prędkość obrotowa, położenie czy napięcie, gdzie wymagana jest precyzyjna kontrola i minimalizacja błędu między wartością docelową (setpointem) a aktualną wartością (procesem).

Klasa `PIDController` stanowi **komponent rejestrujący i przetwarzający błędy w czasie rzeczywistym**, a jej zadaniem jest generowanie sygnału sterującego (output) na podstawie obecnego błędu oraz historii poprzednich błędów. W kontekście systemu, ten sterownik może być używany np. w sterownikach PLC, robotykach, systemach HVAC, lub w aplikacjach sterowania silnikami.

Z punktu widzenia architektury systemowej, klasa `PIDController` działa jako **moduł niezależny**, który może być używany przez inne komponenty, np. `ThermostatController`, `MotorDriver`, lub `FlightController`. Zależność od interfejsu `IPIDController` (zakładamy, że został zdefiniowany w innym pliku) pozwala na elastyczne testowanie, mockowanie oraz wdrażanie różnych implementacji sterowania PID bez zmiany kodu klienckiego.

Ponadto, klasa `PIDController` posiada **cykl życia**, który obejmuje:
- Inicjalizację z parametrami PID (`Kp`, `Ki`, `Kd`, `outputMin`, `outputMax`)
- Ustawienie wartości docelowej (`Setpoint`)
- Cykl `Update`, który otrzymuje aktualną wartość procesu i oblicza nową wartość sterującą
- Możliwość dynamicznej modyfikacji parametrów PID (`SetTunings`)

## 2. Analiza Logiki Biznesowej (Deep Dive)

### 2.1. Inicjalizacja i Konstruktor

W konstruktorze `PIDController` przekazywane są trzy parametry: `Kp`, `Ki`, `Kd` oraz opcjonalne `outputMin` i `outputMax`. Te parametry są przypisywane do prywatnych zmiennych członkowskich (`_kp`, `_ki`, `_kd`, `_outputMin`, `_outputMax`), które są wykorzystywane w obliczeniach PID.

Warto zauważyć, że nie ma żadnej walidacji danych wejściowych w konstruktorze – jeśli `Kp`, `Ki`, `Kd` są ujemne lub nieprawidłowe, kod może działać nieprawidłowo lub prowadzić do niestabilności sterowania. W systemach produkcyjnych warto dodać walidację lub logikę zabezpieczającą.

### 2.2. Metoda `Setpoint`

Metoda `Setpoint` ustawia wartość docelową (setpoint), która jest używana do obliczania błędu (`error = setpoint - processValue`). Wartość ta może być zmieniana dynamicznie w trakcie działania systemu – co pozwala na adaptację sterowania do zmieniających się warunków.

### 2.3. Metoda `Update`

To najważniejsza metoda w klasie – odpowiada za obliczenie wartości sterującej. W szczególności:

#### 2.3.1. Obliczenie błędu
```csharp
double error = setpoint - input;
```
Obliczenie błędu jest podstawą działania PID. Wartość ta reprezentuje odchylenie od wartości docelowej. W systemach, gdzie wartość procesu może się zmieniać bardzo szybko, może to prowadzić do problemów z przetwarzaniem, np. zjawiska „integral windup”.

#### 2.3.2. Obliczenie członu proporcjonalnego (P)
```csharp
double proportional = _kp * error;
```
Człon proporcjonalny reaguje natychmiast na błąd. Im większy błąd, tym większa siła działania sterująca. Wartość `Kp` musi być dobrana do charakteru procesu – zbyt duża wartość może prowadzić do oscylacji, zbyt mała – do wolnego reagowania.

#### 2.3.3. Obliczenie członu całkującego (I)
```csharp
_integral += _ki * error;
```
Człon całkujący akumuluje błąd w czasie i ma za zadanie eliminować błąd stały. Wartość `_integral` jest zwiększana o iloczyn `ki * error`. W systemach z dużym opóźnieniem lub dużym szumem, integral term może prowadzić do **integral windup**, czyli do przesadnego akumulowania błędu. W kodzie nie ma mechanizmu ograniczającego `_integral`, co może być problematyczne.

#### 2.3.4. Obliczenie członu różniczkowego (D)
```csharp
double derivative = _kd * (error - _lastError);
```
Człon różniczkowy reaguje na szybkość zmiany błędu. Wartość ta pozwala „przepowiadać” przyszły błąd i zapobiegać przeregujcowi. Wartość `_lastError` jest zapisywana po każdym wywołaniu `Update`, co pozwala na obliczenie różnicy.

#### 2.3.5. Obliczenie wyniku PID
```csharp
double output = proportional + _integral + derivative;
```
Wynik PID to suma trzech członów. Następnie następuje ograniczenie wartości do zakresu `[outputMin, outputMax]`:
```csharp
if (output > _outputMax) output = _outputMax;
if (output < _outputMin) output = _outputMin;
```

#### 2.3.6. Zapisanie ostatniego błędu
```csharp
_lastError = error;
```
To pozwala na obliczenie członu różniczkowego w kolejnym cyklu.

### 2.4. Metoda `SetTunings`

Umożliwia dynamiczną zmianę parametrów PID (`Kp`, `Ki`, `Kd`). Może być używana w systemach, gdzie parametry PID są dostosowywane w czasie rzeczywistym – np. w systemach adaptacyjnych. Warto zauważyć, że metoda `SetTunings` nie resetuje `_integral` ani `_lastError`, co może prowadzić do niestabilności, jeśli parametry są zmieniane drastycznie.

## 3. Szczegóły Techniczne

### 3.1. Zależności i Dziedziczenie

Klasa `PIDController` implementuje interfejs `IPIDController`, który może wyglądać mniej więcej tak:
```csharp
public interface IPIDController
{
    double Update(double input);
    void Setpoint(double value);
    void SetTunings(double kp, double ki, double kd);
}
```
Zaimplementowanie interfejsu pozwala na:
- **Polimorfizm** – różne implementacje sterowania mogą być używane w tym samym miejscu
- **Testowalność** – można łatwiej napisać testy jednostkowe, mockując interfejs
- **Rozszerzalność** – w przyszłości można dodać inne algorytmy sterowania, np. fuzzy logic, MPC

### 3.2. Przepływ Danych (Data Flow)

#### 3.2.1. Wejścia

- `double input` – wartość aktualnego procesu (np. temperatura z czujnika, prędkość obrotowa)
- `double setpoint` – wartość docelowa (np. 25°C, 1000 RPM)
- `double kp, ki, kd` – parametry PID
- `double outputMin, outputMax` – zakres wyjścia sterującego (np. 0–100% PWM)

#### 3.2.2. Wyjścia

- `double output` – wartość sterująca (np. sygnał PWM, napięcie, prąd)
- Zmienne `_integral`, `_lastError` – używane wewnętrznie do obliczeń, nie są eksponowane

### 3.3. Kluczowe Zmienne

- `_kp`, `_ki`, `_kd` – stałe PID, które kontrolują wpływ odpowiednich członów
- `_outputMin`, `_outputMax` – ograniczenia wyjścia, zapobiegają nadmiernemu sterowaniu
- `_integral` – akumulowany błąd, używany w członie całkującym
- `_lastError` – poprzedni błąd, używany do obliczenia członu różniczkowego

## 4. Kącik Edukacyjny (Mentoring) 🎓

### 4.1. Wzorce i Architektura

#### 4.1.1. Wzorzec: Komponent Sterujący (Control Loop Component)
Klasa `PIDController` reprezentuje **komponent sterowania**, który działa w cyklu: odczyt → obliczenie → sterowanie. Jest to klasyczny wzorzec stosowany w systemach sterowania przemysłowego (SCADA, PLC, RTOS). Zastosowanie tego wzorca pozwala:
- Rozdzielić logikę obliczeniową od logiki sterowania fizycznego
- Wspierać testowanie (np. można symulować czujniki)
- Ułatwiać integrację z innymi komponentami sterującymi

#### 4.1.2. Wzorzec: Strategy (dla interfejsu `IPIDController`)
Użycie interfejsu `IPIDController` pozwala na stosowanie wzorca Strategy – różne implementacje PID mogą być używane w zależności od kontekstu (np. PID klasyczny, PID z anti-windup, PID z filtrowaniem). W systemach z dużą ilością sterowania, wzorzec ten pozwala na łatwe przełączanie algorytmów bez zmiany kodu klienckiego.

### 4.2. Analiza Code-Level

#### 4.2.1. C++/C# Syntax Constructs

- **Referencje i wartości**: W C# nie ma wskaźników, ale obiekty są przekazywane przez referencję. W tym przypadku metoda `Update` nie ma parametrów przekazywanych przez referencję – wszystko jest przekazywane przez wartość, co jest bezpieczne, ale może być mniej wydajne w przypadku dużych struktur.

- **Virtual Methods**: Jeśli `Update` byłby zdefiniowany jako `virtual`, to można by tworzyć klasy dziedziczące z `PIDController`, np. `AdvancedPIDController`, który dodaje funkcje takie jak anti-windup, output filtering, lub integracja z logiką fuzzy. Obecnie nie ma takiej możliwości.

- **Zmienne prywatne i chronione**: Wszystkie zmienne są prywatne, co zapewnia enkapsulację i chroni przed nieprawidłowym dostępem z zewnątrz.

#### 4.2.2. Potencjalne Ryzyka i Rozwiązania

- **Integral Windup**: Brak ograniczenia `_integral` może prowadzić do przesadnego akumulowania błędu. Rozwiązaniem może być:
  ```csharp
  if (_integral > _integralMax) _integral = _integralMax;
  if (_integral < _integralMin) _integral = _integralMin;
  ```

- **Zła inicjalizacja `_lastError`**: Wartość `_lastError` jest inicjalizowana jako `0`, co może prowadzić do dużego członu różniczkowego przy pierwszym wywołaniu `Update`. Rozwiązaniem może być:
  ```csharp
  _lastError = double.NaN;
  ```

- **Brak synchronizacji**: W systemach wielowątkowych (`Task`, `Thread`, `async`) brak synchronizacji może prowadzić do błędnego wyniku `Update`. Warto zastosować `lock` lub `Interlocked` przy dostępach do zmiennych `_integral`, `_lastError`.

- **Brak walidacji danych wejściowych**: Warto dodać walidację:
  ```csharp
  if (double.IsNaN(kp) || double.IsNaN(ki) || double.IsNaN(kd)) throw new ArgumentException("PID coefficients must be valid numbers");
  ```

### 4.3. Praktyczne Zastosowanie

W systemach sterowania przemysłowego, np. w HVAC, PID może być używany do regulacji temperatury:
- `input` = temperatura z czujnika
- `setpoint` = temperatura docelowa
- `output` = ustawienie mocy wentylatora lub zaworu

W przypadku systemów robotycznych, np. sterowania silnikiem krokowym:
- `input` = aktualna pozycja
- `setpoint` = docelowa pozycja
- `output` = sygnał sterujący silnikiem

### 4.4. Wskazówki dla Juniorów

- **Nie używaj PID bez wiedzy o jego działania** – PID to nie tylko wzór, ale system z wieloma parametrami do dostrajania.
- **Zrozumienie integral windup** – błąd może się akumulować i prowadzić do niestabilności.
- **Zrozumienie wpływu `Kd`** – człon różniczkowy może być bardzo wrażliwy na szum.
- **Testowanie PID** – najlepiej testować PID w symulatorze, zanim wdroży się w rzeczywisty system.

### 4.5. Wskazówki dla Seniorsów

- **Użyj `Interlocked` lub `lock`** w systemach wielowątkowych.
- **Zadbaj o filtrowanie danych wejściowych** – szum w czujnikach może prowadzić do niestabilności.
- **Zadbaj o skalowanie danych** – np. przekształcanie wartości z zakresu 0–1023 do 0–100.
- **Zadbaj o dynamiczne dostrajanie PID** – np. w systemach adaptacyjnych, warto używać algorytmów auto-tuning.
- **Zadbaj o dokumentację parametrów PID** – w systemach produkcyjnych trzeba znać wartości `Kp`, `Ki`, `Kd` dla każdego procesu.

---

Jeśli chcesz, mogę również przygotować **wersję dla testów jednostkowych**, np. `PIDControllerTests.cs`, lub wersję z obsługą **anti-windup**, **filtrowaniem**, **dostosowywania czasu próbkowania**, itd.