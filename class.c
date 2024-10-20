#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct student {
    char nom[20];
    char prenom[20];
    int age;
    int ID;
    long int date;
    long int last_modif;
    float notes[40];
    int notesnb;
    unsigned int status; // bit 1 : inscrit, bit 2 : en atente, bit 3 : dipome, bit 4 : suspendu 
    unsigned int languages; // bit 1 : Anglais, bit 2 : francais, bit 3 : allemend, bit 4 : espagnol, bit 5 : italien, bit 6 : latin, bit 7 : grec, bit 8 : russe
    struct student *next;
} etudiant;

void set_bit(unsigned int* bitmap, int n) {
    *bitmap |= (1 << n);  // Active le n-ième bit
}

void clear_bit(unsigned int* bitmap, int n) {
    *bitmap &= ~(1 << n);  // Désactive le n-ième bit
}

int is_bit_set(unsigned int bitmap, int n) {
    return (bitmap & (1 << n)) != 0;  // Vérifie si le n-ième bit est activé
}

void affiche_student(etudiant* ed) {
    struct tm* local_time = localtime(&ed->date);
    char* date_str = (char*)malloc(100 * sizeof(char)); 
     if (date_str == NULL) {
        perror("Erreur d'allocation de mémoire");
        return;
    }
    struct tm* local_time2 = localtime(&ed->last_modif);
    char* date_str2 = (char*)malloc(100 * sizeof(char)); 
     if (date_str == NULL) {
        perror("Erreur d'allocation de mémoire");
        return;
    }
    strftime(date_str, 100, "%d/%m/%Y %H:%M:%S", local_time);
    strftime(date_str2, 100, "%d/%m/%Y %H:%M:%S", local_time2);
    printf("Student :\nNom : %s\nPrénom : %s\nÂge : %d\nID : %d\nDate de création : %s\nDerniere modif : %s\nNotes :", ed->nom, ed->prenom, ed->age,ed->ID,date_str,date_str2);
    for (int i = 0 ; i<ed->notesnb; i++){
        printf(" %f ", ed->notes[i]);
    }
    char stat[4][10] = {"inscrit", "en attente", "diplome", "suspendu"};
    char langue[8][10] = {"anglais", "francais", "allemand", "espagnol", "italien", "latin", "grec", "russe"};
    printf("\nstatut :");
    for (int i = 0 ; i<4 ; i++){
        if (is_bit_set(ed->status,i)){
            printf(" %s ", stat[i]);
        }
    }
    printf("\nlangues :");
    for (int i = 0 ; i<8 ; i++){
        if (is_bit_set(ed->languages,i)){
            printf(" %s ", langue[i]);
        }
    }
    free(date_str);
    free(date_str2);
}

void modiify_student(etudiant* ptr){
    affiche_student(ptr);
    printf("\nque voulez vous modifier?\n1 : nom\n2 : prénom\n3 : age\n4 : notes\n5 : status\n6 : langues\n");
    int h;
    char a;
    int y;
    scanf(" %d", &h);
    switch (h)
    {
    case 1:
        printf("entrez le nouveau nom de l'étudiant\n");
        scanf("%s", ptr->nom);
        break;
    case 2:
        printf("entrez le nouveau prénom de l'étudiant\n");
        scanf("%s", ptr->prenom);
        break;
    case 3 :
        printf("entrez le nouvel age de l'étudiant\n");
        scanf("%d", &ptr->age);
        break;
    case 4:
        printf("souhaitez vous ajouter une note (a) ou modifier une note existante (m)\n");
        scanf(" %c", &a);
        if(a == 'a'){
            printf("entrez la note\n");
            scanf("%f", &ptr->notes[ptr->notesnb]);
            ptr->notesnb++;
        }
        else if (a == 'm')
        {
            printf("selectionnez le numero de la note a modifier (a partir de 0)\n");
            scanf("%d", &y);
            printf("entre la nouvelle note\n");
            scanf("%f", &ptr->notes[y]);
        }
        break;
    case 5:
        printf("selectionnez le statut que vous voulez rajouter (ou enlever si il est deja présent)\n1 : inscrit\n2 : en attente\n3 : diplomé\n4 :suspendu\n");
        scanf("%d", &y);
        set_bit(&ptr->status, y -1);
        break;
    case 6:
        printf("selctionner les langeus a rajouter ou enlever si elles sont deja présentes\n1 : Anglais\n2 : Francais\n3 : Allemand\n4 : Espagnol\n5 : Italien\n6 : Latin\n7 : grec\n8 : russe\n");
        scanf("%d", &y);
        set_bit(&ptr->languages, y -1);
        break;
    }
    time(&ptr->last_modif);
    affiche_student(ptr);
}

void affiche_list(etudiant* head) {
    etudiant* tmp = head;
    while (tmp != NULL) {
        affiche_student(tmp);
        tmp = tmp->next;
    }
}

