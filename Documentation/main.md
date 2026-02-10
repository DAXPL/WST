# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Ten plik definiuje klasę `PIDController`, która implementuje regulator typu PID (Proporcjonalno-Różniczkowo-Integrujący). Regulator PID jest szeroko stosowanym elementem w systemach sterowania automatycznego, szczególnie tam, gdzie konieczne jest precyzyjne dopasowanie odpowiedzi systemu do zadanej wartości (setpointu). Klasa ta została zaprojektowana do pracy w środowisku embedded systemów, gdzie wydajność i precyzja są kluczowe.

Klasa `PIDController` służy do obliczania wartości sterującej (output) na podstawie bieżącej wartości zmierzonej (input), wartości zadanej (setpoint) oraz parametrów regulatora: wagi proporcjonalnej (Kp), całkującej (Ki) i różniczkującej (Kd). Jest ona częścią większego systemu sterowania, w którym może być używana do regulacji temperatury, prędkości, położenia czy innych wielkości fizycznych. Klasa może być używana zarówno w środowiskach czasu rzeczywistego, jak i w systemach symulacyjnych, gdzie wymagana jest precyzyjna kontrola dynamiczna.

Klasa implementuje cykl życia, który obejmuje inicjalizację parametrów regulatora, aktualizację wartości sterującej oraz resetowanie stanu regulatora. Zawiera również mechanizmy do konfiguracji zakresu wyjścia, co pozwala na dopasowanie regulatora do konkretnego systemu sterowanego (np. do ograniczenia sygnału PWM do zakresu 0–255).

## 2. Analiza Logiki Biznesowej (Deep Dive)

### 2.1. Inicjalizacja regulatora

W konstruktorze klasy `PIDController` inicjalizowane są wartości parametrów regulatora: `Kp`, `Ki`, `Kd`, a także zakres wyjścia (`outputMin`, `outputMax`). Inicjalizacja odbywa się poprzez przekazanie tych wartości jako argumentów konstruktora. Dodatkowo, inicjalizowane są zmienne wewnętrzne, takie jak `lastError`, `integral`, `lastInput`, które są potrzebne do obliczeń w kolejnych krokach.

### 2.2. Metoda `Update`

Metoda `Update` jest głównym punktem wejścia do obliczeń regulatora PID. Wywoływana jest w każdym cyklu sterowania (np. co 10 ms). Proces działania metody wygląda następująco:

1. **Obliczenie błędu:**  
   ```csharp
   double error = setpoint - input;
   ```
   Wartość błędu (error) to różnica między wartością zadana (setpoint) a aktualną wartością pomiarową (input).

2. **Obliczenie członu całkującego:**  
   ```csharp
   integral += error * dt;
   ```
   Człon całkujący akumuluje błąd w czasie, co pozwala na eliminację błędów ustalonych. Wartość `dt` to czas od ostatniego wywołania metody `Update`, co pozwala na dokładne obliczenie całki.

3. **Obliczenie członu różniczkującego:**  
   ```csharp
   double derivative = (input - lastInput) / dt;
   ```
   Człon różniczkujący reaguje na szybkość zmiany błędu, co pozwala na redukcję nadmiarowego oscylowania. Wartość `lastInput` przechowuje poprzednią wartość wejścia.

4. **Obliczenie wartości wyjścia:**  
   ```csharp
   double output = Kp * error + Ki * integral - Kd * derivative;
   ```
   Wartość wyjścia jest obliczana jako suma trzech członów PID:
   - Człon proporcjonalny: `Kp * error`
   - Człon całkujący: `Ki * integral`
   - Człon różniczkujący: `Kd * derivative`

5. **Ograniczenie zakresu wyjścia:**  
   ```csharp
   output = Math.Max(outputMin, Math.Min(outputMax, output));
   ```
   Wartość wyjścia jest ograniczona do zakresu `[outputMin, outputMax]`, co zapewnia, że nie przekracza wartości dopuszczalnych dla systemu sterowanego (np. zakres sygnału PWM).

6. **Aktualizacja stanu:**  
   ```csharp
   lastError = error;
   lastInput = input;
   ```
   Wartości `lastError` i `lastInput` są aktualizowane, aby móc obliczyć człon różniczkujący w kolejnym cyklu.

### 2.3. Czas działania i efektywność

Ważne jest, aby `dt` (czas od ostatniego wywołania) był dokładnie mierzony, ponieważ wpływa bezpośrednio na dokładność obliczeń członów całkującego i różniczkującego. W systemach czasu rzeczywistego, `dt` może być obliczany z użyciem `System.Diagnostics.Stopwatch` lub podobnego mechanizmu.

## 3. Szczegóły Techniczne

### 3.1. Zależności i Dziedziczenie

Klasa `PIDController` nie dziedziczy z żadnej klasy, ale może być używana w kontekście systemów sterowania, które wymagają implementacji interfejsu `IController`, jeśli taki został zdefiniowany w większym systemie. W tym przypadku, klasa jest niezależna i może być używana samodzielnie.

### 3.2. Przepływ Danych

