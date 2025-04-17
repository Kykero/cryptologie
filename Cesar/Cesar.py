
alphabet = ['A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
            'P','Q','R','S','T','U','V','W','X','Y','Z']

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






