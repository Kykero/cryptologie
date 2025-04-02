import java.util.*;

public class Hamming {
    private int n; // longueur du mot de code : n = 2^m - 1
    private int k; // longueur du mot source : k = n - m
    private int[][] G; // matrice génératrice
    private int[][] H; // matrice de contrôle non réordonnée
    private int[][] Gsys; // matrice génératrice systématique
    private int[][] Hsys; // matrice de contrôle systématique

    // Tableau de décodage
    private int[][] decodingTable;

    // Méthode de débuggage pour afficher les matrices
    public static void printMatrice(int[][] matrice) {
        for (int i = 0; i < matrice.length; i++) {
            for (int j = 0; j < matrice[i].length; j++) {
                System.out.print(matrice[i][j] + " ");
            }
            System.out.println();
        }
    }

    // Question 1 : Génère la matrice de contrôle H à partir de l’énumération de
    // tous les
    // mots de m bits non nuls. (Le professeur a proposé cette correction).
    public void genereH(int m, int n) {
        int nbBase10;
        int puissance = 2;
        int ii = m - 1;
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

    // Qestuin 2 : SystematiqueH

    public void systematiqueH() {
        int m = H.length;
        int n = H[0].length;

        // Liste des colonnes qui ne sont pas des vecruers unitaires
        List<Integer> nonIdentite = new ArrayList<>();

        int[] ColonnesIdentite = new int[m];
        Arrays.fill(ColonnesIdentite, -1); // -1 permet de vérifier les passages de la boucle qui arrive après

        for (int j = 0; j < n; j++) {
            int nbUn = 0;
            int PositionUn = -1;
            for (int i = 0; i < m; i++) {
                if (H[i][j] == 1) {
                    nbUn++;
                    PositionUn = i;
                }
            }
            if (nbUn == 1) {
                ColonnesIdentite[PositionUn] = j;
            } else {
                nonIdentite.add(j);
            }
        }

        // On construit maintenant le nouvel ordre des colonnes, d'abord les colonnes
        // non identité, puis
        // les colonnes identités
        int[] nouvelOrdre = new int[n];
        int position = 0;

        for (int index : nonIdentite) {
            nouvelOrdre[position++] = index;
        }

        for (int r = 0; r < m; r++) {
            if (ColonnesIdentite[r] == -1) {
                System.out.println("Colonne identité non trouvée pour la ligne (systematiqueH) " + r);
            } else {
                nouvelOrdre[position++] = ColonnesIdentite[r];
            }
        }

        // On injecte dans Hsys en réordonnant les colonnes de H selon le nouvel Ordre
        Hsys = new int[m][n];
        for (int nouvelCol = 0; nouvelCol < n; nouvelCol++) {
            int ancienneCol = nouvelOrdre[nouvelCol];
            for (int i = 0; i < m; i++) {
                Hsys[i][nouvelCol] = H[i][ancienneCol];
            }
        }

    }// systematiqueH

    // Question 3 ExtraitP (sous matrice de vérification de parité)
    public int[][] extraitP() {
        int m = Hsys.length;
        int n = Hsys[0].length;
        int k = n - m; // k = 2^m - 1 - m
        int[][] P = new int[k][m]; // P de dimension k x m

        // Transposition de tout sauf l'identité
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < m; j++) {
                P[i][j] = Hsys[j][i];
            }
        }
        return P;
    }// extraitP

    // Question 4 : Construction de la matrice génératrice systématique Gsys
    public void systematiqueG() {
        int m = H.length;
        int n = H[0].length;
        int k = n - m;
        int[][] P = extraitP(); // P est de dimension k x m

        Gsys = new int[k][n];

        // Construction de l'identité dans les k premières colonnes
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                Gsys[i][j] = (i == j) ? 1 : 0; // Si on est sur la diagonale on assigne 1, sinon 0 (cela permet à
                                               // construire l'identité)
            }
        }

        // On injecte P dans les m dernières colonnes
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < m; j++) {
                Gsys[i][k + j] = P[i][j];
            }
        }
    }

    // Question 6 : C0 C1 C2 C3 mot source
    //
    // Codage d'un mot source via la matrice génératrice systématique Gsys (mod 2)
    public int[] encodeSystematic(int[] source) {
        if (Gsys == null) {
            System.out.println("Gsys n'as pas été construit ! (encodeSystematic)");
            return null;
        }
        int n = Gsys[0].length; // Longueur du mot de code
        int k = source.length; // Longueur du mot source
        int[] code = new int[n]; // Code généré

        // Produit matriciel u * Gsys
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int i = 0; i < k; i++) {
                sum ^= (source[i] * Gsys[i][j]);
            }
            code[j] = sum;
        }
        return code;
    }

    // Méthode pour convertir en tableau de bits
    public static int[] stringToBitArray(String s) {
        String cleaned = s.replaceAll("\\s+", ""); // Regular expression pour nettoyer tout les espaces
        int[] bits = new int[cleaned.length()];
        for (int i = 0; i < cleaned.length(); i++) {
            bits[i] = cleaned.charAt(i) - '0'; // converti chaque caractère
        }
        return bits;
    }

    // Convertit un tableau de bits en chaîne (pour affichage)
    public static String bitArrayToString(int[] bits) {
        StringBuilder sb = new StringBuilder();
        for (int bit : bits) {
            sb.append(bit);
        }
        return sb.toString();
    }

    // Question 7 Ajout du bruit dans le code (il s'agit d'une opération logique
    // XOR)
    public static int[] ajoutBruit(int[] code, int[] bruit) {
        if (code.length != bruit.length) {
            System.out.println("Les vecteurs ne sont pas de même longueurs (ajoutBruit)");
            System.exit(-1);
        }
        int[] result = new int[code.length];
        for (int i = 0; i < code.length; i++) {
            result[i] = code[i] ^ bruit[i];
        }
        return result;
    }

    // Question 8 Décodage par tableau

    // On commence par faire la méthode de calcul du syndrome
    public static int[] calculSyndrome(int[] y, int[][] Hmatrice) {
        int m = Hmatrice.length;
        int n = Hmatrice[0].length;
        int[] syndrome = new int[m];

        for (int i = 0; i < m; i++) {
            int sum = 0;
            for (int j = 0; j < n; j++) {
                sum ^= (y[j] * Hmatrice[i][j]); // Logique XOR
            }
            syndrome[i] = sum;
        }
        return syndrome;
    }

    // Conversion du syndrôme en entier pour faciliter la recherche (on récupère
    // directement l'indice)
    /*
     * On considère le syndrome comme un nombre binaire et on le converti en entier
     */
    public int SyndromEnEntier(int[] syndrome) {
        int SyndromeValeur = 0;
        for (int i = 0; i < syndrome.length; i++) {
            SyndromeValeur = (SyndromeValeur << 1) | syndrome[i];
        }
        return SyndromeValeur;
    }

    // IL nous faut une méthode pour extraire le mot source à partir d'un mot de
    // code systématique (k premiers bits)
    public int[] Extraction(int[] code) {
        int[] message = new int[k]; // On prend les k premiers bits
        System.arraycopy(code, 0, message, 0, k); // Permet de copier rapidement le tableau vers un autre !
        return message;
    }

    // Table de décodage
    public void ConstructionTable() {
        int m = Hsys.length;
        int n = Hsys[0].length;
        int nombreDeSyndrome = (int) Math.pow(2, m);

        decodingTable = new int[nombreDeSyndrome][n];

        for (int j = 0; j < n; j++) {
            int[] SyndromeColonne = new int[m];
            for (int i = 0; i < m; i++) {
                SyndromeColonne[i] = Hsys[i][j];
            }
            int ValeurSyndrome = SyndromEnEntier(SyndromeColonne);
            decodingTable[ValeurSyndrome][j] = 1;
        }
        // Le cas ou le syndrôme est nul
        for (int j = 0; j < n; j++) {
            decodingTable[0][j] = 0;
        }
    }

    // Décodage à l'aide du tableau
    public int[] DecodeTable(int[] vecteur) {
        int[] syndrome = calculSyndrome(vecteur, Hsys);
        int ValeurSyndrome = SyndromEnEntier(syndrome);

        int[] erreur = decodingTable[ValeurSyndrome];

        int[] correction = new int[vecteur.length];
        for (int i = 0; i < vecteur.length; i++) {
            correction[i] = vecteur[i] ^ erreur[i]; // Correction par la porte logique XOR
        }
        return Extraction(correction);
    }


    // Question 9 





    public static void main(String[] args) {

        // Question 5 : m = 2 à 7
        for (int m = 2; m <= 7; m++) {
            Hamming h = new Hamming();
            // Calcul de n et k
            h.n = (int) Math.pow(2, m) - 1;
            h.k = h.n - m;

            // Allocation de la matrice H
            h.H = new int[m][h.n];

            // Génération de H non systématique
            h.genereH(m, h.n);
            System.out.println("Matrice H (non systématique) pour m = " + m + "(dimension " + m + "x" + h.n + "):");
            printMatrice(h.H);
            System.out.println();

            // Réordonnancement pour obtenir Hsys
            h.systematiqueH();
            System.out.println("Matrice H_sys (systématique) pour m = " + m + "(dimension " + m + "x" + h.n + "):");
            printMatrice(h.Hsys);
            System.out.println();

            // Extraction de la sous-matrice P (de dimension k x m)
            int[][] P = h.extraitP();
            System.out.println("Sous-matrice P pour m = " + m + " (dimension " + h.k +
                    "x" + m + "):");
            printMatrice(P);
            System.out.println();

            // Construction de la matrice génératrice systématique Gsys
            h.systematiqueG();
            System.out.println("Matrice G_sys (systématique) pour m = " + m + "(dimension " + h.k + "x" + h.n + "):");
            printMatrice(h.Gsys);
            System.out.println("------------------------------------------------------");
        }

        // ===============================
        // Question 6 : Codage d'un mot source
        // ===============================
        // Mots source donnés (avec espaces pour la lisibilité, on les nettoie)

        int mVal = 4;
        int nVal = (int) Math.pow(2, mVal) - 1; // 15
        int kVal = nVal - mVal; // 11

        Hamming h = new Hamming();
        h.n = nVal;
        h.k = kVal;

        // Allocation de la matrice H (ordre naturel)
        h.H = new int[mVal][nVal];
        h.genereH(mVal, nVal);

        // Passage en forme systématique et génération de la matrice génératrice Gsys
        h.systematiqueH();
        h.systematiqueG();
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
        // Question 7 : Ajout du bruit
        // ===============================
        String e1Str = "000 1000 0000 0000";
        String e2Str = "000 0010 0000 0000";
        String e3Str = "000 0000 0010 0000";
        String e4Str = "000 0000 0000 0001";

        int[] e1 = stringToBitArray(e1Str);
        int[] e2 = stringToBitArray(e2Str);
        int[] e3 = stringToBitArray(e3Str);
        int[] e4 = stringToBitArray(e4Str);

        int[] y1 = ajoutBruit(c0, e1);
        int[] y2 = ajoutBruit(c1, e2);
        int[] y3 = ajoutBruit(c2, e3);
        int[] y4 = ajoutBruit(c3, e4);

        System.out.println("Mots de code après bruitage (systématique) :");
        System.out.println("y1 = " + bitArrayToString(y1));
        System.out.println("y2 = " + bitArrayToString(y2));
        System.out.println("y3 = " + bitArrayToString(y3));
        System.out.println("y4 = " + bitArrayToString(y4));
        System.out.println();

        // ===============================
        // Question 8 : Décodage par tableau
        // ===============================
        h.ConstructionTable();
        int[] dec0 = h.DecodeTable(y1);
        int[] dec1 = h.DecodeTable(y2);
        int[] dec2 = h.DecodeTable(y3);
        int[] dec3 =h. DecodeTable(y4);

        System.out.println("Décodage par tableau standard :");
        System.out.println("u0 décodé = " + bitArrayToString(dec0));
        System.out.println("u1 décodé = " + bitArrayToString(dec1));
        System.out.println("u2 décodé = " + bitArrayToString(dec2));
        System.out.println("u3 décodé = " + bitArrayToString(dec3));

    }// main

}// class
