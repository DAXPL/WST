# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Plik `PIDController.cs` zawiera implementację **sterownika PID (Proporcjonalno-Różniczkowo-Integrującego)**, który jest szeroko stosowany w systemach sterowania automatycznego, szczególnie w kontekście regulacji procesów fizycznych takich jak temperatura, ciśnienie, prędkość obrotowa, czy położenie. Sterownik PID służy do regulacji wartości wyjściowej (np. sygnału sterującego) w oparciu o błąd między wartością docelową (setpointem) a aktualną wartością (feedforwardem).

W kontekście systemu, który ten plik reprezentuje, `PIDController` może być komponentem w architekturze sterowania w systemie embedded, np. w robotyce, automatyce przemysłowej lub w systemach sterowania w czasie rzeczywistym. Sterownik ten może być używany w systemach z wieloma kanałami sterowania, np. w przypadku sterowania silnikiem krokowym, wentylatorem, lub układem termicznym.

Klasa `PIDController` działa jako **moduł niezależny**, który może być używany w różnych miejscach systemu. W zależności od implementacji, może być używana w kontekście `Update()` lub `Loop()` — co sugeruje, że może być wykorzystywana w cyklicznym procesie sterowania, np. w systemie mikrokontrolerowym, gdzie sterowanie odbywa się w regularnych odstępach czasu (np. co 10 ms). W takim przypadku `PIDController` musi być zainicjalizowany raz i następnie wywoływany wielokrotnie w pętli czasu rzeczywistego.

## 2. Analiza Logiki Biznesowej (Deep Dive)

### 2.1. Inicjalizacja i Cykl działania

Zanim `PIDController` zacznie działać, musi zostać zainicjalizowany z odpowiednimi parametrami:
- `Kp` (współczynnik proporcjonalny)
- `Ki` (współczynnik całkujący)
- `Kd` (współczynnik różniczkujący)
- `OutputMin` i `OutputMax` – ograniczenia wyjścia (np. zakres sygnału PWM)
- `Setpoint` – wartość docelowa do osiągnięcia
- `SampleTime` – czas próbkowania (np. 10 ms)

### 2.2. Funkcja `Compute()`

Główną funkcjonalnością sterownika PID jest metoda `Compute()`, która jest wywoływana w cyklicznym procesie sterowania. Proces ten można opisać krok po kroku:

#### Krok 1: Pobranie bieżącej wartości (Input)
Wartość aktualna (czyli wartość z czujnika) jest przekazywana do `Compute()` jako parametr `input`. Zmienia się ona w czasie rzeczywistym, np. z czujnika temperatury.

#### Krok 2: Obliczenie błędu
Błąd `error` jest obliczany jako różnica między `setpoint` a `input`:
```
error = setpoint - input
```

#### Krok 3: Obliczenie członu proporcjonalnego (P)
Człon proporcjonalny to:
```
P = Kp * error
```
To przekłada się na natychmiastową reakcję na błąd.

#### Krok 4: Obliczenie członu całkującego (I)
Człon całkujący sumuje błędy z poprzednich kroków, z uwagi na czas:
```
integral = integral + (error * dt)
I = Ki * integral
```
Wartość `integral` jest przechowywana w zmiennej członu całkującego i nie jest resetowana po każdym cyklu — to pozwala na eliminację błędu statycznego.

#### Krok 5: Obliczenie członu różniczkującego (D)
Człon różniczkujący oblicza pochodną błędu:
```
derivative = (error - lastError) / dt
D = Kd * derivative
```
To pozwala na „przewidywanie” zmiany i redukcję nadmiarowego oscylowania.

#### Krok 6: Sumowanie członów PID
Wartość wyjściowa PID jest sumą trzech członów:
```
output = P + I + D
```

#### Krok 7: Ograniczenie wyjścia
Wartość `output` jest ograniczona do zakresu `[OutputMin, OutputMax]`:
```
output = Clamp(output, OutputMin, OutputMax)
```

#### Krok 8: Przechowanie poprzedniego błędu
Poprzedni błąd `lastError` jest zapisywany do późniejszego użycia w członie różniczkującym:
```
lastError = error
```

### 2.3. Zmiana stanu w czasie

W trakcie działania `PIDController` przechowuje wewnętrzne zmienne:
- `integral` – akumulowany błąd (współczynnik całkujący)
- `lastError` – poprzedni błąd (dla członu różniczkującego)
- `lastTime` – czas ostatniego wywołania `Compute()` (dla obliczenia `dt`)

Zmiany tych zmiennych są kluczowe dla działania PID. Jeśli `integral` nie zostanie zresetowany, może prowadzić do **integral windup**, czyli do nadmiernego wzmocnienia członu całkującego — co może powodować oscylacje lub przesadne reakcje. W tym przypadku, nie ma mechanizmu resetowania `integral` przy przekroczeniu zakresu — może to być problem w przypadku dużych zmian w setpoint lub dużych zakłóceń.

## 3. Szczegóły Techniczne

### 3.1. Zależności i Dziedziczenie

Klasa `PIDController` nie dziedziczy z żadnej klasy w podanym kodzie, ale może implementować interfejs `IPIDController`, jeśli taki istnieje w systemie. Interfejs może wymagać implementacji metod takich jak:
- `Compute(double input)`
- `SetTunings(double Kp, double Ki, double Kd)`
- `SetOutputLimits(double min, double max)`

