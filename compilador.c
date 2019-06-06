/*
Henrique da Silva Alves
Diego Barros Aguirre
Gabriel Rodrigues de Souza

###################################################################################################

Etapa 4:

Funções de manipulação da tabela de simbolos:

	buscarVariavel
	buscarTipoVariavel
	definir_tipo
	removerSimboloEscopo
	inserirSimbolo
	imprimirTabela
	

Onde são utilizadas:

	A função inserirSimbolo() é utilizada na função declaracao_procedimento() e na função lista_identificadores()
ao encontrar um IDENTIFICADOR

	A função removerSimboloEscopo() é utilizado na função declaracao_procedimento() ao sair do bloco

	A função imprimirTabela() é utilizada quando for reconhecido a palava WRITE

	A função buscarTipoVariavel() é utilizada na função atribuicao() para buscar o tipo da variavel que será atribuido um valor
e na função variavel() para validar se a variável que está sendo atribuída em outra tem o tipo correspondente.


Mudanças da ETAPA 3:

	As funções expressao(), expressao_simples(), termo(), fator() e variavel() receberam uma variável tipo_v a mais em 
seus parâmetros. Se essa variável tiver o valor 0 significa que não é pra fazer validação de tipo, caso contrário é 
necessário fazer a validação do tipo.



###################################################################################################*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define COMENTARIO 100
#define IDENTIFICADOR 101
#define INT 102
#define BOOL 103
#define PROGRAM 106
#define IF 107
#define DO 108
#define VAR 109
#define FALSE 110
#define BEGIN 111
#define THEN 112
#define AND 113
#define WRITE 114
#define END 115
#define ELSE 116
#define OR 117
#define PROCEDURE 118
#define WHILE 119
#define NOT 120
#define TRUE 121
#define NUM 122
#define MAIS 123
#define MENOS 124
#define DIVISAO 125
#define MULTIPLICACAO 126
#define MENOR 127
#define MAIOR 128
#define DIFERENTE 129
#define MENOR_IGUAL 130
#define MAIOR_IGUAL 131
#define ATRIBUICAO 132
#define ABRE_PARENTESES 133
#define FECHA_PARENTESES 134
#define VIRGULA 135
#define DOIS_PONTOS 136
#define PONTO 137
#define PONTO_VIRGULA 138
#define IGUAL 139
#define ERRO 0


int programa(char palavra[], int *inicio);
int bloco(char palavra[], int *inicio);
int parte_declaracao_variaveis(char palavra[], int *inicio);
int declaracao_variaveis(char palavra[], int *inicio);
int declaracao_variaveis_linha(char palavra[], int *inicio);
int lista_identificadores(char palavra[], int *inicio);
int parte_declaracao_sub_rotinas(char palavra[], int *inicio);
int declaracao_procedimento(char palavra[], int *inicio);
int parametros_formais(char palavra[], int *inicio);
int parametro_formal(char palavra[], int *inicio);
int parametro_formal_linha(char palavra[], int *inicio);
int tipo(char palavra[], int *inicio);
int comando_composto(char palavra[], int *inicio);
int comando(char palavra[], int *inicio);
int atribuicao(char palavra[], int *inicio);
int chamada_procedimento(char palavra[], int *inicio);
int lista_parametros(char palavra[], int *inicio);
int lista_parametros_linha(char palavra[], int *inicio);
int comando_condicional(char palavra[], int *inicio);
int comando_repetitivo(char palavra[], int *inicio);
int expressao(char palavra[], int *inicio, int tipo_v);
int relacao(char palavra[], int *inicio);
int expressao_simples(char palavra[], int *inicio, int tipo_v);
int termo(char palavra[], int *inicio, int tipo_v);
int fator(char palavra[], int *inicio, int tipo_v);
int variavel(char palavra[], int *inicio, int tipo_v);
int bool(char palavra[], int *inicio);
void imprimirTabela();
void imprime_palavra(char palavra[], int inicio, int fim);

//Itens da tabela de somvolos
typedef struct simbolo{
	int palavra_inicio;
  	int palavra_fim;
	int categoria;
	int tipo;
  	int escopo;
	struct simbolo *prox;
}Simbolo;

/*
global
categoria_simbolo = Se o simbolo da tabela é um VAR ou PROCEDURE
tipo_simbolo = Se o simbolo for VAR então pode ser BOOL ou INT
escopo = Em que bloco o simbolo foi instanciado
erro_sematico = 0 quando não for semântico e 1 quando for
S = tabela de simbolos
*/
int lookahead, categoria_simbolo, tipo_simbolo, escopo, erro_semantico;
Simbolo *S;


//Busca a variável na tabela de simbolos e retorna a sua posição
int buscarVariavel(char palavra[], int inicio, int fim, int escopo_aux){

  	int escopo_atual, cont, variavel, i, achou;
  	Simbolo *aux;

  	escopo_atual = escopo;

	//Primeiro percorre o escopo do bloco atual, se não achar a variável será feito a busca
	//nos escopos anteriores
  	while(escopo_atual >= 1){
    	aux = S;
    	variavel = 0;
    	while (aux != NULL){
    		
      		if(aux->escopo == escopo_atual){
        	
				achou = 1;
        		cont = aux->palavra_inicio; 
        		
        		for(i = inicio; i < fim; i++){
          			if(palavra[cont] != palavra[i]){
            			achou = 0;
          			}
          			cont++;
        		}
        		
        		if(achou == 1 && (escopo_atual == escopo_aux || escopo_aux == 0) && cont == aux->palavra_fim){return variavel;}
      		}
      		
      	aux = aux->prox;
      	variavel ++;
    	}
    	
    	escopo_atual --;
  	}
  	return -1;
}

