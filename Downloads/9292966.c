#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct tipo{

    char type;
    int key;
    int size;
    char name[50];

} TIPO;
typedef struct bin{

    TIPO ** st;
    char * name;
    int nelements;
    int sizeperrow;

} SCHEMAO;
typedef struct idx{

    void * val;
    int end;

}IDX;
char * corrector(char * str){ //funcao simples que corrige um \n indesejado numa string, que dificultaria a leitura de um arquivo

    int i = 0;

    while(str[i] != '\0'){

        if(str[i] == '\n')str[i] = '\0';

        ++i;

    }

    return str;
}
char * dotdata(char * str){

    str = corrector(str);

    int i = 0;

    int t = strlen(str);

    char * str2 = (char *)malloc(t + 6);

    strcpy(str2, str);

    str2[t] = '.';
    str2[t + 1] = 'd';
    str2[t + 2] = 'a';
    str2[t + 3] = 't';
    str2[t + 4] = 'a';
    str2[t + 5] = '\0';

    return str2;

}
char * dotidx(char * str){

    str = corrector(str);

    int i = 0;

    int t = strlen(str);

    char * str2 = (char *)malloc(t + 5);

    strcpy(str2, str);

    str2[t] = '.';
    str2[t + 1] = 'i';
    str2[t + 2] = 'd';
    str2[t + 3] = 'x';
    str2[t + 4] = '\0';

    return str2;

}
char findtype(SCHEMAO * schema, char * namevar, int * varpos){

    int i = 0;

    while(schema->nelements){

        if(!strcmp(schema->st[i]->name, namevar)){
                *varpos = i;
                return schema->st[i]->type;
        }

        ++i;
    }

}
void dumpindex(SCHEMAO * schema, char * namevar){

    char * path = dotidx(schema->name);

    int flip = 1, i = 0;

    int * ii;
    double * di;
    char * ci;

    int varpos, aux;
    char typevar = findtype(schema, namevar, &varpos);

    FILE * fp = fopen(path, "r+");
    if(fp == NULL)printf("Deu ruim\n");

    while(!feof(fp)){

        if(i >= schema->nelements)i = 0;

        if(flip > 0){

            void * s = malloc(schema->st[varpos]->size);

            if(fread(s, schema->st[varpos]->size, 1, fp) != 1)break;

            switch(typevar){

                case 'i':
                    ii = (int *)s;
                    printf("%d", *ii);
                    break;

                case 'd':
                    di = (double *)s;
                    printf("%.2lf", *di);
                    break;

                case 'c':
                    ci = (char *)s;

                    printf("%s", ci);
                    break;

            }

            flip = -flip;

            free(s);
        }
        else {

            if(fread(&aux,4, 1, fp) != 1)break;

            printf(" = %d\n", aux);

            flip = -flip;
        }
    }
    fclose(fp);
}
void dumpdata(SCHEMAO * schema){

    char * path = dotdata(schema->name);

    int i = 0;

    int * ii;
    double * di;
    char * ci;

    FILE * fp = fopen(path, "r+");
    if(fp == NULL)printf("Deu ruim\n");

    while(!feof(fp)){

        if(i >= schema->nelements)i = 0;

        void * s = malloc(schema->st[i]->size);

        if (fread(s, schema->st[i]->size, 1, fp) != 1)break;

        printf("%s = ", schema->st[i]->name);

        switch(schema->st[i]->type){

            case 'i':
                ii = (int *)s;
                printf("%d\n", *ii);
                break;

            case 'd':
                di = (double *)s;
                printf("%.2lf\n", *di);
                break;

            case 'c':
                ci = (char *)s;

                printf("%s\n", ci);
                break;

        }

        free(s);

        ++i;

    }
    fclose(fp);
}
void swapstruct(IDX * vet, int pos1, int pos2){

    IDX aux;

    aux.end = vet[pos1].end;
    aux.val = vet[pos1].val;

    vet[pos2].end = vet[pos1].end;
    vet[pos2].val = vet[pos1].val;

    vet[pos2].end = aux.end;
    vet[pos2].val = aux.val;

}
int combsort(IDX * vet, int t, int (*compare)(IDX *, IDX *)){ //algoritmo de ordencao combsort, é um algoritmo de troca, é parecido com o buble sort mas tem algumas partcularidades que
                                                                                // fazem dele um algoritmo eficiente e de facil implementacao

    int gap = t;

    int i = 0, n, counter = 0;

    do{

        n = 0;

        if(gap / 1.24733 > 1 )gap = (int)gap / 1.24733;
        else gap = 1;

        i = 0;

        while(i + gap < t){

            if(compare(&vet[i], &vet[i + gap]) > 0){

                swapstruct(vet, i, i + gap);

                n = 1;

                ++counter;
            }

            ++i;

        }

        }while(n == 1 || gap > 1);

        return counter;

}
int comparec(IDX * pos1, IDX * pos2){

    char * c1 = pos1->val;
    char * c2 = pos2->val;

    if(strcmp(c1, c2))return 1;
    else return -1;

}
int compared(IDX * pos1, IDX * pos2){

    double * d1 = pos1->val;
    double * d2 = pos2->val;

    if(*d1 > *d2)return 1;
    else return -1;

}
int comparei(IDX * pos1, IDX * pos2){

    double * i1 = pos1->val;
    double * i2 = pos2->val;

    if(*i1 > *i2)return 1;
    else return -1;

}
void imprimearquivo(SCHEMAO * schema, IDX * index, int t, int varsize){

    int i = 0;

    char * path = dotidx(schema->name);
    FILE * fp = fopen(path, "a+");

    while(i < t){

        fwrite(index->val, varsize, 1, fp);

        fwrite(&index->end, 4, 1, fp);

        ++i;
    }
    fclose(fp);
}
IDX * createindex(SCHEMAO * schema, char * namevar){

    char * path = dotdata(schema->name);

    IDX * index = NULL;

    int i = 0, aux, j = 0, varpos, varsize;

    FILE * fp = fopen(path, "r+");

    if(fp == NULL)printf("Deu ruim\n");

    printf("1111\n");

    while(!feof(fp)){

        printf("While...\n");

        if(i >= schema->nelements)i = 0;

        if(!strcmp(schema->name, namevar)){

            index = realloc(index, (j + 1) * sizeof(IDX));

            void * s = malloc(schema->st[i]->size);

            if (fread(s, schema->st[i]->size, 1, fp) != 1)break;

            index[j].val = s;

            free(s);

            aux = j * schema->sizeperrow;

            index[j].end = aux;

            ++j;

            varsize = schema->st[i]->size;

        }

        fseek(fp, schema->st[i]->size, SEEK_CUR);

        ++i;

    }

    fclose(fp);

    switch(findtype(schema, namevar, &varpos)){

        case 'i':
            combsort(index, j, comparei);
            break;
        case 'd':
            combsort(index, j, compared);
            break;
        case 'c':
            combsort(index, j, comparec);
            break;

    }

    imprimearquivo(schema, index, j, varsize);

    free(index);

}
void readschema(char * path, SCHEMAO * schema){

    schema->st = NULL;

    char * text1 = (char*)malloc(50 * sizeof(char));
    char * text3 = (char*)malloc(6 * sizeof(char));
    char * table = (char*)malloc(6 * sizeof(char));
    schema->name = (char*)malloc(3 * sizeof(char));

    FILE * fp = fopen(path, "r+");

    if(fp == NULL)printf("Deu ruim\n");

    fscanf(fp, "%s", table);
    fscanf(fp, "%s", schema->name);

    text3[2] = '\0';

    int i = 0;

    while(!feof(fp)){

        do{
            fscanf(fp, "%s", text1);

        }while(text1[0] == '\n');

        if(!strcmp(text1, "order")){

                schema->st[i - 1]->key = 1;

        }
        else{
            if(!strcmp(text1, "int")){

                schema->st[i - 1]->type = 'i';
                schema->st[i - 1]->size = 4;

                //printf("iiiiiiint\n");


            }
            else{
                if(!strcmp(text1, "double")){
                    schema->st[i - 1]->type = 'd';

                    schema->st[i - 1]->size = 8;



                }
                else{
                    if(text1[0] == 'c' && text1[1] == 'h' && text1[2] == 'a' && text1[3] == 'r' && text1[4] == '['){

                        text3[0] = text1[5];
                        text3[1] = text1[6];

                        schema->st[i - 1]->size = atoi(text3);
                        schema->st[i - 1]->type = 'c';

                    }
                    else{

                        ++i;
                        schema->st = (TIPO **)realloc(schema->st, (i + 1) * sizeof(TIPO *));

                        schema->st[i - 1] = (TIPO*)malloc(sizeof(TIPO));

                        schema->st[i - 1]->key = 0;

                        strcpy(schema->st[i - 1]->name, text1);
                        //printf("nome %s \n", schema->st[i - 1]->name);
                    }
                }
            }
        }
    }

    fclose(fp);
    free(text1);
    free(text3);

    schema->nelements = i;

}
int switchstring(char * str){

    if(!strcasecmp(str, "dump_schema"))return 1;
    else{
        if(!strcmp(str, "exit")) return 2;
        else{
            if(!strcmp(str, "dump_data"))return 3;
            else {
                if(!strcmp(str, "dump_index"))return 4;
            }
        }
    }
}
void impressora(SCHEMAO * schema){

    int i = 0;

    printf("table %s(%d bytes)\n", schema->name, schema->sizeperrow);

    while(i < schema->nelements){

        printf("%s ", schema->st[i]->name);

        switch(schema->st[i]->type){
            case 'i':

                printf("int");
                if(schema->st[i]->key)printf(" order");
                printf("(%d bytes)\n", schema->st[i]->size);

                break;
            case 'd':

                printf("double");
                if(schema->st[i]->key)printf(" order");
                printf("(%d bytes)\n", schema->st[i]->size);

                break;
            case 'c':

                printf("char[%d]", schema->st[i]->size);
                if(schema->st[i]->key)printf(" order");
                printf("(%d bytes)\n", schema->st[i]->size);

                break;
        }

        ++i;
    }

}
int sizeperrow(TIPO ** schema, int size){

    int i = 0;

    int size2 = 0;

    while(i < size){

        size2 = size2 + schema[i]->size;

        ++i;
    }

    return size2;

}
int main(){

    SCHEMAO * schema = (SCHEMAO *)malloc(sizeof(SCHEMAO));

    int i = 0;
    char * path = (char*)malloc(50 * sizeof(char));
    char * op = (char*)malloc(50 * sizeof(char));
    scanf("%s", path);

    path = corrector(path);

    readschema(path, schema);
    schema->sizeperrow = sizeperrow(schema->st,schema->nelements);

    while(i < schema->nelements){

        if(schema->st[i]->key == 1)createindex(schema, schema->st[i]->name);

        ++i;
    }

    while(1){

        scanf("%s", op);

        op = corrector(op);

        switch(switchstring(op)){

            case 1:
                impressora(schema);
                break;

            case 2:
                free(path);
                free(op);
                return 0;
                break;
            case 3:
                dumpdata(schema);
                break;
            case 4:
                i = 0;
                while(i < schema->nelements){
                    if(schema->st[i]->key == 1)dumpindex(schema, schema->st[i]->name);
                    ++i;
                }
                break;
        }
    }
}
