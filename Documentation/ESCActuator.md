# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Plik `PIDController.cs` definiuje klasę `PIDController`, która implementuje kontroler proporcjonalno-różniczkowo-integralny (PID). Kontroler ten znajduje zastosowanie w systemach sterowania automatycznego, gdzie wymagane jest precyzyjne dostosowanie wyjścia do zmieniającego się sygnału błędu. W kontekście systemu, który zarządza sterowaniem silnikiem lub innym elementem dynamicznym, ten kontroler służy do regulacji wartości wyjściowej w oparciu o różnicę między wartością docelową (setpointem) a aktualną wartością (procesem).

Klasa `PIDController` jest zaimplementowana w sposób modułowy i niezależny, co pozwala na jej użycie w różnych kontekstach sterowania — od prostych systemów do złożonych aplikacji przemysłowych. Zawiera w sobie mechanizmy do konfiguracji parametrów PID, obliczania sterowania oraz zarządzania historią błędów i czasem, co pozwala na dynamiczne dostosowanie działania kontrolera do zmieniających się warunków.

Kontroler ten może być wykorzystywany jako komponent w architekturze systemu sterowania, np. jako część modułu `MotorController`, `TemperatureController`, `PositionController` — w zależności od tego, co jest obiektem regulowanym. Właściwości klasy, takie jak `Kp`, `Ki`, `Kd`, `Setpoint`, `OutputMin`, `OutputMax`, są dostępne do ustawiania dynamicznie, co umożliwia dostosowanie działania kontrolera w czasie rzeczywistym — co jest kluczowe w systemach, gdzie warunki mogą się zmieniać.

Klasa `PIDController` nie zawiera logiki przetwarzania danych wejściowych, ale zapewnia interfejs do obliczania sterowania w oparciu o dane wejściowe. Zatem, jej cykl życia zaczyna się od utworzenia instancji, a kończy się wywołaniem metody `Compute()` z odpowiednimi danymi wejściowymi. W systemie, który korzysta z tego kontrolera, może być wielokrotnie wywoływany w pętli sterowania, np. co 10 ms, co zapewnia stabilne i precyzyjne sterowanie.

## 2. Analiza Logiki Biznesowej (Deep Dive)

### 2.1 Inicjalizacja i Ustawienia

Klasa `PIDController` zawiera konstruktor, który inicjalizuje podstawowe parametry kontrolera — `Kp`, `Ki`, `Kd`, `Setpoint`, `OutputMin`, `OutputMax`. Wartości te są przekazywane jako argumenty konstruktora, co pozwala na konfigurację kontrolera na poziomie instancji. Parametry te są kluczowe w działaniu kontrolera:

- `Kp` (współczynnik proporcjonalny) — odpowiada za natychmiastową reakcję na błąd.
- `Ki` (współczynnik całkujący) — eliminuje błąd stały.
- `Kd` (współczynnik różniczkujący) — zapobiega nadmiernemu drganiu.

Wartości te są przekazywane do prywatnych zmiennych klasy, które są używane w metodzie `Compute()`.

### 2.2 Metoda `Compute()`

Metoda `Compute()` jest centralnym punktem działania kontrolera PID. Przyjmuje dwa argumenty: `input` (aktualna wartość procesu) i `timestamp` (czas obliczenia — używany do obliczenia różniczki czasowej `dt`).

#### Krok 1: Obliczenie błędu

Błąd obliczany jest jako różnica między `Setpoint` a `input`:

```csharp
double error = setpoint - input;
```

To jest podstawowe równanie PID — kontroler reaguje na różnice między wartością docelową a aktualną.

#### Krok 2: Obliczenie członu proporcjonalnego

Człon proporcjonalny jest prosty:

```csharp
double proportional = Kp * error;
```

Wartość ta jest bezpośrednio proporcjonalna do błędu — im większy błąd, tym większa wartość członu proporcjonalnego.

#### Krok 3: Obliczenie członu całkującego

Człon całkujący akumuluje błąd w czasie:

```csharp
integral += error * dt;
double integralTerm = Ki * integral;
```

Wartość `integral` jest zmienną przechowującą sumę błędów w czasie. Wartość `dt` jest różniczką czasową — obliczana z `timestamp` poprzedniego i aktualnego wywołania `Compute()`. Celem tego członu jest eliminacja błędu stałego.

#### Krok 4: Obliczenie członu różniczkowego

Człon różniczkowy reaguje na szybkość zmiany błędu:

```csharp
double derivative = (error - lastError) / dt;
double derivativeTerm = Kd * derivative;
```

Wartość `lastError` to wartość błędu z poprzedniego wywołania `Compute()`. Wartość `dt` jest znowu różniczką czasową. Celem tego członu jest zapobieganie drganiom i przesunięciom w systemie.

#### Krok 5: Obliczenie całkowitego sterowania

Wartość sterowania obliczana jest jako suma trzech członów:

```csharp
double output = proportionalTerm + integralTerm + derivativeTerm;
```

#### Krok 6: Ograniczenie wyjścia

Wartość `output` jest ograniczana do zakresu określonego przez `OutputMin` i `OutputMax`:

```csharp
output = Math.Max(OutputMin, Math.Min(OutputMax, output));
```

To zapewnia, że sterowanie nie przekracza dopuszczalnych granic, np. nie może przekraczać maksymalnej mocy silnika.

#### Krok 7: Aktualizacja stanu kontrolera

Po obliczeniu sterowania, wartość `lastError` jest aktualizowana:

```csharp
lastError = error;
```

To zapewnia, że w kolejnym wywołaniu `Compute()` błąd różniczkowy będzie poprawnie obliczony.

### 2.3 Zmienne stanu

Klasa przechowuje kilka zmiennych stanu, które są kluczowe dla działania kontrolera:

- `integral` — akumulowana wartość błędu całkującego.
- `lastError` — błąd z poprzedniego obliczenia — potrzebny do obliczenia członu różniczkowego.
- `lastTime` — czas ostatniego wywołania `Compute()` — potrzebny do obliczenia `dt`.

### 2.4 Wpływ czasu na działanie

Kontroler PID działa w oparciu o różniczkę czasową `dt`. Wartość `dt` jest obliczana jako różnica między aktualnym i poprzednim `timestamp`. W systemach czasu rzeczywistego, wartość `dt` może się zmieniać, co wpływa na działanie członu różniczkowego. W systemach z niską częstotliwością próbkowania, człon różniczkowy może być niestabilny — dlatego często stosuje się filtrowanie lub ograniczenia `dt`.

## 3. Szczegóły Techniczne

### 3.1 Zależności i Dziedziczenie

Klasa `PIDController` nie dziedziczy z żadnej klasy — jest klasą samodzielnie zaimplementowaną. Nie korzysta z interfejsów, ale może być implementowana jako część większego systemu sterowania. W systemie, gdzie kontroler może być używany w różnych kontekstach, można rozważyć interfejs `IController`, który może wymagać implementacji metody `Compute()`.

### 3.2 Przepływ Danych

#### Wejścia

- `input` (double) — aktualna wartość procesu (np. temperatura, pozycja).
- `timestamp` (long) — czas w milisekundach (np. `DateTime.Now.Ticks`).
- `Kp`, `Ki`, `Kd`, `Setpoint`, `OutputMin`, `OutputMax` — parametry kontrolera.

#### Wyjścia

- `output` (double) — wartość sterowania obliczona przez kontroler PID.
- `integral` — wartość akumulowana dla członu całkującego.
- `lastError` — wartość błędu z poprzedniego obliczenia.

### 3.3 Kluczowe Zmienne

- `Kp`, `Ki`, `Kd` — współczynniki PID, które wpływają na reakcję kontrolera.
- `Setpoint` — wartość docelowa, do której system ma się dążyć.
- `OutputMin`, `OutputMax` — ograniczenia sterowania.
- `integral` — zmienna akumulująca błąd całkowity.
- `lastError` — błąd z poprzedniego obliczenia — potrzebny do członu różniczkowego.
- `lastTime` — czas ostatniego wywołania — potrzebny do obliczenia `dt`.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### 4.1 Wzorce i Architektura

