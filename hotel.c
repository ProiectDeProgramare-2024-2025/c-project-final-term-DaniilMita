#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"

typedef struct {
    int id;
    char tip[100];
    char facilitati[100];
    int pret;
    int nrpers;
    int valabil;
} Camera;

typedef struct {
    int id;
    char optiune[100];
    int pret;
} Serviciu;

Camera *camere = NULL;
int num_camere = 0;
Serviciu *servicii = NULL;
int num_servicii = 0;

void incarca_camere() {
    FILE *file = fopen("camere.txt", "r");
    if (file == NULL) {
        file = fopen("camere.txt", "w");
        Camera initial[] = {
            {1, "Cameră pentru o persoană", "Wi-Fi, TV, mini-frigider, aer condiționat, baie privată, fără balcon", 150, 1, 1},
            {2, "Cameră dublă", "Wi-Fi, TV, mini-frigider, aer condiționat, baie privată, cu balcon", 250, 2, 1},
            {3, "Cameră pentru 5 persoane", "Wi-Fi, TV, mini-frigider, aer condiționat, baie privată, fără balcon", 350, 5, 1},
            {4, "Cameră pentru o persoană", "Wi-Fi, TV, mini-frigider, baie privată, fără balcon", 100, 1, 0},
            {5, "Cameră dublă", "Wi-Fi, TV, mini-frigider, baie privată", 150, 2, 1},
            {6, "Cameră pentru o persoană", "Wi-Fi, TV, mini-frigider, baie privată, fără balcon", 100, 1, 0},
            {7, "Cameră pentru o persoană", "Wi-Fi, TV, mini-frigider, aer condiționat, baie privată, fără balcon", 150, 1, 1},
            {8, "Cameră dublă", "Wi-Fi, TV, mini-frigider, aer condiționat, baie privată, cu balcon", 250, 2, 1}
        };
        for (int i = 0; i < sizeof(initial)/sizeof(Camera); i++) {
            fprintf(file, "%d;%s;%s;%d;%d;%d\n",
                initial[i].id, initial[i].tip, initial[i].facilitati,
                initial[i].pret, initial[i].nrpers, initial[i].valabil);
        }
        fclose(file);
        file = fopen("camere.txt", "r");
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        Camera c;
        sscanf(line, "%d;%99[^;];%99[^;];%d;%d;%d",
            &c.id, c.tip, c.facilitati, &c.pret, &c.nrpers, &c.valabil);

        Camera *temp = realloc(camere, (num_camere + 1) * sizeof(Camera));
        camere = temp;
        camere[num_camere++] = c;
    }
    fclose(file);
}

void incarca_servicii() {
    FILE *file = fopen("servicii.txt", "r");
    if (file == NULL) {
        file = fopen("servicii.txt", "w");
        Serviciu initial[] = {
            {1, "Servicii de îngrijire a animalelor (pet sitting, grooming)", 200},
            {2, "Piscină", 100},
            {3, "Saună", 115},
            {4, "Masaj", 125},
            {5, "Excursii organizate", 300},
            {6, "Mic dejun", 40}
        };
        for (int i = 0; i < sizeof(initial)/sizeof(Serviciu); i++) {
            fprintf(file, "%d;%s;%d\n",
                initial[i].id, initial[i].optiune, initial[i].pret);
        }
        fclose(file);
        file = fopen("servicii.txt", "r");
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        Serviciu s;
        sscanf(line, "%d;%99[^;];%d", &s.id, s.optiune, &s.pret);

        Serviciu *temp = realloc(servicii, (num_servicii + 1) * sizeof(Serviciu));
        servicii = temp;
        servicii[num_servicii++] = s;
    }
    fclose(file);
}

void salveaza_camere() {
    FILE *file = fopen("camere.txt", "w");
    for (int i = 0; i < num_camere; i++) {
        fprintf(file, "%d;%s;%s;%d;%d;%d\n",
            camere[i].id, camere[i].tip, camere[i].facilitati,
            camere[i].pret, camere[i].nrpers, camere[i].valabil);
    }
    fclose(file);
}

void afisare_camere() {
    printf(">>> Camere disponibile <<<\n");
    for (int i = 0; i < num_camere; i++) {
        if (camere[i].valabil) {
            printf(GREEN "ID: %d " RESET BLUE "| Tip: %s " RESET YELLOW "| Capacitate: %d " RESET GREEN "| Preț: %d\n" RESET,
                   camere[i].id, camere[i].tip, camere[i].nrpers, camere[i].pret);
        }
    }
}

