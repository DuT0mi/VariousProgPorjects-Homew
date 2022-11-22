; -----------------------------------------------------------
; #Mikrokontroller alapú rendszerek házi feladat
; #Konzulens: 	Kovács Adorján
; #Készítette: 	Dudás Tamás Alex
; #Neptun code: XXXXXX
; 	#Feladat leírása:
;		Regiszeterekben található 16 bites előjeles egész átalakítása 5db BCD kódú számmá + előjellé
; 		Az eredményt 3 regiszterben kapjuk vissza
;			Az elsőben a legfelső bit az előjel, az alatta lévő 3 bit 0, ez alatt a legmagasabb helyiértékű
;			A második regiszterben a következő 2 digit
;			A harmadikban a legkisebb helyiértékű két digit
;	#Bemenet:
;		Az átalakítandó szám 2 regiszterben	; 	R1-ben az alsó, R2-ben a felső
;	#Kimenet:
;		Az átalakított szám 3 regiszterben	;	R7-ben az előjelbit + a legmagasabb helyiértékű, R6 ban a következő 2 digit, és R5-ben a legkisebb kettő digit
; -----------------------------------------------------------
;	#Demó ábra az algoritmushoz:
;		Mivel 16 bites szám a bemenet és 8 bites az architektúra ezért nem olyan triviális az algoritmus implementlása:
;		- Egyrészt meg kell oldani, hogy lehessen léptetni az egész számot R1-től R5-ig (16bitet), akkor vagyunk kész, ha R1 és R2 tartalma azonosan egyenlő nullával
;			- Közbünső folyamatként:  hozzáadni 3 (3D), ha nagyobb mint 5(5D) a BCD digit értéke
;
;	Erre a következő gondolatmeneten indultam el:
;	-------------------------------------------------------------------------------------------------------------------------------------------------
;	|					Ez a rész lesz a kimenet (mentés előtt)	|	Bemenet felső bátjta 	|	Bemenet alsóbátjta	|
;	------------------------------------------------------------------------------------------------------------------------------------------------|
;	|	R5 regiszter	|	R4 regiszter	|	R3 regiszter		|	R2 regiszter		|	R1 regiszter		|
;	|   ----_xxxx		|   xxxx_xxxx		|	xxxx_xxxx		|	xxxx_xxxx		|	xxxx_xxxx		|
;	-------------------------------------------------------------------------------------------------------------------------------------------------
;	Legyen ez az ábra az 1.0-ás ábra
;	Ahol az < x > azon biteket jelenti amelyek lényegesek a feladat megoldása szempontjából, míg
;	a < - > jelölik a lényegtelen (don't care) biteket
;
;	R5- ben van 10^4-es nagyságrend tartalma			(továbbiakban R5_bottom) hisz a felső lényegtelen mi
;	R4- ben van 10^3-es nagyságrend tartalma (felső 4 bitje) 	(továbbiakban R4_upper)
;	R4- ben van 10^2-es nagyságrend tartalma (alsó 	4 bitje)	(továbbiakban R4_bottom)
;	R3- ben van 10^1-es nagyságrend tartalma (felső 4 bitje)	(továbbiakban R3_upper)
;	R3- ben van 10^0-es nagyságrend tartalma (alsó 	4 bitje)	(továbbiakban R3_bottom)
; 	R2-ben a bemenet felső bájtja
;	R1-ben a bemenet alsó bájtja
; -----------------------------------------------------------
$NOMOD51 ; a sztenderd 8051 regiszter definíciók nem szükségesek

$INCLUDE (SI_EFM8BB3_Defs.inc) ; regiszter és SFR definíciók

; Ugrótábla létrehozása
	CSEG AT 0
	SJMP Main

myprog SEGMENT CODE					; saját kódszegmens létrehozása
RSEG myprog 						; saját kódszegmens kiválasztása
; ------------------------------------------------------------------------------
; Konstansok
	SIGN_VALUE 		EQU 10000000b	; előjel meghatározáshoz
	ZERO			EQU 00000000b	; hogy átláthatóbb legyen
	LOWER_DATA		EQU 01h			; alsó input megadása,  ennek a változtatásával lehet konfigurálni az alsó bájtot
	UPPER_DATA		EQU 01h			; felső input megadása, ennek a változtatásával lehet konfigurálni a felső bájtot
; ------------------------------------------------------------------------------
; Főprogram
; ------------------------------------------------------------------------------
; Feladata:
; A teljes átváltási folyamat menedzselése
; Vagyis:
;		- Kezdeti állapot beállítása
;		- Az előjel meghatározása
;		- A Double Dabble algoritmus végrehajtása
;		- Végeredmény elmentése
;		- Majd a nem használt regiszterek kiürítése
; ------------------------------------------------------------------------------
Main:
	; kezdeti állapot beállítás és az előjel bit meghatározása
		CALL startState						; alapállapot beállítása
			MOV R0, #LOWER_DATA				; alsó input bevitele
			MOV R1, #UPPER_DATA				; felső input bevitele
			MOV R0,	#ZERO					; Kinullázás, így alapból nincs előjelbit vagyis 0 teháh nem negatív
		CALL getSignBit						; bekerült R0-ba az előjelbit, így azzal a regiszterekbe mentésig nem igen lehet dolgozni
											; ha az R0 7. (8.)bitje az 0 akkor nem negatív, tehát nem kell invertálni a bemenetet
											; de, ha 1 akkor az előjel negatív és invertáljuk a bemenetet
			CJNE R0,#00000000b,invertData	; Itt válik el, hogy kell e invertálni vagy sem
			LJMP nonInvertData				; ha nem kell invertálni, akkor nincs semmi dolgunk csak elkezdeni az algoritmust (persze előtte az értékeket felhozni)
			invertData:						; ha igen invertálálni mindet + 1
				MOV A,#LOWER_DATA
				CPL A
				MOV R1,A
				MOV A,#UPPER_DATA
				CPL A
				MOV R2,A
				MOV A,R1
				INC A
				MOV R1,A
				JC OF						; ha csordul, akkor le kell kezelni
				LJMP goAlgo					; ha nem csordult akkor kezdődhet az algoritmus
					 OF:
						MOV A,R2
						INC A
						MOV R2,A
						LJMP goAlgo			; lekezelés után mehet az algortimus
			nonInvertData:
				MOV R2,#UPPER_DATA
				MOV R1,#LOWER_DATA
				LJMP goAlgo

		goAlgo:								; itt kezdődik az algortimus
			MOV R7,#15						; A Double Dabble algoritmus O(n) lépésszámú, ahol N a bitek száma, ami itt 16 de az utolsónál nem kell már shiftelni
			Loop:
				CLR C						; a psw-t "eltrontja" a djnz
				CALL shiftReg
				CALL check
				DJNZ R7,Loop
			CALL shiftReg					; 16th shift
			CALL saveToLocal				; mentés a regiszterekbe
			LJMP endState					; A program vége
; ------------------------------------------------------------------------------
;  saveToLocal szubrutin
; ------------------------------------------------------------------------------
; Feladata:
;	A regiszterekbe való mentés koordinálása
;	Bemenete:
;		- R0-R7, A,B
; ------------------------------------------------------------------------------
	saveToLocal:
			CALL saveR5
			CALL saveR4
			CALL saveR3
			CALL cleanToSee
		RET
; ------------------------------------------------------------------------------
;  saveR5 szubrutin
; ------------------------------------------------------------------------------
; Feladata:
;	Az R5-ben lévő adatok mentése + az előjelbit hozzáfűzése
;	Bemenete:
;		- R0
;		- R5
;	Kimenete:
;		- R7
; ------------------------------------------------------------------------------
	saveR5:
		MOV A,R5
		ANL A,#00001111b						; csak az alsó része kell, mivel a felső lényegtelen (lásd: 1.0-ás ábra)
		MOV B,R0
		ORL A,R0								; vagyolás,így megvan az előjel az elején + a végén a legnagyobb helyiértékű
		MOV R7,A								; R7-ben lesznek
	RET
; ------------------------------------------------------------------------------
;  saveR4 szubrutin
; ------------------------------------------------------------------------------
; Feladata:
;	Az R4-ben lévő adatok mentése ( itt már sorrendben van a felhasznált algortimus miatt)
;	Bemenete:
;		- R4
;	Kimenete:
;		- R6
; ------------------------------------------------------------------------------
	saveR4:
		MOV A,R4
		MOV R6,A								; R6-ban a következő két digit
	RET
; ------------------------------------------------------------------------------
;  saveR3 szubrutin
; ------------------------------------------------------------------------------
; Feladata:
;	Az R3-ben lévő adatok mentése ( itt már sorrendben van a felhasznált algortimus miatt)
;	Bemenete:
;		- R3
;	Kimenete:
;		- R5
; ------------------------------------------------------------------------------
	saveR3:
		MOV A,R3
		MOV R5,A								; R5-ben a következő kettő digit
	RET
; ------------------------------------------------------------------------------
;  cleanToSee szubrutin
; ------------------------------------------------------------------------------
; Feladata:
;	A javító számára átláhatóbbá teszi a debuggernél látható értékeket
;	Bemenete:
;		- R0..4
;	Kimenete:
;		- "tiszta" R0...4
; ------------------------------------------------------------------------------
	cleanToSee:
		MOV R4,#ZERO
		MOV R3,#ZERO
		MOV R2,#ZERO
		MOV R1,#ZERO
		MOV R0,#ZERO
	RET
; ------------------------------------------------------------------------------
; check szubrutin
; ------------------------------------------------------------------------------
; Feladata:
;	Az algoritmus közbünső folyamatáért felelős,mivel 1-1 regiszter megfelelethető 1 db bcd számnak (lásd 1.0-ás ábra)
;	ezért, ha valamelyik értéke nagyobb mint 5 (5D), akkor ahhoz hozzá kell adni hármat, hogy megmaradjon az átvitel
;	Illetve a vizsgálat sorrendje se mindegy, muszáj fentről (R5) lefele (R3) haladni, hogy a helyes eredményhez jussunk
;	Bemenete és kimenete:
;		- R3,R4,R5
; ------------------------------------------------------------------------------
	check:
		CALL checkR5_bottom
		CALL checkR4_upper
		CALL checkR4_bottom
		CALL checkR3_upper
		CALL checkR3_bottom
	RET
; ------------------------------------------------------------------------------
; checkR3_bottom szubrutin
; ------------------------------------------------------------------------------
; Feladata:
;	Az algoritmus, R3 alsó részének figyelése, ha nagyobb mint 5 (5D) akkor hozzáad hármat
;	Bemenete:
;		- R3
; ------------------------------------------------------------------------------
	checkR3_bottom:
			MOV A,R3
			MOV B,R3
			ANL B,#11110000b		; csak a felső, nem veszhet el a visszatöltés miatt
			ANL A,#00001111b		; csak az alsó része legyen
			CJNE A,#5D,notEqualR3b
									;ha egyenlő 5d-vel
				LJMP greaterR3b
			notEqualR3b:
				JNC greaterR3b 		; kisebb mint 5d
				LJMP contR3b
			greaterR3b: 				;nagyobb
				ADD A,#00000011b		; +3
				ORL A,B
				MOV R3,A
				LJMP contR3b
			contR3b:
			RET
; ------------------------------------------------------------------------------
; checkR3_upper szubrutin
; ------------------------------------------------------------------------------
; Feladata:
;	Az algoritmus, R3 felső részének figyelése, ha nagyobb mint 5 (5D) akkor hozzáad hármat
;	Bemenete:
;		- R3
; ------------------------------------------------------------------------------
	checkR3_upper:
			MOV A,R3
			MOV B,R3
			ANL B,#00001111b		; alsó, visszatöltéshez, nem veszhet el a visszatöltés miatt
			ANL A,#11110000b		; csak a felső része legyen
			SWAP A
			CJNE A,#5D,notEqualR3u
									;ha egyenlő 5d-vel
				LJMP greaterR3u
			notEqualR3u:
				JNC greaterR3u 		; kisebb mint 5d
				LJMP contR3u
			greaterR3u: 				;nagyobb
				ADD A,#00000011b		; +3
				SWAP A
				ORL A,B
				MOV R3,A
				LJMP contR3u
			contR3u:
			RET
; ------------------------------------------------------------------------------
; checkR4_bottom szubrutin
; ------------------------------------------------------------------------------
; Feladata:
;	Az algoritmus, R4 alsó részének figyelése, ha nagyobb mint 5 (5D) akkor hozzáad hármat
;	Bemenete:
;		- R4
; ------------------------------------------------------------------------------
	checkR4_bottom:
			MOV A,R4
			MOV B,R4
			ANL B,#11110000b		; felső, visszatöltéshez
			ANL A,#00001111b		; csak az alsó része legyen
			CJNE A,#5D,notEqualR4b
									;ha egyenlő 5d-vel
				LJMP greaterR4b
			notEqualR4b:
				JNC greaterR4b 		; kisebb mint 5d
				LJMP contR4b
			greaterR4b: 				;nagyobb
				ADD A,#00000011b		; +3
				ORL A,B
				MOV R4,A
				LJMP contR4b
			contR4b:
			RET
; ------------------------------------------------------------------------------
; checkR4_upper szubrutin
; ------------------------------------------------------------------------------
; Feladata:
;	Az algoritmus, R4 felső részének figyelése
;	Bemenete:
;		- R4
; ------------------------------------------------------------------------------
	checkR4_upper:
			MOV A,R4
			MOV B,R4
			ANL B,#00001111b		;alsó visszatöltéshez
			ANL A,#11110000b		; csak a felső része legyen
			SWAP A
			CJNE A,#5D,notEqualR4u
									;ha egyenlő 5d-vel
				LJMP greaterR4u
			notEqualR4u:
				JNC greaterR4u 		; kisebb mint 5d
				LJMP contR4u
			greaterR4u: 				;nagyobb
				ADD A,#00000011b		; +3
				SWAP A
				ORL A,B
				MOV R4,A
				LJMP contR4u
			contR4u:
			RET
; ------------------------------------------------------------------------------
; checkR5_bottom szubrutin
; ------------------------------------------------------------------------------
; Feladata:
;	Az algoritmus, R5 alsó részének figyelése (felső egyébként sem kell)
;	Bemenete:
;		- R5
; ------------------------------------------------------------------------------
	checkR5_bottom:
			MOV A,R5
			MOV B,R5
			ANL B,#11110000b		; csak a felsők, nem veszhet el a visszatöltés miatt
			ANL A,#00001111b		; csak az alsó része legyen
			CJNE A,#5D,notEqualR5b
				;ha egyenlő 5d-vel
				LJMP greaterR5b
			notEqualR5b:
				JNC greaterR5b 		; kisebb mint 5d
				LJMP contR5b
			greaterR5b: 				;nagyobb
				ADD A,#00000011b		; +3
				ORL A,B
				MOV R5,A
				LJMP contR5b
			contR5b:
			RET
; shiftReg szubrutin
; ------------------------------------------------------------------------------
; Feladata:
;	Egy shiftregiszter megvalósítása. Az algoritmus az RLC utasítással dolgozik,
;	ami azért jó mert carry-vel forgat, amire szükségünk van mert ha R1 7. bitjéről (ha 0. az első), kell shiftelni akkor azt
;	át kell tenni az R2-be és így tovább vihető az analógia egészen R5-ig
;	Működést szemléltető/segítő ábra: 1.0-ás a meta adatoknál a kód elején
;	Bemenete és kimenete:
;		Minden az algoritmusban résztvevő regiszter: R1..R5
; ------------------------------------------------------------------------------
		shiftReg:
			MOV A, R1
			RLC A
			MOV R1,A
			MOV A,R2
			;R1-en volt carry, akkor forgatni is kell meg át vinni
			JC R1Carry
			LJMP R1half
				R1Carry:
						CLR C
						RLC A
						INC A
						LJMP R1cont ; mehetünk a következőre
			;R1-en nem volt carry akkor simán csak forgatunk
			R1half:
				RLC A
			R1cont:
				MOV R2,A
				MOV A,R3
			;R2-ön volt carry, akkor forgatni is kell meg át vinni
			JC R2Carry
			LJMP R2half
				R2Carry:
						CLR C
						RLC A
						INC A
						LJMP R3cont ; mehetünk a következőre
			; nem volt R2-n carry akkor simán csak forgatunk
			 R2half:
				RLC A
			R3cont:
				MOV R3,A
				MOV A,R4
				;R3-on volt carry, akkor forgatni is kell meg át vinni
				JC R3Carry
				LJMP R3half
					R3Carry:
						CLR C
						RLC A
						INC A
						LJMP R4cont	; mehetünk a következőre
				;R3-on nem volt carry akkor simán csak forgatunk
				 R3half:
					RLC A
				R4cont:
				MOV R4,A
				MOV A,R5
				;R4-en volt carry, akkor forgatni is kell meg át vinni
				JC R4Carry
				LJMP R5half
					R4Carry:
						CLR C
						RLC A
						INC A
						LJMP R5cont ; mehetünk a következőre
				;R4-en nem volt carry akkor simán csak forgatunk
					R5half:
					 RLC A
				R5cont:
				MOV R5,A
				RET
; ------------------------------------------------------------------------------
; getSignBit(szubrutin), inverter(kódrészlet)
; ------------------------------------------------------------------------------
; Feladata:
;	Az előjel bit meghatározása
; Invertálás algoritmusa:
; invertálom minden bitet majd + 1 az alsóhoz ( vagy a felsőhőz lásd: OverFlow:)
; Demó:
;	a referencia 1000_0000b
;	Demó1:
;		0xxx_xxxx
;		1000_0000
;		ebből az éselés után ez lesz:
;		0000_0000 --> vagyis az előjelbit az 0 tehát nem kell invertálni
;	Demó2:
;		1xxx_xxxx
;		1000_0000
;		ebből az éselés után ez lesz:
;		1000_0000 --> vagyis az előjelbit az 1 tehát invertálni kell,hisz kettes komplenesben dolgozik
;	OverFlow:
;		Demó:
;		<felső bájt><alsó bájt>
;	input:		1111_1111_0000_0000
;	invertálás:	0000_0000_1111_1111
;	+1:			0000_0001_0000_0000 <-- overflow, mivel 8 bitesek a regiszterek az alsó full nulla lesz és a felső is ezért kell
;										hozzáadni a +1t
; ------------------------------------------------------------------------------
	getSignBit:
		MOV A,#UPPER_DATA		; felhozatal
		ANL A,#SIGN_VALUE		; éselés
		JNZ inverter 			; ha nem 0 -> invertálás, lásd @brief
		RET	; getSignBit vége
	inverter:
		MOV A,#UPPER_DATA		; felhozatal (felső)
		CPL A					; invertálás
		MOV R1,A				; vissza (felső)
		MOV A,R0				; felhozalat (alsó)
		CPL	A					; komplementer vétele
		INC A					; (CPL A) += 1 -> most jön az elágazás
		; ha nem nulla akkor jók vagyunk alias no OF
		CJNE A,#ZERO, goodInverterState
		; ha nulla -> felső biten + 1
		MOV R0,A		; vissza (alsó)
		MOV A, R1		; felső be
		INC A					; +1
		MOV R1,A		; felső vissza
	goodInverterState:
		MOV R0,#SIGN_VALUE
	RET ; goodInverterState vége

; ------------------------------------------------------------------------------
; starState szubrutin
; ------------------------------------------------------------------------------
; Feladata:
;	A kezdeti állapot létrehozása, kinullázza a regisztereket
; A következőket módosítja:
;	A - Akkumulátor + PSW
;	B - segédregiszter
;	R0...R7 regiszterek
; ------------------------------------------------------------------------------
	startState:
		CLR C
		MOV A ,#ZERO
		MOV B ,#ZERO
		MOV R0,#ZERO
		MOV R1,#ZERO
		MOV R2,#ZERO
		MOV R3,#ZERO
		MOV R4,#ZERO
		MOV R5,#ZERO
		MOV R6,#ZERO
		MOV R7,#ZERO
	RET
	endState:
		END
