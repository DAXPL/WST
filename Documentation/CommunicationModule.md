# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Plik `PIDController.cs` definiuje klasę `PIDController`, która implementuje regulator PID (Proporcjonalno-Integralno-Pochodny) w kontekście systemów sterowania automatycznego. Regulator ten służy do precyzyjnego zarządzania procesami fizycznymi, np. regulacja temperatury, prędkości silnika, położenia mechanizmu, czy innych wielkości dynamicznych, które wymagają ciągłego dostosowywania sterowania w oparciu o błąd między wartością docelową a aktualną.

Klasa `PIDController` została zaprojektowana jako komponent wielokrotnego użytku, który może być używany w różnych częściach systemu sterowania. W kontekście architektury systemowej, może być używana jako komponent w systemie sterowania robotów, sterowniku procesu przemysłowego, lub w systemach embedded, gdzie wymagana jest precyzyjna kontrola dynamiczna. Regulator PID jest elementem kluczowym w systemach sterowania, ponieważ umożliwia szybkie i stabilne reagowanie na odchylania się wartości sterowanej od wartości zadanej.

W kontekście cyklu życia obiektu `PIDController`, klasa może być inicjalizowana z parametrami regulatora (Kp, Ki, Kd) oraz zdefiniowaną wartością docelową (setpoint). Następnie, w każdej iteracji (np. w pętli sterującej), obiekt otrzymuje aktualną wartość pomiarową (process variable) i na tej podstawie oblicza odpowiednią wartość sterującą (output), która może być np. przekazywana do silnika lub innego elementu sterowanego.

## 2. Analiza Logiki Biznesowej (Deep Dive)

### Początkowa Inicjalizacja Regulatora

Obiekt `PIDController` jest inicjalizowany z trzema parametrami: `Kp`, `Ki`, `Kd`, które są współczynnikami proporcjonalnym, całkowitym i różniczkowym. Dodatkowo, obiekt może mieć zdefiniowaną wartość docelową (`Setpoint`) oraz zakresy wyjściowe (`MinOutput`, `MaxOutput`) dla sterowania.

### Metoda `Compute()`

Główną logiką działania regulatora PID jest metoda `Compute()`. W tym miejscu następuje obliczenie nowej wartości sterującej (output) na podstawie bieżącego błędu.

#### Krok po kroku:

1. **Obliczenie błędu (Error):**
   ```csharp
   double error = setpoint - inputValue;
   ```
   Błąd jest różnicą między wartością docelową (`setpoint`) a aktualną wartością pomiarową (`inputValue`). Wartość ta może być zarówno dodatnia, jak i ujemna, co oznacza, że system jest za wysoki lub za niski w porównaniu do zadania.

2. **Obliczenie całkowitego błędu (Integral Term):**
   ```csharp
   integral += error * dt;
   ```
   Wartość `integral` przechowuje sumę błędów z poprzednich iteracji. Zmiana ta jest ważna, ponieważ pozwala regulatorowi „pamiętać” o długotrwałych odchylaniach i zniechęcać do stabilizacji w kierunku wartości docelowej.

3. **Obliczenie pochodnej błędu (Derivative Term):**
   ```csharp
   double derivative = (error - lastError) / dt;
   ```
   Pochodna błędu pokazuje, jak szybko błąd zmienia się w czasie. Używana jest do "przewidywania" przyszłych zmian i zapobiegania nadmiernemu oscylowaniu.

4. **Obliczenie wartości sterującej (Output):**
   ```csharp
   double output = Kp * error + Ki * integral + Kd * derivative;
   ```
   To jest kluczowe równanie regulatora PID, gdzie:
   - `Kp * error` odpowiada za natychmiastową reakcję na błąd,
   - `Ki * integral` eliminuje błąd stały (offset),
   - `Kd * derivative` zapobiega nadmiernemu oscylowaniu.

5. **Ograniczenie wartości wyjściowej:**
   ```csharp
   output = Math.Max(minOutput, Math.Min(maxOutput, output));
   ```
   Zapobiega to przekroczeniu zakresu sterowania, co może prowadzić do uszkodzenia sterowanego obiektu.

6. **Zapisanie błędu poprzedniego:**
   ```csharp
   lastError = error;
   ```
   Przechowuje błąd z poprzedniej iteracji, co jest konieczne do obliczenia pochodnej.

### Czasowy Wpływ (dt)

Wartość `dt` (delta time) jest kluczowa dla poprawnego działania regulatora. Musi być dostarczana z zewnątrz, np. z pętli sterującej, i odpowiada za czas upływu między dwoma kolejnymi obliczeniami. W przypadku nieprawidłowego `dt` (np. zbyt duże lub zbyt małe), regulator może być niestabilny lub nieefektywny.

## 3. Szczegóły Techniczne

### Zależności i Dziedziczenie

Klasa `PIDController` nie dziedziczy z żadnej klasy bazowej ani nie implementuje interfejsu. W tym kontekście, może to być świadome decyzje architektoniczne, aby uniknąć niepotrzebnych abstrakcji, jeśli regulator ma być używany w kontekście prostych systemów. Jeśli system byłby bardziej złożony, można rozważyć implementację interfejsu `IController`, który zapewniałby kontrakt dla różnych typów regulatorów.

