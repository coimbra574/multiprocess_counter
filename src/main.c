#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define N_PROCESSOS 4

int primo(unsigned long int vetor) {   //funcao que verifica numero primo
  unsigned long int k=0;
  int achei, contador=0;

 if(vetor==0 || vetor==1){achei=0;}
 else{
	for(k=1;k<=vetor;k++){
		if(vetor%k==0) contador++;
	}
	if(contador>2) achei=0;
	else achei=1;
 }
return achei;
}


struct retorno{
  unsigned long int vetor_int[100];
  int tamanho;
};

struct retorno entrada_inteiros(char *input){ //estrutura que retorna um vetor de inteiros e seu tamanho
  struct retorno output;
  char buffer[40];
  int k=0,i=0,l=0,contador=0;
  output.tamanho=0;

  for(k=0; k<=strlen(input); k++){
    contador++;
	if(input[k]==32 || input[k]=='\0'){
	  for(i=0; i<contador-1; i++){
	    buffer[i] = input[k-(contador-1)+i];
	  }
	  output.vetor_int[l]=atoi(buffer);
	  l++;
	  output.tamanho++;
	  contador=0;

	  for(i=0;i<40;i++){   //zera o buffer
	  buffer[i]='a';	
	  }
	}
  }
 return output;
}



int main() {
  pid_t filho[N_PROCESSOS];
  int i,k,retorno=0,limite=0,resto=0,div=0,max=0;
  char vetor[100];
  struct retorno saida; 

  /* Definir flags de protecao e visibilidade de memoria */
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;

  /* Criar area de memoria compartilhada */
  int *quant_primo;
  quant_primo = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);

  scanf("%[^\t\n]s", &vetor);
  saida = entrada_inteiros(vetor);

  div=saida.tamanho/4;
  resto=saida.tamanho%4;
 
  if(resto!=0) limite=div+1;
  else limite=div;
 

for(k=0; k<limite; k++){  //processos sao executados de 4 em 4 (No caso de 7, o programa executa 4 e 3) 

  if(div!=0) max=4;
  else max=resto;

 for (int i=4*k; i<4*k+max; i++) {
    filho[i-4*k] = fork();
    if (filho[i-4*k] == 0) {
      /* Esta parte do codigo executa no processo filho */
      retorno = primo(saida.vetor_int[i]);

      if(retorno==1){
      (*quant_primo)++;
     }
      exit(0);
    }
  } 
  for (int i=0; i<max; i++) {
    waitpid(filho[i], NULL, 0);
  }
  div--;
}
  

  printf("%d\n", *quant_primo);
  return 0;

}
