# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Plik `PIDController.cs` zawiera implementację kontrolera PID (Proporcjonalno-Różniczkowo-Integrującego), który jest kluczowym elementem w systemach sterowania automatycznego. Kontroler PID służy do regulacji wartości wyjściowej w oparciu o błąd między wartością docelową (setpointem) a aktualną wartością mierzoną (procesem). W kontekście tego projektu, kontroler może być używany np. do regulacji temperatury, prędkości silnika, lub innego parametru dynamicznego w systemie robotycznym lub przemysłowym.

Klasa `PIDController` została zaprojektowana jako komponent wielokrotnego użytku, który może być używany w różnych kontekstach sterowania. Może być wykorzystywana zarówno w systemach wbudowanych (np. mikrokontrolery), jak i w środowiskach o zwiększonej wydajności (np. serwery sterujące). Przeznaczenie tej klasy to zapewnienie stabilnego i precyzyjnego sterowania w obecności zakłóceń i zmieniających się warunków.

Kontroler PID ma cykl życia, który zaczyna się od inicjalizacji parametrów (współczynników Kp, Ki, Kd), a następnie przeprowadza iteracyjne obliczenia, które są aktualizowane w czasie rzeczywistym. W zależności od implementacji, może być używany w sposób synchroniczny (np. w pętli sterującej) lub asynchroniczny (np. w systemie z zadaniami w tle). W tym przypadku, klasa jest zaprojektowana do pracy w środowisku synchronicznym, gdzie metoda `Update` jest wywoływana w regularnych odstępach czasu.

## 2. Analiza Logiki Biznesowej (Deep Dive)

### 2.1. Inicjalizacja i Konfiguracja

Klasa `PIDController` ma konstruktor, który przyjmuje trzy parametry: `Kp`, `Ki`, `Kd` – współczynniki proporcjonalnego, całkującego i różniczkującego. Te współczynniki są kluczowe w definiowaniu reakcji systemu na błąd.

```csharp
public PIDController(double kp, double ki, double kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
    PreviousError = 0;
    Integral = 0;
}
```

W tym miejscu inicjalizowane są zmienne pomocnicze:
- `PreviousError` – przechowuje poprzednią wartość błędu, potrzebna do obliczenia pochodnej.
- `Integral` – przechowuje sumę błędów z poprzednich iteracji, potrzebna do obliczenia członu całkującego.

### 2.2. Metoda `Update`

Metoda `Update` jest centralnym punktem logiki sterowania. Przyjmuje dwa parametry:
- `setpoint` – wartość docelowa (np. temperatura, prędkość).
- `processValue` – aktualna wartość mierzonego parametru.

#### 2.2.1. Obliczanie Błędu

```csharp
double error = setpoint - processValue;
```

Obliczany jest błąd jako różnica między wartością docelową a aktualną. Ten błąd jest podstawą dla całej logiki PID.

#### 2.2.2. Obliczanie Członu Proporcjonalnego (P)

```csharp
double proportional = Kp * error;
```

Człon proporcjonalny jest bezpośrednio proporcjonalny do błędu. Im większy błąd, tym większa siła działania sterująca.

#### 2.2.3. Obliczanie Członu Całkującego (I)

```csharp
Integral += error * dt;
double integral = Ki * Integral;
```

Człon całkujący akumuluje błędy z poprzednich kroków. W ten sposób system może „zapamiętać” błędy i działać w taki sposób, aby minimalizować błąd w długim okresie. Wartość `dt` (delta time) jest czasem między dwoma kolejnymi wywołaniami `Update`. Zmienia się w zależności od częstotliwości wywołań, więc należy zwrócić uwagę na konsekwencje numeryczne.

#### 2.2.4. Obliczanie Członu Różniczkującego (D)

```csharp
double derivative = Kd * (error - PreviousError) / dt;
```

Człon różniczkujący reaguje na szybkość zmiany błędu. W przypadku szybkiego zmieniającego się błędu, system reaguje silniej, co pozwala na redukcję nadmiarowego wahań.

#### 2.2.5. Sumowanie Wszystkich Członów

```csharp
double output = proportional + integral + derivative;
```

Wszystkie trzy człony są sumowane w celu uzyskania wartości sterującej. Wartość ta może być np. przekazywana do silnika lub innego elementu sterującego.

#### 2.2.6. Aktualizacja Zmiennych

```csharp
PreviousError = error;
```

Zmienna `PreviousError` jest aktualizowana po każdym kroku, aby móc obliczyć pochodną w kolejnym kroku.

### 2.3. Właściwości i Zmienne Wewnętrzne

Klasa ma kilka kluczowych zmiennych:
- `Kp`, `Ki`, `Kd` – współczynniki PID.
- `PreviousError` – wartość błędu z poprzedniego kroku.
- `Integral` – akumulowana wartość błędu.
- `dt` – czas między kolejnymi krokami (przyjmuje się, że jest ustawiany z zewnątrz).

### 2.4. Przepływ Wykonania

1. `Update` jest wywoływany z `setpoint` i `processValue`.
2. Obliczany jest błąd.
3. Obliczany jest człon proporcjonalny.
4. Obliczany jest człon całkujący (z akumulacją).
5. Obliczany jest człon różniczkujący.
6. Wszystkie trzy składniki są sumowane do wartości wyjściowej.
7. `PreviousError` jest aktualizowany.
8. `Integral` jest aktualizowany.

