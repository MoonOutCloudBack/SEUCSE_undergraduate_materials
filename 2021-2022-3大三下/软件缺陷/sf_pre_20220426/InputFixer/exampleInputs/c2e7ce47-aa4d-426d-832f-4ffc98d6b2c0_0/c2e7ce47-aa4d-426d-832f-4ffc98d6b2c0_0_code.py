zoznam = []

while True:
    a=int(input("Zadaj číslo a 0 pre ukončenie"))
    if a==0 or a in zoznam:
        break
    zoznam.append(a)

k=int(input("Zadaj k-kolky najmensi sucet"))

zoznam=sorted(zoznam)
vysledok = 0
mensich=0

def generuj(pocet, odprvku, zatial_sucet):
    global zoznam, mensich, vysledok
    print("vyvolanie pocet=", pocet, " odprvku=", odprvku, "zatial_sucet=", zatial_sucet)
    for i in range(odprvku, len(zoznam)):
        print("i=", i, "zoznam_i=", zoznam[i])
        if pocet > 1 :
            # stale to nie je posledny clen - vyvolam rekurzivne seba
            generuj( pocet - 1, i+1, zatial_sucet + zoznam[i] )
        else :
            # tak toto uz je posledny clen. Urobim vyhodnotenie
            novy_sucet = zatial_sucet + zoznam[i]
            print( "novy_sucet=", novy_sucet)
            if mensich < k :
                # zatial nemam dost mensich vysledkov
                mensich = mensich + 1
                # musim nastavit najvyssi vysledok
                if vysledok < novy_sucet :
                   vysledok = novy_sucet
                print("mensich=", mensich,"vysledok=", vysledok)
            else :
                # uz som nasiel potrebny pocet mensich
                if vysledok >= novy_sucet :
                   vysledok = novy_sucet
                print("vysledok=", vysledok)

generuj(3, 0, 0)
print("KONIEC mensich=", mensich, " vysledok=", vysledok)