# Lithophane Light

Autonoomne silindrikujulise litofaanvalgusti juhtseade, mis kasutab **NodeMCU ESP8266 (ESP-12E)** ja **40 WS2812B RGB LED-i**. LED-riba on keritud spiraalina ümber ligikaudu 20 mm läbimõõduga toru. Seade loob oma Wi-Fi pääsupunkti ja pakub eestikeelset seadistuslehte. Internetiühendust pole tarvis.

> Staatus: funktsionaalne prototüüp. Varasem 20 LED-iga versioon on kasutaja riistvaral üldjoontes töötanud. See 40 LED-i ja uuendatud veebiliidesega versioon ei ole riistvaral eraldi testitud.

## Funktsioonid

- Staatiline valgus, vabalt valitava RGB värvi ja heledusega.
- Viis animatsiooni: küünlavalgus, hingamine, värviüleminek, vertikaalne valguslaine, tuleleek.
- 24 värvi kiirvaliku nuppu ning brauseri vabavärvivalik.
- Üldine heledus, animatsiooni kiirus ja tugevus (vastavalt režiimile).
- Valgustuse sisse-/väljalülitamine.
- Seadistuste salvestamine LittleFS-i kolme sekundi möödumisel viimasest muudatusest.
- LED-ide eelvaadet veebilehel ei ole.

## Riistvara ja ühendused

### Praegune prototüüp

Kasutaja kinnitatud ühendus (2026-10-31): LED-riba saab toite **4 × 1,5 V AA patareikarbist otse, ilma pingeregulaatorita**. NodeMCU saab eraldi toite USB-juhtme kaudu. Ühise GND olemasolu ei ole veel kinnitatud.

**Tähelepanu:** nelja jadamisi ühendatud 1,5 V patarei nimipinge on 6 V. See ei ole reguleeritud 5 V toide ning võib 5 V WS2812B-riba kahjustada. Värskete patareide pinge võib olla nimipingest kõrgem. Ära kasuta seda otseühendust enne konkreetse LED-riba lubatud toitepinge kontrollimist; kasuta ribale sobivat reguleeritud 5 V toidet. Tarkvaraline heleduse vähendamine ei vähenda riba toitepinget.

