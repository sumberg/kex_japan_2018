# Presentationsförslag

## Introduktion

* Beskriv vad modernisering innebär
* Wrapping
* Konceptualisera vår idé om att använda använda en mikrokontroller för att styra äldre komponenter när de behövs, som ett sätt att modernisera legacy-system

## Bakgrund

* Beskriv 6502 och RP2A03
	+ 6502 användes i t ex Apple II, Commodore VIC-20, Commodore 64, NES
* Visa NESizer2
* Förklara hur vi tänkte att man kunde expandera grundidéerna från NESizer2 till en wrapper

## Utförande

* Beskriv hur vi designade våra experiment utifrån experimental research kanske
* Visa hur vi designade hårdvaran (kretsschema och grejer kanske)

## Resultat

* Visa diagram och prata om varför de är relevanta
	+ I synnerhet de räta linjerna och de lika höga stapeldiagrammen
* Förklara varför det var svårt att felsöka de kassa minnesoperationerna
	+ Vi kunde inte veta om det var vårt system det var fel på eller om det var något fel i 6502an, pga att det inte gick att veta om skrivning eller läsning inte fungerade som det skulle. Förmodligen var skrivning problemet, då vi inte kunde kicka igång den minnesmappade APU:n enligt NESizer2s beskrivning heller.

## Övrigt

* Visa våra försök att konstruera den andra metoden med delat minne
	+ Visa även den kaotiska sladdhärvan
* Berätta om problematiken med att vi inte kunde identifiera chippet ordentligt, och vikten av att använda NESizer2's identifierings-rutin
	+ Har vi en delare på 12, 15 eller 16?
