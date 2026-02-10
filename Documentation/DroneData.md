# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Plik `PIDController.cs` zawiera implementację kontrolera typu PID (Proporcjonalno-Różniczkowo-Integrującego), który jest szeroko stosowany w systemach sterowania, szczególnie w zastosowaniach przemysłowych, robotyce oraz sterowaniach dynamicznym. W kontekście systemu, który zarządza procesem, np. regulacją temperatury, prędkości silnika lub położenia mechanizmu, kontroler PID pełni rolę regulatora, który dostosowuje sygnał sterujący w oparciu o błąd między wartością docelową (setpointem) a aktualną wartością (feedbackem).

Klasa `PIDController` została zaprojektowana z myślą o integracji z systemem sterowania w czasie rzeczywistym, gdzie dane wejściowe (np. z czujników) są przekazywane w sposób cykliczny, a wynik działania kontrolera (np. sygnał PWM) jest stosowany do sterowania elementem wykonawczym. Zatem, kontroler ten działa w kontekście cyklicznego wykonywania się procedury `Update()`, która reprezentuje jeden krok w czasie rzeczywistym systemu sterowania.

W kontekście architektury systemu, klasa `PIDController` może być częścią większego systemu sterowania, np. `MotorController`, `TemperatureControlSystem`, `NavigationSystem`, gdzie kontroler PID jest jednym z elementów w strukturze sterowania. Może być również używany jako komponent w systemie typu "component-based architecture", gdzie kontrolery PID są instancjami, które mogą być dynamicznie tworzone, konfigurowane i łączone z innymi komponentami sterującymi.

Klasa `PIDController` nie posiada jawnej logiki życia, ale jej cykl życia jest kontrolowany przez zewnętrzny system sterowania, który wywołuje metodę `Update()` w regularnych odstępach czasu. Zatem, jej cykl życia można opisać jako: inicjalizacja (ustawienie parametrów PID, ustawienie wartości początkowej), następnie cykliczne wywołanie `Update()` w pętli sterowania, a na końcu dezaktywacja (np. przez ustawienie wartości sterującego na zero lub wyzerowanie zmiennych wewnętrznych).

## 2. Analiza Logiki Biznesowej (Deep Dive)

### 2.1. Inicjalizacja i Ustawianie Parametrów

Klasa `PIDController` ma konstruktor, który przyjmuje trzy parametry: `Kp`, `Ki`, `Kd` (współczynniki PID). Te wartości są przechowywane w prywatnych zmiennych klasy (`_kp`, `_ki`, `_kd`). Dodatkowo, ustawiane są zmienne `_previousError` i `_integral`, które są potrzebne do obliczeń PID.

```csharp
public PIDController(double kp, double ki, double kd)
{
    _kp = kp;
    _ki = ki;
    _kd = kd;
    _previousError = 0;
    _integral = 0;
}
```

W tym miejscu nie następuje żadna logika obliczeniowa, ale ustawione są wartości, które będą wykorzystywane w dalszej części obliczeń. Wartości te są typu `double`, co zapewnia dużą dokładność obliczeń, co jest istotne w systemach sterowania, gdzie błędy mogą mieć poważne konsekwencje.

### 2.2. Metoda `Update()`

Główną logiką działania kontrolera PID jest metoda `Update()`, która przyjmuje dwa argumenty: `setpoint` i `processValue`. Wartość `setpoint` to wartość docelowa, a `processValue` to aktualna wartość z procesu (np. temperatura, prędkość). Metoda ta zwraca wartość sterującą (output), która może być np. sygnał PWM do silnika.

```csharp
public double Update(double setpoint, double processValue)
{
    double error = setpoint - processValue;

    _integral += error * _dt;
    double derivative = (error - _previousError) / _dt;

    double output = _kp * error + _ki * _integral + _kd * derivative;

    _previousError = error;

    return output;
}
```

#### 2.2.1. Obliczenie błędu (`error`)

