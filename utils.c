/* =======================================================
 *   Feito por Eduardo Gobbo Willi Vasconcellos Goncalves
 *   GRR20203892
 *  ======================================================= */

#include "utils.h"

void kill(char* message, int sig){
    /* mensagens de erro para limpar o codigo */
    if(sig == 1)
    {
        fprintf(stderr, YELLOW "%s\n" NORMAL, message);
        help();
    }
    else
    {
        fprintf(stderr, RED "PPM: %s\n" NORMAL, message);
    }
    exit(sig);
}

void help(void)
{
    /* pequeno manual de como usar o programa */
    fprintf(stderr, "\nUsage: mosaico [ -p diretório ] [ -i arquivo ] [ -o arquivo ]\n");
    fprintf(stderr, "if [ opcao ] not provided program defaults to\n");
    fprintf(stderr, "        -i      input                   stdin\n");
    fprintf(stderr, "        -o      output                  stdout\n");
    fprintf(stderr, "        -p      tiles directory         ./tiles\n");
    fprintf(stderr, "        -h      displays this message and exit\n");
    fprintf(stderr, "\n\n");

    fprintf(stderr, "Exit status:\n");
    fprintf(stderr, GREEN  "   0  if OK,\n" NORMAL);
    fprintf(stderr, YELLOW "   1  if minor problems (e.g., wrong usage of parameters),\n" NORMAL);
    fprintf(stderr, "   2  if serious trouble (e.g., cannot access command-line argument).\n");
}

void remove_comentario(FILE *file)
{
    char spc = ' ', nl = '\n', c;
    int count = 0;

    rewind(file);

    while(count != 4)
    {
        c = fgetc(file);

        if(c == '#')
        {
            while(c != '\n'){
                /*  
                -volta um espaco 
                -substitui por espaco
                -avanca
                -le proximo
                */
                ungetc(spc, file);
                fputc(spc, file);
                c = fgetc(file);
            }
        }
        else if(c == spc || c == nl)
            count++;
    }
}

