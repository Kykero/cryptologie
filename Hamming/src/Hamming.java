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


    public static void main(String args[]){
        Hamming h = new Hamming();
        // Calcul de n et k
        h.n = (int) Math.pow(2, 2) - 1;
        h.k = h.n - 2;

        h.H = new int[2][h.n];
        h.genereH(2, h.n);
        h.systematiqueH();
        printMatrice(h.Hsys);
    }



}//class
