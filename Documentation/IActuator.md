# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Ten plik definiuje klasę `PIDController`, która jest implementacją regulatora PID (Proporcjonalno-Różniczkowo-Integralnego) używanego w systemach sterowania automatycznego, szczególnie w kontekście sterowania silnikami, procesami termicznymi lub ruchem mechanicznym. Regulator PID jest szeroko stosowany w przemyśle, robotyce i systemach sterowania dynamicznym, gdzie konieczne jest dokładne dopasowanie sygnału sterującego do zadanej wartości (setpoint) poprzez analizę błędu w czasie.

Klasa `PIDController` jest zaprojektowana jako komponent wielokrotnego użytku, który może być używany w różnych kontekstach sterowania, np. w sterowniku mikrokontrolerowym lub systemie symulacyjnym. Jako klasa, posiada pełną kontrolę nad cyklem życia obiektu, w tym inicjalizacją parametrów regulatora, aktualizacją wartości sygnału sterującego oraz resetowaniem stanu regulatora. W systemach złożonych, np. w robotyce lub przemyśle, regulator PID może być częścią większego systemu sterowania, który może zawierać więcej niż jeden regulator, a ten obiekt może być zarządzany przez inny obiekt sterujący, np. `ControllerManager`.

Z punktu widzenia architektury systemowej, `PIDController` działa jako element **wewnętrznego sterownika**, który otrzymuje dane wejściowe (np. wartość aktualna z czujnika), porównuje je z wartością zadana (setpoint), a następnie generuje sygnał sterujący (output) do sterowania urządzeniem (np. silnikiem). Dlatego też, jako komponent, `PIDController` powinien być **niezawodny**, **szybki** i **dostosowywalny** — wszystko to zapewnia jego projektowanie w sposób obiektowy z odpowiednim zarządzaniem stanem i parametrami.

## 2. Analiza Logiki Biznesowej (Deep Dive)

### Początkowy stan i inicjalizacja

Klasa `PIDController` zawiera konstruktor, który oczekuje na przekazanie trzech parametrów: `Kp`, `Ki` i `Kd`, które są współczynnikami regulatora PID. Wartości te definiują wpływ proporcjonalnego, integralnego i różniczkowego członu na sygnał wyjściowy. Wartości te są przypisywane do prywatnych zmiennych członów (`proportional`, `integral`, `derivative`), które są wykorzystywane w dalszym obliczeniach.

```csharp
public PIDController(double kp, double ki, double kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
    Reset();
}
```

### Metoda `Update`

Główną logiką działania regulatora jest metoda `Update`, która oblicza nową wartość sygnału sterującego na podstawie aktualnej wartości i wartości zadanej.

```csharp
public double Update(double setpoint, double processValue)
{
    double error = setpoint - processValue;
    integral += error * dt;
    double derivative = (error - previousError) / dt;
    double output = Kp * error + Ki * integral + Kd * derivative;
    previousError = error;
    return output;
}
```

#### Krok po kroku:

1. **Obliczenie błędu (error)**:  
   Błąd to różnica między wartością zadana (`setpoint`) a aktualną wartością procesu (`processValue`). Ten błąd jest podstawowym wskaźnikiem, który regulator używa do podejmowania decyzji sterujących.

2. **Obliczenie członu całkującego (Integral)**:  
   Człon całkujący sumuje błędy w czasie, co pozwala na eliminację błędu statycznego. W tym miejscu `integral` jest zwiększany o `error * dt`. Wartość `dt` to czas między kolejnymi aktualizacjami — jest ona konieczna do poprawnego obliczenia całki. Jeśli `dt` jest zbyt duże, integral może się przeładować, a jeśli za małe — może być niestabilny. W systemach czasu rzeczywistego, `dt` powinno być stałe lub dynamicznie obliczane.

3. **Obliczenie członu różniczkowego (Derivative)**:  
   Człon różniczkowy reaguje na szybkość zmiany błędu. Oblicza się go jako różnicę między bieżącym błędem a poprzednim błędem podzieloną przez `dt`. To pozwala na "przewidzenie" przyszłego stanu systemu i zapobiega nadmiernemu przeregułowaniu.

4. **Obliczenie wyjścia (output)**:  
   Sygnał sterujący jest sumą trzech członów:  
   $$
   \text{output} = K_p \cdot \text{error} + K_i \cdot \text{integral} + K_d \cdot \text{derivative}
   $$  
   To jest klasyczna forma obliczenia regulatora PID. Wartości `Kp`, `Ki`, `Kd` są ustawiane w zależności od charakteru systemu, np. w przypadku systemów o dużej inercji, może być potrzebne większe `Ki` lub `Kd`.

5. **Zapisanie poprzedniego błędu**:  
   Wartość `previousError` jest aktualizowana po każdej iteracji, co pozwala na obliczenie członu różniczkowego w kolejnym kroku.

### Resetowanie regulatora

Metoda `Reset()` jest używana do przywrócenia stanu regulatora do wartości początkowych:

```csharp
public void Reset()
{
    integral = 0;
    previousError = 0;
}
```

To pozwala na restartowanie regulatora w przypadku zmiany warunków działania lub konieczności zresetowania historii błędu. W systemach dynamicznych, np. przy zmianie zadania, resetowanie może być konieczne, aby uniknąć "przestawienia" regulatora w wyniku zbyt dużego błędu.

## 3. Szczegóły Techniczne

### Zależności i Dziedziczenie

