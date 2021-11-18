/* =======================================================
 *   Feito por Eduardo Gobbo Willi Vasconcellos Goncalves
 *   GRR20203892
 *  ======================================================= */

#include "utils.h"

/* constantes usadas no codigo */
#define DEFAULT_OUT "output_placeholder"
#define DEFAULT_INP "input_placeholder"
#define DEFAULT_DIR "tiles"
#define NAME_MAX_SZ 256


struct pixel{
    int r;
    int g;
    int b;
};

struct image{
    char *name;
    char *type;
    int width;
    int height;
    int max_value;
    struct pixel avg;
    struct pixel **map;
    struct image *prox;
    struct image *anter;
};

struct tile_list{
    int list_size;
    struct image *ini;
    struct image *end;
    FILE* inp_file;
    FILE* out_file;
};

/* headers das funcoes */

void load_files(char* in_f, char* out_f, struct image *in_img, struct image *out_img, struct tile_list *p);    
void build_mosaic( struct image *in_img, struct image *out_img, struct tile_list *tiles);
void place_tile(struct image *pict, struct image *tile, int comeco_w, int comeco_h);
void load_tiles(const char* source, struct tile_list *tiles);
void save_mosaic(FILE *out_file, struct image *out_img);
void create_tile(struct image *tile, char* name);
void read_image(FILE *file, struct  image *pict);


struct pixel avg_tile(struct image *pict, int comeco_h, int comeco_w, int height, int width);
struct image* search_tile(struct tile_list *list, struct pixel avg);

void flux(int num, char const *args[], char *dir, char *inp, char *out);
void insert_tile(struct tile_list *p, struct image *n);
float rgb_distance(struct pixel x, struct pixel y);
void create_list(struct tile_list *l);
void drop_tiles(struct tile_list *t);
void drop_image(struct image *pict);
