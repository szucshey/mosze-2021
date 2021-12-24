## Modern szoftverfejlesztési eszközök 2021

### 0. Feladat: Ennek a felsorolásnak a felülírása a következő adatokkal:

1. Szűcs Dávid - szucshey
2. Bogáth Máté - hihetetlentehin
3. Őri Viktória - Viki0929
4. Kozma Zsolt Alex - Zsolti05

### 1. Feladat (határidő a PR nyitására: 10.06.)

A parancssoros táblázatkezelő program alábbi funkcióit kell implementálni egy **külön branchen**, majd nyitni belőle egy PR-t (Pull Requestet) a main branchre. Itt reviewernek fel kell kérni @oliverosz oktatót, aki majd vagy értékeli, vagy meghív egy másik oktatót értékelésre.

Elvárt funkciók:

* Tetszőleges méretű, egysoros szöveget tartalmazó cellákból álló táblázat tárolása
  * A program indításakor 1x1 méretű, 1 üres cellával
* Megjelenítés:
  * Indításkor és minden művelet után legyen megjelenítve (stdout-ra kiírva) a táblázat
  * A sorok legyenek számozva, az oszlopok nagy betűkkel jelölve (nem kell 26-nál több oszlopot kezelni)
  * Egy sor oszlopait 1-1 tabulátor karakter válassza el egymástól (hosszabb szövegeknél el fog csúszni, de most még ezzel nem kell foglalkozni)
* Műveletek (stdin-ről beolvasva):
  * `edit XY string` írja be a megadott stringet az XY koordinátájú cellába (pl. A2), a stringben lehetnek szóközök, de sortörés nem
  * `add N rows/cols` adjon hozzá N db új, üres cellákból álló sort/oszlopot a táblázat végéhez
  * `delete X/Y` törölje az X oszlopot vagy Y sort (ha betű, akkor oszlop, ha szám, akkor sor)
  * `insert N rows/cols before/after X/Y` mint az `add`, csak megadott pozícióba szúrjon be
  * `exit` lépjen ki a program
  * Bármelyik műveletnél hibás koordináta vagy paraméter esetén írjon hibaüzenetet (nem kell minden lehetséges hibára felkészíteni, de azért legyen input ellenőrzés)

### 2. Feladat (határidő a PR nyitására: 10.20.)

Ezt is (és minden további feladatot) egy új branchen kell megoldani. Ha az előző feladatos branch még nincs mergelve a mainbe, ezért abból kell leágaztatni az új branchet, nem a mainből.

A feladatokat érdemes szétosztani a csapattagok között, ehhez lehet akár több külön branchet is létrehozni, de a végén 1 branchbe legyenek egyesítve, és abból készüljön egy PR. Ha az 1. feladatos branch még nem lett mergelve a mainbe, akkor ez lesz a PR target branch, különben a main.

* A táblázatkezelő tudjon beolvasni egy táblázatot parancssori argumentumként megadott CSV fájlból ([példa](example.csv))
  * Alapból a pontosvesszőt tekintse elválasztó karakternek, de egy opcionális `-sep` kapcsolóval meg lehessen adni más karaktert is, pl.: `./prog table.csv -sep ,`
  * Ha a fájl soraiban nem egyforma számú cellák vannak, a rövidebb sorok végei legyenek üres cellákkal kiegészítve
  * A cellák végén lévő whitespace-eket hagyja figyelmen kívül
* A programban legyen egy `save filename.csv [-sep ,]` parancs, ami CSV fájlba írja ki az aktuális táblázatot
  * Ha létezik a megadott fájl, írja felül
  * Alapból pontosvesszőt használjon az értékek elválasztására, de a `-sep c` megadásával lehessen ezt változtatni
* Az elválasztó karakterek escape-elésével nem kell foglalkozni, feltételezhető, hogy a cellákban nem szerepel az elválasztó karakter
* Megjelenítés javítása:
  * Minden sorban egyforma szélességűek legyenek az egy oszlophoz tartozó cellák, a legszélesebb cella tartalmához igazodva (tartalom balra igazítva)
  * Jelenjenek meg cellaszegélyek '-' és '|' karakterekből, pl.:
```
 |A  |B    |
------------
1|Key|Value|
------------
2|0  |1    |
------------
3|1  |     |
------------
4|2  |3    |
------------
```
* Készüljön egy [GitHub Actions Workflow](https://docs.github.com/en/actions):
  * Minden push eseményre fordítsa le a kódot és végezzen egy input/output tesztet
  * Legyen egy `input.txt` fájl, ami a felhasználó által bevitt parancsokat tartalmazza, és egy elvárt kimenet, amivel összehasonlítja a program által kiírt szöveget

### 3. feladat (határidő a PR nyitására: 10.27.)

Dokumentáció:
* A függvények és osztályok legyenek ellátva Doxygen kommentekkel, ez a későbbi feladatokra is vonatkozik
* A GitHub Actions workflow-ba kerüljön bele egy új job, ami Doxygennel automatikusan legenerálja a dokumentációt, és artifactokként elérhetővé teszi HTML-ben és PDF-ben
  * A runnerre a szükséges előfeltételeket csomagkezelőből lehet telepíteni, pl.: `run: sudo apt install doxygen graphviz texlive`

Új funkciók:
* `sort by X/Y [asc/desc (default: asc)]` rendezze a táblázat sorait a megadott oszlop szerint (vagy oszlopait a megadott sor szerint) növekvő/csökkenő sorrendbe
  * A szöveges cellák string-összehasonlítással, a numerikus cellák számérték alapján legyenek összehasonlítva, egy szöveges és egy numerikus cella közül pedig mindig a szöveg a "kisebb"
    * pl.: `"" < "Abc" < "Abcd" < "Bcd" < 0 < .5 < 4 < 11 < 100.1`
  * Nem kell automatikusan újra rendezni a tartalom változása esetén
* `swap MN XY` felcseréli a megadott két cella tartalmát
* `align XY left/right` beállítja, hogy balra/jobbra igazítva legyen megjelenítve a cella tartalma
  * A cellák kezdeti igazítása tetszőlegesen megválasztható
* Range műveletek:
  * Az `MN:XY` formában megadott cellatartomány két szemközti sarka `MN` és `XY`, pl. az ábrán kijelölt range megadható úgy, hogy `B2:D4`, `D4:B2`, `B4:D2`, vagy `D2:B4` ![Range example](range.png)
  * `align MN:XY left/right` egyszerre több cellának állítja be az igazítását
  * `clear MN:XY` üresre állítja a cellák tartalmát

### 4-6. feladatok

Ez a 3 feladat nem épül egymásra, párhuzamosan lehet rajtuk dolgozni, de mivel az előadásokon elhangzó tananyagra épülnek, azt a sorrendet követik a határidők.

A 3 feladat mindegyikéhez legyen 1-1 különböző csapattag kijelölve felelős fejlesztőnek, aki a PR-ben legyen assignee-nak jelölve. Továbbá legyen 1-1 tesztelő és code reviewer is kijelölve és a descriptionben megadva (ezeknek a szerepeknek a felosztása tetszőleges, csak a felelős fejlesztőtől térjen el).

A tesztelő feladata ellenőrizni a helyes működést, manuális és automatikus tesztek elvégzésével. A code reviewer feladata a kódminőség ellenőrzése és javítása. Először ő írjon code review-t, és csak utána legyek én felkérve review-ra (@oliverosz).

## 4. feladat (határidő a PR nyitására: 11.17.)

Az új funkcióhoz a tesztelő készítsen egységteszteket.

Új funkció: aggregátor függvények támogatása

* Ha egy cellába a user egy aggregátor függvényt ír, pl.:`=SUM(MN:XY)`, akkor megjelenítésnél a függvény eredménye jelenjen meg a cellában
  * Aggregátor függvények:
    * `SUM` összeg
    * `AVG` átlag
    * `MIN` minimum
    * `MAX` maximum
* A paraméter egy tartomány, ennek nem kell változnia, ha törlésre kerülnek hivatkozott cellák, ha érvénytelen a tartomány (pl. mert kilóg a táblázatból), akkor jelenjen meg a cellában a `#NAME?` szöveg
* A tartományban szereplő nem-numerikus cellákat (amire az `std::stof` exceptiont dob, és nem függvényt tartalmaz) hagyja figyelmen kívül a függvény, tehát pl. az átlag nevezőjébe ne számítsanak bele
* A számított értékek fixen 3 tizedesjegy pontossággal jelenjenek meg (`std::cout << std::fixed << std::setprecision(3)`)
* A függvényt tartalmazó cellák fájlba mentésnél is képletként legyenek mentve, és fájlbetöltés után is működjenek
* A megadott tartományban is lehetnek függvényt tartalmazó cellák, ekkor azok számított értékét használja (ezt érdemes a cella eredeti szöveges tartalmától független módon tárolni)
  * Ha körkörös hivatkozás van, akkor a körben résztvevő cellákban a `#CYCLE!` szöveg jelenjen meg
  * Egy lehetséges megoldás vázlata:
    1. Minden XY cellára: `evaluated[XY] = true`
    2. Minden XY cellára, ami függvényt tartalmaz: `evaluated[XY] = false`
    3. Amíg van XY cella, amire `!evaluated[XY]`:
        1. `updated = 0`
        2. Minden XY cellát, amire `!evaluated[XY]`, de a hivatkozott tartományában minden cella `evaluated`:
            * Értékeljük ki a függvényt és `evaluated[XY] = true`
            * `updated++`
        3. Ha `updated == 0`, akkor a megmaradt függvényeket nem lehet kiértékelni, az eredményüket állítsuk a `#CYCLE!` szövegre

## 5. feladat (határidő a PR nyitására: 11.24.)

* Készüljön egy új, `barchart MN:XY filename` parancs a programban, ami a megadott tartományból előállít egy csoportosított oszlopdiagramot ábrázoló SVG ábrát
  * A tartomány első oszlopában lévő cellák alkossák az X-tengely pontjainak feliratait, az első sor pedig az adatsorok neveit
  * A kimenet legyen egy `filename.svg` ábra, vagy egy `filename.html` weblap egy beágyazott SVG-vel és a megjelenítésért felelős JS, CSS, stb. kóddal
  * Az SVG elkészíthető saját implementációval vagy tetszőleges 3rd-party library-t / segédprogramot fel szabad hozzá használni
* Készüljön egy Docker image, ami tartalmaz minden programot, amire szükség van a projekt fordításához, dokumentáció generálásához, program futtatásához
  * A Dockerfile kerüljön be a csapat repójába, az image pedig legyen publikálva a Docker Hub-ra
  * A GitHub Actions workflow használja ezt az image-et, és ne telepítsen semmilyen további csomagot

## 6. feladat (határidő a PR nyitására: 12.01.)

* A program tudjon kezelni több megnyitott táblázatot
  * Megjelenítés:
    * Egyszerre egy táblázat aktív, csak ennek a cellái jelennek meg
    * Az aktív táblázat alatt legyenek listázva a megnyitott táblázatok indexei és nevei, az aktív táblázatot *-gal jelölve, pl.: `0: Nevek  1*: Cimek  2: Telefonszamok`
  * Ha induláskor parancssori argumentumként megadott fájlból lett beolvasva a táblázat, akkor a neve legyen a fájlnév, ha üres táblázattal lett elindítva, akkor ennek a neve legyen `Table`
  * A táblázatok közötti egymásra hivatkozásokat nem kell kezelni
  * Új parancsok:
    * `new sheet name` létrehoz egy új táblázatot a megadott névvel, és beállítja aktívnak
    * `open filename [-sep ,]` létrehoz egy új táblázatot, betölti a megadott fájlból, a nevét beállítja a fájlnévre, és beállítja aktívnak
    * `close N` bezárja az N indexű táblázatot, és felszabadítja az általa foglalt memóriát
      * Az N-nél nagyobb indexű táblázatok indexe 1-gyel csökken
      * Ha az utolsó megnyitott táblázat is be lett zárva, akkor lépjen ki a program
    * `rename N newname` átnevezi az N indexű táblázatot a megadott új névre
* A CI workflow egészüljön ki egy olyan teszttel, ami AddressSanitizer vagy Valgrind/Memcheck segítségével ellenőrzi, hogy nincs memóriaszivárgás vagy egyéb memóriahiba, és hiba esetén jelölje Failed-nek a workflow-t

### 7. feladat (TBD)

További programozási feladat legfeljebb szorgalmiként, 5-ös megajánlott jegyért lesz, de a Makefile témakörhöz kapcsolódó kisebb feladat még várható.