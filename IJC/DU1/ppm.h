// error.h
   // Řešení IJC-DU1, příklad b), 22.3.2023
   // Autor: Jakub Hamadej, FIT
   // Přeloženo: gcc 9.4.0

struct ppm{
    unsigned xsize;
    unsigned ysize;
    char data[];
}

/**
 * load content of PPM file into dynamic allocated structure by this function
 * @return pointer to loaded structure, if fail return NULL
*/
struct ppm * ppm_read(const char * filename);

/**
 * free dynamicly allocated memory of structure ppm
*/
void ppm_free(struct ppm *p);