W pierwszej linii następuje obliczenie błędu jako różnicy między wartością docelową (`setpoint`) a aktualną wartością (`processValue`). Ten błąd jest kluczowym elementem w algorytmie PID, ponieważ decyduje o kierunku i intensywności działania regulatora.

#### 2.2.2. Obliczenie całki (`_integral`)

W kolejnym kroku obliczana jest wartość całki błędu. Wartość `_integral` jest zwiększana o iloczyn błędu i czasu (`_dt`), co odpowiada zasadzie całkowania numerycznego (np. metoda trapezów). Wartość ta reprezentuje sumę błędu w czasie i jest używana do redukcji błędu statycznego.

#### 2.2.3. Obliczenie pochodnej (`derivative`)

Pochodna błędu jest obliczana jako różnica między aktualnym błędem a poprzednim błędem podzielona przez czas (`_dt`). Obliczenie to pozwala na reakcję kontrolera na szybkość zmiany błędu, co pozwala na zapobieganie nadmiernemu wahań i poprawia stabilność systemu.

#### 2.2.4. Obliczenie wartości wyjściowej (`output`)

W końcowym kroku obliczana jest wartość wyjściowa, która jest sumą trzech członów PID:

- Człon proporcjonalny: `Kp * error`
- Człon całkowity: `Ki * integral`
- Człon różniczkowy: `Kd * derivative`

Wszystkie te składniki są mnożone przez odpowiednie współczynniki i sumowane, co daje ostateczną wartość sterującą. Wartość ta może być np. przekazywana do sterownika PWM lub innego elementu sterującego.

#### 2.2.5. Aktualizacja zmiennych wewnętrznych

Po obliczeniu wartości wyjściowej, zmienna `_previousError` jest aktualizowana na wartość bieżącego błędu. Zmienna `_integral` jest aktualizowana w sposób cykliczny, co zapewnia, że błąd w czasie będzie sumowany i uwzględniony w przyszłych obliczeniach.

### 2.3. Zmienne wewnętrzne i ich wpływ na działanie

- `_previousError`: Przechowuje poprzedni błąd, który jest potrzebny do obliczenia pochodnej.
- `_integral`: Przechowuje sumę błędów w czasie, co pozwala na eliminację błędu statycznego.
- `_dt`: Czas między kolejnymi wywołaniami `Update()`. Wartość ta musi być konsekwentna i stała lub odpowiednio przeliczana w przypadku nieregularnych wywołań.

## 3. Szczegóły Techniczne

### 3.1. Zależności i Dziedziczenie

Klasa `PIDController` nie dziedziczy z żadnej klasy ani nie implementuje żadnego interfejsu. Jest to klasa samodzielna, co pozwala na jej prostą integrację z systemem sterowania. Jednak w bardziej rozbudowanym systemie, klasa może być częścią hierarchii interfejsów, np. `IController`, `IRegulator`, które mogłyby wymuszać implementację metody `Update()`.

### 3.2. Przepływ Danych

#### Wejścia:
- `setpoint` – wartość docelowa (np. temperatura 25°C)
- `processValue` – aktualna wartość procesu (np. temperatura z czujnika)
- `_dt` – czas między kolejnymi cyklami (np. 0.01 sekundy)

#### Wyjścia:
- `output` – wartość sterująca (np. sygnał PWM 0–255)
- Zmienne wewnętrzne `_integral`, `_previousError` – modyfikowane w miejscu, nie zwracane bezpośrednio

### 3.3. Kluczowe Zmienne

- `_kp`, `_ki`, `_kd` – współczynniki PID, które są ustawiane w konstruktorze i nie zmieniają się w trakcie działania.
- `_previousError` – przechowuje poprzedni błąd, potrzebny do obliczenia pochodnej.
- `_integral` – zmienna akumulująca błąd w czasie, wykorzystywana w członie całkowitym.
- `_dt` – czas cyklu, który musi być stały lub odpowiednio przeliczany w przypadku nieregularnych cykli.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### 4.1. Wzorce i Architektura