void afisare_servicii() {
    printf(">>> Servicii disponibile <<<\n");
    for (int i = 0; i < num_servicii; i++) {
        printf(GREEN "ID: %d " RESET BLUE "| Serviciu: %s " RESET YELLOW "| Preț: %d\n" RESET,
               servicii[i].id, servicii[i].optiune, servicii[i].pret);
    }
}

void rezervare_camera(int camera_id, int num_pers, int durata) {
    for (int i = 0; i < num_camere; i++) {
        if (camere[i].id == camera_id) {
            if (!camere[i].valabil) {
                printf("🚫 Cameră deja rezervată!\n");
                return;
            }
            if (num_pers > camere[i].nrpers) {
                printf("🚫 Capacitate maximă: %d persoane!\n" , camere[i].nrpers);
                return;
            }
            camere[i].valabil = 0;
            salveaza_camere();
            printf("✅ Rezervare confirmată:\nID: %d\nTip: %s\nDurata: %d nopți\nPreț total: %d\n" ,camera_id, camere[i].tip, durata, camere[i].pret * durata);
            return;
        }
    }
    printf("🚫 Cameră invalidă!\n");
}

void anulare_rezervare(int camera_id) {
    for (int i = 0; i < num_camere; i++) {
        if (camere[i].id == camera_id) {
            if (camere[i].valabil) {
                printf("🚫 Nu există rezervare pentru această cameră!\n" );
                return;
            }
            camere[i].valabil = 1;
            salveaza_camere();
            printf("✅ Rezervare anulată cu succes!\n" );
            return;
        }
    }
    printf("🚫 Cameră invalidă!\n" );
}

void adauga_camera(int id, const char* tip, const char* facilitati, int pret, int nrpers) {
    for(int i = 0; i < num_camere; i++) {
        if(camere[i].id == id) {
            printf("🚫 Eroare: ID %d există deja!\n" , id);
            return;
        }
    }

    Camera noua;
    noua.id = id;
    noua.pret = pret;
    noua.nrpers = nrpers;
    noua.valabil = 1;

    strncpy(noua.tip, tip, sizeof(noua.tip)-1);
    noua.tip[sizeof(noua.tip)-1] = '\0';

    strncpy(noua.facilitati, facilitati, sizeof(noua.facilitati)-1);
    noua.facilitati[sizeof(noua.facilitati)-1] = '\0';

    Camera *temp = realloc(camere, (num_camere + 1) * sizeof(Camera));
    camere = temp;
    camere[num_camere++] = noua;
    salveaza_camere();
    printf("✅ Camera %d a fost adăugată cu succes!\n" , id);
}

void help() {
    printf("Comenzi disponibile:\n");
    printf("afiseaza_camere - Afișează camere disponibile\n");
    printf("afiseaza_servicii - Afișează servicii disponibile\n");
    printf("rezervare_camera - Rezervă cameră ( <id(ex 1)> <persoane(ex 4)> <nopți(ex 2)> )\n");
    printf("anulare_rezervare - Anulează rezervare ( <id(ex 1)> )\n");
    printf("adauga_camera - Adaugă cameră nouă ( <id(ex 1)> <tip(ex Cameră pentru o persoană)> <facilități(ex Wi-Fi, TV)> <preț(ex 400)> <capacitate(ex 2)> )\n");
    printf("help - Afișează acest mesaj\n");
}

int main(int argc, char *argv[])
{
    incarca_camere();
    incarca_servicii();

    if(argc < 2) {
        help();
        return 0;
    }

    if(strcmp(argv[1], "afiseaza_camere") == 0) {
        afisare_camere();
    }
    else if(strcmp(argv[1], "afiseaza_servicii") == 0) {
        afisare_servicii();
    }
    else if(strcmp(argv[1], "rezervare_camera") == 0) {
        if(argc != 5) {
            printf("Utilizare: rezervarea_camerei <ID(ex 8)> <persoane(ex 4)> <nopți(ex 2)>\n");
        }
        rezervare_camera(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    }
    else if(strcmp(argv[1], "anulare_rezervare") == 0) {
        if(argc != 3) {
            printf("Utilizare: anulare <ID>\n");
        }
        anulare_rezervare(atoi(argv[2]));
    }
    else if(strcmp(argv[1], "adauga_camera") == 0){
        if(argc != 7)
        {
            printf("Utilizare: adauga_camera <ID> \"<Tip>\"  \"<Facilități>\" <Preț(ex 400)> <Capacitate(ex 2)>\n");
        }
            adauga_camera(
                atoi(argv[2]),
                argv[3],
                argv[4],
                atoi(argv[5]),
                atoi(argv[6])
            );
        }
        else if(strcmp(argv[1], "help") == 0) {
            help();
        }
        else {
            printf("🚫 Comandă invalidă!\n");
            help();
        }
        free(camere);
        free(servicii);
        return 0;
}











