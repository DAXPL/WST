# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Ten plik definiuje klasę `PIDController`, która implementuje **algorytm sterowania typu PID (Proporcjonalno-Integralno-Różniczkowy)**, znany z szerokiego zastosowania w systemach automatyki i sterowaniach procesów. Klasa ta pełni rolę centralnego elementu sterującego w systemach, gdzie należy osiągnąć dokładne dopasowanie wartości wyjściowej do wartości docelowej (setpointu) przy minimalnym przebiciu i możliwie szybkim osiągnięciu ustalonego stanu ustalonego.

W kontekście architektury systemu, `PIDController` może być używany jako komponent w większym systemie sterowania, np. w sterownikach mikrokontrolerów, systemach automatyki przemysłowej, robotyce, lub systemach sterowania ruchem w samochodach (np. w systemach ESP, ABS). Może być również używany w systemach symulacyjnych lub testowych, gdzie symulowane są fizyczne procesy (np. temperatura, położenie, prędkość) i należy je stabilizować.

Klasa `PIDController` implementuje cykl życia typowy dla komponentów sterujących:
- Inicjalizacja z parametrami PID (`Kp`, `Ki`, `Kd`) oraz granicami wyjścia (`outputMin`, `outputMax`).
- Odbiór wartości aktualnej (`input`) oraz wartości docelowej (`setpoint`).
- Obliczenie sterowania `output` na podstawie błędu i historii błędu.
- Resetowanie historii błędu w przypadku konieczności (np. po przeprowadzeniu zmiany ustawień).
- Współpraca z innymi komponentami poprzez metody `Update()` i `Reset()`.

## 2. Analiza Logiki Biznesowej (Deep Dive)

### 2.1 Cykl działania `Update()`

Metoda `Update()` jest centralnym punktem działania kontrolera PID. Przebieg działania jest następujący:

1. **Obliczenie błędu**:
   ```csharp
   double error = setpoint - input;
   ```
   Błąd to różnica między wartością docelową (`setpoint`) a aktualną (`input`). W przypadku, gdy `input` przekracza `setpoint`, błąd będzie ujemny. W przeciwnym razie – dodatni.

2. **Akumulacja błędu (czynnik całkujący)**:
   ```csharp
   integral += error * dt;
   ```
   Wartość `integral` reprezentuje sumę błędów z poprzednich kroków. `dt` to czas od ostatniego wywołania `Update()`. W ten sposób czynnik `Ki` wpływa na tendencję do eliminacji błędu statycznego (tzw. błędu ustalonego).

3. **Obliczenie pochodnej błędu (czynnik różniczkujący)**:
   ```csharp
   double derivative = (error - lastError) / dt;
   ```
   Pochodna błędu reprezentuje szybkość zmiany błędu w czasie. W przypadku szybkiego zmieniającego się błędu, czynnik `Kd` zapobiega przebiciom.

4. **Obliczenie wyjścia PID**:
   ```csharp
   double output = Kp * error + Ki * integral + Kd * derivative;
   ```
   To podstawowe równanie PID. Zmienia się ono w zależności od wartości `Kp`, `Ki`, `Kd`, które są parametrami dostrajania systemu.

5. **Ograniczenie wyjścia**:
   ```csharp
   output = Math.Max(outputMin, Math.Min(outputMax, output));
   ```
   Wyjście kontrolera jest ograniczone do zakresu `[outputMin, outputMax]`, co zapobiega nadmiernemu wzbudzeniu sterownika (np. silnika, zaworu).

6. **Zapisanie błędu poprzedniego**:
   ```csharp
   lastError = error;
   ```
   Błąd z poprzedniego kroku jest zapisywany w `lastError`, aby móc obliczyć pochodną w kolejnym kroku.

7. **Zwrócenie wartości sterującej**:
   ```csharp
   return output;
   ```

### 2.2 Przepływ danych w czasie

W czasie działania `PIDController` następuje cykliczne wywoływanie metody `Update()`. Przykład:

1. **Początek działania**:
   - `input` = 100, `setpoint` = 200 → `error` = 100
   - `integral` = 0 (początkowo)
   - `lastError` = 0 (początkowo)

2. **Po pierwszym kroku**:
   - `integral` = 100 * dt
   - `derivative` = (100 - 0) / dt = 100 / dt
   - `output` = Kp * 100 + Ki * integral + Kd * derivative

3. **Po kolejnym kroku**:
   - `error` = 150 (np. input wzrósł do 150)
   - `integral` = poprzedni integral + 150 * dt
   - `derivative` = (150 - 100) / dt = 50 / dt

### 2.3 Wpływ parametrów PID

- **Kp (Proporcjonalny)**: Im większa wartość, tym szybsze reagowanie na błąd, ale także większe przebicia.
- **Ki (Całkujący)**: Eliminuje błąd ustalony, ale może powodować oscylacje, jeśli jest zbyt duży.
- **Kd (Różniczkujący)**: Zmniejsza przebicia, ale może zwiększyć czas reakcji, jeśli jest zbyt duży.

## 3. Szczegóły Techniczne

### 3.1 Zależności i Dziedziczenie

