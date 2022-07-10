# Elio   SAMAHA [21105733]
# Justin CAREL  [21102190]
import math

   # # # # # # #
   # PARTIE: 1 #
   # # # # # # #

def identite(s :str)->str:
    """ Renvoie la chaine s telle quelle """
    return s

assert identite('acbdjkbkjdbhjkdbkj') == 'acbdjkbkjdbhjkdbkj'

def identite_texte(nom :str)->None :
    """ Precondition :<nom>.txt est un fichier existantrecopie le contenu du fichier<nom>.txt dans<nom>−copie.txt """
    with open(nom + ".txt", "r") as source :
        with open(nom + "-copie.txt", "w") as destination :
            ligne :str
            for ligne in source.readlines() :
                destination.write(identite(ligne))


# Comme les fonctions au dessus sont compliquées pour rien on crée les templates ci dessous, plus simples à utiliser.

def read(path: str) -> str:
    """Précondition: `path` is a valid path to an existing file
    Returns the content of the file"""
    with open(path, 'r') as file:
        return file.read()


def write(path: str, content: str) -> None:
    """Précondition: `path` is a valid filepath
    Write content to the file"""
    with open(path, 'w') as file:
        file.write(content)


   # # # # # # #
   # PARTIE: 2 #
   # # # # # # #

######### Question 1 #########

def est_majuscule(c: str) -> bool:
    """Précondition: len(c) == 1
    Returns whether `c` is a capital ASCII letter"""
    return 'A' <= c and c <= 'Z'

def est_minuscule(c: str) -> bool:
    """Précondition: len(c) == 1
    Returns whether `c` is a capital ASCII letter"""
    return 'a' <= c and c <= 'z'

assert est_majuscule("C")
assert est_minuscule("c")
assert not est_majuscule("c")
assert not est_majuscule(" ")
assert not est_minuscule("C")
assert not est_minuscule(" ")


######### Question 2 #########

def ord_base(c: str) -> int:
    """Précondition: len(c) == 1
    Return the ASCII code of the start of `c`'s alphabet, or None when `c` isn't a letter"""
    if est_minuscule(c):
        return ord('a')
    if est_majuscule(c):
        return ord('A')
    return -1
    # If not for MrPython issuing "Typage imprécis" warnings upon use of the returned value (even though there were tests for None prior to said usage), this function would return an Optional[int]

assert ord_base('c') == ord('a')
assert ord_base('C') == ord('A')
assert ord_base(' ') == -1

def caractere_decale(c: str, n: int) -> str:
    """Précondition: len(c) == 1
    Return the character shifted by `n` letters to the righ.
    Equivalent to +n when identifying alphabet to Z/26Z"""
    base: int = ord_base(c)
    if base == -1:
        return c
    return chr((ord(c) - base + n) % 26 + base)

assert caractere_decale("a", -1) == "z"
assert caractere_decale("a", 0) == "a"
assert caractere_decale("a", 3) == "d"
assert caractere_decale("A", 3) == "D"
assert caractere_decale("V", 8) == "D"
assert caractere_decale(" ", 3) == " "


######### Question 3 #########

def ligne_chiffre_cesar(line: str, n: int) -> str:
    """Returns `line` encrypted by monoalphabetic substitution (n-th shift)"""
    s: str = ''
    c: str
    for c in line:
        s = s + caractere_decale(c, n)
    return s

assert ligne_chiffre_cesar("Bonjour LU1IN011", 3) == "Erqmrxu OX1LQ011"
assert ligne_chiffre_cesar("Bonjour LU1IN011", 0) == "Bonjour LU1IN011"


######### Question 3 le retour #########

def ligne_dechiffre_cesar(line: str, n: int) -> str:
    """Returns `line` decrypted by monoalphabetic substitution (n-th shift)"""
    return ligne_chiffre_cesar(line, -n)

assert ligne_dechiffre_cesar("Erqmrxu OX1LQ011", 3) == "Bonjour LU1IN011"
assert ligne_dechiffre_cesar("Bonjour LU1IN011", 0) == "Bonjour LU1IN011"
beaute1: str = "Je suis belle, o mortels ! comme un reve de pierre,"
assert ligne_dechiffre_cesar(ligne_chiffre_cesar(beaute1, 12), 12) == beaute1


######### Question 4 #########

def chiffre_fichier_cesar(path: str, n: int) -> None:
    """Précondition: `path`.txt is a valid path pointing to an existing file
    Encrypts the content of the file with monoalphabetic substitution (n-th shift)
    and writes the ciphertext to `path`.enc"""
    content: str = read(path + '.txt')
    write(path + '.enc', ligne_chiffre_cesar(content, n))


def dechiffre_fichier_cesar(path: str, n: int) -> None:
    """Précondition: `path`.enc is a valid path pointing to an existing file
    Decrypts the content of the file with monoalphabetic substitution (n-th shift)
    and writes the plaintext to `path`.txt"""
    content: str = read(path + '.enc')
    write(path + '.txt', ligne_dechiffre_cesar(content, n))


   # # # # # # # # #
   # SUGGESTION: 3 #
   # # # # # # # # #