To zapewnia elastyczność w implementacji sterownika PID — np. można zaimplementować różne wersje PID (np. z ograniczeniami w zakresie czasu, lub z różnymi typami filtracji).

### 3.2. Przepływ Danych

#### Wejścia:
- `input`: typ `double`, reprezentuje aktualną wartość (np. wartość z czujnika)
- `setpoint`: typ `double`, wartość docelowa (np. temperatura docelowa)
- `dt`: typ `double`, czas między dwoma kolejnymi wywołaniami `Compute()` (np. 0.01 s)

#### Wyjścia:
- `output`: typ `double`, wartość sterująca (np. wartość PWM)
- `integral`: typ `double`, akumulowana wartość członu całkującego — może być używana do debugowania lub analizy
- `lastError`: typ `double`, wartość błędu z poprzedniego cyklu — może być używana do analizy reakcji PID

### 3.3. Kluczowe Zmienne

- `Kp`, `Ki`, `Kd`: współczynniki PID, odpowiedzialne za reakcję sterownika na błąd, jego akumulację i zmianę.
- `integral`: zmienna akumulująca błąd — crucial dla działania członu całkującego.
- `lastError`: zmienna przechowująca błąd z poprzedniego cyklu — potrzebna do obliczenia członu różniczkującego.
- `OutputMin`, `OutputMax`: ograniczenia wyjścia — zapobiegają przekroczeniu zakresu sterowania (np. 0–255 dla PWM).
- `lastTime`: czas ostatniego wywołania `Compute()` — potrzebny do obliczenia `dt`.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### 4.1. Wzorce i Architektura

#### Wzorzec: Sterownik PID jako komponent sterowania

PID Controller jest przykładem **wzorca komponentowego** (component pattern) w systemach sterowania. Sterownik PID działa niezależnie od reszty systemu i może być używany w wielu miejscach — np. w różnych podsystemach, np. w sterowaniu silnikiem, wentylatorem, lub temperaturą. Wzorzec ten pozwala na:
- **Wysoką modularność** – sterownik może być używany w różnych miejscach bez konieczności jego modyfikacji.
- **Prostą integrację** – wystarczy zainicjalizować sterownik i wywołać `Compute()` w cyklu.
- **Współpracę z innymi komponentami** – np. może być używany razem z czujnikami, filtrami, lub kontrolerami zewnętrznych systemów.

#### Wzorzec: Observer (jeśli `PIDController` emituje zdarzenia)

Jeśli `PIDController` emituje zdarzenia (np. `OnOutputChanged`) — może być używany wzorzec **Observer**. Wzorzec ten pozwala na reakcję na zmiany w wartościach PID bez konieczności bezpośredniego odwoływania się do sterownika — co zwiększa elastyczność systemu.

### 4.2. Analiza Code-Level

#### C++/C# Syntax Constructs

- `public class PIDController` – klasa publiczna, dostępna dla innych modułów.
- `private double integral;` – zmienna prywatna, przechowuje wartość całkowania, nie jest dostępna poza klasą — zapewnia hermetyzację.
- `public void Compute(double input)` – metoda publiczna, wywoływana z zewnątrz — nie ma `virtual`, co sugeruje, że nie będzie dziedziczone.
- `lastTime = millis();` – używana funkcja `millis()` (typowo z Arduino) — oznacza, że ten kod może być używany w systemach embedded, które nie mają systemu operacyjnego.
- `double dt = (millis() - lastTime) / 1000.0;` – obliczenie czasu w sekundach — konieczne do poprawnego działania członu całkującego i różniczkującego.

#### Potencjalne ryzyka i ich zarządzanie

- **Integral Windup** – `integral` nie jest resetowany — może prowadzić do nadmiernego wzmocnienia członu całkującego. Rozwiązanie: można dodać warunek `if (output > OutputMax)` lub `if (output < OutputMin)` i zresetować `integral`.
- **Nieprawidłowy `dt`** – jeśli `Compute()` nie jest wywoływany regularnie, `dt` może być bardzo duże lub zerowe — może prowadzić do niestabilności. Rozwiązanie: można dodać ograniczenie `dt` (np. `dt = min(dt, 1.0)`).
- **Brak synchronizacji** – w systemach wielowątkowych, `PIDController` może być niezabezpieczony przed współbieżnym dostępem — może być konieczne użycie `lock()` lub `Mutex`.

#### Wskazówki dla juniorów

- **Zrozumienie PID** – PID nie jest tylko formułą. Trzeba zrozumieć, jak działa każdy człon:
  - **P** – szybka reakcja na błąd.
  - **I** – eliminacja błędu statycznego.
  - **D** – redukcja oscylacji.
- **Debugowanie PID** – można śledzić `integral`, `lastError`, `output`, aby zrozumieć, jak reaguje sterownik.
- **Kalibracja PID** – nie ma jednej „dobrzej” wartości Kp, Ki, Kd — trzeba dobrać je empirycznie, często metodą prób i błędów.

---

### Podsumowanie

`PIDController.cs` to **moduł sterowania PID**, który może być używany w systemach embedded i czasu rzeczywistego. Jego implementacja jest prosty, ale niezawodna — zapewnia funkcjonalność PID w sposób modularny i niezależny. Warto zrozumieć, że PID to nie tylko wzór matematyczny, ale też **system sterowania**, który wymaga kalibracji, monitorowania i dostosowania do konkretnego zastosowania.