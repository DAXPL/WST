# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Plik `PIDController.cs` definiuje klasę `PIDController`, która implementuje regulator typu PID (Proporcjonalno-Integralno-Różniczkowy), szeroko stosowany w systemach sterowania automatycznego. Klasa ta pełni rolę centralnego elementu w systemie sterowania, który dostarcza odpowiedzi na sygnały wejściowe, przetwarza je za pomocą algorytmu PID, a następnie generuje sygnał sterujący do sterownika (np. silnika, zaworu, itp.). Jest to kluczowa komponenta w architekturze systemów przemysłowych, robotycznych, a także w aplikacjach sterowania dynamicznego, takich jak sterowanie temperaturą, pozycją, prędkością czy ciśnieniem.

Klasa `PIDController` jest zazwyczaj używana w kontekście pętli sterowania, gdzie:

- W każdej iteracji pobierane są dane z czujników (np. wartość aktualnej temperatury),
- Na podstawie tych danych obliczany jest błąd (różnica między wartością docelową a aktualną),
- Regulator PID przetwarza ten błąd i generuje sygnał sterujący,
- Sygnał ten jest następnie przekazywany do sterownika (np. do sterownika PWM),
- Proces powtarza się w czasie rzeczywistym.

W kontekście architektury systemowej, `PIDController` może być częścią większej struktury sterowania, np. w systemie zarządzania procesem produkcyjnym, sterowaniu robotem, czy systemie monitorowania i sterowania temperatury w kominie przemysłowym.

Klasa `PIDController` nie posiada publicznego konstruktora, co sugeruje, że może być tworzona przez fabrykę lub inny mechanizm, np. przez `Dependency Injection` w aplikacji ASP.NET Core lub w systemach embedded. Warto zauważyć, że nie ma też publicznych metod `Start`, `Stop` ani `Reset`, co sugeruje, że jej cykl życia jest zarządzany przez zewnętrzny mechanizm sterujący, np. przez `Update` lub `Loop` w pętli sterowania.

## 2. Analiza Logiki Biznesowej (Deep Dive)

### Przepływ danych i logika działania

Klasa `PIDController` działa w oparciu o trzy składniki: proporcjonalny (P), integralny (I) i różniczkowy (D). Każdy z nich ma swoje znaczenie w kontekście sterowania:

1. **Składnik proporcjonalny (P):** Reaguje natychmiastowo na bieżący błąd. Im większy błąd, tym większa wartość sygnału sterującego.
2. **Składnik integralny (I):** Zbiera błędy z poprzednich kroków i eliminuje stały błąd (offset). Używany jest do eliminacji błędów statycznych.
3. **Składnik różniczkowy (D):** Reaguje na zmianę błędu w czasie, co pozwala na "przewidywanie" zmian i redukuje nadmiarowe drgania.

W implementacji:

- Metoda `Update(double error)` jest centralnym punktem działania regulatora.
- Wewnątrz tej metody obliczane są trzy składniki PID:
  - `proportional = Kp * error`
  - `integral = integral + Ki * error * dt`
  - `derivative = Kd * (error - previousError) / dt`
- Następnie sumowane są one zgodnie z wzorem:
  - `output = proportional + integral + derivative`

### Czasowe ustawienia i zmienne stanu

- `previousError` przechowuje błąd z poprzedniego kroku, aby obliczyć pochodną.
- `integral` przechowuje sumę błędów z poprzednich kroków — w tym miejscu zachodzi akumulacja błędu.
- `dt` (delta time) jest czasem między kolejnymi krokami — musi być dokładnie obliczony, aby nie wpływać na stabilność regulatora.

Warto zwrócić uwagę, że `integral` może być ograniczany (np. przez `integral = Math.Clamp(integral, -maxIntegral, maxIntegral)`), aby uniknąć efektu windupu — sytuacji, w której integralny składnik zaczyna „przeładowywać” sygnał sterujący, co może prowadzić do niestabilności.

### Wpływ zmiany parametrów PID

Zmiana wartości `Kp`, `Ki`, `Kd` ma znaczący wpływ na dynamikę systemu:

- `Kp` — im większa wartość, tym szybsza reakcja, ale też większa szansa na drgania.
- `Ki` — im większa wartość, tym szybsza eliminacja błędu statycznego, ale też większa szansa na oscylacje.
- `Kd` — im większa wartość, tym większa stabilność, ale też większa czułość na szum.

### Pętla sterowania

W systemie sterowania, `PIDController` może być wywoływany w pętli `Update` co kilka milisekund (np. 10ms). W każdej iteracji:

1. Pobierana jest wartość aktualna z czujnika.
2. Obliczany jest błąd: `error = setpoint - actualValue`.
3. Wywoływana jest metoda `Update(error)`.
4. Wynik `output` jest przekazywany do sterownika.

W przypadku braku zmiany `dt`, np. w systemach niezależnych od czasu, może to prowadzić do nieprawidłowego działania regulatora.

## 3. Szczegóły Techniczne

### Zależności i dziedziczenie

Klasa `PIDController` nie dziedziczy z żadnej klasy, ani nie implementuje interfejsu. Może to sugerować, że:

- Jest to klasa samodzielna, przeznaczona do bezpośredniego użycia.
- Może być używana w kontekście testowym lub w systemach, gdzie nie ma potrzeby implementacji interfejsu.
- W bardziej zaawansowanym systemie, np. z interfejsem `IController`, ta klasa może być implementacją tego interfejsu.

