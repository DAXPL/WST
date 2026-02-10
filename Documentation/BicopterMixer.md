# Dokumentacja Szczegółowa: PIDController.cs

## 1. Kontekst i Przeznaczenie

Plik `PIDController.cs` zawiera implementację regulatora PID (Proporcjonalno-Różniczkowo-Integralnego), który jest jednym z najpopularniejszych i najbardziej szeroko stosowanych algorytmów sterowania w systemach automatyki i robotyki. Regulator ten służy do regulacji wartości wyjściowej (np. mocy silnika, temperatury, położenia) w taki sposób, aby osiągnąć żądaną wartość (setpoint) z minimalnym błędem i bez nadmiernych oscylacji. W kontekście systemu, który ten plik implementuje, regulator PID może być używany jako komponent centralny w sterowaniu dynamicznym procesów fizycznych, np. w systemach sterowania ruchem, regulacji temperatury, czy sterowaniu pozycją mechaniczną.

Klasa `PIDController` działa jako **komponent cykliczny** w systemie sterowania, gdzie cykl działania polega na odbieraniu wartości aktualnej (procesu), porównywaniu jej z wartością docelową, a następnie generowaniu sygnału sterującego, który przekazywany jest do urządzenia sterowanego. Regulator ten może być używany w różnych kontekstach, np. jako część systemu sterowania autonomicznego pojazdu, sterowania temperaturą w systemie HVAC, czy sterowania pozycją w robotyce przemysłowej.

W kontekście architektury systemu, `PIDController` może być rozpatrywany jako komponent **wewnętrznego sterownika**, który nie ma bezpośredniego dostępu do zewnętrznych urządzeń, ale zwraca sygnał sterujący, który jest następnie przekazywany do systemu zewnętrznego (np. przez interfejs PWM, sterownik silnika, czy moduł komunikacyjny). Dlatego jego implementacja musi być **niezawodna, precyzyjna i wydajna**, ponieważ błędy w obliczeniach mogą prowadzić do niestabilności całego systemu.

## 2. Analiza Logiki Biznesowej (Deep Dive)

### 2.1. Inicjalizacja regulatora

Klasa `PIDController` posiada konstruktor, który inicjalizuje wartości parametrów regulatora PID: `Kp` (współczynnik proporcjonalny), `Ki` (współczynnik całkujący), `Kd` (współczynnik różniczkujący) oraz `outputMin` i `outputMax`, które definiują zakres możliwego wyjścia regulatora. Te wartości są ustawiane z zewnątrz, co pozwala na dynamiczne dostosowanie działania regulatora do konkretnego procesu.

Wartości te są przechowywane w zmiennych prywatnych, które są używane w metodzie `Update`, która jest wywoływana w cyklu sterowania. W tym miejscu następuje obliczenie błędu między wartością docelową a aktualną (`error = setpoint - processValue`), a następnie wyliczenie wartości sterującej `output`.

### 2.2. Obliczenia PID

Metoda `Update` wykonuje następujące kroki:

1. **Obliczenie błędu**:  
   `error = setpoint - processValue`  
   Jest to podstawowe równanie, które mówi, jak bardzo obecna wartość procesu różni się od żądanej. Wartość ta może być zarówno dodatnia, jak i ujemna, co pozwala na odpowiednie sterowanie w obu kierunkach.

2. **Obliczenie członu całkującego (Integral)**:  
   `integral += error * dt`  
   Człon całkujący akumuluje błędy z poprzednich kroków, co pozwala na eliminację błędu statycznego (tzw. błędu steady-state). Wartość `dt` (delta time) jest czasem między kolejnymi krokami cyklu sterowania, co zapewnia, że akumulacja błędu jest proporcjonalna do czasu.

3. **Obliczenie członu różniczkującego (Derivative)**:  
   `derivative = (error - previousError) / dt`  
   Człon różniczkujący reaguje na szybkość zmiany błędu, co pozwala na "przewidywanie" przyszłych zmian i ograniczenie nadmiernych oscylacji. W praktyce może być stosowany z filtrowaniem, aby uniknąć szumów.

4. **Obliczenie wyjścia regulatora**:  
   `output = Kp * error + Ki * integral + Kd * derivative`  
   Wartość `output` jest następnie ograniczona do zakresu `[outputMin, outputMax]` (zastosowanie `Math.Clamp` lub podobnego mechanizmu), aby zapewnić, że sygnał sterujący nie przekracza dopuszczalnych granic.

5. **Zapisanie błędu poprzedniego**:  
   `previousError = error`  
   To pozwala na obliczenie członu różniczkującego w kolejnym cyklu.

### 2.3. Cykl działania regulatora

W systemie sterowania, `PIDController` jest wywoływany w regularnych odstępach czasu, np. co 10 ms, w zależności od częstotliwości cyklu sterowania. Zatem `dt` (czas między krokami) jest stale aktualizowany i musi być precyzyjny, aby obliczenia były stabilne.

Jeśli `dt` jest zbyt duże, człon całkujący może się za bardzo akumulować, co prowadzi do nadmiernego reagowania regulatora. Jeśli `dt` jest zbyt mały, człon różniczkujący może być niestabilny lub szumny.

## 3. Szczegóły Techniczne

### 3.1. Zależności i Dziedziczenie