//Verifica se a variável já foi instanciada no escopo
int existeVariavel(char palavra[], int  pos, int flag){
	
	//flag 0 = sem escopo
	//flag 1 = escopo atual
	
	int inicio, fim, variavel;
	
	//é definido o intervalo do identificador da variavel
	fim = pos;
	while(palavra[fim] == ' '){
	    fim = fim - 1;
	}
		
	inicio = fim;
	while(palavra[inicio] != ' '){
		inicio = inicio - 1;
	}
	
	inicio = inicio + 1;
	fim = fim + 1;
	
	if (flag == 0) variavel = buscarVariavel(palavra, inicio, fim, 0);
	else variavel = buscarVariavel(palavra, inicio, fim, escopo);
	
	if(variavel == -1){
		return 0;
	} else{
		return 1;
	}
	
}

//Busca o tipo da variável atual
int buscarTipoVariavel(char palavra[], int pos){
	
	int inicio, fim, variavel, i;
	Simbolo *aux;
	
	aux = S;
	
	//é definido o intervalo do identificador da variavel
	fim = pos;
	while(palavra[fim] == ' '){
	    fim = fim - 1;
	}
		
	inicio = fim;
	while(palavra[inicio] != ' '){
		inicio = inicio - 1;
	}
	
	inicio = inicio + 1;
	fim = fim + 1;
	
	//busca a variavel correspondente na tabela de simbolos
	variavel = buscarVariavel(palavra, inicio, fim, 0);
	
	//se a variável já foi declarada retorna o tipo
	if(variavel != -1){
		for(i = 0; i <= variavel; i++){
	    	if(i == variavel){
	        	return aux->tipo;
	    	}
	      	aux = aux->prox;
	    }
	}

  	return 0;
}

//define o tipo das variávels
void definir_tipo(){
  
  	Simbolo *aux;
  	aux = S;

	//adiciona o tipo para todas as variaveis que estão sem tipo definido
  	while(aux != NULL){

    	if(aux->tipo == 0 && aux->categoria == VAR){
      		aux->tipo = tipo_simbolo;
    	}

    	aux = aux->prox;
  	}

}

//Remove os simbolos de um escopo ao sair dele
void removerSimboloEscopo(){

  	Simbolo *aux;
  	Simbolo *aux_anterior;
  	aux = S;
    
  	while(aux != NULL){

    	if(aux->escopo == escopo){
      		aux_anterior->prox = aux->prox->prox;
      		aux = aux_anterior;
    	}

    	aux_anterior = aux;
    	aux = aux->prox;
  	}

	//escopo atual  é atualizado
  	escopo --;

}

//Insere um novo simbolo ao declarar uma variavel ou uma procedure
void inserirSimbolo(char palavra[], int pos){

  	int fim_palavra, inicio_palavra, i, cont;
	Simbolo *S1 = (Simbolo *) malloc(sizeof(Simbolo));
	Simbolo *aux;
	
	//define a cadeia de caracteres pertecentes ao identificador da variavel
  	fim_palavra = pos;
  	while(palavra[fim_palavra] == ' '){
    	fim_palavra = fim_palavra - 1;
  	}

  	inicio_palavra = fim_palavra;
  	while(palavra[inicio_palavra] != ' '){
    	inicio_palavra = inicio_palavra - 1;
  	}

  	inicio_palavra = inicio_palavra + 1;
  	fim_palavra = fim_palavra + 1;

	//Cria um simbolo
  	S1->palavra_inicio = inicio_palavra;
  	S1->palavra_fim = fim_palavra;
	S1->categoria = categoria_simbolo;
  	S1->tipo = 0;
  	S1->prox = NULL;
  	S1->escopo = escopo;
	
	//Se a tabela não estiver vazia adiionar o novo simbolo no final
  	if(S != NULL){

    	aux = S;
    	while(aux->prox != NULL){
      		aux = aux->prox;
    	}
    	aux->prox = S1;
  
  	} else{
  	//Se estiver vazia adicionar o simbolo no inicio

    	S = S1;

  	}
}

//Tokens usados na tabela de simbolo
void imprimirToken(int codigo){
  if(codigo == VAR){
    printf("VAR");
  } else if(codigo == INT){
    printf("INT");
  } else if(codigo == BOOL){
    printf("BOOL");
  } else if(codigo == 0){
    printf("-");
  } else if(codigo == PROCEDURE){
    printf("PROCEDURE");
  }	else if(codigo == PROGRAM){
    printf("PROGRAM");
  }
}