#### Wejścia:
- `input`: wartość aktualnie mierzona (double), typowo w jednostkach fizycznych (np. °C, rpm, m/s).
- `setpoint`: wartość zadana (double), typowo w tych samych jednostkach co `input`.
- `dt`: czas od ostatniego wywołania (double), w sekundach.
- `Kp`, `Ki`, `Kd`: współczynniki regulatora PID (double), bez jednostek.
- `outputMin`, `outputMax`: zakres wyjścia regulatora (double), np. 0–255 dla PWM.

#### Wyjścia:
- `output`: obliczona wartość sterująca (double), ograniczona do zakresu `[outputMin, outputMax]`.
- Zmienne `lastError`, `integral`, `lastInput`: są aktualizowane wewnętrznie, nie są zwracane, ale są niezbędne do poprawnego działania regulatora.

### 3.3. Kluczowe Zmienne

- `lastError`: przechowuje poprzedni błąd, potrzebny do obliczenia członu różniczkującego.
- `integral`: akumulowana wartość błędu, potrzebna do obliczenia członu całkującego.
- `lastInput`: poprzednia wartość wejścia, potrzebna do obliczenia członu różniczkującego.
- `Kp`, `Ki`, `Kd`: współczynniki regulatora PID, które wpływają na reakcję regulatora.
- `outputMin`, `outputMax`: ograniczenia wyjścia regulatora, zapewniające bezpieczeństwo i dopasowanie do systemu sterowanego.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### 4.1. Wzorce i Architektura

Klasa `PIDController` nie implementuje żadnego konkretnego wzorca projektowego (np. Strategy, Observer), ale może być częścią większego systemu, który może wykorzystywać wzorce takie jak:

- **Strategy Pattern:** Jeśli `PIDController` może być zamieniany z innym typem regulatora (np. regulator PI, regulator Fuzzy), można zastosować wzorzec Strategy, aby ułatwić testowanie i rozbudowę systemu.
- **Observer Pattern:** Jeśli system sterowania ma wiele regulatorów, które muszą reagować na zmiany w systemie, można zastosować wzorzec Observer, by rozdzielić logikę sterowania od logiki reakcji.

W tym przypadku, `PIDController` jest implementacją samodzielnej jednostki sterującej, co pozwala na jego wielokrotne użycie w różnych systemach, co jest korzystne w kontekście modułowości i testowalności.

### 4.2. Analiza Code-Level

#### C++/C# Syntax Constructs

- **`double` jako typ danych:** W C# `double` to typ 64-bitowy, co zapewnia wysoką precyzję obliczeń, co jest istotne w regulatorach PID, gdzie błędy mogą się gromadzić.
- **`Math.Max` i `Math.Min`:** Służą do ograniczenia wartości wyjścia do zakresu `[outputMin, outputMax]`. Są one wydajne i zapewniają czytelność kodu.
- **`virtual` i `override`:** Jeśli ta klasa miałaby być rozszerzana, warto byłoby zadeklarować metody jako `virtual`, co pozwalałoby na nadpisywanie ich w klasach pochodnych.
- **Zmienne `lastError`, `integral`, `lastInput`:** Są zmiennymi stanowymi, które muszą być zachowane między wywołaniami. W systemach wielowątkowych, należy zastosować mechanizmy synchronizacji, aby zapobiec błędom w przypadku współbieżnego dostępu do tych zmiennych.

#### Potencjalne Ryzyka

- **Brak synchronizacji w systemach wielowątkowych:** Jeśli `Update` jest wywoływany z wielu wątków, może dojść do błędnego stanu w zmiennych `integral`, `lastError`, `lastInput`. W takim przypadku konieczne jest użycie mechanizmów synchronizacji, np. `lock` w C#.
- **Błędy numeryczne:** W przypadku bardzo małych wartości `dt`, może dojść do dzielenia przez zero lub zbyt dużych wartości w członie całkującym. W praktyce, `dt` powinien być ograniczony do minimalnej wartości, np. 0.001 sekundy.
- **Brak walidacji wejść:** Brak walidacji `Kp`, `Ki`, `Kd` może prowadzić do niestabilności regulatora. W praktyce, warto dodać walidację, np. sprawdzić, czy wartości są dodatnie.

#### Praktyczne Zastosowanie

W systemach embedded, takich jak Arduino lub Raspberry Pi, `PIDController` może być wykorzystywany do sterowania silnikiem krokowym, regulacji temperatury, sterowania położeniem serwa, czy też do kontrolowania prędkości wentylatora. Warto pamiętać, że obliczenia PID są czasochłonne, ale dla większości zastosowań są wykonalne w czasie rzeczywistym, o ile `dt` jest odpowiednio wybrany.

### Podsumowanie

Klasa `PIDController` to wydajna, samodzielną implementacja regulatora PID, która może być używana w szerokim zakresie systemów sterowania. Jej prostota i przejrzystość ułatwia debugowanie i testowanie, a przy odpowiednim wyborze parametrów `Kp`, `Ki`, `Kd` może zapewnić bardzo dobrą stabilność i reakcję systemu. Warto jednak pamiętać o potencjalnych problemach z numerycznymi błędami i współbieżnością w zastosowaniach wielowątkowych.