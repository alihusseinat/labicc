#include<stdio.h>
#include "mudancalista.h"
#include<stdlib.h>

struct lista{

    NODE * no;

    NODE * inicio;
    NODE * fim;

    int tamanho;

};
struct node_ {

    ITEM * item;

    NODE * proximo;
    NODE * anterior;

};
struct item_{

    int chave;

};
ITEM *criar_item (int chave){

          ITEM *item;

          item = (ITEM *)malloc(sizeof(ITEM));

          if (item != NULL){

             item->chave = chave;
             return(item);

          }
          return(NULL);

}
NODE * criar_no(ITEM *item){

   NODE * n = (NODE *)malloc(sizeof(NODE));

   if ((n != NULL) && (item != NULL)){

      n->item = item;
      n->anterior = NULL;
      n->proximo = NULL;
      return n;

   }
   else return(NULL);
}
void inserirelemento(LISTA *lista, NODE *n){

    if(vazia(lista)){

        lista->inicio = n;
        lista->fim = n;

        n->proximo = NULL;
        n->anterior = NULL;

    }
    else{

        NODE * aux = lista->fim;

        aux->proximo = n;

        n->proximo = NULL;
        n->anterior = aux;
        lista->fim = n;
    }

    ++lista->tamanho;

}
int remover_item(LISTA *lista, int chave){

    if(vazia(lista))return 0;
	
	int i = 0;
	NODE * end = lista->inicio;
	NODE * aux;
    
	while(end != NULL && end->item->chave != chave){
		end = end->proximo;
		++i;
	}
		

	
	if(i == lista->tamanho)return 0;
	
	if(lista->tamanho == 1){

            free(lista->inicio->item);
            free(lista->inicio);

            lista->inicio = NULL;
            lista->fim = NULL;

            --lista->tamanho;

            return 1;
    	}

	if(i == 0){

		aux = end->proximo;
		
		lista->inicio = aux;

		if(aux != NULL)aux->anterior = NULL;
		
		free(end->item);
		free(end);

		--lista->tamanho;
		return 1;
	}
	if(i == lista->tamanho - 1){


		aux = end->proximo;
		aux = end->anterior;

		free(end->item);
		free(end);

		aux->proximo = NULL;
		--lista->tamanho;
		return 1;

	}
   	else{
	

		aux = end->proximo;
		aux = end->proximo;
		
		end->proximo->anterior = end->anterior;
		end->anterior->proximo = aux;

		free(end->item);
		free(end);
		--lista->tamanho;		

		return 1;
		
	}
   

}
LISTA * criar_lista(void){

    LISTA * lista = (LISTA *)malloc(sizeof(LISTA));
    if(lista != NULL) {

        lista->inicio = NULL;

        lista->fim = NULL;

        lista->tamanho = 0;
    }

   return lista;
}
int vazia(LISTA *lista){

    if(lista->inicio == NULL && lista->tamanho == 0)return 1;

    return 0;
}

void maior60(LISTA * l1, LISTA * l2){

    int i = 1, j = 1, remover;
    NODE * end = l1->inicio;

    NODE * aux;

    while(end != NULL){

        aux = end->proximo;

        if(end->item->chave >= 60){

            printf("%d - %d\n", j, i);

            remover = end->item->chave;

            remover_item(l1, remover);
            inserirelemento(l2, criar_no(criar_item(remover)));

            ++j;

        }

        end = aux;

        ++i;
    }

}
