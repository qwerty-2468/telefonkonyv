#ifndef ADATBAZIS_H_INCLUDED
#define ADATBAZIS_H_INCLUDED
#include <stdbool.h>

typedef struct Nevjegy{
    int id; // pozitív egész szám, mely kulcsként szerepel az adatbázisban
    char nev[30+1];
    char foglalkozas[20+1];
    char cim[30+1];
    char telefonszam[12+1];
    char email[20+1];
    struct Nevjegy *elozo, *kov;
} Nevjegy;

typedef struct Nevjegyzek{
    Nevjegy* elso;
    Nevjegy* utolso;
} Nevjegyzek;

bool adatb_letrehoz(Nevjegyzek* adatbazis);
bool adatb_inicializal(Nevjegyzek* adatbazis);
void adatb_felszabadit(Nevjegyzek* adatbazis);
void nevjegy_mentes(Nevjegy * const nevjegy, Nevjegyzek* adatbazis);
bool memoriaba_iras(Nevjegy const* const nevjegy, Nevjegyzek* adatbazis);
void nevjegy_id_general(Nevjegyzek const * const adatbazis, Nevjegy* const nevjegy);
bool nevjegy_torles(Nevjegyzek const * const adatbazis, Nevjegy * const torlendo);
Nevjegy* nevjegy_keres(Nevjegyzek const * const adatbazis, int const id);
bool nevjegy_modosit(Nevjegyzek* const adatbazis, Nevjegy const * const nevjegy, Nevjegy* const eredeti);
void adat_fixalas(Nevjegy* const nevjegy);
bool wildcard_keres(char const * const kereso_string, char const * const szoveg);

#endif // ADATBAZIS_H_INCLUDED
