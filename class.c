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
    printf("Student :\nNom : %s\nPrénom : %s\nÂge : %d\nID : %d\nDate de création : %s\nDerniere modif : %s\n", ed->nom, ed->prenom, ed->age,ed->ID,date_str,date_str2);
    free(date_str);
    free(date_str2);
}

void modiify_student(etudiant* ptr){
    affiche_student(ptr);
    printf("que voulez vous modifier?\n1 : nom\n2 : prénom\n3 : age\n");
    int h;
    scanf(" %d", &h);
    switch (h)
    {
    case 1:
        printf("entrez le nouveau nom de l'étudiant");
        scanf("%s", ptr->nom);
        break;
    case 2:
        printf("entrez le nouveau prénom de l'étudiant");
        scanf("%s", ptr->prenom);
        break;
    case 3 :
        printf("entrez le nouvel age de l'étudiant");
        scanf("%d", &ptr->age);
        break;
    default:
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
    scanf("%d", &ptr->age);  // Attention : il faut passer l'adresse de ptr->age avec &
    ptr->ID = nb;
    time(&ptr->date);
    time(&ptr->last_modif);

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
    while (head != NULL || head->ID != index){
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
        *new_node = temp;  // Copier le contenu de temp dans new_node
        new_node->next = NULL;  // Le nouvel étudiant pointe sur NULL pour le moment

        // Si la liste est vide, ce nouvel étudiant devient la tête
        if (head == NULL) {
            head = new_node;
        } else {
            prev->next = new_node;  // Sinon, relier le nœud précédent au nouveau
        }

        prev = new_node;  // Mettre à jour prev pour le prochain tour
    }

    fclose(fp);
    return head;  // Retourner la tête de la liste remplie
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
    while(y){
        printf("Menu principal :\nq to quit\na to add a student\nd to display the students list\nm to modify a student\n");
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
            printf("select the student number you wish to modify\n");
            scanf(" %d",&s);
            ptr2 = ptr;
            ptr2 = select_student(ptr2, s);
            if(ptr2 == NULL){
                printf("le numéro que vous avez rentré n'existe pas");
            }
            else{
                modiify_student(ptr2);
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

    return 0;  // Retourner 0 pour indiquer que le programme s'est terminé correctement
}