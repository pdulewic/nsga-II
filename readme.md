To do:

- implementacja parsera do czytania wyrażeń matematycznych. Wyrażenie powinno być obliczane w funkcji NSGA::objectiveFunction1/2 dla zadanych argumentów (liczba argumentów od 1 do 10). Tymczasowo funkcje NSGA::objectiveFunction1/2 mają przypisaną jakąś trywialną funkcję jednej zmiennej. 
- GUI jest w stanie surowym, można by je ładniej ogarnąć, pogrupować w QGroupBox'y, dodać użytkownikowi możliwość modyfikacji niektórych parametrów z pliku constants.h (np. prawdopodobieństwa i siły mutacji)
- przetestować algorytm dla funkcji celu podanych w temacie projektu. Te funkcje mają skomplikowany zapis, więc można by dać użytkownikowi wybór: albo wpisuje własną funkcję (tu potrzebny parser), albo wybiera jedną z gotowych, bardziej skomplikowanych
