#ifdef _WIN32
    #include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include "adatbazis.h"
#include "io.h"


typedef enum Kategoria{
    nev = 1,
    foglalkozas = 2,
    cim = 3,
    telefonszam = 4,
    email = 5
} Kategoria;

void fomenu_kiiras(int* valasztas){
    system("cls");
    printf("NÉVJEGYZÉK\n\n"
            "(1) Új névjegy hozzáadása\n"
            "(2) Keresés\n"
            "(3) Importálás\n"
            "(4) Kilépés\n"
            "\nKiválasztott menüpont: ");
    // menüpont kiválasztása
    scanf(" %d", valasztas);
    getchar(); // (enter beolvasása)
}

// meglévõ névjegy esetén kiírja a régi adatokat
void adatbevitel(Nevjegy* nevjegy){
    system("cls");
    printf("Név: ");
    printf("%s ", nevjegy->nev);
    gets(nevjegy->nev);
    printf("\nFoglalkozás: ");
    printf("%s ", nevjegy->foglalkozas);
    gets(nevjegy->foglalkozas);
    printf("\nCím: ");
    printf("%s ", nevjegy->cim);
    gets(nevjegy->cim);
    printf("\nTelefonszám: ");
    printf("%s ", nevjegy->telefonszam);
    gets(nevjegy->telefonszam);
    printf("\nEmail: ");
    printf("%s ", nevjegy->email);
    gets(nevjegy->email);
    adat_fixalas(nevjegy);
    nevjegy_kiir(nevjegy);
}

// megkérdezi, hogy szeretné-e menteni és ezt visszaadja
bool mentie(){
    char input;
    do{
    printf("\nMenti a névjegyet? ( igen(i) / nem(n) ) ");
    input = getchar();
    } while (input != 'i' && input != 'n');
    if(input=='i') return true;
    else return false;
}

// egy névjegy adatmezõinek kiírása
void nevjegy_kiir(Nevjegy const *const nevjegy){
    printf("\nNév: %s\n"
           "Foglalkozás: %s\n"
           "Cím: %s\n"
           "Telefonszám: %s\n"
           "Email cím: %s\n",
           nevjegy->nev,
           nevjegy->foglalkozas,
           nevjegy->cim,
           nevjegy->telefonszam,
           nevjegy->email);
}

// a választott kategória alapján keres és kilistázza a megfelelõ neveket
void nevek_listaz(Nevjegyzek const *const adatbazis, const int kategoria, const char* keresett_string){
    system("cls");
    printf("Az adatbázisban szereplõ nevek:\n");
    Nevjegy* mozgo = adatbazis->elso->kov;
    while (mozgo != adatbazis->utolso) {
        switch(kategoria){
            case nev:{
                    if(wildcard_keres(keresett_string, mozgo->nev))
                        printf("(%d) %s\n", mozgo->id, mozgo->nev);
                    break;
            }

            case foglalkozas:
                    if(strcmp(mozgo->foglalkozas, keresett_string)==0)
                        printf("(%d) %s\n", mozgo->id, mozgo->nev);
                    break;

            case cim:
                    if(strcmp(mozgo->cim, keresett_string)==0)
                        printf("(%d) %s\n", mozgo->id, mozgo->nev);
                    break;

            case telefonszam:
                    if(strcmp(mozgo->telefonszam, keresett_string)==0)
                        printf("(%d) %s\n", mozgo->id, mozgo->nev);
                    break;

            case email:
                    if(strcmp(mozgo->email, keresett_string)==0)
                        printf("(%d) %s\n", mozgo->id, mozgo->nev);
                    break;
            default: break;

        }
        mozgo = mozgo->kov;
    }
}

void kategoria_valasztas_menu(int* kategoria_kod){
    system("cls");
    printf("Milyen adatra szeretne keresni?\n\n"
           "(1) Név\n"
           "(2) Foglalkozás\n"
           "(3) Cím\n"
           "(4) Telefonszám\n"
           "(5) Email cím\n"
           "(6) Vissza a fõmenübe\n\n"
           "Kategória: ");
    scanf(" %d", kategoria_kod);
}

void kereses_mezo(char* keresett_string){
    system("cls");
    printf("Keresett kifejezés: ");
    scanf(" %[^\n]", keresett_string);
}

void varakozas_bemenetre(void){
    printf("\nEnter a továbblépéshez...");
    while(getchar() != '\n')
    ;
}