### Przepływ danych

#### Wejścia:

- `double error` — błąd obliczony jako różnica między wartością docelową a aktualną. Jednostka: bezwymiarowa, ale w kontekście systemu może być np. stopnie Celsjusza, RPM, mm, itp.
- `double dt` — czas w sekundach między dwoma kolejnymi krokami pętli sterowania. Jednostka: sekundy.
- `double setpoint` — wartość docelowa, np. temperatura 100°C.
- `double actualValue` — wartość aktualna z czujnika.

#### Wyjścia:

- `double output` — wartość sygnału sterującego, np. wartość PWM z zakresu 0–255, lub wartość sterująca napędem (np. 0–100%).
- `double proportional`, `double integral`, `double derivative` — wartości poszczególnych składników PID, używane głównie do debugowania lub do wizualizacji działania regulatora.

### Kluczowe zmienne

- `Kp`, `Ki`, `Kd` — współczynniki PID. Przechowują wartości skalujące składniki.
- `integral` — akumulowana wartość błędu, używana do obliczenia składnika integralnego.
- `previousError` — przechowuje błąd z poprzedniego kroku — potrzebne do obliczenia pochodnej.
- `dt` — czas między krokami — używany do obliczeń różniczkowych.
- `maxIntegral` — ograniczenie wartości całkującej — zapobiega efektowi windupu.
- `output` — wynik końcowy regulatora — może być przekazywany do sterownika.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### Wzorce i Architektura

Klasa `PIDController` nie stosuje żadnego konkretnego wzorca projektowego (np. Strategy, Factory, Observer). Jest to klasa **implementacyjna**, która realizuje konkretną logikę obliczeniową. Jednak może być częścią większego systemu, który stosuje wzorce:

- **Strategy Pattern** — jeśli `PIDController` może być podmieniany przez inne sterowniki (np. regulator PID, regulator MPC, regulator fuzzy), to może być zaimplementowany jako strategia.
- **Observer Pattern** — jeśli `PIDController` informuje inne obiekty o zmianach wyniku, np. przez eventy, to może być wykorzystany wzorzec obserwatora.

W kontekście projektowania systemów sterowania, warto zastosować **Dependency Injection**, aby `PIDController` był łatwiejszy do testowania i konfigurowania w różnych środowiskach.

### Analiza Code-Level

#### C++/C# syntax constructs

- **`double`** — typ zmiennoprzecinkowy o podwójnej precyzji. Używany w obliczeniach PID, gdzie dokładność jest kluczowa.
- **`Math.Clamp()`** — używany do ograniczania wartości `integral` — zapobiega efektowi windupu. Jest to bardzo ważna praktyka w systemach sterowania.
- **`virtual` / `override`** — jeśli ta klasa miałaby być dziedziczona, to metody `Update` i `Reset` mogłyby być `virtual`, aby umożliwić nadpisanie.
- **`private` / `public`** — dostęp do zmiennych i metod kontrolowany — zapewnia enkapsulację, co jest kluczowe dla bezpieczeństwa i niezawodności.
- **`const` / `readonly`** — jeśli `Kp`, `Ki`, `Kd` byłyby `readonly`, to zapewniałyby, że nie zmieniają się podczas działania programu — co jest bardzo istotne w systemach czasu rzeczywistego.

#### Potencjalne zagrożenia

- **Nieprawidłowe `dt`** — jeśli `dt` jest zerowy lub zbyt duże, to może prowadzić do dzielenia przez zero lub niestabilności.
- **Brak synchronizacji w środowisku wielowątkowym** — jeśli `PIDController` jest wywoływany z wielu wątków, może wystąpić warunek wyścigu. W takim przypadku należy zastosować mechanizmy synchronizacji, np. `lock`, `Monitor`, `Interlocked`.
- **Brak walidacji danych wejściowych** — jeśli `error` lub `dt` są nieprawidłowe, może to prowadzić do nieprawidłowego działania regulatora.
- **Efekt windupu** — integralny składnik może przekroczyć zakres sygnału sterującego, co może prowadzić do niestabilności. Ograniczenie `integral` jest kluczowe.

#### Praktyki do wdrożenia

- **Testowanie jednostkowe** — `PIDController` powinien mieć testy jednostkowe, które sprawdzają zachowanie dla różnych wartości `Kp`, `Ki`, `Kd`, oraz dla różnych wartości `dt`.
- **Wizualizacja działania** — do debugowania można zaimplementować funkcję `Log()` lub `DebugOutput()`, która zapisuje wartości `proportional`, `integral`, `derivative` w celu wizualizacji działania regulatora.
- **Zarządzanie pamięcią** — w systemach embedded, `PIDController` powinien być zoptymalizowany pod kątem zużycia pamięci i wydajności — unikanie alokacji pamięci w pętli sterowania.

---

### Podsumowanie

Klasa `PIDController` to podstawowy element systemu sterowania, który implementuje algorytm PID w sposób prosty i efektywny. W kontekście projektowania systemów sterowania, jej wydajność i stabilność zależą od dokładnego ustawienia parametrów `Kp`, `Ki`, `Kd`, a także od poprawnego zarządzania `dt`. W środowiskach czasu rzeczywistego, warto zastosować mechanizmy synchronizacji i walidacji danych, aby zapewnić niezawodność działania regulatora.