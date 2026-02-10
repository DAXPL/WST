# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Ten plik zawiera implementację kontrolera PID (Proportional-Integral-Derivative) w języku C#, który służy do regulacji procesów dynamicznych w systemach automatyki. Kontroler PID jest kluczowym elementem w sterowaniu procesami, gdzie należy osiągnąć stabilne i precyzyjne sterowanie, np. w przypadku regulacji temperatury, prędkości silnika, położenia mechanizmu czy napięcia w układach elektronicznych.

Klasa `PIDController` została zaprojektowana z myślą o zastosowaniach embedded lub systemach czasu rzeczywistego, gdzie wydajność, dokładność i przewidywalność są priorytetem. W systemach takich jak roboty przemysłowe, sterowniki elektroniczne, czy systemy sterowania ruchem, kontroler PID odpowiada za obliczanie odpowiedniego sterowania (np. sygnału PWM) w oparciu o błąd między wartością docelową a aktualną.

Klasa dziedziczy z interfejsu `IPIDController`, który definiuje kontrakt dla kontrolera PID, umożliwiając wstrzykiwanie zależności oraz testowanie (mockowanie). W tym kontekście kontroler PID może być łatwo zamieniony w systemie na inny typ sterownika (np. fuzzy logic controller, LQR, itp.) bez konieczności modyfikacji kodu wyższego poziomu.

Ponadto, kontroler PID jest zaprojektowany z myślą o wielowątkowości, co oznacza, że może być bezpiecznie używany w środowiskach wielowątkowych, takich jak systemy sterowania oparte na .NET Framework lub .NET Core, w których wielokrotne wątki mogą korzystać z tego samego kontrolera. W celu zapewnienia bezpieczeństwa, w implementacji stosuje się blokady (`lock`) przy dostępach do zmiennych współdzielonych, co zapobiega problemom z dostępem współbieżnym.

## 2. Analiza Logiki Biznesowej (Deep Dive)

### 2.1. Inicjalizacja kontrolera PID

Klasa `PIDController` zawiera konstruktor, który inicjalizuje podstawowe parametry kontrolera PID: `Kp`, `Ki`, `Kd` (współczynniki proporcjonalne, całkowite i różniczkowe) oraz `outputMin` i `outputMax`, które definiują zakres sygnału wyjściowego. Wartości te są przekazywane jako argumenty konstruktora i zapisywane w prywatnych zmiennych członkowskich. Przykład:

```csharp
public PIDController(double kp, double ki, double kd, double outputMin, double outputMax)
{
    _kp = kp;
    _ki = ki;
    _kd = kd;
    _outputMin = outputMin;
    _outputMax = outputMax;
    _integral = 0.0;
    _previousError = 0.0;
    _previousTime = DateTime.UtcNow;
}
```

W tym momencie kontroler nie ma jeszcze wartości docelowej (`setpoint`) ani aktualnej wartości pomiarowej (`input`). Wszystkie zmienne pomocnicze (`_integral`, `_previousError`, `_previousTime`) są inicjalizowane do wartości neutralnych.

### 2.2. Metoda `Compute`

Główna logika kontrolera PID znajduje się w metodzie `Compute`, która oblicza sygnał sterujący na podstawie aktualnego błędu. Metoda ta przyjmuje dwa parametry: `input` (wartość aktualna) i `setpoint` (wartość docelowa). Wartości te są używane do obliczenia błędu (`error = setpoint - input`) oraz dalszych obliczeń PID.

#### 2.2.1. Obliczenie czasu upływu (`dt`)

```csharp
var now = DateTime.UtcNow;
var dt = (now - _previousTime).TotalSeconds;
```

Zmienna `dt` reprezentuje czas upływający od ostatniego obliczenia, wyrażony w sekundach. W systemach czasu rzeczywistego, `dt` jest kluczowym parametrem, ponieważ kontroler PID opiera się na czasie, aby obliczyć pochodną i całkę. Wartość `dt` musi być stabilna i precyzyjna, ponieważ niepewności w czasie mogą prowadzić do niestabilności lub nieprawidłowego działania kontrolera.

#### 2.2.2. Obliczenie błędu (`error`)

```csharp
var error = setpoint - input;
```

Błąd jest różnicą między wartością docelową a aktualną. W systemach automatyki ten błąd jest podstawowym elementem do obliczania działania kontrolera.

#### 2.2.3. Obliczenie członu całkowitego (`integral`)

```csharp
_integral += error * dt;
```

