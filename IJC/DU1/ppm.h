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
