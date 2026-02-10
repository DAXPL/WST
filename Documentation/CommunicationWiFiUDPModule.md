# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Ten plik zawiera implementację kontrolera typu PID (Proporcjonalno-Różniczkowo-Integralny), który jest szeroko stosowany w systemach sterowania, szczególnie w kontekście automatyzacji procesów przemysłowych, robotyki oraz sterowania dynamicznym. W kontekście systemu, w którym znajduje się ten plik, kontroler PID może być używany do regulacji temperatury, prędkości obrotowej silnika, położenia mechanizmu lub innych wielkości fizycznych, które wymagają stabilnego i precyzyjnego sterowania.

Klasa `PIDController` jest zaprojektowana jako komponent niezależny, który może być używany w różnych częściach systemu, zarówno w środowiskach embedded, jak i w systemach komputerowych. Zawiera w sobie pełną logikę obliczeniową kontrolera PID, a także mechanizmy do konfiguracji parametrów, ustawiania wartości docelowych (setpoint) oraz do pobierania aktualnych wyników sterowania. W systemie architektonicznym, klasa ta może być częścią większego modułu sterującego, np. w strukturze mikrokontrolerowej lub w systemie sterowania typu "controller" w architekturze mikroserwisowej.

W kontekście cyklu życia obiektu, `PIDController` jest inicjalizowany z parametrami PID (Kp, Ki, Kd), a następnie może być używany wielokrotnie w cyklu sterowania, np. w pętli głównej systemu, gdzie otrzymuje dane wejściowe, oblicza odpowiedź PID i zwraca wartość sterującą. Obiekt ten nie zarządza pamięcią w sposób automatyczny, lecz może być zarządzany przez inny mechanizm (np. przez `using` w C# lub przez system zarządzania pamięcią w C++), co oznacza, że jego cykl życia jest kontrolowany przez kod wywołujący.

## 2. Analiza Logiki Biznesowej (Deep Dive)

Proces działania kontrolera PID opiera się na trzech składnikach:

1. **Wartość proporcjonalna (P)** – opiera się na różnicy między wartością docelową (setpoint) a aktualną wartością (process variable). Im większa różnica, tym większa odpowiedź.
2. **Wartość całkowita (I)** – akumuluje błędy z poprzednich kroków, co pozwala na eliminację błędu ustalonego.
3. **Wartość różniczkowa (D)** – reaguje na zmianę błędu w czasie, co pozwala na redukcję nadmiarowego oscylowania.

### Przepływ danych i obliczenia:

W metodzie `Update(double error)` obliczana jest wartość sterująca PID:

1. **Obliczenie członu proporcjonalnego (P):**
   $$
   P = K_p \cdot e(t)
   $$
   gdzie $ K_p $ to współczynnik proporcjonalny, a $ e(t) $ to błąd (różnica między setpoint a aktualną wartością).

2. **Obliczenie członu całkowitego (I):**
   $$
   I = I_{\text{prev}} + K_i \cdot e(t) \cdot \Delta t
   $$
   Człon całkowity akumuluje błędy z poprzednich kroków, co pozwala na eliminację błędu ustalonego. Wartość $ \Delta t $ to czas między kolejnymi krokami.

3. **Obliczenie członu różniczkowego (D):**
   $$
   D = K_d \cdot \frac{d e(t)}{d t}
   $$
   W praktyce, jako że nie mamy dostępu do pochodnej analitycznej, oblicza się ją jako różnicę między aktualnym i poprzednim błędem podzieloną przez czas:
   $$
   D = K_d \cdot \frac{e(t) - e(t-1)}{\Delta t}
   $$

4. **Wartość sterująca:**
   $$
   u(t) = P + I + D
   $$

W metodzie `Update` obliczane są wszystkie te składniki i zwracana jest wartość wynikowa. Wartości `I_prev` i `e_prev` są przechowywane w zmiennej klasy, co pozwala na kontynuację obliczeń w kolejnych krokach.

W przypadku przekroczenia zakresu sterowania (np. przekroczenia maksymalnej wartości wyjściowej), wykonywane są operacje ograniczające wynik do dopuszczalnego zakresu.

### Właściwości sterujące:

W klasie zdefiniowane są również właściwości `MinOutput` i `MaxOutput`, które są używane do ograniczania zakresu wartości sterujących. Ograniczenie to zapobiega sytuacjom, w których sterowanie może przekroczyć dopuszczalne granice, np. w przypadku przekroczenia maksymalnego napięcia lub mocy.

## 3. Szczegóły Techniczne

### Zależności i Dziedziczenie

Klasa `PIDController` nie dziedziczy z żadnej klasy w tym pliku. Jest to klasa samodzielna, zaprojektowana zgodnie z zasadami enkapsulacji i niezależności. W przypadku potrzeby rozszerzenia funkcjonalności, można rozważyć dziedziczenie lub implementację interfejsów, np. `IController`, które mogłyby wymagać implementacji metod takich jak `Update`, `Reset`, `SetSetpoint`.

### Przepływ Danych

#### Wejścia:
- `double error` – parametr wejściowy metody `Update`, reprezentujący różnicę między wartością docelową a aktualną (setpoint - process variable). Jednostka zależy od kontekstu, ale zazwyczaj jest to wartość bezwzględna, np. stopnie Celsjusza, obroty na minutę, itp.
- `double dt` – czas między kolejnymi krokami sterowania (delta time). Zależnie od implementacji, może być przekazywany jako osobny parametr lub obliczany wewnętrznie.

#### Wyjścia:
- `double` – wartość sterująca, wynik obliczeń PID. Może być dalej przekazywana do urządzenia sterującego, np. do silnika, zaworu, itp.

### Kluczowe Zmienne

- `Kp`, `Ki`, `Kd` – współczynniki PID. Są ustawiane w konstruktorze lub poprzez właściwości i wpływają na odpowiedź systemu.
- `integral` – zmienna przechowująca akumulowaną wartość członu całkowitego. Jest aktualizowana w każdej iteracji.
- `previousError` – przechowuje poprzedni błąd, potrzebny do obliczenia członu różniczkowego.
- `minOutput`, `maxOutput` – ograniczenia wartości wyjściowej, zapobiegające nadmiernemu sterowaniu.
- `previousTime` – czas ostatniego obliczenia, używany do obliczenia `dt` w przypadku braku przekazania czasu.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### Wzorce i Architektura

W tym przypadku nie stosuje się jawnie żadnego konkretnego wzorca projektowego, ale implementacja kontrolera PID jest zgodna z zasadami **modularności** i **reusable component**. Jeśli rozważyć rozszerzenie tego kodu, można zastosować wzorzec **Strategy**, np. dla różnych typów kontrolerów (np. PID, PI, PD). W takim przypadku, `PIDController` mógłby implementować interfejs `IController`, który miałby metodę `Compute(double error)`.

Zastosowanie takiego wzorca pozwala na łatwe zamienianie kontrolerów w zależności od potrzeb, np. w systemach, gdzie kontroler może być dynamicznie przełączany w zależności od warunków pracy.

### Analiza Code-Level

#### Właściwości (`Properties`)

W C# wersja `PIDController` wykorzystuje właściwości (`get; set;`) do kontroli dostępu do parametrów PID. To pozwala na ustawianie wartości w czasie działania programu bez konieczności tworzenia nowego obiektu.

#### `private double integral`, `private double previousError`

Zmienne te są przechowywane w obiekcie i nie są resetowane automatycznie. To oznacza, że w przypadku resetowania kontrolera (np. po zmianie ustawień), konieczne jest ręczne zresetowanie tych wartości. W kodzie nie ma mechanizmu resetu, co może prowadzić do błędów w przypadku dynamicznej zmiany ustawień.

#### `public double Update(double error)`

Metoda `Update` nie jest `virtual`, co oznacza, że nie można jej nadpisać w klasach dziedziczących. W przypadku potrzeby modyfikacji logiki obliczeniowej, konieczne będzie stworzenie nowej klasy, która implementuje nową logikę.

#### `public void Reset()`

Metoda `Reset()` jest zaimplementowana w taki sposób, że resetuje zarówno `integral` jak i `previousError`. Jest to kluczowe, ponieważ bez resetowania tych wartości kontroler może nadal „pamiętać” poprzednie błędy, co może prowadzić do niestabilności lub nieprawidłowego działania po zmianie ustawień.

#### `private double previousTime`

Zmienna `previousTime` jest używana do obliczania `dt` w przypadku, gdy nie został przekazany jako parametr. To pozwala na elastyczność, ale może prowadzić do problemów, jeśli `Update` nie jest wywoływany regularnie. W systemach czasu rzeczywistego, gdzie dokładność czasu jest istotna, warto rozważyć przekazywanie `dt` jako parametru.

#### Potencjalne ryzyka

1. **Brak synchronizacji w środowiskach wielowątkowych:** Jeśli `PIDController` będzie używany w wielowątkowym środowisku, może wystąpić problem z dostępem do zmiennych `integral`, `previousError` i `previousTime`. W takim przypadku konieczne będzie zastosowanie mechanizmów synchronizacji, np. `lock` w C#.
2. **Brak walidacji danych wejściowych:** Brak walidacji parametrów `Kp`, `Ki`, `Kd` może prowadzić do niestabilności systemu. W systemach produkcyjnych warto dodać walidację, np. sprawdzanie, czy parametry nie są ujemne lub zbyt duże.
3. **Brak ograniczenia czasu:** W przypadku, gdy `Update` nie jest wywoływany regularnie, `dt` może być bardzo duże, co może prowadzić do niestabilności. W systemach czasu rzeczywistego warto zastosować mechanizm limitujący `dt` do pewnego maksimum.

### Podsumowanie

Klasa `PIDController` to elegancki i funkcjonalny moduł sterowania, który może być używany w wielu kontekstach. Zawiera wszystkie podstawowe elementy kontrolera PID, a jego architektura pozwala na łatwe rozbudowanie. Jednak w środowiskach produkcyjnych lub czasu rzeczywistego konieczne jest zastosowanie dodatkowych mechanizmów bezpieczeństwa i synchronizacji.