[Adafruiti NeoPixeli toitejuhend](https://learn.adafruit.com/adafruit-neopixel-uberguide/powering-neopixels) hoiatab nelja leelispatarei 6 V otseühenduse eest. Täpne LED-versioon ja patareide keemia on veel tuvastamata.

### Soovitatav ühendus reguleeritud 5 V toitega

| Ühendus | Selgitus |
| --- | --- |
| NodeMCU D2 (GPIO4) | WS2812B esimese LED-i DIN |
| LED-riba +5 V | Reguleeritud 5 V toiteallika positiivne väljund; mitte 4 × 1,5 V karbi otseväljund |
| LED-riba GND | Välise toiteallika GND |
| NodeMCU GND | Ühine maa LED-riba GND-ga |
| NodeMCU USB | NodeMCU toide sobivast USB 5 V allikast |

LED-riba tuleb toita eraldi **5 V / vähemalt 3 A** allikast; soovitatav on 4 A varu. 40 RGB LED-i maksimaalne teoreetiline vool on ligikaudu 2,4 A, kui kõik kanalid on täielikult sees. Ärge toitke riba NodeMCU 3,3 V väljundist.

Patareitoite säilitamisel peab pingemuundur sobima patareipaki kogu kasutatava pingevahemiku ning LED-riba koormusega. Lihtne pinget alandav muundur ei pruugi patareide tühjenedes enam 5 V hoida. AA-patareide, patareikarbi ja juhtmete sobivus vajab mõõtmist; tööaega pole hinnatud. NodeMCU jääb USB-toitele: ühenda toiteallikate GND-d, kuid ära ühenda patareipaki plussi NodeMCU USB/5 V/3,3 V toiterööbastega.

Praegune prototüüp ei kasuta nivoomuundurit ega silumiskondensaatorit. See on kompromiss: 3,3 V andmesignaal ei pruugi 5 V LED-ribaga alati stabiilselt töötada. Töökindluse parandamiseks on soovitatavad 74AHCT125/74HCT245 nivoomuundur, 330–470 Ω andmetakisti ning toitesisendi silumiskondensaator; prototüüp ei eelda nende olemasolu.

## Paigaldamine

1. Paigalda Arduino IDE ja **ESP8266 by ESP8266 Community 3.1.2** plaadipakett. Lisa plaadihalduri URL: `https://arduino.esp8266.com/stable/package_esp8266com_index.json`.
2. Paigalda Library Managerist **Adafruit NeoPixel 1.15.5**.
3. Ava `Lithophane_Light/Lithophane_Light.ino`.
4. Vali plaat **NodeMCU 1.0 (ESP-12E Module)**, vali õige jadaport ning laadi kood üles.
5. Ühenda LED-riba vastavalt ülaltoodud tabelile ja käivita seade.
6. Ühendu Wi-Fi võrku `Lithophane-Light`, parool `lithophane123`.
7. Ava brauseris **http://192.168.4.1** (kirjuta aadress `http://`, mitte `https://`).

Telefon võib teatada internetiühenduse puudumisest: see on ootuspärane. Säilita ühendus seadme Wi-Fi võrguga.

## Seadistamine

`LED_COUNT = 40` määrab LED-ide arvu. Kui LED 0 asub **ülal**, määra `REVERSE_SPIRAL = true`; vaikimisi eeldatakse, et LED 0 asub **all**. Suund mõjutab eelkõige tuleleeki ja vertikaalset lainet.

| Režiim | Värv | Kiirus | Tugevus |
| --- | --- | --- | --- |
| Staatiline | Valitav | – | – |
| Küünlavalgus | Soe eelseadistatud | Jah | Jah |
| Hingamine | Valitav | Jah | Jah |
| Värviüleminek | Automaatne | Jah | – |
| Vertikaalne laine | Valitav | Jah | Jah |
| Tuleleek | Soe eelseadistatud | Jah | Jah |

Seade salvestab režiimi, heleduse, kiiruse, tugevuse, värvi ja sisse-/väljalülituse oleku faili `/lithophane-v2.txt` ESP8266 LittleFS-is. Salvestamine toimub kolm sekundit pärast viimast muutust. Vahetult pärast muutmist toite väljalülitamisel ei pruugi uus väärtus säilida.

## HTTP-liides

- `GET /` – kasutajaliides.
- `GET /state` – praegused väärtused JSON-ina.
- `POST /set` – seadistuste muutmine, `application/x-www-form-urlencoded` kehas (parameetrid on valikulised).
- Nõutav päis: `X-Lithophane-Control: 1`. Kasuta hosti `192.168.4.1` (valikuliselt `:80`). `GET /set` tagastab 405.
- Vahemikud: `mode` 0–5, `brightness` 0–255, `speed` 1–100, `strength` 0–100, `power` 0 või 1, `color` täpselt kuus kuueteistkümnendmärki ilma `#` märgita. Vigane väärtus annab HTTP 400 ja kogu muudatus jäetakse rakendamata.

Näide (Windowsis kasuta vajadusel `curl.exe`):

```sh
curl -X POST http://192.168.4.1/set -H "X-Lithophane-Control: 1" --data "mode=2&brightness=120&speed=50&strength=75&color=FFE0B0&power=1"
```

Võrk kasutab WPA2 parooli, kuid HTTP juhtimine ise pole autentitud. Kõik pääsupunktiga ühendunud saavad valgustust juhtida. Muuda lähtekoodi `AP_PASSWORD` väärtus enne kasutuselevõttu unikaalseks 8–63 märgi pikkuseks parooliks ja laadi firmware uuesti üles. Ära lisa oma tegelikku parooli Giti. POST ja kontrollpäis kaitsevad tavaliste võõralt veebilehelt saadetud päringute eest, kuid ei tuvasta kasutajat. Ära ühenda seda lahendust otse internetti.

## Arendus ja piirangud

- Värviefektid on kohandatud hajutatud valgusele silindrilises litofaanis.
- 40 LED-i värvide eelvaadet ei kuvata: kasutaja näeb muudatust otse valgustis.
- 40 LED-i korral sõltub vertikaalse laine visuaalne liikumine spiraali sammust ja LED-ide paigutusest.
- RGB WS2812B ei tooda spektraalselt sama valget kui eraldi valged LED-id; fotolitofaanis võib parim tulemus olla soe staatiline valgus.
- Seade kasutab ühekeermelist ESP8266 veebiserverit ja 30 kaadrit sekundis animatsiooni.

## Failid

- `Lithophane_Light/Lithophane_Light.ino` – firmware ja sisseehitatud HTML/JavaScript/CSS.
- `README.md` – ülevaade ja kasutusjuhend.
- `docs/DEVELOPMENT.md` – tarkvaraarhitektuur ja arendusmärkmed.
- `docs/TESTING.md` – testimisjuhend.
- `docs/REVIEW.md` – koodi-, turva- ja dokumentatsiooniülevaade koos allesjäänud riskidega.
- `docs/BUILD.md` – korratav käsurea kompileerimine ja kontrollid.
- `SECURITY.md` – turvamudel ja probleemidest teatamine.
- `tests/web-ui.test.cjs` – veebiskripti automaatkontrollid.
- `.github/workflows/firmware.yml` – kompileerimine ja veebiskripti testid GitHub Actionsis.
- `.gitignore` – lokaalsed Arduino väljundid ja tööriistad.

## Litsents

Litsentsi pole veel valitud. Enne avalikku levitamist tuleb valida sobiv tarkvaralitsents.