Klasa `PIDController` nie stosuje jawnie żadnego wzorca projektowego, ale może być traktowana jako przykład wzorca **Komponentu Sterującego** (Control Component) w systemach sterowania. Wzorzec ten pozwala na modularne i niezależne sterowanie systemem, co zwiększa skalowalność i łatwość testowania.

W kontekście architektury systemu, kontroler PID może być częścią większej architektury sterowania typu **Observer Pattern** (jeśli kontroler reaguje na zmiany w systemie) lub może być częścią systemu sterowania typu **Strategy Pattern**, gdzie różne kontrolery PID mogą być używane w zależności od typu procesu (np. PID dla temperatury, PID z ograniczeniem dla prędkości).

### 4.2. Analiza Code-Level

#### 4.2.1. Właściwości `double` i precyzja obliczeń

Wartości `_kp`, `_ki`, `_kd`, `error`, `integral`, `derivative`, `output` są typu `double`. W języku C# typ `double` zapewnia precyzję do 15–16 cyfr dziesiętnych, co jest konieczne w systemach sterowania, gdzie nawet niewielkie różnice mogą mieć znaczący wpływ na działanie systemu.

#### 4.2.2. Zmienna `_dt` i jego wpływ na stabilność

Wartość `_dt` musi być stała i dokładnie określona. W przypadku nieregularnych cykli (np. z powodu przetwarzania w systemie), może to prowadzić do niestabilności lub błędów w obliczeniach pochodnej. W bardziej zaawansowanych implementacjach, `_dt` może być obliczany dynamicznie lub weryfikowany, aby zapewnić stabilność.

#### 4.2.3. Zmienne `_integral` i `_previousError` jako zmienne stanu

Zmienne `_integral` i `_previousError` są zmiennymi stanu kontrolera. W systemach wielowątkowych lub asynchronicznych, może to prowadzić do problemów z dostępem do zmiennych współdzielonych. W takich przypadkach konieczne jest stosowanie mechanizmów synchronizacji, np. `lock`, aby zapobiec błędom w obliczeniach.

#### 4.2.4. Rzutowanie i konwersje typów

W kodzie nie ma jawnych rzutowań, ale w bardziej rozbudowanych systemach może być konieczne rzutowanie typów, np. z `float` do `double` w przypadku przekazywania danych z czujników. Warto zwrócić uwagę na potencjalne utraty precyzji w takich przypadkach.

#### 4.2.5. Wydajność i wykorzystanie pamięci

Klasa `PIDController` nie korzysta z dynamicznej alokacji pamięci, co oznacza, że nie ma ryzyka wycieku pamięci. Wszystkie zmienne są alokowane statycznie. Jednak w systemach wielowątkowych, zbyt częste wywoływania `Update()` mogą prowadzić do problemów z wydajnością, szczególnie jeśli obliczenia są złożone. W takich przypadkach warto rozważyć optymalizację obliczeń lub zastosowanie `Span<T>` lub `Memory<T>`.

#### 4.2.6. Potencjalne zagrożenia

- **Brak walidacji danych wejściowych**: W przypadku przekazania `NaN` lub `Infinity` do `setpoint` lub `processValue`, wynik działania może być nieprzewidywalny.
- **Nieprawidłowe `_dt`**: Jeśli `_dt` wynosi zero lub jest ujemny, obliczenia pochodnej mogą być nieprawidłowe.
- **Brak ograniczeń na `output`**: W systemach sterowania, wartość wyjściowa może być ograniczona (np. do zakresu 0–255 dla PWM). Brak ograniczeń może prowadzić do nadmiernego działania sterownika.

---

### Podsumowanie

Klasa `PIDController` reprezentuje podstawową implementację kontrolera PID w systemie sterowania. Działa on w sposób cykliczny, przyjmując dane wejściowe i zwracając wartość sterującą. Jest to klasa prostych, ale bardzo istotnych obliczeń, które są kluczowe dla stabilności i dokładności systemu sterowania. Warto zwrócić uwagę na dokładność obliczeń, zarządzanie zmiennymi stanu oraz potencjalne zagrożenia związane z nieprawidłowymi danymi wejściowymi.