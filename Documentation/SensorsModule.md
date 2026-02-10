# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Ten plik definiuje klasę `PIDController`, która implementuje regulator PID (Proporcjonalno-Różniczkowo-Integralny) w kontekście systemów sterowania dynamicznego, najczęściej stosowanego w aplikacjach przemysłowych, robotyce, sterowaniach procesów technologicznych oraz systemach autonomicznych. Klasa `PIDController` działa jako komponent centralny w architekturze sterowania, zapewniając precyzyjne dopasowanie odpowiedzi systemu do zadanej wartości (setpointu) poprzez obliczanie odpowiednich wartości sterujących na podstawie błędu pomiarowego.

W większym kontekście systemowym, `PIDController` może być używany w ramach większego systemu sterowania, np. w mikrokontrolerach (np. Arduino, STM32), w systemach PLC, albo jako komponent w systemach sterowania typu embedded w kontenerach mikroserwisów (np. w systemach IoT). Klasa może być wykorzystywana w kontekście sterowania temperatury, obrotów silnika, położenia mechanizmu, lub innych wielkości fizycznych, które wymagają stabilnego i szybkiego reagowania na zmiany.

Klasa `PIDController` ma cykl życia zdefiniowany w sposób proceduralny, gdzie inicjalizacja odbywa się poprzez konstruktor, a następnie w pętli sterowania (np. w `Update` lub `Loop`) obliczane są nowe wartości sterujące. W systemach czasu rzeczywistego, może być wywoływana z określonym interwałem (np. co 10 ms), co wymaga odpowiedniego zarządzania pamięcią oraz konieczności optymalizacji obliczeń.

## 2. Analiza Logiki Biznesowej (Deep Dive)

### 2.1. Inicjalizacja i Ustawienia PID

Klasa `PIDController` zawiera trzy główne parametry: `Kp`, `Ki`, `Kd` – odpowiednio współczynniki proporcjonalny, integralny i różniczkowy. Wartości te są ustawiane w konstruktorze lub dynamicznie poprzez publiczne właściwości (`ProportionalGain`, `IntegralGain`, `DerivativeGain`). Wartości te definiują charakterystykę regulatora:

- `Kp` (proporcjonalny): Współczynnik, który determinuje natychmiastową reakcję regulatora na błąd. Im wyższy `Kp`, tym szybsza reakcja, ale może to prowadzić do drgań.
- `Ki` (integralny): Współczynnik, który eliminuje błąd ustalony (offset). Pracuje na podstawie sumy błędów w czasie. Zbyt wysoka wartość może powodować przebięcia.
- `Kd` (różniczkowy): Współczynnik, który „przewiduje” przyszły błąd na podstawie zmiany błędu w czasie. Zmniejsza nadmiarowe drgania i poprawia stabilność.

### 2.2. Obliczenia PID

Główne obliczenia odbywają się w metodzie `Compute()` (lub `Update()` w innych implementacjach), która oblicza wartość sterującą `output` na podstawie:

1. **Błędu (`error`)**: Różnica między wartością zadana (`setpoint`) a aktualną wartością pomiarową (`input`).
2. **Całkowitego błędu (`integral`)**: Suma błędów z poprzednich kroków (z uwagi na czas, tzn. `integral += error * dt`).
3. **Zmiany błędu (`derivative`)**: Różnica między bieżącym i poprzednim błędem, podzielona przez czas (`derivative = (error - previousError) / dt`).

Wzór PID:

```
output = Kp * error + Ki * integral + Kd * derivative
```

### 2.3. Przepływ Obliczeń

1. **Pobranie danych wejściowych**: `input` i `setpoint` są pobierane z zewnętrznego źródła (np. czujnika).
2. **Obliczenie błędu**: `error = setpoint - input`.
3. **Aktualizacja całki**: `integral += error * dt`.
4. **Obliczenie pochodnej**: `derivative = (error - previousError) / dt`.
5. **Obliczenie wyniku PID**: `output = Kp * error + Ki * integral + Kd * derivative`.
6. **Zapisanie poprzedniego błędu**: `previousError = error`.

### 2.4. Obsługa Przepełnień i Stabilności

W implementacji może być dodana logika zapobiegająca przepełnieniu `integral` (np. `integral = clamp(integral, -maxIntegral, maxIntegral)`). Dodatkowo, `output` może być ograniczony do zakresu `outputMin` i `outputMax`, aby zapobiec nadmiernemu wyjściu sterującemu (np. w przypadku silnika, który nie może wyjść poza zakres 0–255).

## 3. Szczegóły Techniczne

### 3.1. Zależności i Dziedziczenie

Klasa `PIDController` nie dziedziczy z żadnej klasy bazowej w podanym kodzie. Może być jednak rozszerzana w przyszłości, np. przez implementację interfejsu `IController` lub `IControlLoop`. Interfejs może wymagać metod takich jak `Compute`, `Reset`, `SetSetpoint`, co zapewnia elastyczność i możliwość wstrzykiwania różnych typów regulatorów (np. PID, LQR, fuzzy).

### 3.2. Przepływ Danych

#### Wejścia:

- `input`: Typ `double` – wartość aktualnego stanu systemu (np. temperatura, położenie). Zwykle pochodzi z czujnika.
- `setpoint`: Typ `double` – wartość zadana, do której system ma dążyć.
- `dt`: Typ `double` – czas od ostatniego obliczenia, wyrażony w sekundach. Istotny dla obliczeń całkowania i różniczkowania.

#### Wyjścia:

