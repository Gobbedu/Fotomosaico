/* =======================================================
 *   Feito por Eduardo Gobbo Willi Vasconcellos Goncalves
 *   GRR20203892
 *  ======================================================= */

/* usado para a função de ler diretorio */
#define _DEFAULT_SOURCE 

/* todas as bibliotecas necessarias */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <math.h>
#include <time.h>

/* cores do terminal */
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define NORMAL "\033[0m"

void remove_comentario(FILE *file);
void kill(char* message, int sig);
void help(void);