Klasa `PIDController` nie implementuje konkretnego wzorca projektowego (np. Strategy, Observer, Factory), ale może być użyta jako komponent w architekturze sterowania, np. w wzorcu **Component** lub **Composite**. W systemie sterowania, kontroler może być częścią większego obiektu sterującego, np. `MotorController`, który zawiera kilka kontrolerów PID — jeden do regulacji prędkości, drugi do regulacji pozycji.

Wzorce takie jak **Strategy** mogą być użyte, jeśli kontroler ma być dynamicznie zmieniany — np. w zależności od warunków działania. W takim przypadku można zdefiniować interfejs `IController` i zaimplementować różne strategie (np. PID, LQR, Fuzzy), które mogą być wybierane w czasie działania aplikacji.

### 4.2 Analiza Code-Level

#### Właściwości i Zmienne

Klasa korzysta z właściwości (`properties`) do ustawiania i odczytywania parametrów kontrolera. Właściwości są zaimplementowane z użyciem `get` i `set`, co pozwala na kontrolę dostępu i potencjalne walidacje:

```csharp
public double Kp
{
    get { return kp; }
    set { kp = value; }
}
```

Wartość `kp` jest prywatną zmienną, co zapewnia enkapsulację.

#### Wartości `double` i Precyzja

Klasa korzysta z typu `double` do przechowywania wszystkich wartości, co zapewnia wysoką precyzję obliczeń. W systemach, gdzie dokładność jest kluczowa (np. sterowanie przemysłowe), typ `double` zapewnia odpowiednią dokładność.

#### Różniczka Czasowa (`dt`)

Wartość `dt` jest obliczana jako różnica między aktualnym i poprzednim `timestamp`. W systemach czasu rzeczywistego, wartość `dt` może się zmieniać — co może wpływać na stabilność kontrolera. W takich przypadkach warto rozważyć interpolację lub filtrowanie `dt`.

#### Obsługa błędów

Klasa nie zawiera jawnej obsługi błędów, ale w systemach produkcyjnych warto rozważyć:

- Walidację `dt` — jeśli `dt` jest zera lub zbyt duże, może to prowadzić do niestabilności.
- Walidację `Kp`, `Ki`, `Kd` — ustawienie ujemnych wartości może prowadzić do nieprawidłowego działania.
- Obsługę wyjątków — np. `ArgumentNullException` w przypadku `null` dla `input`.

#### Wątkowość

Klasa nie jest zaprojektowana do pracy w wielu wątkach — w systemach wielowątkowych, dostęp do `integral`, `lastError` i `lastTime` może prowadzić do problemów synchronizacyjnych. W takim przypadku konieczne jest zastosowanie mechanizmów synchronizacji (np. `lock`).

#### Wydajność

Klasa działa w czasie stałym — obliczenia są proste i nie wymagają złożonych operacji. W systemach czasu rzeczywistego, takie obliczenia są wykonywane w czasie rzeczywistym, co zapewnia odpowiednią reakcję systemu.

#### Możliwe ulepszenia

- Dodanie funkcji `Reset()` — do resetowania `integral` i `lastError`.
- Dodanie `SetTunings()` — do dynamicznej zmiany parametrów PID.
- Dodanie opcji `AntiWindup` — do zapobiegania przesyceniu członu całkującego.

---

### Podsumowanie

Klasa `PIDController` to podstawowy, ale bardzo ważny komponent w systemach sterowania automatycznego. Jej implementacja jest zarówno prosty, jak i efektywny, a jej działanie opiera się na podstawowym równaniu PID. W kontekście systemu, który wymaga stabilnego i precyzyjnego sterowania, kontroler ten może być używany w wielu aplikacjach — od prostych regulatorów do złożonych systemów przemysłowych. Warto zrozumieć, jak działa każdy człon PID oraz jak wpływają na stabilność systemu parametry kontrolera.