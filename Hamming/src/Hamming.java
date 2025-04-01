import java.util.*;

public class Hamming {
    private int n; // longueur du mot de code : n = 2^m - 1
    private int k; // longueur du mot source : k = n - m
    private int[][] G; // matrice génératrice (non utilisée directement ici)
    private int[][] H; // matrice de contrôle non réordonnée
    private int[][] Gsys; // matrice génératrice systématique : (I_k | P)
    private int[][] Hsys; // matrice de contrôle systématique : (P^T | I_m)




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
        
        //Liste des colonnes qui ne sont pas des vecruers unitaires 
        List<Integer> nonIdentite = new ArrayList<>();

        int[] ColonnesIdentite = new int[m];
        Arrays.fill(ColonnesIdentite, -1); // -1 permet de vérifier les passages de la boucle qui arrive après
        
        for(int j = 0;j < n; j++){
            int nbUn = 0;
            int PositionUn = -1;
            for(int i = 0; i < m; i++){
                if(H[i][j] == 1){
                    nbUn++;
                    PositionUn = i;
                }
            }
            if(nbUn == 1){
                ColonnesIdentite[PositionUn] = j;
            }else{
                nonIdentite.add(j);
            }
        }

        //On construit maintenant le nouvel ordre des colonnes, d'abord les colonnes non identité, puis
        // les colonnes identités
        int[]  nouvelOrdre = new int[n];
        int position = 0;

        
        for(int index : nonIdentite){
            nouvelOrdre[position++] = index;
        }

        for(int r = 0; r < m; r++){
            if(ColonnesIdentite[r] == -1){
                System.out.println("Colonne identité non trouvée pour la ligne (systematiqueH) " + r);
            }else{
                nouvelOrdre[position++] = ColonnesIdentite[r];
            }
        }

        //On injecte dans Hsys en réordonnant les colonnes de H selon le nouvel Ordre
        Hsys = new int[m][n];
        for(int nouvelCol = 0; nouvelCol < n; nouvelCol++){
            int ancienneCol = nouvelOrdre[nouvelCol];
            for(int i = 0; i < m; i++){
                Hsys[i][nouvelCol] = H[i][ancienneCol];
            }
        }

    }//systematiqueH


    //Question 3 ExtraitP (sous matrice de vérification de parité)
    public int[][] extraitP(){
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
    }//extraitP

    // Question 4 : Construction de la matrice génératrice systématique Gsys
    public void systematiqueG() {
        int m = H.length;    
        int n = H[0].length; 
        int k = n - m;      
        int[][] P = extraitP();  // P est de dimension k x m

        Gsys = new int[k][n];

        // Construction de l'identité dans les k premières colonnes
        for(int i = 0; i < k ; i++){
            for(int j = 0; j < k; j++){
                Gsys[i][j] = (i == j) ? 1 : 0; // Si on est sur la diagonale on assigne 1, sinon 0 (cela permet à construire l'identité)
            }
        }

        // On injecte P dans les m dernières colonnes
        for(int i = 0; i < k; i ++) {
            for (int j = 0; j < m; j++){
                Gsys[i][k+j] = P[i][j];
            }
        }
    }





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
            System.out.println("Matrice H (non systématique) pour m = " + m + " (dimension " + m + "x" + h.n + "):");
            printMatrice(h.H);
            System.out.println();
            
            // Réordonnancement pour obtenir Hsys 
            h.systematiqueH();
            System.out.println("Matrice H_sys (systématique) pour m = " + m + " (dimension " + m + "x" + h.n + "):");
            printMatrice(h.Hsys);
            System.out.println();
            
            // Extraction de la sous-matrice P (de dimension k x m)
            int[][] P = h.extraitP();
            System.out.println("Sous-matrice P pour m = " + m + " (dimension " + h.k + "x" + m + "):");
            printMatrice(P);
            System.out.println();
            
            // Construction de la matrice génératrice systématique Gsys
            h.systematiqueG();
            System.out.println("Matrice G_sys (systématique) pour m = " + m + " (dimension " + h.k + "x" + h.n + "):");
            printMatrice(h.Gsys);
            System.out.println("------------------------------------------------------");
        }
    }//main


}//class