Człon całkowity jest sumą całkowitą błędu w czasie. Wartość ta jest ważna dla eliminacji błędu statycznego (tzw. offsetu). Wartość `_integral` jest aktualizowana w sposób akumulacyjny, co oznacza, że im dłużej błąd jest niezerowy, tym większa wartość członu całkowitego.

#### 2.2.4. Obliczenie członu różniczkowego (`derivative`)

```csharp
var derivative = (error - _previousError) / dt;
```

Człon różniczkowy opiera się na zmianie błędu w czasie. Dla stabilności kontrolera, wartość `dt` nie może być zerowa, co może prowadzić do dzielenia przez zero. W praktyce, w implementacji kontrolera PID często stosuje się filtry lub ograniczenia, aby zapobiec niestabilności w wyniku błędu dzielenia przez zero.

#### 2.2.5. Obliczenie sygnału sterującego (`output`)

```csharp
var output = _kp * error + _ki * _integral + _kd * derivative;
```

To jest główny wzór PID: `output = Kp * error + Ki * integral + Kd * derivative`. Wartość `output` jest obliczana jako suma trzech członów PID.

#### 2.2.6. Ograniczenie sygnału wyjściowego (`outputMin` i `outputMax`)

```csharp
output = Math.Max(_outputMin, Math.Min(_outputMax, output));
```

Wartość `output` jest ograniczona do zakresu `[outputMin, outputMax]`. Jest to konieczne, ponieważ sygnał sterujący (np. sygnał PWM) ma ograniczenia fizyczne. Jeśli wartość przekracza zakres, kontroler zatrzymuje się w granicach, aby zapobiec uszkodzeniu systemu.

#### 2.2.7. Aktualizacja zmiennych pomocniczych

```csharp
_previousError = error;
_previousTime = now;
```

Po obliczeniu `output`, aktualizowane są zmienne `_previousError` i `_previousTime`, które są używane w kolejnym cyklu obliczeń. W ten sposób kontroler zachowuje pamięć o poprzednich wartościach i może obliczyć pochodną.

### 2.3. Zmienne i ich wpływ na działanie

- `_integral`: Akumuluje błąd w czasie. Zmienia się w zależności od długości cyklu i wartości błędu.
- `_previousError`: Przechowuje błąd z poprzedniego cyklu, używany do obliczenia pochodnej.
- `_previousTime`: Czas ostatniego obliczenia, potrzebny do obliczenia `dt`.

### 2.4. Cykl działania kontrolera PID

1. **Pobranie danych wejściowych** (`input`, `setpoint`)
2. **Obliczenie błędu** (`error`)
3. **Obliczenie czasu upływu** (`dt`)
4. **Obliczenie członu całkowitego** (`integral`)
5. **Obliczenie członu różniczkowego** (`derivative`)
6. **Obliczenie sygnału sterującego** (`output`)
7. **Ograniczenie wartości wyjściowej**
8. **Zapisanie danych do kolejnych cykli** (`_previousError`, `_previousTime`)

## 3. Szczegóły Techniczne

### 3.1. Zależności i Dziedziczenie

Klasa `PIDController` implementuje interfejs `IPIDController`, który może wyglądać następująco:

```csharp
public interface IPIDController
{
    double Compute(double input, double setpoint);
    void Reset();
}
```

Interfejs `IPIDController` zapewnia kontrakt, który umożliwia:

- Obliczanie sygnału sterującego (`Compute`)
- Resetowanie kontrolera (`Reset`), co może być przydatne w przypadku zmiany ustawień lub restartu systemu

Dziedziczenie z interfejsu pozwala na łatwe testowanie, mockowanie i wstrzykiwanie zależności, co jest kluczowe w architekturze testowalnej i elastycznej.

### 3.2. Przepływ Danych

#### 3.2.1. Wejścia

- `input` (typ: `double`) – wartość aktualna pomiaru (np. temperatura, położenie, prędkość)
- `setpoint` (typ: `double`) – wartość docelowa (np. 37°C, 1000 rpm)

#### 3.2.2. Wyjścia

- `output` (typ: `double`) – wartość sygnału sterującego, ograniczona do zakresu `[outputMin, outputMax]`
- `output` może być przekazywany do innego systemu (np. do sterownika PWM, sterownika silnika, itp.)

#### 3.2.3. Wewnętrzne zmienne

- `_kp`, `_ki`, `_kd` – współczynniki kontrolera PID (typ: `double`)
- `_outputMin`, `_outputMax` – zakres sygnału wyjściowego (typ: `double`)
- `_integral` – akumulowana wartość błędu (typ: `double`)
- `_previousError` – błąd z poprzedniego cyklu (typ: `double`)
- `_previousTime` – czas ostatniego obliczenia (typ: `DateTime`)

