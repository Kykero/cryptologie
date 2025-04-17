
alphabet = ['A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
            'P','Q','R','S','T','U','V','W','X','Y','Z']

# ----------- Code de César ----------------------------
def encodage(message, k):
    code = []    
    for a in message:
        if a != ' ':
            if a in alphabet:
                # Récupère l'indice de la lettre dans l'alphabet
                index = alphabet.index(a)
                # Cyclique avec le modulo
                new_index = (index + k) % len(alphabet)
                code.append(alphabet[new_index])
            else:
                # Si le caractère n'est pas dans l'alphabet, on le laisse inchangé
                code.append(a)
        else:
            # Conserve les espaces dans le message
            code.append(' ')
    return ''.join(code) # join permet de faire une seule chaîne pour la phrase ça rend juste le résultat plus lisible

def decryptage(message,k):
    message_decrypte = []
    for a in message :
        if a != ' ':
            if a in alphabet:
                #On récupère l'indice dans l'alphabet
                index = alphabet.index(a)
                #Indice Cyclique
                new_index = (index - k) % len(alphabet)
                message_decrypte.append(alphabet[new_index])
            else:
                #On converve les espaces
                message_decrypte.append(a)
        else:
            #On garde les espaces
            message_decrypte.append(' ')
    return ''.join(message_decrypte)

# ----------- Décryptage Vigenère ----------------------------


def decryptage_vigenere(code, key):

    reponse = []         # Liste où on stockera le message décrypté
    key_length = len(key)    # Longueur de la clé pour la faire tourner
    key_index = 0          # Indice pour parcourir la clé

    for char in code:
        if char in alphabet:
            # On récupère l'indice de la lettre du texte chiffré dans l'alphabet
            index_cipher = alphabet.index(char)
            # Sélection de la lettre correspondante de la clé (avec rotation)
            key_letter = key[key_index % key_length]
            # On récupère l'indice du décalage en fonction de la lettre de la clé
            index_key = alphabet.index(key_letter)
            # Calcul de l'indice décrypté en faisant le décalage inverse (soustraction) et modulo 26
            new_index = (index_cipher - index_key) % len(alphabet)
            reponse.append(alphabet[new_index])
            # On ne met à jour l'indice de la clé que pour les caractères alphabétiques
            key_index += 1
        else:
            # On conserve les caractères non alphabétiques tels quels (espaces, ponctuation, etc.)
            reponse.append(char)
    
    # On rassemble la liste en une chaîne de caractères
    return ''.join(reponse)



# ----------- Main César ----------------------------

# Question 1
print("Pour la question 1, un cercle serait une forme géometrique mieux adapté pour représenter l'alphabet du code César l'alphabet")
print(' ------------ ')

# Question 2
print(' ------------ ')
print('Question 2')
message_encode = encodage('LES VACANCES SONT TERMINEES', 11)
print(message_encode)

# Question 3 
print(' ------------ ')
print('Question 3')
message_decrypte = decryptage('YHQL YLGL YLFL',3)
print(message_decrypte)

# Question 4 
print(' ------------ ')
print('Question 4')

for k in range(0,25):
    message_decrypte3 = decryptage('MOVK OCD ZVEC PKMSVO AEO ZBOFE',k)
    print(message_decrypte3)

print(' ')
message_decrypte4 = decryptage('MOVK OCD ZVEC PKMSVO AEO ZBOFE',10)
print(message_decrypte4)

# ----------- Main Vigenère ----------------------------
print(' ----------- ')
v = decryptage_vigenere('UI VVBMW IUK MEDMUZFEW', 'SECRET')
print(v)