Klasa `PIDController` nie dziedziczy z żadnej klasy, ani nie implementuje interfejsu. Jest to klasa samodzielna, zaprojektowana do bezpośredniego użycia w systemach sterowania. W przyszłości, w bardziej zaawansowanych systemach, można rozważyć implementację interfejsu `IController`, który zapewnia wspólny kontrakt dla różnych regulatorów, np. `PIDController`, `LQRController`, `FuzzyController`.

### Przepływ Danych

#### Wejścia:

1. `setpoint` – wartość zadana, typ `double`, jednostka zależy od kontekstu systemu (np. kąt w stopniach, prędkość w km/h, temperatura w °C).
2. `processValue` – aktualna wartość procesu, typ `double`, również jednostka zależy od kontekstu.
3. `dt` – czas między kolejnymi aktualizacjami regulatora, typ `double`, jednostka [sekundy].

#### Wyjścia:

1. `output` – wartość sygnału sterującego, typ `double`, może być np. wartością PWM (np. 0–255), lub napięciem sterującym.
2. Wartości `integral` i `previousError` są modyfikowane wewnętrznie, ale nie są zwracane — są wykorzystywane do dalszych obliczeń.

### Kluczowe Zmienne

1. `Kp`, `Ki`, `Kd` – współczynniki regulatora PID, typ `double`. Współczynniki te są ustawiane w konstruktorze i są kluczowe dla działania regulatora.
2. `integral` – zmienna przechowująca sumę błędów w czasie, typ `double`. Jest wykorzystywana do obliczenia członu całkującego.
3. `previousError` – zmienna przechowująca poprzedni błąd, typ `double`. Jest potrzebna do obliczenia członu różniczkowego.
4. `dt` – czas między kolejnymi krokami, typ `double`. W systemach czasu rzeczywistego powinien być stały lub dynamicznie obliczany, aby zapewnić stabilność regulatora.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### Wzorce i Architektura

W tym przypadku nie stosujemy jawnie żadnego wzorca projektowego, ale klasa `PIDController` może być rozszerzona w kierunku wzorca **Strategy** lub **Template Method**, jeśli system będzie miał różne rodzaje regulatorów (np. PID, LQR, fuzzy). Wzorzec Strategy pozwala na dynamiczne zmienianie typu regulatora w czasie działania aplikacji.

### Analiza Code-Level

#### C++/C# Syntax Constructs

1. **Właściwości i pola prywatne**:  
   W klasie `PIDController` wszystkie zmienne są prywatne (`integral`, `previousError`), co zapewnia hermetyzację danych. W C# nie ma potrzeby deklarowania `private` — wszystkie pola są domyślnie prywatne, ale jawne użycie `private` poprawia czytelność i zgodność z konwencjami.

2. **Metoda `Update` jako funkcja obliczeniowa**:  
   Metoda `Update` jest bezstronna — nie modyfikuje stanu obiektu poza zmiennymi prywatnymi. Jest to bardzo dobre podejście w systemach sterowania, gdzie niechciane efekty uboczne mogą prowadzić do niestabilności.

3. **Wartości `Kp`, `Ki`, `Kd` jako parametry**:  
   Wartości te są przekazywane w konstruktorze, co pozwala na dynamiczne konfigurowanie regulatora. W systemach złożonych, np. w robotyce, może być potrzebne ustawienie tych wartości z zewnątrz (np. z konfiguracji), co może być zaimplementowane poprzez dodatkowe metody lub konfigurację.

#### Potencjalne ryzyka i ich zarządzanie

1. **Nieprawidłowe `dt`**:  
   Jeśli `dt` jest nieprawidłowe (np. zero lub bardzo małe), może to prowadzić do dzielenia przez zero lub niestabilności w członie różniczkowym. W systemach czasu rzeczywistego, warto zabezpieczyć się przed tym przypadkiem, np. sprawdzając `if (dt > 0)`.

2. **Przepełnienie całki (integral overflow)**:  
   Jeśli system działa długo i błąd jest stały, `integral` może się bardzo zwiększyć, co może prowadzić do przepełnienia. W praktyce, często stosuje się tzw. **windup protection**, czyli ograniczenie zakresu `integral` lub resetowanie go w przypadku przekroczenia pewnego progu.

3. **Brak synchronizacji w systemach wielowątkowych**:  
   Jeśli `PIDController` będzie używany w środowisku wielowątkowym (np. w systemie z wieloma wątkami sterującymi), konieczne będzie zastosowanie mechanizmów synchronizacji (np. `lock`, `Monitor`, `Interlocked`) przy dostępie do `integral` i `previousError`.

4. **Brak walidacji danych wejściowych**:  
   Brak walidacji `setpoint` i `processValue` może prowadzić do nieprzewidywalnego zachowania. W systemach produkcyjnych warto dodać walidację, np. `if (double.IsNaN(error))` lub `if (double.IsInfinity(error))`.

### Podsumowanie

Klasa `PIDController` to solidna implementacja regulatora PID, która może być używana w systemach sterowania dynamicznego. Dla juniorów, warto zrozumieć, że regulator PID to nie tylko wzór matematyczny, ale też narzędzie do stabilizacji systemów, które może być podstawą do bardziej zaawansowanych algorytmów sterowania. Dla seniorów, ważne jest, aby zrozumieć, jak `dt` wpływa na stabilność, jak można zabezpieczyć się przed przepełnieniami całki, oraz jak implementować rozszerzenia (np. zabezpieczenia przed windup).