etudiant* insert(etudiant* lstptr, int nb) {
    etudiant* ptr = (etudiant*)malloc(sizeof(etudiant));  // Allouer dynamiquement la mémoire pour un nouvel étudiant
    int yesno;
    if (ptr == NULL) {
        printf("Erreur d'allocation de mémoire.\n");
        return lstptr;  // Retourner la liste inchangée si allocation échoue
    }

    // Lecture des données de l'étudiant
    printf("Entrez un étudiant\nNom: ");
    scanf("%s", ptr->nom);
    printf("Entrez leur prénom: ");
    scanf("%s", ptr->prenom);
    printf("Entrez leur âge: ");
    scanf("%d", &ptr->age);
    ptr->ID = nb;
    time(&ptr->date);
    time(&ptr->last_modif);
    printf("Si vous souhaitez entrer des notes pour cet eleve indiquez le nombre de notes que vous voulez ajouter, sinon 0\n");
    scanf("%d", &yesno);
    ptr->notesnb = yesno;
    for(int i = 0 ; i<yesno; i++){
        printf("entrez une note\n");
        scanf("%f", &ptr->notes[i]);
    }
    ptr->status = 0;
    printf("quel est le statut de l'étudiant?\n1 : inscrit\n2 : en attente\n3 : diplomé\n4 :suspendu\n0 to pass\n");
    scanf("%d", &yesno);
    if (yesno!=0){
        set_bit(&ptr->status, yesno -1);
    }
    ptr->languages = 0;
    printf("quelles langues sont parlées par l'étudiant? 0 pour passer\n1 : Anglais\n2 : Francais\n3 : Allemand\n4 : Espagnol\n5 : Italien\n6 : Latin\n7 : grec\n8 : russe\n");
    scanf("%d", &yesno);
    while (yesno!=0){
        set_bit(&ptr->languages, yesno -1);
        scanf("%d", &yesno);
    }
    ptr->next = lstptr;  // Insérer le nouvel étudiant au début de la liste
    return ptr;  // Retourner le nouvel élément en tête de liste
}

void free_list(etudiant* head) {
    etudiant* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);  // Libérer chaque élément de la liste
    }
}

etudiant* select_student(etudiant* head, int index){
    //pas oublier faire une copie du pointeur avant de le passer a la focntion select
    while (head != NULL && head->ID != index){
        head = head->next;
    }
    return head;
}

void write_list(etudiant *head){
    FILE * fp = fopen("databin", "wb");
    etudiant* tmp = head;
    while (tmp != NULL) {
        fwrite(tmp, sizeof(etudiant), 1, fp);
        tmp = tmp->next;
    }
    fclose(fp);
}

etudiant* recup_list() {
    FILE* fp = fopen("databin", "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return NULL;  // Retourner NULL en cas d'erreur d'ouverture
    }

    etudiant* head = NULL;  // Tête de la liste
    etudiant* prev = NULL;  // Pointeur vers le nœud précédent
    etudiant temp;

    // Lire les étudiants du fichier un par un
    while (fread(&temp, sizeof(etudiant), 1, fp) == 1) {
        // Allouer de la mémoire pour un nouvel étudiant
        etudiant* new_node = (etudiant*)malloc(sizeof(etudiant));
        if (new_node == NULL) {
            perror("Memory allocation failed");
            fclose(fp);
            return head;  // Retourner la liste partielle en cas de problème d'allocation
        }

        // Copier les informations lues dans le nouvel étudiant
        *new_node = temp;  
        new_node->next = NULL;  

        // Si la liste est vide, ce nouvel étudiant devient la tête
        if (head == NULL) {
            head = new_node;
        } else {
            prev->next = new_node;  
        }

        prev = new_node;  
    }

    fclose(fp);
    return head; 
}

void affiche_moyenne(etudiant* st){
    float moyenne = 0;
    for(int i = 0; i < st->notesnb; i++)
    {
        moyenne += st->notes[i];
    }
    printf("%f\n", moyenne/st->notesnb);
}

void affiche_classe_moyenne(etudiant* st){
    float moyenne = 0;
    int nbrecord = 0;
    etudiant* tmp = st;
    while (tmp != NULL) {
        nbrecord += tmp->notesnb;
        for (int i = 0; i < tmp->notesnb; i++)
        {
            moyenne += tmp->notes[i];
        }
        
        tmp = tmp->next;
    }
    printf("%f\n", moyenne/nbrecord);
}

int main() {
    etudiant* ptr = NULL;  // Initialiser la liste à NULL
    etudiant* ptr2;
    ptr  = recup_list();
    int debut;
    if (ptr == NULL){
        debut = 0;
    }
    else{
        debut = ptr->ID +1;
    }
    int y = 1;
    char t;
    int s;
    int z;
    while(y){
        printf("\nMenu principal :\nq to quit\na to add a student\nd to display the students list\nm to modify a student\nv to get the average of a student or the class\n");
        scanf(" %c",&t);
        switch (t)
        {
        case 'q':
            y = 0;
            break;
        case 'a':
            ptr = insert(ptr,debut);
            debut++;
            break;
        case 'd':
            affiche_list(ptr);
            break;
        case 'm':
            affiche_list(ptr);
            printf("\nselect the student number you wish to modify\n");
            scanf("%d",&s);
            ptr2 = ptr;
            ptr2 = select_student(ptr2, s);
            if(ptr2 == NULL){
                printf("le numéro que vous avez rentré n'existe pas");
            }
            else{
                modiify_student(ptr2);
            }
        case 'v':
            printf("voulez vous la moyenne d'un student (1) ou de la classe(2)\n");
            scanf("%d",&z);
            if (z==1){
                affiche_list(ptr);
                printf("\nselect the student number\n");
                scanf("%d",&s);
                ptr2 = ptr;
                ptr2 = select_student(ptr2, s);
                if(ptr2 == NULL){
                    printf("le numéro que vous avez rentré n'existe pas\n");
                }
                else{
                    affiche_moyenne(ptr2);
                }
            }
            if(z==2){
                affiche_classe_moyenne(ptr);
            }
        }
    }
    // Insérer 5 étudiants dans la liste
    /*for (int i = 0; i < 3; i++) {
        debut++;
        ptr = insert(ptr,debut);  
    }*/
    
    write_list(ptr);

    // Libérer la mémoire allouée pour la liste
    free_list(ptr);

    return 0;  
}