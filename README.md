# kombajn_rpm
Fortschritt E-516 kombájn fordulatszámainak megjelenítése esp32-vel

Egy 2x16 karakteres LCD kijelzővel megjelenítek 4 forulatszámértéket.
Három induktív jeladó és egy pedig HALL szenzorból sszármaznak a jelek.
A kapcsolási rajz úgy készűlt, hogy akár hat fodulatszámjelet is fogadni tudjon.
Mind a hat fogadott jel lehet HALL illetve induktív jeladóból származó is, 
ennek fügvényében kell a kívánt ellenálást vagy rövidzárat beforrasztani.
