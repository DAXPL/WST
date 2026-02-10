# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Plik `PIDController.cs` definiuje klasę `PIDController`, która implementuje **regulator PID (Proporcjonalno-Różniczkowo-Integralny)** — kluczowy element w systemach sterowania automatycznego. Regulator ten służy do precyzyjnego zarządzania procesami fizycznymi, takimi jak temperatura, ciśnienie, prędkość obrotowa silnika czy położenie mechanizmu. W kontekście systemu, klasa `PIDController` stanowi **komponent sterujący**, który otrzymuje wartość docelową (setpoint) oraz aktualną wartość pomiarową (process variable), a następnie generuje sygnał sterujący (output) w celu minimalizacji różnicy między nimi.

Klasa `PIDController` działa w ramach **cyklu sterowania**, który może być uruchamiany w czasie rzeczywistym (real-time) w systemach embedded lub w symulatorach. Dla przykładu, może być używana do regulacji temperatury w piecu, gdzie wartość pomiarowa to aktualna temperatura, a wartość docelowa to żądana temperatura. Regulator PID dostosowuje wartość sterującą (np. moc grzałki) w sposób dynamiczny, aby osiągnąć i utrzymać wartość docelową.

Klasa ta jest **niezależna od konkretnego systemu**, co pozwala jej być wielokrotnie wykorzystywana w różnych kontekstach. Jest to klasa **komponentowa**, która może być zainicjalizowana z różnymi parametrami PID (Kp, Ki, Kd), a także może być używana w różnych miejscach systemu, np. w sterowniku silnika, regulatorze temperatury, sterowniku ruchu robotów.

## 2. Analiza Logiki Biznesowej (Deep Dive)

### 2.1. Inicjalizacja regulatora

Klasa `PIDController` zawiera konstruktor, który przyjmuje trzy parametry: `Kp`, `Ki`, `Kd` oraz opcjonalnie `outputMin` i `outputMax`. Te wartości są przypisywane do prywatnych zmiennych klasy (`_kp`, `_ki`, `_kd`, `_outputMin`, `_outputMax`), które są wykorzystywane w dalszej części obliczeń. Parametry te określają wpływ proporcjonalnego, całkowitego i różniczkowego członu regulatora.

### 2.2. Metoda `Compute`

Główną logiką działania regulatora jest metoda `Compute`, która przyjmuje dwa argumenty: `input` (wartość pomiarowa) oraz `setpoint` (wartość docelowa). Proces działania tej metody przebiega w następujących krokach:

1. **Obliczenie błędu**:  
   `error = setpoint - input`  
   Zmienna `error` reprezentuje różnicę między wartością docelową a aktualną. W przypadku, gdy `input` jest mniejszy niż `setpoint`, błąd będzie dodatni, a gdy `input` jest większy, błąd będzie ujemny.

2. **Obliczenie całkowitego błędu (Integral)**:  
   `_integral += error * _dt`  
   Wartość `_integral` reprezentuje sumę wszystkich błędów z poprzednich kroków, przemnożonych przez czas (`_dt`). Ten składnik zapewnia, że regulator "pamięta" o długotrwałych odchylaniach i dąży do zera błędu w długim okresie.

3. **Obliczenie pochodnej błędu (Derivative)**:  
   `_derivative = (error - _previousError) / _dt`  
   `_previousError` przechowuje poprzednią wartość błędu, co pozwala obliczyć pochodną błędu w czasie. Ten składnik pomaga w redukcji drgań i zapobiega nadmiernemu reagowaniu regulatora na szybkie zmiany.

4. **Obliczenie wartości wyjściowej (Output)**:  
   `output = _kp * error + _ki * _integral + _kd * _derivative`  
   Wartość `output` to kombinacja trzech członów PID:  
   - `Kp * error` — reaguje natychmiast na błąd  
   - `Ki * _integral` — eliminuje błąd stały  
   - `Kd * _derivative` — zapobiega drganiom i przereguje system  

5. **Ograniczenie wyjścia (Output Clamping)**:  
   `output = Math.Max(_outputMin, Math.Min(_outputMax, output))`  
   Wartość `output` jest ograniczana do zakresu `[_outputMin, _outputMax]`, co zapobiega nadmiernemu sterowaniu, np. w przypadku, gdy sterowanie nie może przekroczyć pewnego poziomu (np. 0–100% mocy silnika).

6. **Aktualizacja poprzedniego błędu**:  
   `_previousError = error`  
   Zmienna `_previousError` jest uaktualniana do nowej wartości, aby móc obliczyć pochodną w kolejnym kroku.

### 2.3. Czasowy aspekt (`_dt`)

Wartość `_dt` (delta time) jest kluczowym elementem działania regulatora. Musi być ustawiona przez użytkownika w sposób odpowiedni, np. jako czas między kolejnymi wywołaniami metody `Compute`. Jeśli `_dt` jest zbyt duże, regulator może być niestabilny. Jeśli jest zbyt mały, może być zbyt wolny. Dlatego też wartość `_dt` powinna być **konsekwentnie aktualizowana** w systemie sterowania.

### 2.4. Zmienne stanu

Klasa `PIDController` zawiera trzy zmienne stanu:
- `_integral` — akumulowana wartość błędu całkowitego
- `_previousError` — poprzednia wartość błędu
- `_dt` — czas między kolejnymi krokami regulatora

Te zmienne są kluczowe dla działania regulatora PID i muszą być **prawidłowo zainicjalizowane i aktualizowane** w systemie.

