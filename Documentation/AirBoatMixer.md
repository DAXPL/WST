# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Plik `PIDController.cs` definiuje klasę `PIDController`, która implementuje **sterownik proporcjonalno-różniczkowo-integralny (PID)** w kontekście systemów sterowania automatycznego. Jest to kluczowa komponent systemu, który przetwarza sygnał błędu (różnicę między wartością docelową a aktualną) i generuje sygnał sterujący do regulatora (np. silnika, zaworu, czy innego elementu sterowanego). W kontekście systemu, ten sterownik może być używany w aplikacjach takich jak sterowanie temperaturą, pozycją, prędkością lub innymi wielkościami fizycznymi, które wymagają stabilnego i precyzyjnego sterowania.

Klasa `PIDController` jest częścią większego systemu sterowania, który może być wykorzystywany w systemach embedded (np. mikrokontrolery), w aplikacjach przemysłowych, lub w symulatorach. W systemie, `PIDController` może być używany jako komponent w architekturze zorientowanej na komponenty (component-based architecture), gdzie każdy komponent odpowiada za konkretną funkcję (np. kontrola temperatury, kontrola położenia, kontrola prędkości). Klasa ma cykl życia, który obejmuje inicjalizację parametrów PID, aktualizację wartości wejściowych oraz obliczenie nowej wartości wyjściowej w każdej iteracji (np. w pętli sterowania).

## 2. Analiza Logiki Biznesowej (Deep Dive)

Klasa `PIDController` implementuje algorytm PID, który działa w sposób iteracyjny w czasie. Przyjmuje ona wartość docelową (`Setpoint`), aktualną wartość (`Input`) oraz oblicza sygnał sterujący (`Output`) na podstawie trzech składników: proporcjonalnego (P), całkującego (I) i różniczkującego (D).

### Proces działania PID:

1. **Obliczenie błędu**: `error = setpoint - input`
   - Jest to podstawowy krok, który określa, jak bardzo obecna wartość różni się od docelowej.
   - W tym miejscu zaczyna się cykl obliczeniowy.

2. **Obliczenie członu całkującego (Integral)**:
   - `integral += error * dt`
   - Wartość całkująca akumuluje błędy z poprzednich kroków, co pozwala na eliminację błędów ustalonych.
   - Wartość `integral` jest mnożona przez `Ki` (współczynnik całkujący), a następnie dodawana do wyniku.

3. **Obliczenie członu różniczkującego (Derivative)**:
   - `derivative = (error - lastError) / dt`
   - Oblicza się szybkość zmiany błędu. Jest to pomocne przy redukcji nadmiarowego wzmocnienia i zapobieganiu oscylacjom.
   - Wartość `derivative` jest mnożona przez `Kd` (współczynnik różniczkujący).

4. **Obliczenie całkowitego wyjścia**:
   - `output = Kp * error + Ki * integral + Kd * derivative`
   - Wynik jest sumą trzech członów PID.
   - Wartość może być ograniczona przez `minOutput` i `maxOutput`, aby zapobiec nadmiernemu wzmocnieniu.

### Przebieg metody `Update`:

Metoda `Update(double input, double dt)` odpowiada za główny cykl obliczeniowy PID. W niej następuje:

1. Obliczenie błędu.
2. Aktualizacja `integral` i `derivative`.
3. Obliczenie `output` zgodnie z formułą PID.
4. Ograniczenie wartości `output` do zakresu `[minOutput, maxOutput]`.
5. Zapisanie błędu do `lastError` dla kolejnego kroku.

W tym miejscu należy zauważyć, że `dt` (czas kroku) jest kluczowym parametrem, który wpływa na dokładność obliczeń. Jeśli `dt` jest zbyt duże, obliczenia mogą być niestabilne. Jeśli za małe, może to prowadzić do obciążenia systemu.

## 3. Szczegóły Techniczne

### Zależności i Dziedziczenie:

Klasa `PIDController` nie dziedziczy z żadnej klasy, ale implementuje interfejs `IPIDController`. Interfejs ten definiuje:

```csharp
public interface IPIDController
{
    double Update(double input, double dt);
    void SetParameters(double kp, double ki, double kd);
    void SetOutputLimits(double min, double max);
}
```

Dzięki temu interfejsowi, `PIDController` może być używany w kontekście innych komponentów, które oczekują obiektu typu `IPIDController`. To zapewnia elastyczność i umożliwia łatwe testowanie oraz wstrzykiwanie zależności (dependency injection).

### Przepływ Danych:

#### Wejścia:
- `double input`: Wartość aktualna (np. temperatura, położenie) – typ `double`, jednostka zależy od kontekstu (np. stopnie Celsjusza, kąt w stopniach).
- `double dt`: Czas kroku (delta czasu) – typ `double`, jednostka sekundy (s).
- `double setpoint`: Wartość docelowa – typ `double`, jednostka taka sama jak `input`.