Klasa `PIDController` **nie dziedziczy** z żadnej klasy w tym pliku, co sugeruje, że jest to klasa samodzielna, używana jako komponent w systemie sterowania. Może być jednak implementowana jako komponent w systemie, który korzysta z interfejsów, np. `IController`, który definiuje wspólny kontrakt dla różnych regulatorów (np. PID, LQR, fuzzy).

W takim przypadku, `PIDController` może implementować interfejs `IController`, który może wyglądać mniej więcej tak:

```csharp
public interface IController
{
    double Update(double setpoint, double processValue);
}
```

### 3.2. Przepływ Danych

#### Wejścia:

- `setpoint` – wartość docelowa (np. 100°C)
- `processValue` – aktualna wartość procesu (np. 98.5°C)
- `dt` – czas od ostatniego wywołania (np. 0.01s)
- `Kp`, `Ki`, `Kd` – parametry regulatora (ustawiane z zewnątrz)
- `outputMin`, `outputMax` – zakres wyjścia regulatora

#### Wyjścia:

- `output` – wartość sygnału sterującego, która może być np. przekazywana do sterownika PWM, sterownika silnika, czy innego urządzenia.
- Wartość `output` jest ograniczona do zakresu `[outputMin, outputMax]`, co zapobiega nadmiernemu reagowaniu regulatora.

### 3.3. Kluczowe Zmienne

- `Kp`, `Ki`, `Kd` – parametry regulatora, które kontrolują wpływ odpowiednich członów PID.
- `integral` – zmienna akumulująca błędy, używana do obliczenia członu całkującego.
- `derivative` – różnica błędu między kolejnymi krokami, używana do obliczenia członu różniczkującego.
- `previousError` – przechowuje błąd z poprzedniego kroku, potrzebny do obliczenia członu różniczkującego.
- `outputMin`, `outputMax` – ograniczenia wyjścia regulatora, zapewniające stabilność systemu.

## 4. Kącik Edukacyjny (Mentoring) 🎓

### 4.1. Wzorce i Architektura

Klasa `PIDController` może być rozpatrywana jako implementacja wzorca **Strategy** w kontekście sterowania – czyli obiekt, który definiuje sposób działania (algorytm) i może być używany w różnych kontekstach. W systemach sterowania, wzorzec ten pozwala na łatwe zamienianie regulatorów – np. z PID na fuzzy controller, bez zmiany logiki systemu.

Dodatkowo, w bardziej zaawansowanych systemach, `PIDController` może być częścią wzorca **Observer**, gdzie system sterowania obserwuje zmiany w wartościach procesu i reaguje na nie. Warto zauważyć, że `PIDController` nie implementuje tego wzorca samodzielnie, ale może być używany przez obserwatory lub systemy, które z niego korzystają.

### 4.2. Analiza Code-Level

#### 4.2.1. C++/C# Syntax Constructs

- `double Update(double setpoint, double processValue)` – metoda publiczna, zwracająca wartość `double`. Używa referencji do wartości, ale w C# nie ma potrzeby używania `ref` w tym przypadku, ponieważ `double` jest typem wartościowym.
- `integral += error * dt` – operator skrócony (`+=`) – bardzo popularny w obliczeniach numerycznych, zapewnia czytelność i szybkość.
- `Math.Clamp(output, outputMin, outputMax)` – funkcja wbudowana w .NET, zapewniająca bezpieczne ograniczenie wartości wyjściowej, co jest kluczowe w systemach sterowania, aby nie przekraczać zakresu sterowania.

#### 4.2.2. Potencjalne zagrożenia i ich zaradzanie

- **Błędy w obliczeniach numerycznych**: W przypadku bardzo małych wartości `dt`, człon różniczkujący może być niestabilny lub szumny. W praktyce można stosować filtrowanie (np. filtr niskoprzepustowy) lub ograniczenie `dt` do minimalnej wartości.
- **Zależność od `dt`**: Jeśli `dt` nie jest stały (np. w systemie z nieregularnym cyklem), obliczenia mogą być niestabilne. W takim przypadku warto używać `DateTime.Now` lub zegara systemowego do dokładnego obliczania `dt`.
- **Brak resetowania integral**: W przypadku długotrwałego działania regulatora, `integral` może się zwiększać do wartości bardzo dużych, co może prowadzić do efektu "windup". W bardziej zaawansowanych implementacjach stosuje się tzw. **anti-windup**, np. ograniczenie `integral` lub jego resetowanie przy przekroczeniu określonej granicy.

#### 4.2.3. Wydajność i optymalizacja

- `PIDController` jest bardzo wydajny, ponieważ obliczenia są prostymi operacjami arytmetycznymi.
- W systemach czasu rzeczywistego, warto zadbć o to, aby `dt` był obliczany zgodnie z rzeczywistym czasem, a nie za pomocą `Thread.Sleep`, które może być niestabilne.
- W przypadku wielu regulatorów PID w systemie, warto rozważyć użycie `Span<T>` lub `Memory<T>` w celu redukcji alokacji pamięci i zwiększenia wydajności.

---

**Podsumowanie techniczne:**

`PIDController` to kluczowy komponent w systemach sterowania, który pozwala na precyzyjne regulowanie wartości procesu w czasie rzeczywistym. Jego implementacja wymaga staranności w obliczeniach, szczególnie w zakresie `dt`, `integral`, oraz `derivative`. Przy odpowiednim dostosowaniu parametrów regulatora (Kp, Ki, Kd) może zapewnić bardzo stabilne i precyzyjne działanie systemu sterowania.