## 3. Szczegóły Techniczne

### 3.1. Zależności i Dziedziczenie

Klasa `PIDController` nie dziedziczy z żadnej klasy, ale może implementować interfejsy, np. `IController` (jeśli taki został zdefiniowany w projekcie). Jeśli taki interfejs istnieje, to zapewnia jednolity kontrakt dla wszystkich kontrolerów sterujących. W tym przypadku, kontroler PID może być używany w kontekście szerszego systemu sterowania, który wymaga jednolitego interfejsu dla różnych kontrolerów.

### 3.2. Przepływ Danych

#### 3.2.1. Wejścia

- `setpoint` – typ `double`, wartość docelowa (np. 100°C).
- `processValue` – typ `double`, wartość mierzona (np. 98°C).
- `dt` – typ `double`, czas w sekundach między kolejnymi krokami sterowania.

#### 3.2.2. Wyjścia

- `output` – typ `double`, wartość sterująca (np. wartość PWM, napięcie, siła).
- `Integral` – typ `double`, akumulowana wartość błędu (dla debugowania lub przesyłu danych).
- `PreviousError` – typ `double`, błąd z poprzedniego kroku.

### 3.3. Kluczowe Zmienne

- `Kp`, `Ki`, `Kd` – współczynniki PID, które wpływają na reakcję systemu:
  - `Kp` – kontroluje szybkość reakcji na błąd.
  - `Ki` – eliminuje błąd ustalony.
  - `Kd` – zapobiega nadmiernemu oscylowaniu.
- `PreviousError` – przechowuje błąd z poprzedniego kroku, potrzebne do obliczenia pochodnej.
- `Integral` – akumuluje błędy w czasie, potrzebne do obliczenia członu całkującego.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### 4.1. Wzorce i Architektura

Klasa `PIDController` nie implementuje żadnego konkretnego wzorca projektowego, ale może być częścią większej architektury sterowania, np. wzorca **Strategy** lub **Template Method**.

- **Strategy**: Jeśli system ma różne typy kontrolerów (np. PID, fuzzy, LQR), to `PIDController` może być jednym z implementacji strategii.
- **Template Method**: Jeśli istnieje klasa bazowa `BaseController`, która definiuje szablon metody `Update`, to `PIDController` może implementować konkretną logikę.

Wzorzec ten pozwala na elastyczne wdrażanie różnych kontrolerów bez konieczności zmiany kodu głównego systemu.

### 4.2. Analiza Code-Level

#### 4.2.1. Zastosowanie `double` i Typów Numerycznych

Wartości `Kp`, `Ki`, `Kd` oraz `error`, `integral`, `derivative` są typu `double`, co zapewnia większą dokładność obliczeń niż `float`. W systemach sterowania, gdzie dokładność ma kluczowe znaczenie, `double` zapewnia lepszą reprezentację wartości i redukuje błędy zaokrągleń.

#### 4.2.2. Zastosowanie `dt` (Delta Time)

W systemach sterowania czas jest kluczowy. Wartość `dt` jest wykorzystywana do obliczeń różniczkowych i całkowych. W przypadku, gdy `dt` jest niezgodne z rzeczywistym czasem (np. w systemach symulacyjnych), może to prowadzić do niestabilności. W tym kodzie `dt` jest przekazywany jako parametr, co pozwala na elastyczność w ustawieniu czasu.

#### 4.2.3. Potencjalne Ryzyka i Ich Obsługa

- **Błędy zaokrągleń**: W przypadku długiego czasu działania, akumulacja błędów w `Integral` może prowadzić do "windupu" (przepełnienia). W bardziej zaawansowanych implementacjach stosuje się ograniczenie `Integral` lub tzw. "anti-windup".
- **Nieprawidłowe `dt`**: Jeśli `dt` jest zerowy lub ujemny, obliczenia pochodnej mogą wygenerować błędy lub nieskończoności. W takim przypadku można dodać walidację lub obsługę błędów.
- **Brak synchronizacji**: W systemach wielowątkowych, jeśli `Update` jest wywoływany z różnych wątków, może to prowadzić do nieprzewidywalnych wyników. W takim przypadku konieczne jest zastosowanie mechanizmów synchronizacji (np. `lock`).

#### 4.2.4. Zastosowanie `virtual` i `override`

W przypadku rozszerzania klasy `PIDController`, można zastosować `virtual` dla metod `Update` lub `Reset`. W ten sposób, jeśli system ma różne typy kontrolerów, można je nadpisać w klasach pochodnych.

#### 4.2.5. Rekomendacja: Implementacja `IDisposable`

W systemach wbudowanych, gdzie pamięć jest ograniczona, warto rozważyć dodanie interfejsu `IDisposable`, aby zwalniać zasoby, jeśli kontroler jest używany w wielu miejscach.

---

### Podsumowanie

Klasa `PIDController` reprezentuje solidną implementację kontrolera PID, który może być wykorzystywany w systemach sterowania automatycznego. Zawiera wszystkie kluczowe elementy logiki PID, a jego konstrukcja pozwala na elastyczne ustawianie parametrów i integrację z innymi komponentami systemu. W kontekście systemów wbudowanych i przemysłowych, takie podejście zapewnia stabilność i precyzję, ale wymaga również odpowiedniego zarządzania czasem i błędami numerycznymi.