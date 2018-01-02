# cplusplus
Wybrane programy opisujące moją znajomość języka C++

W niniejszym "repozytorium", zamieściłem 4 programy, które obrazują moją znajomość języka C++. 
Niestety ze względu na charakter moich studiów (fizyka techniczna), są to głównie programy rozwiązujące specyficzne zagadnienia numeryczne.
W folderze 2015 (początek mojego poznawania języka C++) znajdują się programy: 

1)Program do rozwiązywania układu równań, umożliwia rozwiązywanie równań z maksymalnie ok. 40 niewiadomymi. 
W programie rozwiązywany jest losowo generowany układ równań, a po otrzymaniu rozwiązania wyniki są sprawdzane przez powtórne podstawienie do układu.
Program można skompilować i uruchomić używając dołączonego pliku makefile.

2)Rozszerzenie środowiska Geant4 (popularnego narzędzia dla fizyków jądrowych i cząstek elementarnych) do symulowania detektora czasu przelotu wykorzystującego 
promieniowanie Czerenkowa. Środowisko Geant4 to oprogramowanie napisane w języku C++, do symulowania wielu zjawisk zachodzących w świecie cząstek elementarnych, 
przy pomocy statystycznych metod Monte Carlo.
Mój program wykonuje kilka czynności:
-definiuje geometrię detektora, właściwości materiałów z których jest zrobiony, pliki src/MyPrimaryGeneratorAction,
-określa lecące cząstki ich położenia i prędkości, pliki src/MyPhotonSD, src/MyPrimaryGeneratorAction,
-definiuje jakie procesy fizyczne będą symulowane, pliki src/MyPhysicsList
-opisuje zliczane statystyki, powierzchnie czułe detektora
-w pliku głównym My.cc, tworzony i uruchomiany jest obiekt klasy G4RunManager odpowiedzialny za sterowanie kolejnymi czynnościami wykonywanymi przez program, 
w tym graficzną reprezentacją przeprowadzanych symulacji
Linki:
strona przedmiotu "Środowisko programistyczne Geant4", opis wykonywanego projektu:
http://home.agh.edu.pl/~leszekad/dydaktyka/wfiis_geant4_2015/geant4_projekt_7.pdf
strona projektu Geant4:
http://geant4.web.cern.ch/geant4/


W folderze 2016 znajduje się program napisany przeze mnie do pracy inżynierskiej, tytuł pracy to:
"Wyznaczanie wewnątrzpasmowych oraz międzypasmowych stałych oddziaływania spin-orbita typu Rashby w półprzewodnikowych studniach kwantowych".
W praktyce oznacza to napisanie programu pozwalającego numerycznie obliczać strukturę elektronową układu wiążącego ładunek elektryczny 
w tzw. kropkach/studniach kwantowych. 
Ze względu na użycie niestandardowej biblioteki do obliczeń algebraicznych na macierzach (jej nazwa to "armadillo"), 
uruchomienie programu nie jest możliwe w szybki sposób.
Struktura programu:
-plik główny, w którym przeprowadzane są symulacje struktury elektronowej układu "Program.cpp" - wyniki symulacji wypisywane są do odpowiednich plików tekstowych.
-pliki tekstowe przetwarzane są przez program "Obrobka.cpp", który wylicza wspomniane stałe oddziaływania
-dla wygody w uruchamianiu programy z różnymi parametrami, oba pliki korzystają z plików konfiguracyjnych obsługiwanych przez "PropertyFileConfiguration.h"

W folderze 2017 zamieściłem rozszerzenie "MF_CurrentFlowEvolver" do środowiska obliczeniowego OOMMF (obiect oriented micro magnetic framework, stona http://math.nist.gov/oommf/doc/userguide12a5/userguide/),
rozwijane przeze mnie w ramach pracy magisterskiej. 
Rozszerzenie to było częścią pracy doktorskiej mojego opiekuna dr Marka Frankowskiego, a ja w ramach pracy magisterskiej kontynuuje rozwój tego rozszerzenia. 
Tematem mojej pracy magisterskiej jest:
"Model mikromagnetyczny pracy magnetycznego złącza tunelowego z wielokrotną barierą izolatora". Temat związany jest z magnetycznymi pamięciami RAM, tzw. MRAM.
Pierwotna wersja rozszerzenia umożliwiała symulowanie jednokrotnej warstwy izolatora w układzie, jednak współczesne pamięci MRAM,
wymagają zastosowania dwóch barier izolatora - stąd potrzeba uogólnienia. Oryginalna wersja rozszerzenia dostępna jest na stronie: 
http://layer.uci.agh.edu.pl/M.Frankowski/download.html
Edytowane przeze mnie fragmenty kodu to głównie:
dodanie klas:
MagneticAtlas, Boundary, LinksBetweenTwoInterfaces,
edycja lub dodanie metod:
CheckBoundaryParameters, RegisterAtlas, getMagneticAtlasByRegion, getBoundaryByName, RegisterBoundary, LinkTwoSurfaces, FillLinkList 
(nie zachowano konwencji mówiącej o nazywaniu metod zaczynając od małej litery - zrobiono tak by nie zmieniać istniejącego porządku nazewniczego).

PODSUMOWANIE

Moje doświadczenie związane z językiem C++ opiera się głównie na rozwiązywaniu różnego typu zagadnień numerycznych. 
Potrafię tworzyć małe programy od podstaw, lub korzystać z rozwiniętych narzędzi napisanych w języku C++. 
Dotychczas nie zajmowałem się tematyką związaną z systemami wbudowanymi.

