/* =======================================================
 *   Feito por Eduardo Gobbo Willi Vasconcellos Goncalves
 *   GRR20203892
 *  ======================================================= */

#include "ppm.h"

int main(int argc, char const *argv[])
{
    struct tile_list Tiles;
    struct image input_img;
    struct image output_img;

    char input_src[NAME_MAX_SZ];
    char outut_src[NAME_MAX_SZ];
    char tiles_src[NAME_MAX_SZ];

    fprintf(stderr, GREEN "Esse programa foi compilado em %s\n" NORMAL , __DATE__);

    /* direciona entradas e saidas vindas por command line arguments */
    flux(argc, argv, tiles_src, input_src, outut_src);

    /* prepara lista para receber tiles */
    create_list(&Tiles);

    /* processa tiles e guarda em lista */
    load_tiles(tiles_src, &Tiles);

    /* prepara estrutura imagem de entrada e saida (salva na lista) */
    load_files(input_src, outut_src, &input_img, &output_img, &Tiles);

    /* constroi mosaico ( e libera memoria das imagens) */
    build_mosaic(&input_img, &output_img, &Tiles);
    
    /* libera memoria na lista */
    drop_tiles(&Tiles);

    fprintf(stderr, GREEN "programa finalizado\n" NORMAL); 

    return 0;
}
