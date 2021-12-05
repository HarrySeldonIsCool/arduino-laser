# arduino-laser

Tohle je muj zdrojak k reseni [science challenge](https://www.sciencechallenge.cz), v src je několik podsložek.

## graphs

tady jsou všechny grafy, graph1.svg je z testu, graph.svg se vytvoří při normálním spuštění.

## krystof/ardu

tady je c++ kód pro arduino, který napsal Kryštof a já ho upravil.

Potom jsou tu už samostatný soubory:

## arduino.rs

Tady se odehrává všechno přesnášení dat rozdělené na několik šikovných funkcí. Všude (i tady) používám f32, ale překvapivě ne kvůli strachu o místo, ale protože arduino přímo nepodporuje double, a nechce se mi převádět f32 na f64 až po přenosu, bylo by to namáhavý a matoucí.

## process.rs

Původně byl součástí arduino.rs, ale potom zabíral moc místa a tak jsem ho přesunul do zvláštní složky, a i když je teď mnohem menší, nechce se mi ho dávat zpět.

## math.rs

Většinu této složky tvoří budování matematiky pro typ Vector, což je wraper pro Vec<f32>. Na konci je potom funkce nedler_mead, což je implementace matematické metody [stejného jména](https://en.wikipedia.org/wiki/Nelder–Mead_method) pro minimalizaci hodnot nějaké křivky.

## main.rs

Tady už jen zavolám funkci z arduino.rs a vypíšu výsledek. Dole je taky testovací funkce, mám v plánu jí přesunout do samostatné složky.

V endian_traits.rs už je jen tvorba a implementace trait pro převádění z bytů na čísla, protože normálně má každé číslo vlastní metodu (takže nemůžu psát genericky).