Klasa `PIDController` nie dziedziczy z żadnej klasy ani nie implementuje interfejsu. Jest to **samodzielna klasa sterująca**. W systemie może być używana w kontekście komponentów sterujących, ale nie jest narzucona przez jakikolwiek konkretny interfejs. W przypadku rozszerzania systemu, można rozważyć implementację interfejsu `IController`, który mógłby wymagać metod `Update()`, `Reset()`, `SetParameters()` itd.

### 3.2 Przepływ Danych

#### Wejścia:

- `input` – typ `double`, reprezentuje aktualną wartość zmiennej sterowanej (np. temperatura, położenie). Jednostka zależy od kontekstu (np. °C, radiany, mm).
- `setpoint` – typ `double`, reprezentuje wartość docelową. Jednostka identyczna jak `input`.
- `dt` – typ `double`, czas od ostatniego wywołania `Update()`, w sekundach. Musi być dodatni i niezerowy.

#### Wyjścia:

- `output` – typ `double`, wartość sterująca (np. wyjście PWM, napięcie sterujące, siła działania). Ograniczona do zakresu `[outputMin, outputMax]`.

### 3.3 Kluczowe Zmienne

- `Kp`, `Ki`, `Kd` – parametry PID. Wartości typu `double`, odpowiadające wagi odpowiednich członów sterowania.
- `integral` – typ `double`, akumulowana wartość błędu. Zmienia się w czasie, w zależności od `dt` i `error`.
- `lastError` – typ `double`, przechowuje błąd z poprzedniego kroku. Używany do obliczenia pochodnej.
- `outputMin`, `outputMax` – typ `double`, ograniczenia wyjścia sterującego.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### 4.1 Wzorce i Architektura

Klasa `PIDController` nie implementuje żadnego konkretnego wzorca projektowego, ale może być użyta w kontekście wzorca **Strategy** w przypadku, gdy system ma różne strategie sterowania (np. PID, fuzzy logic, LQR). W takim przypadku `PIDController` może być jednym z implementacji interfejsu `IController`.

Wzór **Observer** może być również używany, jeśli `PIDController` emituje zdarzenia przy zmianie wyjścia (np. `OnOutputChanged`), ale nie jest to obecnie zaimplementowane.

### 4.2 Analiza Code-Level

#### C++/C# Syntax Constructs

- **`double`** – typ zmiennoprzecinkowy o podwójnej precyzji. Wartość `double` zapewnia dokładność potrzebną w systemach sterowania, gdzie błędy mogą być bardzo małe.
  
- **`Math.Max()` i `Math.Min()`** – funkcje do ograniczania wartości. W kontekście PID, ograniczenie wyjścia zapobiega sytuacjom, gdzie sterowanie może przekroczyć zakres dostępnych zasobów (np. maksymalne napięcie, maksymalny kąt obrotu).

- **`virtual`** – w kontekście C#, metoda `Update()` może być oznaczona jako `virtual`, co pozwala na nadpisywanie jej w klasach pochodnych (np. w przypadku PID z ograniczeniami wejściowymi, lub PID z adaptacyjnymi parametrami).

#### Potencjalne ryzyka

- **Brak walidacji `dt`** – jeśli `dt` wynosi zero lub jest ujemne, to w obliczeniach pochodnej może wystąpić dzielenie przez zero. W praktyce powinno się sprawdzić, czy `dt > 0` i ewentualnie zwrócić błąd lub wykorzystać wartość domyślną.

- **Zmienne `integral` i `lastError` nie są zresetowane w przypadku zmiany ustawień** – w systemach dynamicznych może być konieczne resetowanie `integral` i `lastError`, jeśli zmienia się `setpoint` lub `Kp`, `Ki`, `Kd`.

- **Brak synchronizacji w środowiskach wielowątkowych** – jeśli `PIDController` jest używany w wielowątkowym środowisku, istnieje ryzyko, że `integral` i `lastError` będą modyfikowane przez różne wątki. W takim przypadku należy użyć mechanizmów synchronizacji (np. `lock`, `Interlocked`).

#### Optymalizacja

- Warto rozważyć zastosowanie `Span<double>` lub `Memory<double>` w przypadku przetwarzania wielu wartości PID w jednym kroku (np. w systemach z wieloma sterowaniami).
- W systemach czasu rzeczywistego warto rozważyć zastosowanie `fixed` w C# lub `const` w C++ dla parametrów PID, jeśli nie zmieniają się w czasie.

---

**Podsumowanie techniczne**:

Klasa `PIDController` to implementacja klasycznego regulatora PID, który działa w systemach sterowania w czasie ciągłym. Działa poprzez obliczanie proporcjonalnego, całkującego i różniczkującego członu błędu i sumowanie ich z odpowiednimi współczynnikami. Używa typu `double` do zapewnienia precyzji i ogranicza wyjście do zakresu `[outputMin, outputMax]`. Może być rozszerzona w przyszłości o dodatkowe funkcje takie jak:
- Resetowanie integralnego członu,
- Adaptacyjne ustawianie parametrów PID,
- Wsparcie dla wielu wejść/wyjść (MIMO),
- Integracja z systemem monitorowania i logowania.

To bardzo ważny element w systemach sterowania, gdzie stabilność i precyzja są kluczowe.