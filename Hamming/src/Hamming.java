import java.lang.Math;

public class Hamming {
	private int n;
	private int k;
	private int[][]G;
	private int[][]H;
	private int[][]Gsys;
	private int[][]Hsys;

	public Hamming(int n,int k){
		this.n=n;
		this.k=k;
		int m = n-k;
		

	}
	

	
        
	public void genereH(int m, int n) {
		
		int nbBase10;
		int puissance = 2;
		int ii = m-1;
		for(int j=0; j<n ;j++) {
			nbBase10 = j + 1;
			if (nbBase10 >= puissance) {
				puissance *= 2;
				ii --;
			}
			for (int i = m-1; i>=ii; i--) {
				H[i][j] = nbBase10 % 2;
				nbBase10 /=2;
			}
		}	
	}

	public
	
	public void main(String[] args) {
		
	}

}
