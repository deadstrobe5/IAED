/* Afonso Ribeiro, 89400
 * Projeto 1 de IAED
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXMATRIX 10000
#define MAXNOME 80 					
#define max(x,y) ((x > y) ? x:y)
#define min(x,y) ((x < y) ? x:y)

/* Estrutura usada para representar a matriz esparsa */
typedef struct elemento{
	unsigned long linha;
	unsigned long coluna;
	double valor;
}elem;

/* Estrutura que guarda a linha e o numero de elementos dessa linha: 
 * vai ser usado no comando 's' para organizar as linhas por densidade */
typedef struct elemmento_aux{
	unsigned long linha;
	int n_elem;
}elem_aux;

/* Funcoes de comparacao do qsort() */
int cmplinhas(const void *x,const void *y);

int cmpcolunas(const void *x,const void *y);

int cmpdense(const void *x,const void *y);


/* Funcao para remover zeros da Matriz */
void remZero(elem M[],int *C,double z);


int main(int arc, char *argv[]){
	elem Matrix[MAXMATRIX];
	double zero=0;
	int Counter=0;		/* Numero de elementos representados da Matriz */
	unsigned long maxi=0,mini=0,maxj=0,minj=0;
	char com, filename[MAXNOME];
	FILE *myfile;



	/* Codigo de 'load' da matriz atraves de um ficheiro: 
	 *
	 *	- Le o valor do Contador, ou seja, o numero de elementos;
	 *
	 *	- Le as linhas do ficheiro 1 a 1 consoante a representacao escolhida
	 *	   pelo comando 'w' (Cada linha do ficheiro corresponde a 1 elemento) 
	 *	   e coloca os valores no sitio certo;
	 *	
	 *  - Apaga os zeros da Matriz
	 */
	 
	if (arc == 2){
		int i;
		strcpy(filename, argv[1]);
		myfile=fopen(filename, "r");
		if (myfile != NULL){				/* Testa se o ficheiro existe */
			fscanf(myfile,"%d",&Counter);	
			for (i=0; i<Counter; i++)
				fscanf(myfile,"%lu%lu%lf",&Matrix[i].linha,&Matrix[i].coluna,&Matrix[i].valor);
			fclose(myfile);

			remZero(Matrix,&Counter,zero);  
		}
	}



	while((com=getchar()) != 'q'){			/* 'q' termina a execucao do programa */


		/* Calculo dos limites da funcao, que sao usados pelos comandos i,l,c,s */
		if ((com == 'i' || com == 'l' || com == 'c' || com == 's') && Counter != 0 ){
			int i=0;
			maxi=mini=Matrix[0].linha;		/* Inicializam-se o max/min com a linha/coluna do primeiro elemento */
			maxj=minj=Matrix[0].coluna;

			while (++i < Counter){
				maxi=max(Matrix[i].linha,maxi);
				maxj=max(Matrix[i].coluna,maxj);
				mini=min(Matrix[i].linha,mini);
				minj=min(Matrix[i].coluna,minj);
			}
		}

		switch(com){

			case 'a':{						
				unsigned long linha, coluna;	/* As colunas/linhas so' podem ser representadas com unsigned long */
				double valor;
				int i=-1;								
				scanf("%lu%lu%lf",&linha,&coluna,&valor);

				/* Respetivamente ao comando 'a', existem 4 casos possiveis:
				 *
				 *	O elemento nao e' zero e a posicao indicada nao esta' ocupada
				 *		- Adiciona-se o elemento 'a Matriz e aumenta-se o Contador;
				 *
				 *	O elemento nao e' zero e a posicao indicada esta ocupada
				 *		- Substitui-se o valor antigo pelo novo;
				 *
				 *	O elemento e' zero e a posicao indicada esta ocupada
				 *		- Elimina-se o elemento, i.e. substitui-se por zero e diminui-se o Contador;
				 *
				 *	O elemento e' zero e a posicao indicada nao esta ocupada
				 *		- Nao se faz nada.
				 */


				/* i vai ficar ou com a posicao final, ou com uma posicao ocupada */
				while (++i < Counter)
					if (Matrix[i].linha == linha && Matrix[i].coluna == coluna)
						break;	

				/* Substitui ou adiciona um valor novo a matriz */
				if (valor != zero){
					Matrix[i].linha = linha;
					Matrix[i].coluna = coluna;
					Matrix[i].valor = valor;
					if (i==Counter) Counter++; /* Se o valor for adicionado numa posicao vazia, aumenta o contador */
				}


				/* Se a posicao estiver ocupada e o valor for 0, elimina-se esse elemento da matriz */
				else if (valor == zero && i != Counter){
					elem aux;

					/* Arrasta o elemento a remover para o final da lista, atraves de trocas sucessivas */
					for (i=i+1;i<=Counter;i++){	
						aux = Matrix[i-1];
						Matrix[i-1]=Matrix[i];
						Matrix[i]=aux;
					}
					Counter--; /* Diminui o contador de elementos, i.e. elimina o ultimo elemento */
				}
				break;
			}


			
			case 'p':{
				int i;

				/* Imprime todos os elementos do vetor de estruturas, na forma indicada */

				if (Counter==0)
					printf("empty matrix\n");
				else
					for(i=0;i<Counter;i++)
						printf("[%lu;%lu]=%.3lf\n", Matrix[i].linha, Matrix[i].coluna, Matrix[i].valor);

				break;
			}



			case 'i':{

				/* Calcula e imprime o tamanho, a densidade e os limites da funcao, na forma indicada */

				if (Counter==0)
					printf("empty matrix\n");
				else{
					int size = (maxi-mini+1)*(maxj-minj+1);
					float dens = (Counter/(float) size)*100;
					printf("[%lu %lu] [%lu %lu] %d / %d = %.3f%%\n",mini,minj,maxi,maxj,Counter,size,dens);
				}
				break;
			}



			case 'l':{
				unsigned long l,i;
				int e=-1;   									
				scanf("%lu",&l);

				/* O ciclo while percorre o vetor de estruturas e para se:
				 * 		- Encontrar um elemento na linha indicada
				 *				ou
				 *		- Chegar ao final do vetor
				 *
				 * 		Se chegar ao final do vetor: e == Counter.
				 */

				while (++e < Counter && Matrix[e].linha != l);

				if (Counter != 0 && e != Counter){
					for (i=minj; i<=maxj; i++){		
						for (e=0; e<Counter; e++)
							if (Matrix[e].coluna == i && Matrix[e].linha == l){
								printf(" %.3lf",Matrix[e].valor);
								break;
							}
						if (e==Counter)				/* Se percorrer o vetor todo sem encontrar um valor */
							printf(" %.3lf",zero);  /*	para a coluna correspondente, imprime zero */
					}
				}
				else
					printf("empty line");
				
				printf("\n");
				break;
			}




			case 'c':{
				unsigned long c,i;
				int e=-1;   								
				scanf("%lu",&c);

				/* A logica do comando 'c' e' igual 'a do comando 'l', mas trabalha
				 * com as colunas e imprime os valores de forma diferente. 
				 */

				while (++e < Counter && Matrix[e].coluna != c);

				if (Counter != 0 && e != Counter){
					for (i=mini; i<=maxi; i++){		
						for (e=0; e<Counter; e++)
							if (Matrix[e].linha == i && Matrix[e].coluna == c){
								printf("[%lu;%lu]=%.3lf\n",Matrix[e].linha,c,Matrix[e].valor);				
								break;
							}
						if (e==Counter)								
							printf("[%lu;%lu]=%.3lf\n",i,c,zero);	
				

					}
				}
				else
					printf("empty column\n");
			
				break;
			}




			case 'o':{
				char p[10],p_limpo[10];
				fgets(p,10,stdin);

				/* No comando 'o' usa-se o fgets() para identificar o que e'
				 * introduzido a seguir ao 'o', ou se nao e introduzido nada.
				 */

				if (p[0]=='\n') 		

					qsort(Matrix,Counter,sizeof(elem),cmplinhas); /* Organiza a Matriz por linhas */

				else {

					/* O sscanf() vai eliminar espacos e mudancas de linha do vetor */
					sscanf(p,"%s",p_limpo);	

					if (strcmp(p_limpo,"column")==0)					
						qsort(Matrix,Counter,sizeof(elem),cmpcolunas); /* Organiza a Matriz por colunas */
				}
				break;				
			}



			case 'z':{
				scanf("%lf",&zero);
				remZero(Matrix,&Counter,zero);
				break;
			}



			case 'w':{
				int i;
				char k[MAXNOME +3];		
				fgets(k,MAXNOME + 3, stdin);

				/* Os 3 espacos extra para o 'k' sao para o whitespace antes e depois (Automatico do Mooshak) 
				 * do nome do ficheiro e a mudanca de linha '\n' 
				 *	
				 * Para guardar a Matriz num ficheiro, guarda-se os varios elementos na forma:
				 *		
				 *		Contador
				 * 		linha1 coluna1 valor1
				 * 		linha2 coluna2 valor2
				 *		(...)
				 *		linhan colunan valorn
				 *
				 *	Sendo que o 'Contador' corresponde ao numero de elementos representados 
				 */
				
				if (k[0]!='\n')
					sscanf(k,"%s",filename);

				myfile = fopen(filename, "w");

				fprintf(myfile,"%d\n",Counter);

				for (i=0; i<Counter; i++)
					fprintf(myfile,"%lu %lu %lf\n",Matrix[i].linha,Matrix[i].coluna,Matrix[i].valor);

				fclose(myfile);
				break;
			}



			case 's':{
				int size = (maxi-mini+1)*(maxj-minj+1);
				float dens = (Counter/(float) size);

				/* 	No comando 's' comeca-se por calcular a densidade e, caso esta seja menor ou igual a 0.5:
				 *
				 *	- Iniciam-se os vetores todos a 0 (Ou Zero) para que sucessivas chamadas ao 's'
				 *	sejam possiveis;
				 *
				 *	- Preenche-se e ordena-se pela maior densidade a Mat_Aux (Estrutura que guarda as linhas e o
				 *	seu numero de elementos) consoante os elementos da Matriz representados.
				 *
				 *
				 *	Após ter as linhas ordenadas por densidade, vai-se executar o mesmo codigo para cada linha:
				 *
				 *		- Cria-se um vetor temporario que guarda os elementos da linha, nos indices
				 *		de coluna correspondentes;
 				 *
				 *		- Calcula-se o offset da linha, atraves de sucessivas comparacoes do vetor temporario
				 *		com o vetor final, ate' ser encontrado o offset certo;
				 *
				 *		- Preenchem-se os vetores index, value e offset consoante o offset calculado e 
				 *		calcula-se o offset maximo ate ao momento.
				 *
				 *	No final, imprime-se os vetores value, index e offset tal como pedido no enunciado.
				 *
				 *	(O calculo do offset maximo e a iniciacao dos vetores index a 0 e value a Zero  
				 *	permite que o padding de zeros seja feito automaticamente)
				 */ 

				if (dens > 0.5)
					printf("dense matrix\n");

				else{

					unsigned long index[2*Counter],p, e, j, k, Counter_aux=0;
					double value[2*Counter], vetor_linha[MAXMATRIX];
					int i, max_offset=0, offset, offset_vet[MAXMATRIX];
					elem_aux Mat_Aux[MAXMATRIX];


					
					for (e=0;e<2*Counter;e++){
						value[e]=zero;
						index[e]=0;
					}
					for (e=0; e<MAXMATRIX;e++){
						vetor_linha[e]=zero;
						offset_vet[e]=0;
						Mat_Aux[e].linha=0;
						Mat_Aux[e].n_elem=0;
					}
					
				


					/* Preenche o vetor de estruturas elem_aux com as linhas */
					for (p=mini;p<=maxi;p++){
						for (e=0;e<Counter;e++)
							if (Matrix[e].linha == p){
								if (Mat_Aux[Counter_aux].linha != p)
									Mat_Aux[Counter_aux].linha = p;
								Mat_Aux[Counter_aux].n_elem ++;
							}
						if (Mat_Aux[Counter_aux].n_elem != 0)
							Counter_aux++;
					}

		

					/* Ordena as linhas por densidade, da mais densa para a menos densa */
					qsort(Mat_Aux,Counter_aux,sizeof(elem_aux),cmpdense);


					/* Para cada linha... */
					for (i=0;i<Counter_aux;i++){

						/* Reinicia o vetor temporario a zero */
						for(e=0;e<maxj-minj+1;e++)
							vetor_linha[e]=zero;

						/* Preenche o vetor temporario com os valores da linha atual */
						for(e=0; e<Counter; e++)
							if (Mat_Aux[i].linha==Matrix[e].linha)
								vetor_linha[Matrix[e].coluna-minj] = Matrix[e].valor;


						/* Calculo do Offset da linha atual */
						offset=-1;
						k=0;
						while(k != maxj-minj+1){ 
							offset++;
							for(k=0; k < maxj-minj+1; k++){
								if (vetor_linha[k] != zero && value[k+offset] != zero)
									break;
							}
						}

						/* Preenche cada vetor com os valores certos na posicao certa */
						for (j=0; j < maxj-minj+1; j++)
							if (vetor_linha[j] != zero){
								value[j+offset] = vetor_linha[j];
								index[j+offset] = Mat_Aux[i].linha;
							}

						offset_vet[Mat_Aux[i].linha - mini] = offset;
						max_offset = max(max_offset,offset);			/* Calcula o offset maximo ate ao momento */

					}
					
			
					
					printf("value =");
					for (e=0; e < maxj-minj+1+max_offset; e++)
						printf(" %.3lf",value[e]);
					

					printf("\nindex =");
					for (e=0; e < maxj-minj+1+max_offset; e++)
						printf(" %lu",index[e]);

					printf("\noffset =");
					for (e=0; e < maxi-mini+1; e++)
						printf(" %d",offset_vet[e]);

					printf("\n");

				}
			break;		
			}				
		}
	}

return 0;
} 


