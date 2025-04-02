import java.util.*;
import java.util.regex.*;

public class Copilot {
    // Paramètres pour un code de Copilot [2^m - 1, 2^m - 1 - m, 3]
    // Ici, pour la partie 6 à 10, nous utilisons m = 4 (donc n = 15, k = 11)
    private int m; // nombre de bits de parité
    private int n; // longueur du mot de code : n = 2^m - 1
    private int k; // dimension du mot source = n - m
    
    // Matrices du code
    private int[][] H;     // matrice de contrôle (générée en ordre naturel)
    private int[][] Hsys;  // matrice de contrôle en forme systématique (P^T | I_m)
    private int[][] Gsys;  // matrice génératrice en forme systématique (I_k | P)
    
    // Table de décodage (pour décodage par tableau) : syndrome (entier) --> vecteur erreur (de dimension n)
    private HashMap<Integer, int[]> decodingTable = new HashMap<>();

    /* ----------------------------------------------------------------
       PARTIE I – Génération des matrices (déjà proposées par le professeur)
       ---------------------------------------------------------------- */
    
    // Méthode fournie pour générer la matrice de contrôle H (ordre naturel)
    public void genereH(int m, int n) {
        int nbBase10;
        int puissance = 2;
        int ii = m - 1;
        // Pour j de 0 à n-1, on génère la représentation binaire de j+1
        for (int j = 0; j < n; j++) {
            nbBase10 = j + 1;
            if (nbBase10 >= puissance) {
                puissance *= 2;
                ii--;
            }
            for (int i = m - 1; i >= ii; i--) {
                H[i][j] = nbBase10 % 2;
                nbBase10 /= 2;
            }
        }
    }
    
    // Réordonne les colonnes de H afin d’obtenir Hsys = (P^T | I_m)
    public void systematiqueH() {
        int m = H.length;
        int n = H[0].length;
        List<Integer> nonIdentity = new ArrayList<>();
        // Pour chaque rangée, on cherche la colonne identité (vecteur avec un seul 1)
        int[] identityCols = new int[m];
        Arrays.fill(identityCols, -1);
        
        for (int j = 0; j < n; j++) {
            int countOnes = 0;
            int unitRow = -1;
            for (int i = 0; i < m; i++) {
                if (H[i][j] == 1) {
                    countOnes++;
                    unitRow = i;
                }
            }
            if (countOnes == 1) {
                identityCols[unitRow] = j;
            } else {
                nonIdentity.add(j);
            }
        }
        
        // Construit le nouvel ordre : d'abord les colonnes non identité, puis les colonnes identité rangées par rangée
        int[] newOrder = new int[n];
        int pos = 0;
        for (int idx : nonIdentity) {
            newOrder[pos++] = idx;
        }
        for (int r = 0; r < m; r++) {
            if (identityCols[r] == -1) {
                System.out.println("Avertissement : colonne identité non trouvée pour la ligne " + r);
            } else {
                newOrder[pos++] = identityCols[r];
            }
        }
        
        // Reconstitution de Hsys à partir de H et de newOrder.
        Hsys = new int[m][n];
        for (int newCol = 0; newCol < n; newCol++) {
            int oldCol = newOrder[newCol];
            for (int i = 0; i < m; i++) {
                Hsys[i][newCol] = H[i][oldCol];
            }
        }
    }
    