//Imprime a tabela de simbolos
void imprimirTabela(char palavra[]){

  printf("\n\nTabela de simbolos:");
  printf("\nEscopo: %d\n", escopo);

  Simbolo *aux = S;

  while(aux != NULL){

    printf("\nsimbolo: ");
    int i;
    for(i = aux->palavra_inicio; i < aux->palavra_fim; i++){
      printf("%c", palavra[i]);
    }
    printf("; categoria: ");
    imprimirToken(aux->categoria);
    printf("; tipo: ");
    imprimirToken(aux->tipo);
    printf("; escopo: %d", aux->escopo);

    aux = aux->prox;

  }

}

//Imprime a cadeia de caracteres da variavel
void imprime_palavra(char palavra[], int inicio, int fim){
	while (inicio <= fim) {
		
		printf("%c", palavra[inicio]);
		inicio = inicio + 1;

	}
}

void item_lexico(int codigo){
	
	if (codigo == COMENTARIO) printf("<COMENTARIO>\n");
	else if (codigo == IDENTIFICADOR)printf("<IDENTIFICADOR>");
	else if (codigo == INT) printf("<INT>\n");
	else if (codigo == BOOL) printf("<BOOL>\n");
	else if (codigo == PROGRAM) printf("<PROGRAM>\n");
	else if (codigo == IF) printf("<IF>\n");
	else if (codigo == DO) printf("<DO>\n");
	else if (codigo == VAR) printf("<VAR>\n");
	else if (codigo == FALSE) printf("<FALSE>\n");
	else if (codigo == BEGIN) printf("<BEGIN>\n");
	else if (codigo == THEN) printf("<THEN>\n");
	else if (codigo == AND) printf("<AND>\n");
	else if (codigo == WRITE) printf("<WRITE>\n");
	else if (codigo == END) printf("<END>\n");
	else if (codigo == ELSE) printf("<ELSE>\n");
	else if (codigo == OR) printf("<OR>\n");
	else if (codigo == PROCEDURE) printf("<PROCEDURE>\n");
	else if (codigo == WHILE) printf("<WHILE>\n");
	else if (codigo == NOT) printf("<NOT>\n");
	else if (codigo == TRUE) printf("<TRUE>\n");
	else if (codigo == NUM) printf("<NUM>");
	else if (codigo == ERRO) printf("<ERRO LEXICO>\n");
	else if (codigo == MAIS) printf("<MAIS>\n");
	else if (codigo == MENOS) printf("<MENOS>\n");
	else if (codigo == DIVISAO) printf("<DIVISAO>\n");
	else if (codigo == MULTIPLICACAO) printf("<MULTIPLICACAO>\n");
	else if (codigo == MENOR ) printf("<MENOR>\n");
	else if (codigo == MAIOR ) printf("<MAIOR>\n");
	else if (codigo == DIFERENTE ) printf("<DIFERENTE>\n");
	else if (codigo == MENOR_IGUAL) printf("<MENOR_IGUAL>\n");
	else if (codigo == MAIOR_IGUAL) printf("<MAIOR_IGUAL>\n");
	else if (codigo == ATRIBUICAO) printf("<ATRIBUICAO>\n");
	else if (codigo == ABRE_PARENTESES) printf("<ABRE_PARENTESES>\n");
	else if (codigo == FECHA_PARENTESES) printf("<FECHA_PARENTESES>\n");
	else if (codigo == VIRGULA) printf("<VIRGULA>\n");
	else if (codigo == DOIS_PONTOS) printf("<DOIS_PONTOS>\n");
	else if (codigo == PONTO) printf("<PONTO>\n");
	else if (codigo == PONTO_VIRGULA) printf("<PONTO_VIRGULA>\n");
	else if (codigo == IGUAL) printf("<IGUAL>\n");
}

