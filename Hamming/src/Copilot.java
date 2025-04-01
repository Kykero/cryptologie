import java.util.*;

public class Copilot {
    private int n; // longueur du mot de code : n = 2^m - 1
    private int k; // longueur du mot source : k = n - m
    private int[][] G;     // matrice génératrice (non utilisée directement ici)
    private int[][] H;     // matrice de contrôle non réordonnée
    private int[][] Gsys;  // matrice génératrice systématique : (I_k | P)
    private int[][] Hsys;  // matrice de contrôle systématique : (P^T | I_m)
    
    // Question 1 : Génère la matrice de contrôle H à partir de l’énumération de tous les 
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
    
    // Question 2 : Réordonne les colonnes de H afin d’obtenir Hsys = (P^T | I_m).
    // On recherche parmi les colonnes celles qui sont des vecteurs unitaires (exactement un 1)
    // et on les place en fin de matrice, dans l’ordre de la ligne où apparaît le 1 (de haut en bas).
    public void systematiqueH() {
        int m = H.length;
        int n = H[0].length;
        // Liste des colonnes qui ne sont pas des vecteurs unitaires
        List<Integer> nonIdentity = new ArrayList<>();
        // Pour chaque rangée, on cherchera la colonne identité : celle qui a un unique 1 sur cette rangée.
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
            if (countOnes == 1) {  // colonne candidate pour faire partie de I_m
                identityCols[unitRow] = j;
            } else {
                nonIdentity.add(j);
            }
        }
        
        // On construit le nouvel ordre des colonnes :
        // d’abord toutes les colonnes non identité, puis les colonnes identité rangées par rangée 0 à m-1.
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
        
        // Reconstruction de Hsys en réordonnant les colonnes de H selon newOrder.
        Hsys = new int[m][n];
        for (int newCol = 0; newCol < n; newCol++) {
            int oldCol = newOrder[newCol];
            for (int i = 0; i < m; i++) {
                Hsys[i][newCol] = H[i][oldCol];
            }
        }
    }
    
    // Question 3 : À partir de Hsys = (P^T | I_m), extrait la sous-matrice de vérification de parité P.
    // La partie gauche (les k premières colonnes) est P^T. On la transpose pour obtenir P.
    public int[][] extraitP() {
        int m = Hsys.length;
        int n = Hsys[0].length;
        int k = n - m; // k = 2^m - 1 - m
        int[][] P = new int[k][m]; // P de dimension k x m
        
        // Transposition du bloc gauche (de dimension m x k) pour obtenir P.
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < m; j++) {
                P[i][j] = Hsys[j][i];
            }
        }
        return P;
    }
    
    // Question 4 : Construit la matrice génératrice systématique Gsys = (I_k | P) à partir de P.
    public void systematiqueG() {
        int m = H.length;    // nombre de bits de redondance
        int n = H[0].length; // n = 2^m - 1
        int k = n - m;       // dimension du mot source
        int[][] P = extraitP();  // P est de dimension k x m
        
        Gsys = new int[k][n];
        
        // Construction de I_k dans les k premières colonnes.
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                Gsys[i][j] = (i == j) ? 1 : 0;
            }
        }
        // Recopie de P dans les m dernières colonnes.
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < m; j++) {
                Gsys[i][k + j] = P[i][j];
            }
        }
    }
    
    // Méthode utilitaire pour afficher une matrice dans la console.
    public static void printMatrix(int[][] matrix) {
        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix[i].length; j++) {
                System.out.print(matrix[i][j] + " ");
            }
            System.out.println();
        }
    }
    
    
    // Question 5 : Génère et affiche les matrices de Copilot pour m = 2 à 7.
    public static void main(String[] args) {
        for (int m = 2; m <= 7; m++) {
            Copilot h = new Copilot();
            // Calcul de n et k
            h.n = (int) Math.pow(2, m) - 1;
            h.k = h.n - m;
            
            // Allocation de la matrice H (taille m x n)
            h.H = new int[m][h.n];
            
            // Génération de H non systématique
            h.genereH(m, h.n);
            System.out.println("Matrice H (non systématique) pour m = " + m + " (dimension " + m + "x" + h.n + "):");
            printMatrix(h.H);
            System.out.println();
            
            // Réordonnancement pour obtenir Hsys = (P^T | I_m)
            h.systematiqueH();
            System.out.println("Matrice H_sys (systématique) pour m = " + m + " (dimension " + m + "x" + h.n + "):");
            printMatrix(h.Hsys);
            System.out.println();
            
            // Extraction de la sous-matrice P (de dimension k x m)
            int[][] P = h.extraitP();
            System.out.println("Sous-matrice P pour m = " + m + " (dimension " + h.k + "x" + m + "):");
            printMatrix(P);
            System.out.println();
            
            // Construction de la matrice génératrice systématique Gsys = (I_k | P)
            h.systematiqueG();
            System.out.println("Matrice G_sys (systématique) pour m = " + m + " (dimension " + h.k + "x" + h.n + "):");
            printMatrix(h.Gsys);
            System.out.println("------------------------------------------------------");
        }
    }
}
