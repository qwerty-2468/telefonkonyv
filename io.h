#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

void fomenu_kiiras(int* valasztas);
void adatbevitel(Nevjegy* nevjegy);
void nevjegy_kiir(Nevjegy const *const nevjegy);
void nevek_listaz(Nevjegyzek const *const adatbazis, const int kategoria, const char* keresett_string);
void kategoria_valasztas_menu(int* kategoria_kod);
void kereses_mezo(char* keresett_string);
bool mentie();
void varakozas_bemenetre(void);

#endif // IO_H_INCLUDED
