    #include <stdio.h>
    #include <malloc.h>
    #include <string.h>
    #include <conio.h>

    typedef struct Operatiune{
        char codBanca[6];
        char codClient[11];
        char dataOperatiune[11];
        double sumaOperatiune;
        struct Operatiune *next;
    }Operatiune;
    int isEmpty(Operatiune *head ){
        if(head==NULL) return 1;
        else return 0;
    }
    void replaceAll(char *source,char oldCharacter,char newCharacter){
        for(int i=0;i< strlen(source);i++)
            if(source[i] == oldCharacter)
                source[i] = newCharacter;
        return;

    }
    void createOperation(const char codBanca[6], const char codClient[11], const  char dataOperatiune[11], const double sumaOperatiune, Operatiune **head) {
        Operatiune *node = malloc(sizeof(Operatiune));

        if (node == NULL) {
            fputs("Error: Out of memory\n", stderr);
            exit(1);
        } else {

            strcpy(node->codBanca, codBanca);
            strcpy(node->codClient, codClient);
            strcpy(node->dataOperatiune, dataOperatiune);
            node->sumaOperatiune = sumaOperatiune;
            if (*head == NULL) {
                node->next = *head;
                *head = node;
            } else {
                Operatiune *iterr = *(head);
                while (iterr->next != NULL)
                    iterr = iterr->next;
                iterr->next = node;
                node->next = NULL;


            }
        }
    }
    void print(Operatiune * head){
        int i=0;
        while(head!=NULL){
            printf("\n Opratiune %i ",++i);
            printf("\n Cod banca %s ",head->codBanca);
            printf("\n Cod client %s ",head->codClient);
            printf("\n Data operatiune %s ",head->dataOperatiune);
            printf("\n Suma operatiune %.3f ",head->sumaOperatiune);
            printf("\n-----------------------------------------------------");
            head=head->next;

        }
    }
    double calculateSum(Operatiune *head){
        double sum=0;
        while(head!=NULL){
            sum+=head->sumaOperatiune;
            head=head->next;

        }
        return sum;
    }
    int parseFile(Operatiune **head){
        FILE *fptr;
        fptr = fopen("operations.txt","r");
        char operation[43];
        if(fptr == NULL)
        {
            printf("Error!");
            getch();
            exit(1);
        }
        char codBanca[6];
        char codClient[11];
        char dataOperatiune[11];
        double sumaOperatiune;
        //Citim toate operațiunile din fișier
        while (fgets(operation, sizeof(operation), fptr)){
            if (operation[0] == '\r' || operation[0]=='\n') {
                continue; // Ignoră caracterul '\r' si '\n'
            }
            //Citim fiecare operațiune rând cu rând  în variabila operation apoi extragem din ea datele
//            fscanf(fptr,"%s",&operation);
            strncpy ( codBanca, operation, 5 );
            codBanca[5] = '\0';
            strncpy ( codClient, operation+5, 10 );
            codClient[10] = '\0';
            strncpy ( dataOperatiune, operation+15, 10 );
            dataOperatiune[10] = '\0';
            //Facem replace la ',' în '.' pentru a putea parsa suma operațiunii
            replaceAll(operation+25,',','.');
            sumaOperatiune = strtod(operation+25, '\0');
            createOperation(codBanca,codClient,dataOperatiune,sumaOperatiune,head);

        }


        fclose(fptr);
        return 0;

    }
    void Swap(Operatiune * operatiune1, Operatiune *operatiune2)
    {
        Operatiune aux;
        Operatiune *next1, *next2;
        next1=operatiune1->next;
        next2=operatiune2->next;
        aux=*operatiune1;
        *operatiune1=*operatiune2;
        *operatiune2=aux;
        operatiune1->next=next1;
        operatiune2->next=next2;
        return;
    }
    void sortByClient(Operatiune * head){
        Operatiune *current = head, *index = NULL;
        if(head == NULL) {
            return ;
        }
        else {
            while(current != NULL) {
                index = current->next;
                while(index != NULL) {
                    if(strcmp(current->codClient ,index->codClient)>0 || strcmp(current->codBanca,index->codBanca)>0) {
                        Swap(current,index);
                    }
                    index = index->next;
                }
                current = current->next;
            }
        }
    }
    int writeSumToFile(const double  sum,Operatiune *head){
        FILE *fptr;
        fptr = fopen("output.txt","w");
        char client[11]="9999999999";
        client[10]='\0';
        double sumClient=0;

        if(fptr == NULL)
        {
            printf("Error!");
            getch();
            exit(1);
        }
        while(head!=NULL){
            sumClient+=head->sumaOperatiune;
            if(head->next!=NULL && strcmp(head->codClient,head->next->codClient)!=0){
                fprintf(fptr,"%s %s %.3f\n",head->codBanca,head->codClient,sumClient);
                sumClient=0;
            }
            else if(head->next==NULL){
                fprintf(fptr,"%s %s %.3f\n",head->codBanca,head->codClient,sumClient);
            }


            head=head->next;
        }
        fprintf(fptr,"      %s %.3f",client,sum);

        fclose(fptr);
        return 0;
    }
    void freeMemory(Operatiune *head){
        while (head != NULL) {
            Operatiune *temp = head;
            head = head->next;
            free(temp);
        }
        printf("\nMemoria a fost eliberata cu succes");
    }
    int main() {
        Operatiune *head=NULL;
        parseFile(&head);
        print(head);
        sortByClient(head);
        printf("\nAfter sorting...");
        print(head);
        writeSumToFile(calculateSum(head),head);
        freeMemory(head);
        getch();
        return 0;
    }