def attaque_cesar(path: str) -> None:
    """Précondition: `path`.txt is a valid path pointing to an existing file
    Bruteforce shift for monoalphabetic substitution (the keyspace has cardinality 26)"""
    enc: str = read(path + '.enc')
    s: str = ''
    k: int
    for k in range(26):
        s = s + '='*32 + '\n Decalage de : ' + str(k) + '\n' + ligne_dechiffre_cesar(enc, k) + '\n'*2

    write(path + '.atk.txt', s)


   # # # # # # # # #
   # SUGGESTION: 4 #
   # # # # # # # # #

def abs_int(a: int) -> int: # MR PYTHON BELIEVES ABS ALWAYS RETURNS FLOATS
    """Same as abs(a)"""
    if a >= 0:
        return a
    return -a

assert abs_int(0) == abs(0)
assert abs_int(1) == abs(1)
assert abs_int(-1) == abs(-1)

def gcd(a: int, b: int) -> int:
    """Returns the greatest common divisor of a and b through the euclidean algorithm
    Only useful because MrPython doesn't believe in the existence of math.gcd"""
    if a == 0 and b == 0:
        return 0

    A: int = abs_int(a) 
    B: int = abs_int(b)
    while B != 0:
        tmp: int = B
        B = A % B
        A = tmp
    return A

assert gcd(2, 5) == 1
assert gcd(20, 48) == 4
assert gcd(0, 4685) == 4685
assert gcd(4685, 0) == 4685

def affine_transformation(c: str, a: int, b: int) -> str:
    """Précondition: len(c) == 1
    Returns a*c + b % 26, with c canonically identified to an element of Z/26Z."""
    base: int = ord_base(c)
    if base == -1:
        return c

    A: int = a
    while gcd(A, 26) != 1:
        A = A + 1

    return chr((A*(ord(c) - base) + b) % 26 + base)

assert affine_transformation('a', 1, 0) == 'a'
assert affine_transformation('A', 1, 0) == 'A'
assert affine_transformation('a', 1, -1) == 'z'
assert affine_transformation('A', 1, -1) == 'Z'
assert affine_transformation('b', 5, 3) == 'i'
assert affine_transformation('B', 5, 3) == 'I'

def inv_mod_26(a: int) -> int:
    """Précondition: gcd(a, 26) == 1"""
    # Simpler than xgcd and runs in O(1) since the modulus is fixed for our purposes
    inv: int
    for inv in range(1, 26):
        if (a * inv) % 26 == 1:
            return inv

assert inv_mod_26(1) == 1
assert inv_mod_26(-1) == 25

def ligne_chiffre_affine(line: str, a: int, b: int) -> str:
    """Returns the encryption of `line` by the affine transformation given by a and b"""
    enc: str = ''
    c: str
    for c in line:
        enc = enc + affine_transformation(c, a, b)
    return enc

def ligne_dechiffre_affine(enc: str, a: int, b: int) -> str:
    """Returns the decryption of `line` by the affine transformation given by a and b"""
    A: int = a
    while gcd(A, 26) != 1:
        A = A + 1

    A = inv_mod_26(A)
    return ligne_chiffre_affine(enc, A, -b*A)

assert ligne_chiffre_affine("Bonjour LU1IN011!", 3, 7) == "Kxuixpg OP1FU011!"
assert ligne_chiffre_affine("Bonjour LU1IN011!", 2, 7) == "Kxuixpg OP1FU011!"
assert ligne_chiffre_affine("Bonjour LU1IN011!", 14, 47) == "Kxiaxjq EJ1LI011!"
assert ligne_dechiffre_affine("Kxuixpg OP1FU011!", 3, 7) == "Bonjour LU1IN011!"
assert ligne_dechiffre_affine("Kxuixpg OP1FU011!", 2, 7) == "Bonjour LU1IN011!"
assert ligne_dechiffre_affine("Kxiaxjq EJ1LI011!", 14, 47) == "Bonjour LU1IN011!"
beaute1_c :str= ligne_chiffre_affine(beaute1, 28, 2016)
assert ligne_dechiffre_affine(beaute1_c , 28, 2016) == beaute1

def chiffre_fichier_affine(path: str, a: int, b: int) -> None:
    """Précondition: `path`.txt is a valid path pointig to an existing file
    Apply the affine transformation given by a and b to the content of the file, then write the result to `path`.enc"""
    content: str = read(path + '.txt')
    write(path + '.enc', ligne_chiffre_affine(content, a, b))    

def dechiffre_fichier_affine(path: str, a: int, b: int) -> None:
    """Précondition: `path`.enc is a valid path pointig to an existing file
    Apply the inverse affine transformation given by a and b to the content of the file, then write the result to `path`.txt"""
    content: str = read(path + '.enc')
    write(path + '.txt', ligne_chiffre_affine(content, a, b))    