    // Extrait la sous-matrice de parité P à partir de Hsys = (P^T | I_m), puis la transpose pour obtenir P (dimension k x m)
    public int[][] extraitP() {
        int m = Hsys.length;
        int n = Hsys[0].length;
        int k = n - m;
        int[][] P = new int[k][m];
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < m; j++) {
                P[i][j] = Hsys[j][i];
            }
        }
        return P;
    }
    
    // Construit la matrice génératrice systématique Gsys = (I_k | P) à partir de P
    public void systematiqueG() {
        int m = H.length;    // nombre de bits de parité
        int n = H[0].length; // n = 2^m - 1
        int k = n - m;       // dimension du mot source
        int[][] P = extraitP();
        
        Gsys = new int[k][n];
        // Remplit I_k dans les k premières colonnes
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                Gsys[i][j] = (i == j) ? 1 : 0;
            }
        }
        // Recopie P dans les m dernières colonnes.
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < m; j++) {
                Gsys[i][k + j] = P[i][j];
            }
        }
    }
    
    // Méthode utilitaire pour afficher une matrice à deux dimensions
    public static void printMatrix(int[][] matrix) {
        for (int[] row : matrix) {
            for (int el : row) {
                System.out.print(el + " ");
            }
            System.out.println();
        }
    }
    
    // Méthode utilitaire pour afficher un vecteur (tableau 1D)
    public static void printVector(int[] vector) {
        for (int el : vector) {
            System.out.print(el);
        }
        System.out.println();
    }
    
    /* ----------------------------------------------------------------
       PARTIE II – Codage, bruit, et décodage (questions 6 à 10)
       ---------------------------------------------------------------- */
    
    // --- Méthodes d'analyse de chaines binaires ---
    
    // Convertit une chaîne binaire (éventuellement avec des espaces) en un tableau d'entiers (0 ou 1)
    public static int[] stringToBitArray(String s) {
        String cleaned = s.replaceAll("\\s+", "");
        int[] bits = new int[cleaned.length()];
        for (int i = 0; i < cleaned.length(); i++) {
            bits[i] = cleaned.charAt(i) - '0';
        }
        return bits;
    }
    
    // Convertit un tableau de bits en chaîne, par exemple pour affichage.
    public static String bitArrayToString(int[] bits) {
        StringBuilder sb = new StringBuilder();
        for (int bit : bits) {
            sb.append(bit);
        }
        return sb.toString();
    }
    
    // --- Codage systématique à l'aide de Gsys ---
    
    // Codage d'un mot source (de dimension k) en le multipliant par la matrice génératrice systématique Gsys (mod 2)
    public int[] encodeSystematic(int[] source) {
        if (Gsys == null) {
            System.out.println("Gsys n'est pas construit.");
            return null;
        }
        int n = Gsys[0].length;
        int k = source.length;
        int[] code = new int[n];
        // Produit matriciel u * Gsys (mod 2)
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int i = 0; i < k; i++) {
                sum ^= (source[i] * Gsys[i][j]); // addition mod 2 (XOR)
            }
            code[j] = sum;
        }
        return code;
    }
    
    // --- Codage en "ordre naturel" classique ---
    // Ici, le codage place les bits de parité aux positions 1,2,4,8 (en numérotation 1-indexée)
    // et insère les bits du message dans les positions restantes.
    public int[] encodeNatural(int[] source) {
        int n = 15;      // Pour m = 4
        int k = 11;
        int[] code = new int[n];
        // Positions des bits de parité (en 0-index : positions 0, 1, 3, 7 correspondant aux positions 1,2,4,8)
        HashSet<Integer> parityPositions = new HashSet<>(Arrays.asList(0, 1, 3, 7));
        int msgPos = 0;
        for (int i = 0; i < n; i++) {
            if (parityPositions.contains(i)) {
                code[i] = 0; // par défaut
            } else {
                code[i] = source[msgPos++];
            }
        }
        // Calcul des bits de parité
        int[] parities = {0, 1, 3, 7}; // indices
        for (int pos : parities) {
            int p = pos + 1; // conversion en numérotation 1-indexée
            int parity = 0;
            // Pour chaque position du mot de code (1-indexé)
            for (int i = 0; i < n; i++) {
                if (((i + 1) & p) != 0) { // si le bit de poids p est présent dans (i+1)
                    parity ^= code[i];
                }
            }
            code[pos] = parity;
        }
        return code;
    }
    
    // --- Ajout de bruit (opération XOR) ---
    // Retourne la somme modulo 2 (bit à bit) entre code et noise.
    public static int[] applyNoise(int[] code, int[] noise) {
        if (code.length != noise.length) {
            throw new IllegalArgumentException("Les vecteurs doivent être de même longueur.");
        }
        int[] result = new int[code.length];
        for (int i = 0; i < code.length; i++) {
            result[i] = code[i] ^ noise[i];
        }
        return result;
    }
    
    // --- Décodage par tableau ---
    
    // Construit le tableau de décodage (ici une table de hachage) associant
    // à chaque syndrome (calculé à partir de Hsys) le vecteur erreur unitaire correspondant.
    public void buildDecodingTable() {
        decodingTable.clear();
        int m = Hsys.length;
        int n = Hsys[0].length;
        // Pour chaque colonne j, calculer le syndrome (c'est la jème colonne de Hsys)
        for (int j = 0; j < n; j++) {
            int[] syndromeCol = new int[m];
            for (int i = 0; i < m; i++) {
                syndromeCol[i] = Hsys[i][j];
            }
            int sVal = syndromeToInt(syndromeCol);
            // Construit le vecteur erreur correspondant (de longueur n, avec un 1 en position j)
            int[] errorVector = new int[n];
            errorVector[j] = 1;
            decodingTable.put(sVal, errorVector);
        }
        // Le syndrome nul correspond à aucun erreur.
        decodingTable.put(0, new int[n]); // vecteur nul
    }
    
    // Calcule le syndrome d'un mot reçu y à l'aide d'une matrice H donnée (y * H^T mod 2)
    public static int[] computeSyndrome(int[] y, int[][] Hmatrix) {
        int m = Hmatrix.length;
        int n = Hmatrix[0].length;
        int[] syndrome = new int[m];
        for (int i = 0; i < m; i++) {
            int sum = 0;
            for (int j = 0; j < n; j++) {
                sum ^= (y[j] * Hmatrix[i][j]);
            }
            syndrome[i] = sum;
        }
        return syndrome;
    }
    
    // Convertit un vecteur syndrome (de longueur m) en entier (en considérant le bit 0 comme MSB)
    public static int syndromeToInt(int[] syndrome) {
        int sVal = 0;
        for (int i = 0; i < syndrome.length; i++) {
            sVal = (sVal << 1) | syndrome[i];
        }
        return sVal;
    }
    
    // Extraction du mot source à partir d'un mot de code systématique : ce sont les k premiers bits
    public int[] extractMessageFromSystematic(int[] code) {
        int[] message = new int[k];
        for (int i = 0; i < k; i++) {
            message[i] = code[i];
        }
        return message;
    }
    
    // Décodage par tableau. Pour un mot bruité reçu, on calcule le syndrome, on récupère
    // le vecteur d'erreur dans la table et on corrige le mot de code, puis on extrait le message.
    public int[] decodeByTable(int[] received) {
        int[] syndrome = computeSyndrome(received, Hsys);
        int sVal = syndromeToInt(syndrome);
        int[] errorVector = decodingTable.get(sVal);
        if (errorVector == null) {
            System.out.println("Syndrome inconnu : " + Arrays.toString(syndrome));
            errorVector = new int[received.length]; // aucun correction
        }
        int[] corrected = new int[received.length];
        for (int i = 0; i < received.length; i++) {
            corrected[i] = received[i] ^ errorVector[i];
        }
        return extractMessageFromSystematic(corrected);
    }
    
    // --- Décodage par l'astuce (trick) sur le codage en ordre naturel ---
    // Dans le codage naturel, les positions 1,2,4,8 (en numérotation 1-indexée) sont les bits de parité.
    // Le syndrome est calculé de façon classique (en vérifiant pour chaque bit de parité la parité
    // sur les positions couvertes). Si le syndrome est non nul, sa valeur (en 1-indexé) indique la position erronée.
    public static int[] decodeByTrick(int[] receivedNatural) {
        int n = receivedNatural.length; // 15
        int syndrome = 0;
        // Les positions de parité en numérotation 1-indexée : 1, 2, 4, 8
        int[] parityPositions = {1, 2, 4, 8};
        for (int p : parityPositions) {
            int parity = 0;
            // Vérifie toutes les positions de 1 à n (1-indexé)
            for (int i = 0; i < n; i++) {
                if (((i + 1) & p) != 0) { // si la position (i+1) est couverte par la parité p
                    parity ^= receivedNatural[i];
                }
            }
            if (parity != 0) {
                syndrome += p;
            }
        }
        // Si syndrome != 0, l'erreur se trouve en position syndrome (1-indexé)
        int[] corrected = Arrays.copyOf(receivedNatural, n);
        if (syndrome != 0) {
            int errorPos = syndrome - 1; // conversion en index 0-indexé
            corrected[errorPos] ^= 1;
        }
        // Extraction du mot source : en codage naturel, ce sont les positions qui ne sont pas des puissances de 2.
        return extractMessageFromNatural(corrected);
    }
    
    // Extraction du message à partir d'un mot de code en ordre naturel.
    // Les positions en numérotation 1-indexée qui sont des puissances de 2 (1,2,4,8) sont les bits de parité.
    public static int[] extractMessageFromNatural(int[] code) {
        int n = code.length; // 15
        ArrayList<Integer> msgList = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            int pos = i + 1;
            // Si pos n'est pas une puissance de 2, c'est un bit de message.
            if (!(pos == 1 || pos == 2 || pos == 4 || pos == 8)) {
                msgList.add(code[i]);
            }
        }
        int[] message = new int[msgList.size()];
        for (int i = 0; i < msgList.size(); i++) {
            message[i] = msgList.get(i);
        }
        return message;
    }
    
    // --- Simulation de bruit BSC ---
    // Pour chaque bit du mot de code, on le renverse (flip) avec probabilité prob.
    public static int[] applyBSCNoise(int[] code, double prob) {
        int n = code.length;
        int[] noisy = new int[n];
        Random rand = new Random();
        for (int i = 0; i < n; i++) {
            if (rand.nextDouble() < prob) {
                noisy[i] = code[i] ^ 1; // inverse le bit
            } else {
                noisy[i] = code[i];
            }
        }
        return noisy;
    }
    
    
    /* ----------------------------------------------------------------
       Main – Mise en œuvre de la partie TP (questions 6 à 10)
       ---------------------------------------------------------------- */
    public static void main(String[] args) {
        // Pour les questions 6 à 10, nous travaillons avec m = 4 (donc n = 15, k = 11)
        int mVal = 4;
        int nVal = (int) Math.pow(2, mVal) - 1; // 15
        int kVal = nVal - mVal;                // 11
        
        Copilot h = new Copilot();
        h.m = mVal;
        h.n = nVal;
        h.k = kVal;
        
        // Allocation de la matrice H (ordre naturel)
        h.H = new int[mVal][nVal];
        h.genereH(mVal, nVal);
        
        // Passage en forme systématique et génération de la matrice génératrice Gsys
        h.systematiqueH();
        h.systematiqueG();
        
        // Construction du tableau de décodage (pour décodage par tableau)
        h.buildDecodingTable();
        
        System.out.println("----- TP – Codage / Bruitage / Décodage pour un Code de Copilot [15,11,3] -----\n");
        
        // ===============================
        // Question 6 : Codage d'un mot source
        // ===============================
        // Mots source donnés (avec espaces pour la lisibilité, on les nettoie)
        String u0Str = "000 0100 0011";
        String u1Str = "000 0100 1111";
        String u2Str = "000 0100 0100";
        String u3Str = "000 0100 0101";
        
        // Conversion en tableaux de bits (longueur 11)
        int[] u0 = stringToBitArray(u0Str);
        int[] u1 = stringToBitArray(u1Str);
        int[] u2 = stringToBitArray(u2Str);
        int[] u3 = stringToBitArray(u3Str);
        
        // Codage systématique via Gsys (u * Gsys)
        int[] c0 = h.encodeSystematic(u0);
        int[] c1 = h.encodeSystematic(u1);
        int[] c2 = h.encodeSystematic(u2);
        int[] c3 = h.encodeSystematic(u3);
        
        System.out.println("Résultats du codage systématique :");
        System.out.println("c0 = " + bitArrayToString(c0));
        System.out.println("c1 = " + bitArrayToString(c1));
        System.out.println("c2 = " + bitArrayToString(c2));
        System.out.println("c3 = " + bitArrayToString(c3));
        System.out.println();
        
        // ===============================
        // Question 7 : Bruitage (1 erreur par mot de code)
        // Bruit donnés (chaînes sur 15 bits)
        String e1Str = "000 1000 0000 0000";
        String e2Str = "000 0010 0000 0000";
        String e3Str = "000 0000 0010 0000";
        String e4Str = "000 0000 0000 0001";
        
        int[] e1 = stringToBitArray(e1Str);
        int[] e2 = stringToBitArray(e2Str);
        int[] e3 = stringToBitArray(e3Str);
        int[] e4 = stringToBitArray(e4Str);
        
        // Bruitage : y = c XOR e
        int[] y1 = applyNoise(c0, e1);
        int[] y2 = applyNoise(c1, e2);
        int[] y3 = applyNoise(c2, e3);
        int[] y4 = applyNoise(c3, e4);
        
        System.out.println("Mots de code après bruitage (systématique) :");
        System.out.println("y1 = " + bitArrayToString(y1));
        System.out.println("y2 = " + bitArrayToString(y2));
        System.out.println("y3 = " + bitArrayToString(y3));
        System.out.println("y4 = " + bitArrayToString(y4));
        System.out.println();
        
        // ===============================
        // Question 8 : Décodage par tableau
        System.out.println("Décodage par tableau (avec Hsys) :");
        int[] dec0 = h.decodeByTable(y1);
        int[] dec1 = h.decodeByTable(y2);
        int[] dec2 = h.decodeByTable(y3);
        int[] dec3 = h.decodeByTable(y4);
        System.out.println("u0 décodé = " + bitArrayToString(dec0));
        System.out.println("u1 décodé = " + bitArrayToString(dec1));
        System.out.println("u2 décodé = " + bitArrayToString(dec2));
        System.out.println("u3 décodé = " + bitArrayToString(dec3));
        System.out.println();
        
        // ===============================
        // Question 9 : Décodage par l'astuce (trick)
        // Dans cette version, on recode les mots sources en "ordre naturel" et applique le décodage par l'astuce.
        // Le codage naturel place les bits de parité aux positions 1,2,4,8 (1-indexé).
        int[] c0Nat = h.encodeNatural(u0);
        int[] c1Nat = h.encodeNatural(u1);
        int[] c2Nat = h.encodeNatural(u2);
        int[] c3Nat = h.encodeNatural(u3);
        // Bruitage sur les codages naturels (les mêmes vecteurs e1 à e4 sont compatibles avec l'ordre naturel)
        int[] y1Nat = applyNoise(c0Nat, e1);
        int[] y2Nat = applyNoise(c1Nat, e2);
        int[] y3Nat = applyNoise(c2Nat, e3);
        int[] y4Nat = applyNoise(c3Nat, e4);
        
        System.out.println("Décodage par l'astuce (ordre naturel) :");
        int[] dec0Nat = decodeByTrick(y1Nat);
        int[] dec1Nat = decodeByTrick(y2Nat);
        int[] dec2Nat = decodeByTrick(y3Nat);
        int[] dec3Nat = decodeByTrick(y4Nat);
        System.out.println("u0 décodé = " + bitArrayToString(dec0Nat));
        System.out.println("u1 décodé = " + bitArrayToString(dec1Nat));
        System.out.println("u2 décodé = " + bitArrayToString(dec2Nat));
        System.out.println("u3 décodé = " + bitArrayToString(dec3Nat));
        System.out.println();
        
        // ===============================
        // Question 10 : Bruit BSC (Binary Symmetric Channel)
        // On applique un bruit aléatoire (ex : probabilité p = 0.1 de renverser chaque bit)
        double p = 0.1;
        int[] codeForBSC = c0Nat; // on utilise, par exemple, le code naturel de u0
        int[] bscCode = applyBSCNoise(codeForBSC, p);
        System.out.println("Code d'u0 après bruit BSC (p = " + p + ") :");
        System.out.println(bitArrayToString(bscCode));
        // Vous pouvez ensuite tenter un décodage (par exemple avec decodeByTrick) pour voir si la correction s'effectue.
        int[] decBSC = decodeByTrick(bscCode);
        System.out.println("u0 décodé après bruit BSC = " + bitArrayToString(decBSC));
        System.out.println();
        
        /* ----------------------------------------------------------------
           BONUS – Pistes complémentaires :
           - D'autres implémentations de bruit peuvent consister à sélectionner aléatoirement
             un nombre d'erreurs (par exemple, entre 0 et 2) par mot de code.
           - Une représentation polynomiale des mots (en utilisant des objets BigInteger ou des structures personnalisées)
             permet d'exprimer le codage/décodage via les opérations sur les polynômes (division, modulo, etc.).
           Ces pistes restent à explorer.
           ---------------------------------------------------------------- */
    }
}
