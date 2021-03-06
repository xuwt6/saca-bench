// This is a demo program for showing how to call SACA_K.

#include<stdio.h>
#include <stdlib.h>
#include <time.h>

void SACA_K(unsigned char *s, unsigned int *SA, unsigned int n, 
           unsigned int K, unsigned int m, int level);

// uncomment the below line to verify the result SA
//#define _verify_sa

// output values:
//  1: s1<s2
//  0: s1=s2
// -1: s1>s2
int sless(unsigned char *s1, unsigned char *s2, unsigned int n) {
	for(unsigned int i=0; i<n; i++) {
		if (s1[i] < s2[i]) return 1;
		if (s1[i] > s2[i]) return -1;
	}
	return 0;
} 

// test if SA is sorted for the input string s
bool isSorted(unsigned int *SA, unsigned char *s, unsigned int n) {
	for(unsigned int i = 0;  i < n-1;  i++) {
	  unsigned int d=SA[i]<SA[i+1]?n-SA[i+1]:n-SA[i];
		int rel=sless(s+SA[i], s+SA[i+1], d);
		if(rel==-1 || rel==0 && SA[i+1]>SA[i])
			return false;
	}
	return true;  
}

int main(int argc, char **argv) {
	fprintf(stderr, "\nComputing suffix array by SACA-K on ");
	if (argc>1) {
		freopen(argv[1], "rb", stdin);
		fprintf(stderr, "%s", argv[1]);
	} else
		fprintf(stderr, "stdin");
	fprintf(stderr, " to ");
	if (argc>2) {
		freopen(argv[2], "wb", stdout);
		fprintf(stderr, "%s", argv[2]);
	} else
		fprintf(stderr, "stdout");
	fprintf(stderr, "\n");
	#if !defined(unix)
	setmode(fileno(stdin), O_BINARY);
	setmode(fileno(stdout), O_BINARY);
	#endif

	// Allocate 5 bytes memory for input string and output suffix array
	fseek(stdin, 0, SEEK_END);
	unsigned int n=ftell(stdin);
	if(n==0) {
		fprintf(stderr, "\nEmpty string, nothing to sort!");
		return 0;
	}
	else if(n+1==0) {
		fprintf(stderr, "\nCan not sort file of n>=%u bytes!", n);
		return 0;
	}

	n++; // append the virtual sentinel
	fprintf(stderr, "Allocating input and output space: %u bytes = %.2lf MB", 5*n, (double)5*n/1024/1024);
	unsigned char *s_ch=new unsigned char[n];
	unsigned int *SA = new unsigned int[n];
	if(s_ch==NULL || SA==NULL) {
		delete [] s_ch; delete [] SA; 
		fprintf(stderr, "\nInsufficient memory, exit!");
		return 0;
	}

	// read the string into buffer.
	fprintf(stderr, "\nReading input string...");
	fseek(stdin, 0, SEEK_SET );
	fread((unsigned char *) s_ch, 1, n-1, stdin);
	// set the virtual sentinel
	s_ch[n-1]=0;

	clock_t start, finish;
	double  duration;
	start = clock();

	fprintf(stderr, "\nConstructing the suffix array...");
	SACA_K(s_ch, SA, n, 256, n, 0);

	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;

	fprintf(stderr, "\nSize: %u bytes, Time: %5.3f seconds\n", n-1, duration);
	
#ifdef _verify_sa  
  fprintf(stderr, "\nVerifying the suffix array...");
	fprintf(stderr, "\nSorted: %d", (int)isSorted(SA+1, s_ch, n-1));
#endif

/*
  fprintf(stderr, "\nOutputing the suffix array...");
	for(unsigned int i=1; i<n; i++)
		fwrite((unsigned char *)(SA+i), 1, sizeof(int), stdout);
*/

	delete [] SA;
	delete [] s_ch;
	return 0;
}
