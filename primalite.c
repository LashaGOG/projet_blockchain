#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int is_prime_naive(long p) {
    /* Etant donné p impaire renvoie 1 s'il est premier, 0 sinon */
    for (int i = 3; i<p; i+=2) {
        if (p%i == 0) return 0; 
    }
    return 1; 
}

long mod_pow_naive(long a, long m, long n){
    /* Prend en entrée trois entiers a, m et n, et qui retourne la valeur a^b mod n par la métchode naïve */
	long mod=1;
	for(int i=0;i<m;i++){
		mod*=a;
		mod=mod%n;
	}
	return mod;
}

int mod_pow(long a, long m, long n){
    /* Realise meme operation que la fonction mod_pow_naive, sauf pour obtenir la complexité logarithmique élévation est fait au carré */
	if(m==1){
		return a%n;
	}
	if(m%2==0){
		int mod=mod_pow(a,(long)(m/2),n);
		return (mod*mod)%n;
	}
	else{
		int mod=mod_pow(a,(long)((m-1)/2),n);
		return (mod*mod*(a%n))%n;
	}
}
int main(){
	clock_t temps_initial;
	clock_t temps_final;
	double temps1,temps2;
	long a=5821352136521;
	long m;
	long n=6525251414;
	char nom[10]="temps.txt";
	FILE *fichier=fopen(nom,"w");
	for(m=10000000;m<=300000000;m+=10000000){
		temps_initial=clock();
		mod_pow_naive(a,m,n);
		temps_final=clock();
		temps1=(double)(temps_final-temps_initial)/CLOCKS_PER_SEC;
		
		temps_initial=clock();
		mod_pow(a,m,n);
		temps_final=clock();
		temps2=(double)(temps_final-temps_initial)/CLOCKS_PER_SEC;
		
		fprintf(fichier,"%ld %lf %lf\n",m,temps1,temps2);
	}
	fclose(fichier);
}