/*  As funcoes cmplinhas, cmpcolunas e cmpdense sao funcoes de comparacao usadas pelo qsort(),
 *
 *	- const void *x e const void *y sao ponteiros genericos que guardam um espaco de memoria 
 *	sem definir o que la' esta' guardado;
 *
 *	- (elem *)x atribui um tipo especifico ao ponteiro generico x, neste caso, o tipo elem;
 *
 *	- Os ponteiros 'a' e 'b' ficam com o valor do ponteiro x e y, ou seja,
 *	o local em memoria das estruturas a avaliar.
 */


/*  A cmplinhas vai organizar elementos do tipo elem por linhas e, dentro de linhas iguais, por colunas.
 *  Organiza da Menor para a Maior, tanto linhas como colunas 
 */
int cmplinhas(const void *x,const void *y){	
	elem *a = (elem*) x;				  
	elem *b = (elem*) y; 
	if (a->linha == b->linha)
		return a->coluna - b->coluna;
	return a->linha - b->linha;
}

/*  A cmpcolunas vai organizar elementos do tipo elem por colunas e, dentro de colunas iguais, por linhas.
 *  Organiza da Menor para a Maior, tanto linhas como colunas 
 */
int cmpcolunas(const void *x,const void *y){
	elem *a = (elem*) x;
	elem *b = (elem*) y; 
	if (a->coluna == b->coluna)
		return a->linha - b->linha;
	return a->coluna - b->coluna;
}

