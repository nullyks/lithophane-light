# Testimisplaan

## Esmased kontrollid

- [ ] Sketch kompileerub NodeMCU 1.0 (ESP-12E Module) jaoks.
- [ ] Kõik 40 LED-i põlevad staatilises režiimis.
- [ ] SoftAP `Lithophane-Light` ilmub ja WPA2 parool toimib.
- [ ] `http://192.168.4.1/` avaneb nii telefonis kui arvutis.
- [ ] Staatilist RGB värvi saab muuta 24 kiirvalikuga ja süsteemse värvivalijaga.
- [ ] Kõik 6 režiimi töötavad (staatiline + viis animatsiooni).
- [ ] Heledus, kiirus ja tugevus mõjutavad sobivaid režiime.
- [ ] Kiire liuguri liigutamine ei peata veebiserverit ega animatsiooni.
- [ ] LED-ide eelvaate plokki ei kuvata.
- [ ] 3 sekundit pärast muutmist taaskäivitamisel taastuvad seadistused.
- [ ] Sisse-/väljalülitus säilib pärast taaskäivitust.
- [ ] `REVERSE_SPIRAL` muudab tuleleegi kõrgusjaotust.
- [ ] Valgus jääb litofaani taga hajusaks ja üksikud LED-id ei paista häirivalt läbi.

## Ohutus ja usaldusväärsus

- [ ] Asendada kasutaja kinnitatud 4 × 1,5 V AA otsetoide ribale sobiva reguleeritud toitega enne koormuskatseid; tuvastada LED-riba täpne mudel ja lubatud pinge.
- [ ] Tuvastada patareide keemia ning kontrollida patareikarbi ja juhtmete koormustaluvust.
- [ ] Patareitoite kasutamisel kontrollida pingemuunduri väljundit värskete ja tühjenevate patareidega enne LED-riba ühendamist.
- [ ] Mõõta toitepinge LED-riba alguses ja lõpus suurima kasutatava heleduse juures.
- [ ] Kontrollida toiteallika, pingemuunduri, patareide, kontaktide ja juhtmete temperatuuri ning koormusvoolu.
- [ ] Kontrollida, et ESP8266 3,3 V andmesignaal ei tekita juhuslikke sähvatusi.
- [ ] Veenduda patareitoite/regulaatori, LED-riba ja USB-toitega NodeMCU ühises GND ühenduses; patareipaki pluss ei tohi olla ühendatud NodeMCU toiterööbastega.
- [ ] Testida Wi-Fi uuesti ühendamist ja võrgu puudumise teadet mobiiltelefonis.

Riistvarakontrollid on kavandatud ja siin läbimata. 2026-10-31 läbiti NodeMCU kompileerimine ja neli veebiskripti automaatkontrolli; käsud ja piirangud on failides `BUILD.md` ning `REVIEW.md`.

## HTTP regressioonikontrollid seadmel (veel läbimata)

- [ ] `GET /set?power=0` annab 405 ja olek ei muutu.
- [ ] `POST /set` ilma kontrollpäiseta annab 403.
- [ ] POST vale Host-päisega annab 403.
- [ ] Korrektsed POST-väärtused piiridel (mode 0/5, brightness 0/255, speed 1/100, strength 0/100, power 0/1, color 000000/FFFFFF) rakenduvad.
- [ ] Tühjad, negatiivsed, ülemäärased, teksti sisaldavad või osaliselt numbrilised väärtused annavad 400.
- [ ] `mode=3&brightness=oops` ei muuda kumbagi väärtust.
- [ ] `color=GGGGGG`, `color=123` ja `power=true` annavad 400.
- [ ] Sama oleku korduv saatmine ei taaskäivita animatsiooni ega tekita uut salvestust.
- [ ] Väliselt veebilehelt vormi/fetch kaudu kirjutamine ei õnnestu.
- [ ] Toitekatkestus salvestamisel: dokumenteerida faili taastumine/vaikimisi väärtused; praegune kirjutus ei ole atomaarne.