- `output`: Typ `double` – wartość sterująca, która może być np. przekazywana do silnika, przekaźnika lub innego elementu sterującego.
- `integral` i `derivative`: Wewnętrzne zmienne, nie są bezpośrednio zwracane, ale wpływają na `output`.

### 3.3. Kluczowe Zmienne

- `Kp`, `Ki`, `Kd`: Współczynniki regulatora PID, ustawiane statycznie lub dynamicznie.
- `integral`: Akumulator błędów – zmienna typu `double`, przechowująca sumę błędów w czasie.
- `derivative`: Zmienna typu `double`, przechowująca pochodną błędu (zmianę w czasie).
- `previousError`: Zmienna typu `double`, przechowująca błąd z poprzedniego kroku – potrzebna do obliczenia pochodnej.
- `outputMin`, `outputMax`: Ograniczenia wyjścia regulatora – zapobiegają nadmiernemu sterowaniu.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### 4.1. Wzorce i Architektura

#### Wzorzec: Komponent Sterowania (Control Component)

`PIDController` działa jak komponent sterowania w architekturze mikroserwisowej lub systemu sterowania typu embedded. Jest to wzorzec typu **"Komponent sterowania"**, który może być wstrzyknięty do systemu sterowania w miejscu, gdzie potrzebne jest dopasowanie do wartości zadanej.

#### Dlaczego ten wzorzec?

Wzorzec ten zapewnia:

- **Niezależność**: `PIDController` nie wie, skąd pochodzą dane wejściowe ani co robi sterowanie – zapewnia tylko funkcję obliczeniową.
- **Elastyczność**: Możliwość podmiany regulatora (np. z PID na fuzzy logic) bez zmiany reszty systemu.
- **Testowalność**: Można przeprowadzać testy jednostkowe z użyciem mockowanych danych.

#### Alternatywa: Brak wzorca (prosty kod)

W przypadku braku wzorca, kod może być zapisany w formie procedury (np. `void ComputePID(...)`), co utrudnia testowanie i utrzymanie. Wzorzec obiektowy (klasa) zapewnia modularność, łatwiejsze debugowanie i skalowalność.

### 4.2. Analiza Code-Level

#### C++/C# Syntax Constructs

- `public double Compute(double input, double setpoint, double dt)`: Metoda publiczna, zwracająca wartość `double`. W C# typ `double` to 64-bitowa liczba zmiennoprzecinkowa, co zapewnia wysoką dokładność obliczeń.
- `private double integral`, `private double derivative`, `private double previousError`: Zmienne prywatne, zapobiegają nieautoryzowanemu dostępowi do stanu regulatora.
- `public double ProportionalGain`, `public double IntegralGain`, `public double DerivativeGain`: Właściwości publiczne (get/set), umożliwiające dynamiczną modyfikację parametrów PID – wykorzystuje się to w systemach z adaptacją PID (np. Auto-tuning).
- `output = clamp(output, outputMin, outputMax)`: Funkcja `clamp` zapewnia, że wartość `output` nie przekracza zakresu. Może być zaimplementowana jako:

```csharp
public static double Clamp(double value, double min, double max)
{
    return value < min ? min : value > max ? max : value;
}
```

#### Potencjalne Ryzyka i Obsługa

- **Przepełnienie całki (`integral`)**: W przypadku długiego czasu działania i dużych błędów, `integral` może przekroczyć zakres `double`. W praktyce może to prowadzić do "windupu integralnego". Rozwiązaniem jest ograniczenie `integral` (np. `integral = clamp(integral, -maxIntegral, maxIntegral)`).
- **Niestabilność z powodu `Kd`**: Wysokie `Kd` może powodować nadmierną reakcję na szumy w danych wejściowych. W praktyce stosuje się filtry lub ograniczenie pochodnej.
- **Brak synchronizacji**: W systemach wielowątkowych, `PIDController` może być niebezpieczny bez synchronizacji. W C# można użyć `lock` lub `Interlocked` do zapewnienia bezpieczeństwa.

#### Przykład kodu z błędem i jego poprawka:

```csharp
// Błędna implementacja – brak ograniczenia całki
integral += error * dt;
output = Kp * error + Ki * integral + Kd * derivative;

// Poprawna implementacja – ograniczenie całki
integral += error * dt;
integral = Clamp(integral, -maxIntegral, maxIntegral);
output = Kp * error + Ki * integral + Kd * derivative;
```

### 4.3. Optymalizacje i Wskazówki

- **Zmniejszenie obciążenia procesora**: W systemach czasu rzeczywistego, obliczenia PID mogą być wykonywane w tle lub w wydzielonym wątku.
- **Czas `dt`**: Wartość `dt` powinna być stała lub interpolowana, aby zapewnić stabilność obliczeń. W systemach nieliniowych można stosować `dt = Time.Now - LastTime`.
- **Zarządzanie pamięcią**: W C# nie ma potrzeby ręcznego zarządzania pamięcią, ale w C++ trzeba uważać na alokację i dealokację pamięci w przypadku dynamicznego tworzenia obiektów.

---

### Podsumowanie

Klasa `PIDController` to kluczowy element w systemach sterowania, zapewniający stabilne i precyzyjne dopasowanie do wartości zadanej. Zastosowanie wzorców, optymalizacji obliczeniowych i odpowiednie zarządzanie stanem pozwala na jej zastosowanie w złożonych systemach czasu rzeczywistego. Warto zwrócić uwagę na potencjalne problemy takie jak przepełnienie całki, szumy i niestabilność, które mogą być eliminowane poprzez odpowiednie filtrowanie i ograniczenia.