/*  A cmpdense vai organizar elementos do tipo elem_aux por numero de elementos (i.e. densidade) e dentro de 
 * densidades iguais, por linhas.
 *  Organiza as densidades da Maior para a Menor
 *  Organiza as linhas da Menor para a Maior */
int cmpdense(const void *x,const void *y){
	elem_aux *a = (elem_aux*) x;
	elem_aux *b = (elem_aux*) y;
	if (b->n_elem == a->n_elem)
		return a->linha - b->linha;
	return b->n_elem - a->n_elem;
}

/* A funcao remZero vai remover os elementos com valor Zero dentro da Matriz,
 *
 * Recebe o vetor de estruturas (M), o numero de elementos (*C) e o Zero a remover (z).
 */ 


void remZero(elem M[],int *C,double z){
	int i,e,Counter = *C;
	for (e=0; e<Counter;e++){
		if (M[e].valor == z){
			elem aux;

			/* Arrasta o elemento a remover para o final da lista, atraves de trocas sucessivas */
			for (i=e+1;i<=Counter;i++){
				aux = M[i-1];
				M[i-1]=M[i];	
				M[i]=aux;
			}
			Counter--;
			e=-1;	/* Volta-se a percorrer o vetor desde o inicio se este for alterado */
		}
	}
	*C = Counter; /* Atualiza o Contador fora da funcao */
}
