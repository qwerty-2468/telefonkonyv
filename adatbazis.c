#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "debugmalloc.h"
#include "adatbazis.h"
#include "fajlkezeles.h"
#include "io.h"

// memória foglalása és esetleges feltöltése adatokkal (amennyiben már van adatbázis fájl)
bool adatb_letrehoz(Nevjegyzek* adatbazis){

    // adatbázis inicializálása (láncolt lista strázsáinak létrehozása)
    if(!adatb_inicializal(adatbazis)){
        printf("Nem sikerült inicializálni az adatbázist.");
        return false;
    } else {

    //adatbázis betöltése fájlból ha van már
    printf("Adatbázis fájl keresése\n");
    if(van_adatb_fajl()){
        if(!adatb_beolvas(adatbazis)){
            printf("Nem sikerült beolvasni");
            return false;

        }
        else{
            printf("Sikeres adatbeolvasás");
            return true;
        }
    } else {
        printf("Nem található adatbázis fájl\n");
        if(!fajl_inicializal_adatb()){
            printf("Nem sikerült létrehozni a fájlt.\n");
            return false;
        }
        printf("Új adatbázis fájl lett létrehozva\n\n");
        return true;
    }
    }
}

// duplán láncolt lista strázsáinak létrehozása és egymásra állítása
bool adatb_inicializal(Nevjegyzek* adatbazis){
    adatbazis->elso = (Nevjegy*) malloc(sizeof(Nevjegy));
    if(adatbazis->elso==NULL) return false;
    adatbazis->utolso = (Nevjegy*) malloc(sizeof(Nevjegy));
    if(adatbazis->utolso==NULL) return false;
    adatbazis->elso->kov = adatbazis->utolso;
    adatbazis->utolso->elozo = adatbazis->elso;
    return true;
}

// felszabadítja az adatbázis által foglalt teljes memóriaterületet
void adatb_felszabadit(Nevjegyzek* adatbazis){
    Nevjegy* iter = adatbazis->elso;
    while(iter != adatbazis->utolso){
        Nevjegy* kov = iter->kov;
        free(iter);
        iter = kov;
    }
    free(adatbazis->utolso);
}

// egy nevjegy valtozóból átmenti az adatokat az adatbázisba (memória) és az adatbázis fájlba is
void nevjegy_mentes(Nevjegy * const nevjegy, Nevjegyzek* adatbazis){
    // ID generálása (a jelenlegi legnagyobbnál eggyel nagyobb)
    nevjegy_id_general(adatbazis, nevjegy);

    // névjegy memóriába mentése
    if(!memoriaba_iras(nevjegy, adatbazis)) printf("Nem sikerült menteni a memóriába");
    // névjegy fájlba mentése
    if(!fajlba_nevjegy_mentes(nevjegy)) printf("Nems sikerült menteni az adatbázis fájlba");

    printf("Adatok sikeresen mentve");
    varakozas_bemenetre();
}

// a paraméterként kapott névjegynek generál egy id-t és el is menti bele
// (elég lenne a legutolsó névjegy id-ját megnézni, de így késõbbi változtatásokhoz is jobban tud alkalmazkodni a program)
void nevjegy_id_general(Nevjegyzek const * const adatbazis, Nevjegy* const nevjegy){
    int max=0;
    Nevjegy* mozgo = adatbazis->elso->kov;
    while (mozgo != adatbazis->utolso) {
        if(mozgo->id > max) max = mozgo->id;
        mozgo = mozgo->kov;
    }
    nevjegy->id = max+1;
}

bool memoriaba_iras(Nevjegy const* const nevjegy, Nevjegyzek* adatbazis){
    Nevjegy* uj = (Nevjegy*) malloc(sizeof(Nevjegy));
    if(uj!=NULL){
        memcpy(uj, nevjegy, sizeof(Nevjegy)); // átmásolja az adatokat
        uj->elozo = adatbazis->utolso->elozo;
        uj->kov = adatbazis->utolso;
        adatbazis->utolso->elozo->kov = uj;
        adatbazis->utolso->elozo = uj;
        return true;
    }
    return false;
}

bool nevjegy_torles(Nevjegyzek const * const adatbazis, Nevjegy * const torlendo){
    torlendo->elozo->kov = torlendo->kov;
    torlendo->kov->elozo = torlendo->elozo;
    free(torlendo);
    return fajlba_adatbazis_mentes(adatbazis);
}

// visszaadja az id alapján keresett névjegyre mutató pointert.
Nevjegy* nevjegy_keres(Nevjegyzek const * const adatbazis, int const id){
    Nevjegy* mozgo = adatbazis->elso->kov;
    while(mozgo != adatbazis->utolso){
        if(mozgo->id == id) return mozgo;
        mozgo = mozgo->kov;
    }
    return NULL;
}

bool nevjegy_modosit(Nevjegyzek* const adatbazis, Nevjegy const * const nevjegy, Nevjegy* const eredeti){
    *eredeti = *nevjegy;
    return fajlba_adatbazis_mentes(adatbazis);
}

// ha valamelyik mezõ üresen lenne hagyva, akkor kitölti
void adat_fixalas(Nevjegy* const nevjegy){
    if(nevjegy->nev[0] == '\0') strcpy(nevjegy->nev, "n/a");
    if(nevjegy->foglalkozas[0] == '\0') strcpy(nevjegy->foglalkozas, "n/a");
    if(nevjegy->cim[0] == '\0') strcpy(nevjegy->cim, "n/a");
    if(nevjegy->telefonszam[0] == '\0') strcpy(nevjegy->telefonszam, "n/a");
    if(nevjegy->email[0] == '\0') strcpy(nevjegy->email, "n/a");
}

// visszaadja, hogy megfelel-e a paraméterként kapott keresõ stringnek a másik paraméterként kapott szöveg
bool wildcard_keres(char const * const kereso_string, char const * const szoveg){
    if(strlen(szoveg)<strlen(kereso_string)) return false; // ebben az esetben biztos, hogy nem felel meg
    char tmp[strlen(kereso_string)+1];
    if(kereso_string[0]=='*'){
        strcpy(tmp, kereso_string+1); // * nélkül másolja
        // a szöveg string utolsó tmp hosszúságú részét kell vizsgálni
        if(strcmp( szoveg+(strlen(szoveg)-strlen(tmp)), tmp) == 0) return true;
    }
    else if(kereso_string[strlen(kereso_string)-1] == '*'){
        strcpy(tmp, kereso_string);
        tmp[strlen(tmp)-1] = '\0'; // nem kell a végére a *
        if(strstr(szoveg, tmp)==szoveg) return true;
    } else {
        sscanf(kereso_string, "%[^*]", tmp);
        char* hely;
        if((hely = strstr(szoveg, tmp))==NULL) return false; // ha már az elsõ rész sincs benne akkor biztos nincs benne az egész
        hely = hely+strlen(tmp);
        // ha benne volt az elsõ rész, akkor megyünk tovább
        sscanf(kereso_string, "%*[^*]*%s", tmp);
        // a szöveg string utolsó tmp hosszúságú részét kell vizsgálni
        if(strcmp( szoveg+(strlen(szoveg)-strlen(tmp)), tmp) == 0) return true;
    }
    return false;
}