int scanner(char palavra[], int *inicio){
	
	char c;
	
	q0: c = palavra[++(*inicio)];
	if (c == ',') goto q86;
	else if (c == ' ') goto q0;
	else if (c == '.') goto q87;
	else if (c == ';') goto q88;
	else if (c == '(') goto q89;
	else if (c == ')') goto q90;
	else if (c == '+') goto q71;
	else if (c == '*') goto q73;
	else if (c == '<') goto q75;
	else if (c == '>') goto q79;
	else if (c == ':') goto q82;
	else if (c == '=') goto q84;
	else if (c == 'p') goto q1;
	else if (c == 'i') goto q14;
	else if (c == 'd') goto q18;
	else if (c == 'v') goto q20;
	else if (c == 'f') goto q23;
	else if (c == 'b') goto q28;
	else if (c == 't') goto q36;
	else if (c == 'a') goto q43;
	else if (c == 'w') goto q46;
	else if (c == 'e') goto q55;
	else if (c == 'n') goto q63;
	else if (c == 'o') goto q61;
	else if (isdigit(c)) goto q67;
	else if (c == '-') goto q72;
	else if (c == '/') goto q66;
	else if (isalpha(c)) goto q101;
	else if (c == '_') goto q119;
	else return 0;
	
	/*VIRGULA*/
	q86: c = palavra[++(*inicio)];	
	if (c == ' ') goto q91;
	else return 0;
		
	q91: return VIRGULA;
		
	/*PONTO FINAL*/
	q87: c = palavra[++(*inicio)];
	if (c == ' ') goto q92;
	else return 0;
	
	q92: return PONTO;
	
	/*PONTO E VIRGULA*/
	q88: c = palavra[++(*inicio)];		
	if (c == ' ') goto q93;
	else return 0;
		
	q93: return PONTO_VIRGULA;
	
	/*ABRE PARENTESES*/
	q89: c = palavra[++(*inicio)];
	if (c == ' ') goto q94;
	else return 0;
		
	
	q94: return ABRE_PARENTESES;
	
	/*FECHA PARENTESES*/
	q90: c = palavra[++(*inicio)];
	if (c == ' ') goto q95;
	else return 0;
	
	q95: return FECHA_PARENTESES;
	
	/*SINAL DE ADIÇÃO*/
	q71: c = palavra[++(*inicio)];
	if (c == ' ') goto q96;
	else return 0;
	
	q96: return MAIS;
	
	/*SINAL DE MULTIPLICAÇÃO*/
	q73: c = palavra[++(*inicio)];
	if (c == ' ') goto q127;
	else return 0;
	
	q127: return MULTIPLICACAO;

	
	/*SINAL DE MENOR*/
	q75: c = palavra[++(*inicio)];
	if (c == ' ') goto q76;
	else if (c == '>') goto q77;
	else if (c == '=') goto q78;
	else return 0;
	
	
	q76: return MENOR;
	
	/*SINAL DE MENOR E MAIOR*/
	q77: c = palavra[++(*inicio)];
	if (c == ' ') goto q99;
	else return 0;
	
	q99:return DIFERENTE;
	
	/*SINAL DE MENOR E IGUAL*/
	q78: c = palavra[++(*inicio)];
	if (c == ' ') goto q100;
	else return 0;
	
	q100: return MENOR_IGUAL;
	
	/*SINAL DE MAIOR*/
	q79: c = palavra[++(*inicio)];
	if (c == ' ') goto q80;
	else if (c == '=') goto q81;
	else return 0;
	
	q80: return MAIOR;
	
	/*SINAL DE MAIOR E IGUAL*/
	q81: c = palavra[++(*inicio)];
	if (c == ' ') goto q102;
	else return 0;
	
	q102: return MAIOR_IGUAL;

	/*DOIS PONTOS*/
	q82: c = palavra[++(*inicio)];
	if (c == ' ') goto q85;
	else if (c == '=') goto q83;
	else return 0;
	
	q85: return DOIS_PONTOS;
	
	/*DOIS PONTOS* E IGUAL*/
	q83: c = palavra[++(*inicio)];
	if (c == ' ') goto q103;
	else return 0;
		
	q103: return ATRIBUICAO;
	
	/*SINAL DE IGUAL*/
	q84: c = palavra[++(*inicio)];
	if (c == ' ') goto q104;
	else return 0;
		
	q104: return IGUAL;
	
	/*PROGRAM E PROCEDURE*/
	q1: c = palavra[++(*inicio)];
	if (c == 'r') goto q2;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q2: c = palavra[++(*inicio)];
	if (c == 'o') goto q3;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q3: c = palavra[++(*inicio)];
	if (c == 'g') goto q4;
	else if (c == 'c') goto q8;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q4: c = palavra[++(*inicio)];
	if (c == 'r') goto q5;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q5: c = palavra[++(*inicio)];
	if (c == 'a') goto q6;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q6: c = palavra[++(*inicio)];
	if (c == 'm') goto q7;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
		
	q7: c = palavra[++(*inicio)];
	if (c == ' ') goto q106;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q8: c = palavra[++(*inicio)];
	if (c == 'e') goto q9;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q9: c = palavra[++(*inicio)];
	if (c == 'd') goto q10;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q10: c = palavra[++(*inicio)];
	if (c == 'u') goto q11;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q11: c = palavra[++(*inicio)];
	if (c == 'r') goto q12;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q12: c = palavra[++(*inicio)];
	if (c == 'e') goto q13;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q13: c = palavra[++(*inicio)];
	if (c == ' ') goto q107;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
		
	q106: return PROGRAM ;
	
	q107: return PROCEDURE ;
	
	/*IF e INT*/
	q14: c = palavra[++(*inicio)];
	if (c == 'f') goto q15;
	else if (c == 'n') goto q16;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;

	q15: c = palavra[++(*inicio)];
	if (c == ' ') goto q108;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q16: c = palavra[++(*inicio)];
	if (c == 't') goto q17;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q17: c = palavra[++(*inicio)];
	if (c == ' ') goto q109;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q108: return IF;
		
	q109: return INT ;
	
	/*DO*/
	q18: c = palavra[++(*inicio)];
	if (c == 'o') goto q19;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q19: c = palavra[++(*inicio)];
	if (c == ' ') goto q110;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
		
	q110: return DO ;
	
	/*VAR*/
	q20: c = palavra[++(*inicio)];
	if (c == 'a') goto q21;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;

	q21: c = palavra[++(*inicio)];
	if (c == 'r') goto q22;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q22: c = palavra[++(*inicio)];
	if (c == ' ') goto q111;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q111: return VAR ;
	
	/*FALSE*/
	q23: c = palavra[++(*inicio)];
	if (c == 'a') goto q24;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q24: c = palavra[++(*inicio)];
	if (c == 'l') goto q25;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q25: c = palavra[++(*inicio)];
	if (c == 's') goto q26;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q26: c = palavra[++(*inicio)];
	if (c == 'e') goto q27;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q27: c = palavra[++(*inicio)];
	if (c == ' ') goto q112;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q112: return FALSE;
	
	/*BEGIN e BOOL*/
	q28: c = palavra[++(*inicio)];
	if (c == 'e') goto q29;
	else if (c == 'o') goto q33;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q29: c = palavra[++(*inicio)];
	if (c == 'g') goto q30;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q30: c = palavra[++(*inicio)];
	if (c == 'i') goto q31;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q31: c = palavra[++(*inicio)];
	if (c == 'n') goto q32;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q32: c = palavra[++(*inicio)];
	if (c == ' ') goto q113;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q33: c = palavra[++(*inicio)];
	if (c == 'o') goto q34;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q34: c = palavra[++(*inicio)];
	if (c == 'l') goto q35;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q35: c = palavra[++(*inicio)];
	if (c == ' ') goto q114;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q113: return BEGIN;
	
	q114: return BOOL;
	
	/*THEN e TRUE*/
	q36: c = palavra[++(*inicio)];
	if (c == 'h') goto q37;
	else if (c == 'r') goto q40;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q37: c = palavra[++(*inicio)];		
	if (c == 'e') goto q38;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q38: c = palavra[++(*inicio)];
	if (c == 'n') goto q39;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
		
	q39: c = palavra[++(*inicio)];
	if (c == ' ') goto q115;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q40: c = palavra[++(*inicio)];
	if (c == 'u') goto q41;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
		
	q41: c = palavra[++(*inicio)];
	if (c == 'e') goto q42;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
		
	q42: c = palavra[++(*inicio)];
	if (c == ' ') goto q116;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q115: return THEN;
	
	q116: return TRUE;
	
	/*AND*/
	q43: c = palavra[++(*inicio)];
	if (c == 'n') goto q44;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q44: c = palavra[++(*inicio)];
	if (c == 'd') goto q45;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;

	q45: c = palavra[++(*inicio)];
	if (c == ' ') goto q117;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q117: return AND;
	
	/*WRITE e WHILE*/
	q46: c = palavra[++(*inicio)];
	if (c == 'r') goto q47;
	else if (c == 'h') goto q51;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q47: c = palavra[++(*inicio)];
	if (c == 'i') goto q48;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q48: c = palavra[++(*inicio)];
	if (c == 't') goto q49;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q49: c = palavra[++(*inicio)];
	if (c == 'e') goto q50;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q50: c = palavra[++(*inicio)];
	if (c == ' ') goto q118;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q51: c = palavra[++(*inicio)];
	if (c == 'i') goto q52;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q52: c = palavra[++(*inicio)];
	if (c == 'l') goto q53;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q53: c = palavra[++(*inicio)];
	if (c == 'e') goto q54;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q54: c = palavra[++(*inicio)];
	if (c == ' ') goto q120;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q118: return WRITE;
	
	q120: return WHILE;
	
	/*END e ELSE*/
	q55: c = palavra[++(*inicio)];
	if (c == 'n') goto q56;
	else if (c == 'l') goto q58;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q56: c = palavra[++(*inicio)];
	if (c == 'd') goto q57;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q57: c = palavra[++(*inicio)];
	if (c == ' ') goto q121;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q58: c = palavra[++(*inicio)];
	if (c == 's') goto q59;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q59: c = palavra[++(*inicio)];
	if (c == 'e') goto q60;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
		
	q60: c = palavra[++(*inicio)];
	if (c == ' ') goto q122;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q121: return END;
	
	q122: return ELSE;
	
	/*NOT*/
	q63: c = palavra[++(*inicio)];
	if (c == 'o') goto q64;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q64: c = palavra[++(*inicio)];
	if (c == 't') goto q65;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;

	q65: c = palavra[++(*inicio)];
	if (c == ' ') goto q123;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q123: return NOT;
	
	/*OR*/
	q61: c = palavra[++(*inicio)];
	if (c == 'r') goto q62;
	else if (c == '_' || isalnum(c)) goto q101;
	else if (c == ' ') goto q105;
	else return 0;
	
	q62: c = palavra[++(*inicio)];
	if (c == ' ') goto q124;
	else if (c == '_' || isalnum(c)) goto q101;
	else return 0;
	
	q124: return OR;
	
	/*NUMERO*/
	q67: c = palavra[++(*inicio)];
	if (isdigit(c)) goto q67;
	else if (c == ' ') goto q98;
	else return 0;
		
	q98: return NUM;
	
	q72: c = palavra[++(*inicio)];
	if (isdigit(c)) goto q67;
	else if (c == ' ') goto q97;
	else return 0;
	
	q97: return MENOS;
	
	/*/ e COMENTARIO*/
	q66: c = palavra[++(*inicio)];
	if (c == ' ') goto q74;
	else if (c == '*') goto q68;
	else return 0;
	
	q74: return DIVISAO;
	
	q68: c = palavra[++(*inicio)];
	if (c == '*') goto q69;
	else goto q68;
	
	q69: c = palavra[++(*inicio)];
	if (c == '/') goto q70;
	else if (c == '*') goto q69;
	else goto q68;
	
	q70: c = palavra[++(*inicio)];
	if (c == ' ') goto q125;
	else return 0;
	
	q125: return COMENTARIO;
	
	/*_ E IDENTIFICADOR*/
	q119: c = palavra[++(*inicio)];
	if (isalnum(c)) goto q101;
	else if (c == '_') goto q119;
	else return 0;
	
	/*IDENTIFICADOR*/
	q101: c = palavra[++(*inicio)];
	if (c == ' ') goto q105;
	else if (isalnum(c) || c == '_')goto q101;
	else return 0;
	
	q105: return IDENTIFICADOR;
}

