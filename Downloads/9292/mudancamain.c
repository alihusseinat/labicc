#include<stdio.h>
#include<stdlib.h>
#include "mudancalista.h"

int main(){

    LISTA * l1 = criar_lista();
    LISTA * l2 = criar_lista();

    int chave;

    while(scanf("%d", &chave) != EOF){

        inserirelemento(l1, criar_no(criar_item(chave)));

    }

    maior60(l1, l2);

    return 0;
}
