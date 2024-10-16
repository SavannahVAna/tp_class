#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct student {
    char nom[20];
    char prenom[20];
    int age;
    int ID;
    char date[20];
    struct student *next;
} etudiant;

void affiche_student(etudiant* ed) {
    printf("Student :\nNom : %s\nPrénom : %s\nÂge : %d\n", ed->nom, ed->prenom, ed->age);
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
    time(ptr->date);

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

void write_list(etudiant *head){
    FILE * fp = fopen("databin", "ab");
    etudiant* tmp = head;
    while (tmp != NULL) {
        fwrite(&tmp, sizeof(etudiant), 1, fp);
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
    ptr  = recup_list();
    // Insérer 5 étudiants dans la liste
    for (int i = 0; i < 3; i++) {
        
        ptr = insert(ptr,1);  // Insérer un nouvel étudiant dans la liste
         // Afficher la liste après chaque insertion
        
    }
    affiche_list(ptr); 
    write_list(ptr);

    // Libérer la mémoire allouée pour la liste
    free_list(ptr);

    return 0;  // Retourner 0 pour indiquer que le programme s'est terminé correctement
}