int match(char palavra[], int *pos, int codigo){
	if (lookahead == codigo){
		lookahead = scanner(palavra, pos);
    	if(lookahead == VAR || lookahead == PROCEDURE){
      		categoria_simbolo = lookahead;
    	}
    	if(lookahead == INT || lookahead == BOOL ){
      		tipo_simbolo = lookahead;
    	}
    	return(1);
	}
	return(0);  
}

// programa ::= program identificador ; bloco .
int programa(char palavra[], int *inicio){
	if (match (palavra, inicio, PROGRAM)){
		if(lookahead == IDENTIFICADOR) inserirSimbolo(palavra, *inicio);
		if(match(palavra, inicio, IDENTIFICADOR) && match(palavra, inicio, PONTO_VIRGULA) && bloco(palavra, inicio) && match(palavra, inicio, PONTO)) return 1; 
		else return 0;
	}
	else return 0;
}

// bloco ::= [parte de declarações de variáveis] [parte de declarações de sub-rotinas] comando composto
int bloco(char palavra[], int *inicio){
  escopo ++;
	if(parte_declaracao_variaveis(palavra, inicio) && parte_declaracao_sub_rotinas(palavra, inicio) 
	&& comando_composto(palavra, inicio)){
		return 1;
	} else return 0;
}