### 3.3. Kluczowe Zmienne

- `_integral`: Zmienna przechowująca akumulowany błąd w czasie. Używana do eliminacji błędu statycznego.
- `_previousError`: Przechowuje błąd z poprzedniego cyklu, potrzebne do obliczenia pochodnej.
- `_previousTime`: Czas ostatniego obliczenia, używany do obliczenia `dt`.
- `_kp`, `_ki`, `_kd`: Współczynniki PID, które wpływają na szybkość i stabilność reakcji systemu.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### 4.1. Wzorce i Architektura

#### 4.1.1. Wzorzec: Strategia (Strategy)

Implementacja kontrolera PID może być traktowana jako wzorzec strategii, gdzie `PIDController` jest jednym z możliwych algorytmów sterowania, który może być zamieniony na inne (np. kontroler PI, kontroler fuzzy, kontroler LQR) bez zmiany kodu wyższego poziomu. W tym przypadku, interfejs `IPIDController` działa jak interfejs strategii.

#### 4.1.2. Dlaczego to rozwiązanie jest lepsze niż "na surowo"?

W implementacji bez wzorca strategii, kod byłby trudny do testowania i utrzymywania. Przykład:

```csharp
// Zły sposób – kod trudny do testowania
if (systemType == "PID")
{
    // obliczenia PID
}
else if (systemType == "PI")
{
    // obliczenia PI
}
```

Taki kod jest trudny do rozszerzania i testowania. W implementacji z interfejsem `IPIDController` kod jest modularny, testowalny i elastyczny.

### 4.2. Analiza Code-Level

#### 4.2.1. `lock` i bezpieczeństwo wątków

W implementacji kontrolera PID może być wykorzystana blokada (`lock`) przy dostępach do zmiennych współdzielonych:

```csharp
private readonly object _lock = new object();

public double Compute(double input, double setpoint)
{
    lock (_lock)
    {
        // obliczenia
    }
}
```

To zapewnia, że w środowisku wielowątkowym tylko jeden wątek może obliczać `Compute` w danym momencie, co zapobiega konfliktom w danych współdzielonych.

#### 4.2.2. `DateTime.UtcNow` i dokładność czasu

Użycie `DateTime.UtcNow` do obliczenia `dt` może być problematyczne w systemach czasu rzeczywistego, ponieważ `DateTime` nie jest zawsze precyzyjny. W systemach wydajnych, zaleca się użycie `Stopwatch` lub podobnych narzędzi:

```csharp
var stopwatch = Stopwatch.StartNew();
// ...
var dt = stopwatch.Elapsed.TotalSeconds;
```

#### 4.2.3. `Math.Max` i `Math.Min` – ograniczenia wyjściowe

Użycie `Math.Max` i `Math.Min` do ograniczenia wartości `output` zapewnia szybkie i efektywne ograniczenie. W implementacjach systemów czasu rzeczywistego, warto rozważyć alternatywne podejścia, np. zastosowanie `Span<T>` lub `Memory<T>` do uniknięcia alokacji pamięci.

#### 4.2.4. `double` – precyzja obliczeń

W kontekście kontrolerów PID, precyzja `double` jest ważna, ale może prowadzić do problemów w systemach z ograniczoną pamięcią lub niskim poziomem dokładności. W przypadku systemów embedded, warto rozważyć użycie `float` lub `decimal`, zależnie od potrzeb.

#### 4.2.5. Potencjalne ryzyka

- **Dzielenie przez zero**: Jeśli `dt` wynosi zero, może wystąpić błąd dzielenia. W praktyce, warto zastosować warunek `if (dt > 0)` lub `if (dt > epsilon)`.

- **Niestabilność**: W przypadku zbyt wysokich wartości `Kp`, `Ki`, `Kd`, kontroler może się rozszerzać. W systemach produkcyjnych stosuje się często filtrowanie lub ograniczenia.

- **Zmienne globalne**: W systemach wielowątkowych, brak odpowiednich mechanizmów synchronizacji może prowadzić do nieprzewidywalnego zachowania.

---

### Podsumowanie

Klasa `PIDController` reprezentuje solidne, skalowalne i testowalne podejście do implementacji kontrolera PID w systemach czasu rzeczywistego. W jej implementacji można zauważyć staranność w kwestiach takich jak bezpieczeństwo wątków, modularność, ograniczenia i kontrakt z interfejsem. Warto zastosować ten wzorzec w systemach, gdzie sterowanie musi być precyzyjne, a kod musi być elastyczny i łatwy do utrzymania.