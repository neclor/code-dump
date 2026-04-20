#include <stdio.h>

#include "prefixe_suffixe.h"

int main(){
   const unsigned int N1 = 9;
   const unsigned int N2 = 10;
   const unsigned int N3 = 9;
   int T1[] = {1,4,2,4,5,1,4,2,4};
   int T2[] = {1,2,3,2,1,1,2,3,2,1};
   int T3[] = {3,2,3,2,1,2,3,2,1};

   printf("Longueur plus long préfixe/suffixe de T1: %u\n", prefixe_suffixe(T1, N1));
   printf("Longueur plus long préfixe/suffixe de T2: %u\n", prefixe_suffixe(T2, N2));
   printf("Longueur plus long préfixe/suffixe de T3: %u\n", prefixe_suffixe(T3, N3));
}