// Livro Texto : parte de declarações de variáveis ::= var declaração de variáveis {; declaração de variáveis };
// Ajuste : parte de declarações de variáveis ::= var declaração de variáveis { DECLARACAO_DE_VARIAVEIS_LINHA };
int parte_declaracao_variaveis(char palavra[], int *inicio){
	if(match(palavra, inicio, VAR)){
		if(declaracao_variaveis(palavra, inicio) && match(palavra, inicio, PONTO_VIRGULA) && declaracao_variaveis_linha(palavra, inicio)){
			return 1;
		} else return 0;
	} else {
		return 1;
	}
}

//  declaração de variáveis ::= lista de identificadores : tipo
int declaracao_variaveis(char palavra[], int *inicio){
	if(lista_identificadores(palavra, inicio) && match(palavra, inicio, DOIS_PONTOS) 
	&& tipo(palavra, inicio)){
    definir_tipo();
		return 1;
	} else return 0;
}

// DECLARACAO_DE_VARIAVEIS_LINHA ::= parte declaracao variaveis
int declaracao_variaveis_linha(char palavra[], int *inicio){
	if(parte_declaracao_variaveis(palavra, inicio)){
		return 1;
	} else return 0;
}

// lista de identificadores ::= identificador {, identificador}
int lista_identificadores(char palavra[], int *inicio){
	if(lookahead == IDENTIFICADOR){
		if(! existeVariavel(palavra, *inicio, 1)) inserirSimbolo(palavra, *inicio);
		else{
			erro_semantico = 4;
			return 0;
		}
  	}
  	if( match(palavra, inicio, IDENTIFICADOR) ){
		if(match(palavra, inicio, VIRGULA) && lista_identificadores(palavra, inicio) ){
			return 1;
		}
		return 1;
	} else return 0;
}

// parte de declarações de subrotinas ::= { declaração de procedimento;}
int parte_declaracao_sub_rotinas(char palavra[], int *inicio){
	if(declaracao_procedimento(palavra, inicio)) return 1;
	else return 0;
}


// declaração de procedimento? ::= procedure identificador [parâmetros formais] ; bloco ;
int declaracao_procedimento(char palavra[], int *inicio){
	if(match(palavra, inicio, PROCEDURE)){

	    if(lookahead == IDENTIFICADOR) {
			if(! existeVariavel(palavra, *inicio, 0)) inserirSimbolo(palavra, *inicio);
			else{
				erro_semantico = 3;
				return 0;
			}
		}
		

		if (match(palavra, inicio, IDENTIFICADOR) && parametros_formais(palavra, inicio) 
		&& match(palavra, inicio, PONTO_VIRGULA) && bloco(palavra, inicio) 
		&& match(palavra, inicio, PONTO_VIRGULA)){
      		removerSimboloEscopo();
			return 1;
		} else return 0;
	}else return 1;
}

// Livro texto : parâmetros formais ::= ( parâmetro formal { ; parâmetro formal } )
// Ajuste : parâmetros formais ::= ( parâmetro formal { PARAMETRO FORMAL LINHA } )
int parametros_formais(char palavra[], int *inicio){
	if(match(palavra, inicio, ABRE_PARENTESES) && parametro_formal(palavra, inicio) 
	&& parametro_formal_linha(palavra, inicio) && match(palavra, inicio, FECHA_PARENTESES)){
		return 1;
	} else return 0;
}

