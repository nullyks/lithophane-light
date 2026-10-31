# Ekraanipildid ja näitevideod

Lisatud 2026-10-31 kasutaja soovil avalikku hoidlasse.

## Kasutajaliidese ekraanipildid

`media/ui-static.jpg`, `media/ui-wave.jpg` ja `media/ui-fire.jpg` on brauseris
renderdatud ekraanipildid faili `Lithophane_Light/Lithophane_Light.ino` muutmata
`PAGE` HTML-ist. Kasutatud firmware’i revisjon: `1ad90fc`.

Kohalik eelvaateserver andis `/state` kaudu näidisoleku: heledus 120/255,
kiirus 50%, tugevus 75%, värv `FFE0B0`, valgustus sees. Režiimid valiti
veebiliidese enda juhtnupuga. See ei olnud ühendus füüsilise ESP8266-ga.
Brauseri ekraanipiltidelt kärbiti tühjad külgservad; juhtnuppude välimust ei muudetud.

## Videote päritolu

Kõik viis videot pärinevad kasutaja antud failidest ja kujutavad füüsilist
40 LED-iga prototüüpi. Kasutaja soovis nummerdatud näiteid; videotele ei ole
omistatud kinnitamata efektinimesid ega seost konkreetse firmware’i revisjoniga.

| Näide | Algfail | Kestus ligikaudu | Avaldatud video |
| --- | --- | --- | --- |
| 1 | `20261031_164317.mp4` | 10,5 s | [effect-01.mp4](media/effect-01.mp4) |
| 2 | `20261031_164347.mp4` | 9,6 s | [effect-02.mp4](media/effect-02.mp4) |
| 3 | `20261031_164420.mp4` | 18,1 s | [effect-03.mp4](media/effect-03.mp4) |
| 4 | `20261031_164455.mp4` | 8,6 s | [effect-04.mp4](media/effect-04.mp4) |
| 5 | `20261031_164515.mp4` | 11,5 s | [effect-05.mp4](media/effect-05.mp4) |

Algfailid jäid muutmata. Avaldatud koopiad säilitavad kogu ajavahemiku ja püstise
orientatsiooni. Need on teisendatud H.264 MP4 vormingusse, 540 × 960 pikslit,
30 kaadrit sekundis, `yuv420p`, CRF 25 ja voogesitust alustava `faststart` päisega.
Taustaheli ning lähtemetadata eemaldati. Värve ega efektide kiirust ei muudetud
tahtlikult; pakkimine ja kaadrisageduse ühtlustamine võivad tekitada väikseid erinevusi.

Iga MP4 kõrval on esimese kuni kuue sekundi korduv GIF-eelvaade
(216 × 384 pikslit, 8 kaadrit sekundis, kuni 96 värvi). GIF-i piiratud värvipalett
ei esita LED-ide värve sama täpselt kui MP4. Kaamera automaatsäri ja valge tasakaal
mõjutavad omakorda salvestatud valguse välimust.

Kontrollitud: kõik viis avaldatud MP4-faili dekodeeruvad veata, kõik kolm
ekraanipilti on visuaalselt üle vaadatud ning README kohalikud meedialingid viivad
olemasolevate failideni. Videod näitavad prototüübi tööd, mitte elektrilisi mõõtmisi.