### Przepływ Danych

#### Wejścia:

- `inputValue`: typ `double`, reprezentuje aktualną wartość pomiarową (np. temperatura, prędkość).
- `setpoint`: typ `double`, wartość docelowa, do której regulator ma doprowadzić system.
- `dt`: typ `double`, czas upływający między dwoma kolejnymi iteracjami regulatora (w sekundach).
- `Kp`, `Ki`, `Kd`: typ `double`, współczynniki regulatora PID.
- `minOutput`, `maxOutput`: typ `double`, ograniczenia wyjściowe regulatora.

#### Wyjścia:

- `output`: typ `double`, wartość sterująca, która może być np. przekazywana do silnika, zaworu, czy innego elementu sterowanego.
- `integral`, `derivative`: typ `double`, wartości pomocnicze, nie są bezpośrednio zwracane, ale są wykorzystywane w obliczeniach.

### Kluczowe Zmienne

- `integral`: przechowuje zsumowany błąd z poprzednich iteracji. Jest kluczowy dla działania członu całkowitego regulatora PID.
- `lastError`: przechowuje błąd z poprzedniej iteracji. Jest używany do obliczenia pochodnej.
- `Kp`, `Ki`, `Kd`: współczynniki regulatora, które wpływają na reakcję regulatora na błąd, jego stabilność oraz nadmiarowość.
- `setpoint`: wartość docelowa, która jest porównywana z aktualną wartością.
- `minOutput`, `maxOutput`: ograniczenia wyjściowe, zapewniające bezpieczeństwo działania regulatora.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### Wzorce i Architektura

Klasa `PIDController` nie implementuje jawnie żadnego wzorca projektowego, ale może być rozszerzona do wzorca Strategy, jeśli mamy różne typy regulatorów PID (np. z różnymi metodami obliczania pochodnej lub całki). Wzorzec Strategy pozwala na dynamiczne wybieranie różnych algorytmów sterowania w czasie działania aplikacji, co jest szczególnie przydatne w systemach, gdzie różne warunki pracy wymagają różnych podejść.

W kontekście architektury systemowej, `PIDController` może być częścią większej struktury sterującej, np. `ControllerManager`, który zarządza wieloma regulatorami PID i odpowiada za ich konfigurację oraz synchronizację.

### Analiza Code-Level

#### `double error = setpoint - inputValue;`

To podstawowe obliczenie błędu. W C# operacje arytmetyczne są bezpieczne, ale w kontekście systemów embedded warto zwrócić uwagę na typy danych i ewentualne zaokrąglenia. W przypadku zastosowania liczb zmiennoprzecinkowych typu `float`, mogą wystąpić błędy zaokrągleń, które mogą się kumulować w czasie.

#### `integral += error * dt;`

To kluczowe obliczenie członu całkowitego. W kontekście numerycznej integracji, `dt` musi być stały lub przynajmniej stabilny, aby uniknąć problemów z konwersją numeryczną. Jeśli `dt` jest zmienny, może to prowadzić do niestabilności. W bardziej zaawansowanych implementacjach, warto rozważyć użycie "anti-windup" — ograniczenia całki, aby uniknąć jej przesycenia.

#### `double derivative = (error - lastError) / dt;`

Obliczenie pochodnej może być problematyczne w kontekście szumów. Jeśli `lastError` i `error` są zbliżone, a `dt` jest małe, pochodna może zostać przeskalowana i zniekształcona. W praktyce stosuje się filtry lub uśrednianie pochodnej, aby zminimalizować wpływ szumów.

#### `output = Math.Max(minOutput, Math.Min(maxOutput, output));`

To zastosowanie ograniczenia wyjściowego. W kontekście systemów embedded, takie ograniczenia są niezbędne, aby zapobiec uszkodzeniu sterowanego elementu. `Math.Max` i `Math.Min` są bezpieczne w C#, ale w kontekście systemów czasu rzeczywistego warto zastanowić się nad implementacją bez użycia `Math`, np. z użyciem prostych porównań, aby uniknąć dodatkowego narzutu.

#### `lastError = error;`

Zachowanie poprzedniego błędu jest kluczowe dla działania pochodnej. W kontekście wielu systemów, może to być źródło problemów, jeśli nie ma się pewności, że `error` jest zawsze ustawiane. Warto zastosować sprawdzenie warunku `lastError` jako `double.NaN` lub `double.PositiveInfinity`, aby uniknąć nieprzewidywalnych wyników.

---

W kontekście systemów czasu rzeczywistego i embedded, `PIDController` może być poddawany optymalizacjom, np. w celu ograniczenia zużycia pamięci (np. unikanie alokacji w `Compute`) oraz w celu zapewnienia stabilności obliczeń. Warto również rozważyć implementację `PIDController` w C++ z użyciem `constexpr` i `noexcept`, aby zapewnić wydajność i bezpieczeństwo w systemach wbudowanych.