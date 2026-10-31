# Arendusmärkmed

## Arhitektuur

Kogu programm on ühes Arduino `.ino` failis. `setup()` käivitab LED-riba, LittleFS-i, Wi-Fi SoftAP-i ja HTTP-serveri. `loop()` teenindab HTTP päringuid, salvestab hilinenud muudatusi ja genereerib animatsioonikaadreid. Staatilises ja välja lülitatud režiimis uuendatakse LED-e ainult muutuse korral.

## Veebiliides

HTML/CSS/JavaScript asuvad `PAGE` PROGMEM stringis; ei vaja internetti ega CDN-e. JavaScript loeb `/state` ja muudab seadeid `/set` kaudu. Liugurite muudatused koondatakse 140 ms viivitusega. Muudatused saadetakse POST-päringuga ja kontrollpäisega ükshaaval; ootel muudatused koondatakse viimaseks olekuks. Juhtnupud aktiveeritakse pärast algse oleku edukat lugemist. API kontrollib väärtusi enne kogu muudatuse rakendamist. Värvipalett koosneb 24 HTML-nupust, mis määravad tavapärase värvivaliku (`input type=color`) väärtuse. LED-ide visuaalset eelvaadet pole.

## Animatsioonid

- `STATIC_LIGHT`: ühtlane, kasutaja valitud RGB.
- `CANDLE`: soe valgus, ajas korreleeritud värelusega.
- `BREATHING`: perioodiline heleduslaine, mille sügavust juhib `strength`.
- `COLOR_FADE`: HSV tooniratta sujuv läbimine.
- `VERTICAL_WAVE`: Gaussi kujuline heledusala piki LED-indeksist tuletatud spiraali kõrgust.
- `FIRE`: oranžikas valgus koos kõrgusest sõltuva baasheleduse ja mitme müra-/lainekomponendiga.

`REVERSE_SPIRAL` pöörab LED-indeksiga seotud kõrguskoordinaadi ümber. Mudel eeldab ühtlast kerimissammu. Kui mehaaniline geomeetria on ebaühtlane, tuleks kasutada individuaalsete LED-ide kõrgustabelit.

## Võimalikud täiustused

1. Seadistusfail on juba versiooniga 2. Lisada kontrollsumma ja atomaarne failikirjutus, et katkestatud salvestus ei jätaks vigast faili.
2. Viia HTML eraldi LittleFS-failidesse, kui kasutajaliides kasvab.
3. Lisada kasutajaliidesesse valgustuse vaikimisi käivitumisolek ja efektipõhised tugevuse vahemikud.
4. Lisada voolupiirang täisheleduse jaoks.
5. Kontrollida ühenduse stabiilsust ja valguse ühtlust pärast silindri kokkupanekut.
