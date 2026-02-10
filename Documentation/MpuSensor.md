# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Ten plik zawiera implementację **sterownika PID (Proporcjonalno-Różniczkowo-Integralnego)**, który pełni kluczową rolę w systemach automatyki i sterowania procesami dynamicznymi. Sterownik PID jest stosowany w wielu zastosowaniach przemysłowych, takich jak regulacja temperatury, ciśnienia, prędkości silników, czy położenia mechanicznego. W kontekście systemu, który ten plik opisuje, sterownik PID służy do automatycznego dostosowywania wartości sterujących (np. mocy silnika, ustawień zaworu) w oparciu o bieżące wartości pomiarowe i wartość zadana (setpoint).

Klasa `PIDController` jest częścią większego systemu sterowania, w którym obiekty tego typu mogą być używane do realizacji złożonych algorytmów regulacji. Zawiera ona logikę obliczeniową oraz zarządzanie stanem sterownika PID, co czyni ją elementem centralnym w systemie automatycznego sterowania. W zależności od konkretnego zastosowania, może być używana w systemach wbudowanych, robotyce, czy systemach sterowania przemysłowego.

Ponadto, klasa jest zaprojektowana w taki sposób, aby być **modułowym komponentem**, który można łatwo wstrzykiwać do innych części systemu, co pozwala na skalowalność i łatwe testowanie. Obiekt `PIDController` może być konstruowany z konkretnymi parametrami (Kp, Ki, Kd) oraz zdefiniowanymi granicami wyjścia, co pozwala na precyzyjne dopasowanie działania do konkretnego procesu.

## 2. Analiza Logiki Biznesowej (Deep Dive)

Zachowanie klasy `PIDController` opiera się na zasadzie działania sterownika PID, który oblicza wartość sterującą w oparciu o różnicę między wartością zadana a aktualną (błędem), oraz na podstawie trzech składowych: proporcjonalnej, całkowitej i różniczkowej.

### 2.1. Inicjalizacja i Konfiguracja

W konstruktorze `PIDController` przekazywane są trzy parametry: `Kp`, `Ki` i `Kd` – odpowiednio współczynniki proporcjonalny, całkowity i różniczkowy. Wartości te są zapisywane do prywatnych zmiennych klasy i wykorzystywane w dalszych obliczeniach. Dodatkowo, definiowane są granice wyjścia (`minOutput`, `maxOutput`), które zapobiegają nadmiernemu wzbudzeniu systemu, np. w przypadku saturacji sterowania.

### 2.2. Metoda `Update`

Główną logiką sterownika PID jest metoda `Update`, która przyjmuje dwa argumenty: `setpoint` (wartość zadana) oraz `currentValue` (aktualna wartość pomiarowa). Proces obliczeniowy przebiega w następujący sposób:

1. **Obliczenie błędu (error):**
   ```csharp
   double error = setpoint - currentValue;
   ```

2. **Obliczenie całkowitej wartości błędu (integral):**
   ```csharp
   integral += error * deltaTime;
   ```

   Wartość `integral` jest akumulowana w czasie, co pozwala na eliminację błędu statycznego. Zastosowanie `deltaTime` zapewnia poprawność obliczeń w przypadku zmiennej częstotliwości wywołań.

3. **Obliczenie pochodnej błędu (derivative):**
   ```csharp
   double derivative = (error - previousError) / deltaTime;
   ```

   Wartość ta reprezentuje szybkość zmiany błędu i pozwala na zapobieganie nadmiernemu wzbudzeniu systemu (damping).

4. **Obliczenie wartości wyjściowej (output):**
   ```csharp
   double output = Kp * error + Ki * integral + Kd * derivative;
   ```

   Wartość `output` jest następnie ograniczona do zakresu `[minOutput, maxOutput]`:
   ```csharp
   output = Math.Max(minOutput, Math.Min(maxOutput, output));
   ```

5. **Aktualizacja poprzedniego błędu:**
   ```csharp
   previousError = error;
   ```

   Wartość `previousError` jest konieczna do obliczenia pochodnej w kolejnym kroku.

### 2.3. Przepływ danych i zmiany stanu

W trakcie działania metody `Update` następuje:
- **Zmiana wartości `integral`**: Zwiększa się z każdym wywołaniem, co oznacza, że system "pamięta" o błędzie w przeszłości.
- **Zmiana wartości `previousError`**: Zostaje zapisana aktualna wartość błędu, co umożliwia obliczenie pochodnej.
- **Zmiana wartości `output`**: Jest to wynik końcowy sterowania, który może być przekazywany dalej do systemu sterującego (np. do silnika).

### 2.4. Algorytm PID – Matematyczne podstawy

Obliczenia oparte są na klasycznym wzorze PID:
$$
u(t) = K_p e(t) + K_i \int e(t) dt + K_d \frac{de(t)}{dt}
$$
gdzie:
- $ u(t) $ to wartość sterująca,
- $ e(t) $ to błąd,
- $ K_p $, $ K_i $, $ K_d $ to współczynniki proporcjonalny, całkowity i różniczkowy.

W implementacji:
- **Proporcjonalna** część reaguje natychmiastowo na błąd.
- **Całkowita** część eliminuje błąd statyczny.
- **Różniczkowa** część zapobiega drganiom i nadmiernemu wzbudzeniu.

## 3. Szczegóły Techniczne

### 3.1. Zależności i Dziedziczenie

