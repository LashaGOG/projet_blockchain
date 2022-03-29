#include <stdio.h>
#include "primalite.h"

int is_prime_naive(long p)
{
	/* Etant donné p impaire renvoie 1 s'il est premier, 0 sinon */
	for (int i = 3; i < p; i += 2)
	{
		if (p % i == 0)
			return 0;
	}
	return 1;
}

long mod_pow_naive(long a, long m, long n)
{
	/* Prend en entrée trois entiers a, m et n, et qui retourne la valeur a^m mod n par la métchode naïve */
	long val_courant = 1;
	for (int i = 0; i < m; i++)
	{
		val_courant *= a;
		val_courant = val_courant % n;
	}
	return val_courant;
}

int mod_pow(long a, long m, long n)
{
	/* Realise meme operation que la fonction mod_pow_naive, sauf pour obtenir la complexité logarithmique élévation est fait au carré */
	// Les cas de base
	if (a == 0)
		return 0;
	if (m == 0)
		return 1;

	// Si m est paire
	long b;
	if (m % 2 == 0)
	{
		b = mod_pow(a, m / 2, n);
		b = (b * b) % n;
	}

	// Si m est impaire
	else
	{
		b = a % n;
		b = (b * mod_pow(a, m - 1, n) % n) % n;
	}
	return (int)((b + n) % n);
}
int witness(long a, long b, long d, long p)
{
	/* teste si a est un témoin de Miller pour p, pour un entier a donné */
	long x = mod_pow(a, d, p);
	if (x == 1)
	{
		return 0;
	}
	for (long i = 0; i < b; i++)
	{
		if (x == p - 1)
		{
			return 0;
		}
		x = mod_pow(x, 2, p);
	}
	return 1;
}

long rand_long(long low, long up)
{
	return rand() % (up - low + 1) + low;
}

int is_prime_miller(long p, int k)
{
	if (p == 2)
	{
		return 1;
	}
	if (!(p & 1) || p <= 1) // on verifie que p est impair et different de 1
	{
		return 0;
	}
	// on determine b et d :
	long b = 0;
	long d = p - 1;
	while (!(d & 1)) // tant que d n’est pas impair
	{
		d = d / 2;
		b = b + 1;
	}
	// On genere k valeurs pour a, et on teste si c’est un temoin :
	long a;
	int i;
	for (i = 0; i < k; i++)
	{
		a = rand_long(2, p - 1);
		if (witness(a, b, d, p))
		{
			return 0;
		}
	}
	return 1;
}
long random_prime_number(int low_size, int up_size, int k)
{
	/* retourne un nombre premier de taille comprise entre low_size et up_size */
	long val_low = 1;
	int t;
	// on obtient la valeure minimal
	for (t = 0; t < low_size; t++)
	{
		val_low *= 2;
	}
	// on obtient la valeure maximale
	long val_up = val_low;
	while (t < up_size)
	{
		val_up *= 2;
		t++;
	}
	val_up--;

	// on cherche le nb premier entre les valeurs obtenus
	long p = rand_long(val_low, val_up);
	while (is_prime_miller(p, k) != 1)
	{
		p = rand_long(val_low, val_up);
	}
	return p;
}

// int main()
// {
// 	clock_t temps_initial;
// 	clock_t temps_final;
// 	double temps1, temps2;
// 	long a = 5821352136521;
// 	long m;
// 	long n = 6525251414;
// 	char nom[10] = "temps.txt";
// 	FILE *fichier = fopen(nom, "w");
// 	for (m = 10000000; m <= 300000000; m += 10000000)
// 	{
// 		temps_initial = clock();
// 		mod_pow_naive(a, m, n);
// 		temps_final = clock();
// 		temps1 = (double)(temps_final - temps_initial) / CLOCKS_PER_SEC;

// 		temps_initial = clock();
// 		mod_pow(a, m, n);
// 		temps_final = clock();
// 		temps2 = (double)(temps_final - temps_initial) / CLOCKS_PER_SEC;

// 		fprintf(fichier, "%ld %lf %lf\n", m, temps1, temps2);
// 	}
// 	fclose(fichier);
//  }