// parâmetro formal ::= [var] identificador : tipo
int parametro_formal(char palavra[], int *inicio){
	match(palavra, inicio, VAR);
	if(match(palavra, inicio, IDENTIFICADOR) && match(palavra, inicio, DOIS_PONTOS) 
	&& tipo(palavra, inicio)){
		return 1;
	} else return 0;
}

// PARAMETRO FORMAL LINHA ::= ; parametro formal
int parametro_formal_linha(char palavra[], int *inicio){
	if(match(palavra, inicio, PONTO_VIRGULA)){
		if(parametro_formal(palavra, inicio)){
			return 1;
		}else return 0;
	} else return 1;
}


// tipo ::= int | bool
int tipo(char palavra[], int *inicio){ 
	if(match(palavra, inicio, INT) || match(palavra, inicio, BOOL)){
		return 1;
	} else return 0;
}

// Livro texto = comando composto ::= begin comando { ; comando } end
// Ajuste = comando composto ::= begin comando end
int comando_composto(char palavra[], int *inicio){
	if (match(palavra, inicio, BEGIN) && comando(palavra, inicio) 
	&& match(palavra, inicio, END)){
		return 1;
	} else{
		return 0;	
	} 
}

// livro texto = comando ::= atribuição | chamada de procedimento | comando composto | comando condicional | comando repetitivo | write( identiicador )
// ajuste = comando ::= atribuição | chamada de procedimento | comando composto | comando condicional | comando repetitivo | write( identiicador ){ [ Se match = ; E lookahead != END: comando ] }
int comando(char palavra[], int *inicio){
	if (atribuicao(palavra, inicio) || chamada_procedimento(palavra, inicio) 
	|| comando_composto(palavra, inicio) || comando_condicional(palavra, inicio) 
	|| comando_repetitivo(palavra, inicio)){
		if(match(palavra, inicio, PONTO_VIRGULA) && lookahead != END){
			if(comando(palavra, inicio)){
				return 1;
			} else return 0;
		}
		return 1;
	} 
  
  if(match(palavra, inicio, WRITE) 
	&& match(palavra, inicio, ABRE_PARENTESES) && match(palavra, inicio, IDENTIFICADOR) 
	&& match(palavra, inicio, FECHA_PARENTESES)){
    imprimirTabela(palavra);
    if(match(palavra, inicio, PONTO_VIRGULA) && lookahead != END){
			if(comando(palavra, inicio)){
        return 1;
      } else return 0;
		}
    return 1;
  }
  
	return 0; 
}

// atribuição ::= variável := expressão
int atribuicao(char palavra[], int *inicio){
	int tipo_v;
  	if(lookahead == IDENTIFICADOR){
    	tipo_v = buscarTipoVariavel(palavra, *inicio);
  	}
  	if(variavel(palavra, inicio, 0)){
    	if(match(palavra, inicio, ATRIBUICAO) && expressao(palavra, inicio, tipo_v) ){
			return 1;
    	} else return 0;
	} else return 0;
}

// chamada de procedimento ::= identificador [(lista de parâmetros)]
int chamada_procedimento(char palavra[], int *inicio){
	if(match(palavra, inicio, IDENTIFICADOR) &&  lista_parametros(palavra, inicio)){
		return 1;
	} else return 0;
}

// Livro texto : lista de parâmetros ::= [ ( identificador | numero | bool ) { ; ( identificador | numero | bool ) } ]
// ajuste : lista de parâmetros ::= [ ( identificador | numero | bool ) { LISTA PARAMETROS LINHA } ]
int lista_parametros(char palavra[], int *inicio){
	if(match(palavra, inicio, ABRE_PARENTESES) ){
		if((match(palavra, inicio, IDENTIFICADOR) || match(palavra, inicio, NUM) 
		|| bool(palavra, inicio)) && match(palavra, inicio, FECHA_PARENTESES) 
		&& lista_parametros_linha(palavra, inicio)){
			return 1;	
		} else return 0;
	} 	else return 1;
}

// LISTA PARAMETROS LINHA ::= [ ; lista parametros ]
int lista_parametros_linha(char palavra[], int *inicio){
	if(match(palavra, inicio, PONTO_VIRGULA)){
		if (lista_parametros(palavra, inicio)) return 1;
		else return 0;
	} else return 1;
}

// comando condicional ::= if ( expressão ) then comando [ else comando ]
int comando_condicional(char palavra[], int *inicio){
	if(match(palavra, inicio, IF) && match(palavra, inicio, ABRE_PARENTESES) 
	&& expressao(palavra, inicio, 0) && match(palavra, inicio, FECHA_PARENTESES) 
	&& match(palavra, inicio, THEN) && comando(palavra, inicio)){
		if(match(palavra, inicio, ELSE)){
			if (comando(palavra, inicio)){
				return 1;
			} else return 0;
		}
		return 1;
	} else return 0;
}

