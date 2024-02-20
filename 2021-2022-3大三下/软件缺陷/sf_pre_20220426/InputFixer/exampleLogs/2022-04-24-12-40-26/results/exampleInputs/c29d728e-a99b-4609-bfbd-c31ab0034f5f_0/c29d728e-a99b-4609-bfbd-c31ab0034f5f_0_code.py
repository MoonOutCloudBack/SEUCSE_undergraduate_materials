string1 = str(input("Entrer une chaine de caractere : "))
string2 = str(input("Entrer une chaine de caractere de meme longueur que la premiere : "))
i = 0
j = 0
tour = 0
compteur = 0
longueur1 = len(string1)
longueur2 = len(string2)
while (longueur1 != longueur2):
    print("Les chaines ne sont pas de meme longueur !")
    string1 = str(input("Entrer une chaine de caractere : "))
    string2 = str(input("Entrer une chaine de caractere de meme longueur que la premiere : "))
    longueur1 = len(string1)
    longueur2 = len(string2)
if (longueur1 == longueur2):
    while (tour < longueur1):
        if (string1[i] != string2[j]):
            if (j != longueur1):
                j += 1
            else:
                tour += 1
                j = 0
                i += 1
        else:
            i += 1
            compteur += 1
            j = 0
            tour += 1
if (compteur == longueur1):
    print(string1, "et ", string2, "sont des anagrammes")
else:
    print(string1, "et ", string2, "ne sont pas des anagrammes")