## 3. Szczegóły Techniczne

### 3.1. Zależności i Dziedziczenie

Klasa `PIDController` nie dziedziczy z żadnej klasy i nie implementuje interfejsu. Jest to klasa **samodzielna**, która działa niezależnie. W kontekście systemu może być używana w systemach sterowania, gdzie nie ma potrzeby implementacji interfejsów, ale w bardziej zaawansowanych systemach można rozważyć dodanie interfejsu `IController` dla lepszej elastyczności i testowalności.

### 3.2. Przepływ Danych

#### Wejścia:
- `input`: typ `double` — wartość aktualnego stanu systemu (np. temperatura, prędkość)
- `setpoint`: typ `double` — wartość docelowa systemu
- `_dt`: typ `double` — czas między kolejnymi krokami regulatora (musi być ustawiony przez użytkownika)

#### Wyjścia:
- `output`: typ `double` — wartość sterująca systemem (np. moc grzałki, prędkość silnika)
- Metoda `Compute` zwraca `double` — wartość wyjściową regulatora PID

### 3.3. Kluczowe Zmienne

- `_kp`, `_ki`, `_kd`: współczynniki regulatora PID. Współczynniki te są ustawiane w konstruktorze i wpływają na sposób działania regulatora.
- `_integral`: akumulowana wartość błędu całkowitego. Przechowuje sumę błędów z poprzednich kroków.
- `_previousError`: przechowuje wartość błędu z poprzedniego kroku. Wykorzystywana do obliczenia pochodnej.
- `_dt`: czas między kolejnymi krokami regulatora. Wpływa na stabilność działania regulatora.
- `_outputMin`, `_outputMax`: ograniczenia wyjściowe. Zapobiegają nadmiernemu sterowaniu.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### 4.1. Wzorce i Architektura

Klasa `PIDController` nie wykorzystuje konkretnego wzorca projektowego, ale może być **rozszerzona** w kierunku wzorca **Strategy** lub **Template Method**, jeśli system będzie miał różne typy regulatorów (np. PID, PI, PD). Wzorzec Strategy pozwala na dynamiczną zmianę logiki działania regulatora w zależności od kontekstu, co jest szczególnie przydatne w systemach sterowania, gdzie różne elementy mogą wymagać różnych strategii sterowania.

### 4.2. Analiza Code-Level

#### C++/C# Syntax Constructs

- **Klasy i metody publiczne/ prywatne**:  
  W C# klasa `PIDController` ma prywatne zmienne (`_kp`, `_ki`, `_kd`, `_integral`, `_previousError`, `_dt`, `_outputMin`, `_outputMax`) oraz publiczną metodę `Compute`. Użycie `_` na początku nazw zmiennych jest konwencją C# dla prywatnych zmiennych instancyjnych.

- **Właściwości i metody**:  
  W systemach bardziej zaawansowanych można rozważyć użycie `get; set;` w celu zwiększenia elastyczności, np. dla dynamicznego ustawiania `Kp`, `Ki`, `Kd` w trakcie działania systemu.

- **Typy danych**:  
  Wartości `double` są używane w celu zapewnienia precyzji obliczeń. W systemach embedded, gdzie zasoby są ograniczone, można rozważyć użycie `float` lub `fixed-point arithmetic`, ale w kontekście aplikacji typu `PIDController` precyzja `double` jest istotna.

- **Zarządzanie pamięcią i wydajność**:  
  Klasa `PIDController` nie korzysta z dynamicznego zarządzania pamięcią (np. `new`, `delete`). Wszystkie zmienne są statyczne i alokowane na stosie, co zapewnia szybki dostęp i minimalizuje koszt zarządzania pamięcią.

#### Potencjalne zagrożenia i ich zaradzenie

- **Brak synchronizacji**:  
  Jeśli `PIDController` jest używany w systemie wielowątkowym, nie ma mechanizmów synchronizacji (np. `lock`, `Monitor`, `Mutex`). W takim przypadku może dojść do problemów z nieprawidłowym odczytem zmiennych `_integral` lub `_previousError`. W systemach embedded, które wykorzystują `RTOS`, należy rozważyć dodanie mechanizmów synchronizacji.

- **Nieprawidłowe ustawienie `_dt`**:  
  Jeśli `_dt` jest ustawiony zbyt duży lub zbyt mały, może to prowadzić do niestabilności lub wolnego reagowania regulatora. W systemach produkcyjnych warto zaimplementować walidację `_dt` lub użyć mechanizmu `Timer` do automatycznego ustawiania tego parametru.

- **Brak resetu `_integral`**:  
  W niektórych systemach warto rozważyć możliwość **resetowania `_integral`** w przypadku przekroczenia pewnego progu błędu, aby zapobiec **windupowi** – czyli sytuacji, gdy integralny człon przekracza zakres sterowania.

---

### Podsumowanie

Klasa `PIDController` to **kluczowy komponent sterowania** w systemach automatycznych. Jej implementacja jest **prosta, ale wydajna** i zapewnia stabilne działanie regulatora PID. Warto zwrócić uwagę na **zakresy wyjściowe**, **czas `_dt`**, oraz **zachowanie zmiennych stanu**. W kontekście systemów embedded, warto rozważyć dodanie mechanizmów synchronizacji i walidacji danych. W systemach wielowątkowych, należy zastosować odpowiednie mechanizmy ochrony przed nieprawidłowym dostępem do zmiennych wewnętrznych.