// comando repetitivo ::= while ( expressão ) do comando
int comando_repetitivo(char palavra[], int *inicio){
	if(match(palavra, inicio, WHILE) && match(palavra, inicio, ABRE_PARENTESES) 
	&& expressao(palavra, inicio, 0) && match(palavra, inicio, FECHA_PARENTESES) 
	&& match(palavra, inicio, DO) && comando(palavra, inicio)){
		return 1;
	} else return 0;
}

//  expressão ::= expressão simples [ relação expressão simples ]
int expressao(char palavra[], int *inicio, int tipo_v){
	if(expressao_simples(palavra, inicio, tipo_v)){
		if(relacao(palavra, inicio)){
			if(expressao_simples(palavra, inicio, tipo_v)){
				return 1;	
			}else return 0;	
		}
		return 1;
	} return 0;
}

// relação ::= = | <>| <| <= | >= | >
int relacao(char palavra[], int *inicio){
	if(match(palavra, inicio, IGUAL) || match(palavra, inicio, DIFERENTE) 
	|| match(palavra, inicio, MENOR_IGUAL) || match(palavra, inicio, MAIOR_IGUAL) 
	|| match(palavra, inicio, MAIOR) || match(palavra, inicio, MENOR)){
		return 1;
	} else return 0;
}

// expressão simples ::= [+ | -] termo {(+ | -) termo}
int expressao_simples(char palavra[], int *inicio, int tipo_v){
	match(palavra, inicio, MAIS);
	match(palavra, inicio, MENOS);
	if(termo(palavra, inicio, tipo_v)){
		if(match(palavra, inicio, MAIS) || match(palavra, inicio, MENOS)){
			if(termo(palavra, inicio, tipo_v)) return 1;
			else return 0;
		}
		return 1;
	}return 0;
}

// termo ::= fator {( * | / ) fator }
int termo(char palavra[], int *inicio, int tipo_v){
	if(fator(palavra, inicio, tipo_v)){
		if(match(palavra, inicio, MULTIPLICACAO) || match(palavra, inicio, DIVISAO)){
			if(fator(palavra, inicio, tipo_v)){
				return 1;
			} else return 0;
		}
		return 1;
	}else return 0;
}

// fator ::= variavel | número | bool | ( expressão simples )
int fator(char palavra[], int *inicio, int tipo_v){
	if(variavel(palavra, inicio, tipo_v) ){
    return 1;
  }
  if (match(palavra, inicio, NUM) ){
    if(tipo_v == INT || tipo_v == 0) {
      return 1;
    }
    else{
      erro_semantico = 1;
      return 0;
    } 
  }
  if (bool(palavra, inicio)){
    if(tipo_v == BOOL || tipo_v == 0){
      return 1;
    }else{
      erro_semantico = 1;
      return 0;
    }
  }
	if(match(palavra, inicio, ABRE_PARENTESES) && expressao_simples(palavra, inicio, tipo_v) 
	&& match(palavra, inicio, FECHA_PARENTESES)){
		return 1;
	}
  return 0;
}

// variável ::= identificador
int variavel(char palavra[], int *inicio, int tipo_v){

  	int var_local;

  	if(lookahead == IDENTIFICADOR){
    	var_local = buscarTipoVariavel(palavra, *inicio);
  	}

	if(match(palavra, inicio, IDENTIFICADOR)){
    	if(var_local == tipo_v || tipo_v == 0){
      		return 1;
    	}
		else{
			if(var_local == 0) erro_semantico = 2;
			else erro_semantico = 1;
      		return 0;
    	}
	} else return 0;
}

// bool ::= true | false
int bool(char palavra[], int *inicio){
	if(match(palavra, inicio, TRUE) || match(palavra, inicio, FALSE)){
		return 1;
	} else return 0;
}

int main(void){

	int inicio, fim, codigo, j;	
	char palavra[5000];
	inicio = -1;
	
	j = 0;


	//Lê o arquivo e armazena o texto na variavel palavra;
	
	FILE *arquivo;
    arquivo = fopen("entrada.txt", "r");

	while(!feof(arquivo)){
		int i;
		char linha[100];
		fgets(linha, 100, arquivo);	
		
		i=0;
		while(linha[i] != '\n'){
			palavra[j] = linha[i];
			i++;
			j++;
		}
	}	
	
	erro_semantico = 0;
  	lookahead = scanner(palavra, &inicio);
  	categoria_simbolo = PROGRAM;
  	escopo = 0;
  	S = NULL;
	
	if (programa(palavra, &inicio)){
		
		printf("\n\nSucesso\n");
		
	}else{
		printf("\n\nRecusado\n\n");
		
		imprime_palavra(palavra, 0, inicio);
		
    if(erro_semantico != 0){
    	if(erro_semantico == 1) printf("<-- erro \n\nErro de tipagem.");
    	else if(erro_semantico == 2) printf("<-- erro \n\nVariavel nao declarada.");
    	else if(erro_semantico == 3) printf("<-- erro \n\nProcedimento ja declarado.");
    	else printf("<-- erro \n\nVariavel ja declarada.");
	} else{
		  	printf("<-- erro \n\nNao era experado o item: ");
		  	item_lexico(lookahead);
    	}
	}

	return 0;   
}