#### Wyjścia:
- `double output`: Wartość sterująca – typ `double`, zakres ograniczony przez `minOutput` i `maxOutput`.
- Metoda `Update` zwraca wartość `output` typu `double`.

### Kluczowe Zmienne:

- `double Kp`: Współczynnik proporcjonalny – determinuje wpływ błędu na wyjście.
- `double Ki`: Współczynnik całkujący – determinuje wpływ akumulowanego błędu.
- `double Kd`: Współczynnik różniczkujący – determinuje wpływ zmiany błędu.
- `double integral`: Akumulowana wartość błędu – używana do obliczenia członu całkującego.
- `double derivative`: Różnica między bieżącym a poprzednim błędem – używana do obliczenia członu różniczkującego.
- `double lastError`: Poprzednia wartość błędu – potrzebna do obliczenia pochodnej.
- `double minOutput`, `double maxOutput`: Ograniczenia wyjścia – zapobiegają nadmiernemu wzmocnieniu.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### Wzorce i Architektura:

Klasa `PIDController` jest przykładem **implementacji wzorca projektowego "Strategy"** w kontekście sterowania. Wzorzec ten pozwala na użycie różnych algorytmów sterowania w taki sposób, że system nie musi wiedzieć, jak dokładnie działa PID, ale tylko że implementuje `IPIDController`. Dzięki temu, w przyszłości można łatwo wdrożyć inne algorytmy sterowania (np. fuzzy logic, LQR), bez konieczności modyfikowania kodu korzystającego z tego sterownika.

Wzorzec ten pozwala również na lepsze **testowanie jednostkowe** – można stworzyć mock `IPIDController` i przetestować logikę sterowania bez konieczności fizycznego działania regulatora.

### Analiza Code-Level:

#### `virtual` i `override`:
W tym przypadku klasa `PIDController` nie jest `virtual`, ale implementuje interfejs `IPIDController`. W kontekście architektury, `virtual` byłby użyteczny, gdyby klasa miała być dziedziczona i nadpisywana, np. w przypadku `AdvancedPIDController`, który implementuje dodatkowe funkcje (np. anti-windup).

#### `double dt` i jego wpływ:
Wartość `dt` (delta time) jest kluczowa dla poprawności obliczeń. W systemach czasu rzeczywistego, `dt` może być niezależne od czasu działania systemu, ale w przypadku symulatorów może być stałe. Warto zwrócić uwagę, że `dt` nie może być zerem, ponieważ prowadziłoby to do błędów dzielenia przez zero. W kodzie nie ma jawnej walidacji, co może być zagrożeniem w środowiskach niezabezpieczonych.

#### `integral` i `derivative`:
- `integral` może prowadzić do **"windup"** – czyli akumulacji błędów w sytuacjach, gdy wyjście jest ograniczone. W bardziej zaawansowanych implementacjach stosuje się **anti-windup**, np. ograniczenie `integral` lub jego resetowanie.
- `derivative` może być niestabilny, jeśli `dt` jest zbyt małe lub jeśli dane wejściowe są szumne. W praktyce stosuje się **filtrację** lub **pochodną ograniczoną**.

#### `SetParameters`, `SetOutputLimits`:
Te metody pozwalają na dynamiczną konfigurację regulatora – czyli w czasie działania systemu można dostosować parametry PID. To pozwala na **adaptacyjne sterowanie** – np. w zależności od warunków otoczenia.

#### Zmienne `lastError` i `integral`:
Są zmiennymi stanowymi – ich wartość zmienia się w czasie i jest wykorzystywana w kolejnych iteracjach. W kontekście wielowątkowości, takie zmienne mogą być źródłem **race condition** – jeśli `Update` jest wywoływany z wielu wątków bez synchronizacji. W tym kodzie nie ma mechanizmów synchronizacji, co może być problematyczne w środowiskach wielowątkowych.

#### `Output` jako `double`:
Wartość wyjściowa jest typu `double`, co zapewnia dużą dokładność obliczeń. W systemach embedded, gdzie pamięć jest ograniczona, można rozważyć użycie `float` lub `fixed-point` do oszczędzania zasobów.

---

### Podsumowanie:

Klasa `PIDController` to solidna implementacja sterownika PID, zbudowana z myślą o skalowalności, testowalności i elastyczności. Warto zwrócić uwagę na obsługę `dt`, zmiennych stanowych (`integral`, `derivative`) oraz potencjalne zagrożenia w środowiskach wielowątkowych. Implementacja może być rozbudowana o funkcje takie jak anti-windup, filtracja pochodnej, lub dynamiczne dostosowanie parametrów, w zależności od kontekstu systemowego.