Klasa `PIDController` nie dziedziczy z żadnej klasy bazowej ani nie implementuje interfejsów. Jest to klasa samodzielna, zaprojektowana z myślą o reużywalności i niezależności. W przypadku potrzeby rozszerzenia, można rozważyć wprowadzenie interfejsu `IPIDController`, który zapewni wspólny kontrakt dla różnych implementacji sterownika PID.

### 3.2. Przepływ Danych

#### Wejścia:
- `setpoint`: typ `double`, reprezentuje wartość zadana (np. temperatura docelowa).
- `currentValue`: typ `double`, reprezentuje aktualną wartość pomiarową (np. temperatura z czujnika).
- `deltaTime`: typ `double`, czas od ostatniego wywołania `Update`, wyrażony w sekundach.

#### Wyjścia:
- `output`: typ `double`, wartość sterująca, ograniczona do zakresu `[minOutput, maxOutput]`.
- Wartość ta może być przekazywana do systemu sterującego, np. do sterownika silnika, regulatora temperatury, czy innego urządzenia.

### 3.3. Kluczowe Zmienne

- `Kp`, `Ki`, `Kd`: współczynniki PID, które wpływają na reakcję systemu.
- `integral`: akumulowana wartość błędu całkowitego, używana do eliminacji błędu statycznego.
- `previousError`: poprzednia wartość błędu, potrzebna do obliczenia pochodnej.
- `minOutput`, `maxOutput`: granice wyjścia sterowania, zapobiegają saturacji.
- `deltaTime`: czas między kolejnymi wywołaniami, potrzebny do poprawnego obliczenia całki i pochodnej.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### 4.1. Wzorce i Architektura

Klasa `PIDController` nie implementuje konkretnego wzorca projektowego (np. Strategy, Factory), ale może być łatwo rozszerzona o taki wzorzec w większym systemie. Przykładem może być implementacja wzorca Strategy, gdzie różne typy sterowników PID (np. z różnymi metodami obliczania pochodnej) mogą być wstrzykiwane w zależności od potrzeb.

Wzorzec **Strategy** może być użyty, jeśli mamy kilka różnych algorytmów PID (np. z adaptacyjnymi parametrami, z użyciem filtra, z obniżeniem szumu). W takim przypadku, `PIDController` może implementować interfejs `IPIDStrategy`, a konkretne implementacje (np. `StandardPIDStrategy`, `FilteredPIDStrategy`) mogą być przekazywane do konstruktora.

### 4.2. Analiza Code-Level

#### `virtual` i `override` – brak w tej klasie

Klasa nie zawiera metod `virtual` ani `override`, co oznacza, że nie jest zaprojektowana do dziedziczenia. W przypadku potrzeby rozszerzenia, można rozważyć wprowadzenie metod wirtualnych, np. `virtual double ComputeOutput()` lub `virtual void Reset()`.

#### `Math.Max` i `Math.Min` – bezpieczne ograniczenia

Użycie `Math.Max` i `Math.Min` do ograniczenia wartości wyjściowej zapewnia, że sterownik nie przekroczy dopuszczalnych granic. Jest to ważna praktyka w systemach wbudowanych, gdzie przekroczenie zakresu może prowadzić do niestabilności lub uszkodzenia sprzętu.

#### `double` – precyzja i potencjalne problemy

Użycie typu `double` zapewnia wysoką precyzję obliczeń, ale może być nadmiarowe w systemach z ograniczoną pamięcią (np. mikrokontrolery). W takich przypadkach można rozważyć użycie `float` lub typów całkowitych z odpowiednim skalowaniem.

#### `deltaTime` – czas zmienny i stabilność

Wartość `deltaTime` może być zmienna, co może prowadzić do niestabilności obliczeń. W praktyce warto zastosować:
- **Filtrowanie** wartości `deltaTime`, np. użycie średniej krocznej.
- **Sprawdzanie zakresu**: jeśli `deltaTime` przekracza np. 1 sekundę, można zatrzymać obliczenia lub zastosować domyślną wartość.

#### Brak mechanizmu resetu

W tej implementacji nie ma metody `Reset()`, która resetowałaby `integral` i `previousError`. W systemach dynamicznych, np. w przypadku zmiany ustawień lub restartu systemu, może być konieczne zresetowanie stanu sterownika. Wprowadzenie takiej metody pozwala uniknąć "pamięci" błędów z poprzednich cykli.

#### Potencjalne zagrożenia

- **Błąd w `deltaTime`**: Jeśli `deltaTime` wynosi 0 lub jest bardzo małe, może to prowadzić do dzielenia przez zero lub niestabilności.
- **Zbyt duże `Ki`**: Może prowadzić do oscylacji i niestabilności systemu.
- **Zbyt duże `Kd`**: Może wpływać na szum w danych, jeśli nie jest stosowany filtr.

---

### Podsumowanie

Klasa `PIDController` jest solidnym i przemysłowym implementacją sterownika PID, który może być łatwo zintegrowany w systemach automatyki. Jej konstrukcja pozwala na precyzyjne sterowanie, a przepływ danych i logika obliczeniowa są dobrze zrozumiałe. W kontekście rozwoju systemu, warto rozważyć dodanie mechanizmów takich jak:
- `Reset()`,
- Filtrowanie `deltaTime`,
- Interfejsy do rozszerzalności,
- Obsługę błędów i walidację danych.

Wartość `PIDController` jako komponentu centralnego w systemie sterowania pozwala na łatwe testowanie, skalowanie i debugowanie, co czyni ją idealnym wyborem do zastosowań przemysłowych i robotycznych.