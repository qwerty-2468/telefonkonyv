#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "debugmalloc.h"
#include "adatbazis.h"
#include "fajlkezeles.h"
#include "io.h"

// mem�ria foglal�sa �s esetleges felt�lt�se adatokkal (amennyiben m�r van adatb�zis f�jl)
bool adatb_letrehoz(Nevjegyzek* adatbazis){

    // adatb�zis inicializ�l�sa (l�ncolt lista str�zs�inak l�trehoz�sa)
    if(!adatb_inicializal(adatbazis)){
        printf("Nem siker�lt inicializ�lni az adatb�zist.");
        return false;
    } else {

    //adatb�zis bet�lt�se f�jlb�l ha van m�r
    printf("Adatb�zis f�jl keres�se\n");
    if(van_adatb_fajl()){
        if(!adatb_beolvas(adatbazis)){
            printf("Nem siker�lt beolvasni");
            return false;

        }
        else{
            printf("Sikeres adatbeolvas�s");
            return true;
        }
    } else {
        printf("Nem tal�lhat� adatb�zis f�jl\n");
        if(!fajl_inicializal_adatb()){
            printf("Nem siker�lt l�trehozni a f�jlt.\n");
            return false;
        }
        printf("�j adatb�zis f�jl lett l�trehozva\n\n");
        return true;
    }
    }
}

// dupl�n l�ncolt lista str�zs�inak l�trehoz�sa �s egym�sra �ll�t�sa
bool adatb_inicializal(Nevjegyzek* adatbazis){
    adatbazis->elso = (Nevjegy*) malloc(sizeof(Nevjegy));
    if(adatbazis->elso==NULL) return false;
    adatbazis->utolso = (Nevjegy*) malloc(sizeof(Nevjegy));
    if(adatbazis->utolso==NULL) return false;
    adatbazis->elso->kov = adatbazis->utolso;
    adatbazis->utolso->elozo = adatbazis->elso;
    return true;
}

// felszabad�tja az adatb�zis �ltal foglalt teljes mem�riater�letet
void adatb_felszabadit(Nevjegyzek* adatbazis){
    Nevjegy* iter = adatbazis->elso;
    while(iter != adatbazis->utolso){
        Nevjegy* kov = iter->kov;
        free(iter);
        iter = kov;
    }
    free(adatbazis->utolso);
}

// egy nevjegy valtoz�b�l �tmenti az adatokat az adatb�zisba (mem�ria) �s az adatb�zis f�jlba is
void nevjegy_mentes(Nevjegy * const nevjegy, Nevjegyzek* adatbazis){
    // ID gener�l�sa (a jelenlegi legnagyobbn�l eggyel nagyobb)
    nevjegy_id_general(adatbazis, nevjegy);

    // n�vjegy mem�ri�ba ment�se
    if(!memoriaba_iras(nevjegy, adatbazis)) printf("Nem siker�lt menteni a mem�ri�ba");
    // n�vjegy f�jlba ment�se
    if(!fajlba_nevjegy_mentes(nevjegy)) printf("Nems siker�lt menteni az adatb�zis f�jlba");

    printf("Adatok sikeresen mentve");
    varakozas_bemenetre();
}

// a param�terk�nt kapott n�vjegynek gener�l egy id-t �s el is menti bele
// (el�g lenne a legutols� n�vjegy id-j�t megn�zni, de �gy k�s�bbi v�ltoztat�sokhoz is jobban tud alkalmazkodni a program)
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
        memcpy(uj, nevjegy, sizeof(Nevjegy)); // �tm�solja az adatokat
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

// visszaadja az id alapj�n keresett n�vjegyre mutat� pointert.
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

// ha valamelyik mez� �resen lenne hagyva, akkor kit�lti
void adat_fixalas(Nevjegy* const nevjegy){
    if(nevjegy->nev[0] == '\0') strcpy(nevjegy->nev, "n/a");
    if(nevjegy->foglalkozas[0] == '\0') strcpy(nevjegy->foglalkozas, "n/a");
    if(nevjegy->cim[0] == '\0') strcpy(nevjegy->cim, "n/a");
    if(nevjegy->telefonszam[0] == '\0') strcpy(nevjegy->telefonszam, "n/a");
    if(nevjegy->email[0] == '\0') strcpy(nevjegy->email, "n/a");
}

// visszaadja, hogy megfelel-e a param�terk�nt kapott keres� stringnek a m�sik param�terk�nt kapott sz�veg
bool wildcard_keres(char const * const kereso_string, char const * const szoveg){
    if(strlen(szoveg)<strlen(kereso_string)) return false; // ebben az esetben biztos, hogy nem felel meg
    char tmp[strlen(kereso_string)+1];
    if(kereso_string[0]=='*'){
        strcpy(tmp, kereso_string+1); // * n�lk�l m�solja
        // a sz�veg string utols� tmp hossz�s�g� r�sz�t kell vizsg�lni
        if(strcmp( szoveg+(strlen(szoveg)-strlen(tmp)), tmp) == 0) return true;
    }
    else if(kereso_string[strlen(kereso_string)-1] == '*'){
        strcpy(tmp, kereso_string);
        tmp[strlen(tmp)-1] = '\0'; // nem kell a v�g�re a *
        if(strstr(szoveg, tmp)==szoveg) return true;
    } else {
        sscanf(kereso_string, "%[^*]", tmp);
        char* hely;
        if((hely = strstr(szoveg, tmp))==NULL) return false; // ha m�r az els� r�sz sincs benne akkor biztos nincs benne az eg�sz
        hely = hely+strlen(tmp);
        // ha benne volt az els� r�sz, akkor megy�nk tov�bb
        sscanf(kereso_string, "%*[^*]*%s", tmp);
        // a sz�veg string utols� tmp hossz�s�g� r�sz�t kell vizsg�lni
        if(strcmp( szoveg+(strlen(szoveg)-strlen(tmp)), tmp) == 0) return true;
    }
    return false;
}
