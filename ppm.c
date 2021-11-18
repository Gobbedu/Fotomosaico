/* =======================================================
 *   Feito por Eduardo Gobbo Willi Vasconcellos Goncalves
 *   GRR20203892
 *  ======================================================= */

#include "ppm.h"

void load_files(char* input_src, char* output_src, struct image *in_img, struct image *out_img, struct tile_list *p) 
{

    /* direciona entrada da imagem (apaga comentarios na entrada) */
    if( strcmp(input_src, DEFAULT_INP) == 0)
        p->inp_file = stdin;
    else
        p->inp_file = fopen(input_src, "r+");
    if( p->inp_file == NULL )
        kill("could not open specified input file", 2);

    /* direciona saida da imagem */
    if( strcmp(output_src, DEFAULT_OUT) == 0)
        p->out_file = stdout;
    else
        p->out_file = fopen(output_src, "w");
    if( p->out_file == NULL )
        kill("could not open specified output file", 2);
    
    /* salva entrada/saida na  estrutura imagem */
    in_img->name = malloc(sizeof(char) * NAME_MAX_SZ);
    strcpy(in_img->name, input_src);

    out_img->name = malloc(sizeof(char) * NAME_MAX_SZ);
    strcpy(out_img->name, output_src);
 
    /* carrega imagem de entrada */
    fprintf(stderr, YELLOW "Reading Input Image...\n" NORMAL);
    read_image(p->inp_file, in_img);
    fprintf(stderr, "input image is %s %dx%d\n", in_img->type, in_img->width, in_img->height);

    /* dados do mosaico sao semelhantes ao da entrada */
    out_img->type = malloc(sizeof(char) * 3);
    strcpy(out_img->type, in_img->type);
    out_img->max_value = in_img->max_value;

    /* tamanho da imagem deve ser maior que uma pastilha */
    if( in_img->width < p->ini->width || in_img->height < p->ini->height)
        kill("a imagem de entrada tem tamanho insuficiente", 2);

}

void build_mosaic( struct image *in_img, struct image *out_img, struct tile_list *tiles)
{
    struct pixel aux;
    struct image *tile;
    int i, j, count = 0;
    

    /* crop nao coberto por tile no mosaico */
    int t_h = tiles->ini->height;
    int t_w = tiles->ini->width;
    int mos_h = in_img->height - (in_img->height  % t_w); 
    int mos_w = in_img->width  - (in_img->width   % t_h); 

    out_img->height = mos_h;
    out_img->width  = mos_w;

    /* alocar estrutura **map para mosaico */
    out_img->map = malloc(sizeof(struct pixel) * mos_h);
    for(i = 0; i < mos_h; i++)
        out_img->map[i] = malloc(sizeof(struct pixel) * mos_w);


    /* ===== constroi mosaico ===== */
    fprintf(stderr, YELLOW "building mosaic...\n" NORMAL);
    for(i = 0; i < mos_h; i+= t_h)
    {
        for(j = 0; j < mos_w; j+= t_w)
        {
            /* tira media de NxN da entrada */
            /* busca tile com menor diferenca entre a media */
            /* gruda tile no mosaico */
            count++;
            aux = avg_tile(in_img, i, j, t_h, t_w);
            tile = search_tile(tiles, aux);
            /* 
            average is ok 
            tile->avg = aux;
            */
            place_tile(out_img, tile, i, j);

            fprintf(stderr, "\rplaced [%5d/%5d] tiles" , count, mos_h/t_h * mos_w/t_w);
        }
    } fprintf(stderr, "\n");

    /* escreve mosaico no arquivo apontado */
    save_mosaic(tiles->out_file, out_img);
    
    fprintf(stderr, "Output image is %s %dx%d\n", out_img->type, out_img->width, out_img->height);

    drop_image(in_img);
    drop_image(out_img);
}

void save_mosaic(FILE *out_file, struct image *out_img)
{
    fprintf(stderr, YELLOW "saving mosaic...\n" NORMAL);

    unsigned char value;
    int i, j, test;

    /* comeca pelo inicio do arquivo */
    rewind(out_file);

    /* imprime dados no header */
    fprintf(out_file, "%s\n", out_img->type);
    fprintf(out_file, "%d %d\n", out_img->width, out_img->height);
    fprintf(out_file, "%d\n", out_img->max_value);

    /* imprime dados da imagem P3/P6 */
    if(strcmp(out_img->type, "P6") == 0)
    {
        for(i = 0; i < out_img->height; i++)
        {
            for(j = 0; j < out_img->width; j++)
            {

                    value = out_img->map[i][j].r;
                    test = fwrite(&value, sizeof(char), 1, out_file);
                    if( !test ) kill("nao foi possivel salvar dados na imagem de saida", 2);

                    value = out_img->map[i][j].g;
                    test = fwrite(&value, sizeof(char), 1, out_file);
                    if( !test ) kill("nao foi possivel salvar dados na imagem de saida", 2);

                    value = out_img->map[i][j].b;
                    test = fwrite(&value, sizeof(char), 1, out_file);
                    if( !test ) kill("nao foi possivel salvar dados na imagem de saida", 2);

            }
        }
    }
    else if( strcmp(out_img->type, "P3") == 0)
    {
        for(i = 0; i < out_img->height; i++)
        {
            for(j = 0; j < out_img->width; j++)
            {
                fprintf(out_file, "%d ", out_img->map[i][j].r);
                fprintf(out_file, "%d ", out_img->map[i][j].g);
                fprintf(out_file, "%d ", out_img->map[i][j].b);
            }
            fprintf(out_file, "\n");
        }
    }
        
}

void load_tiles(const char* directory, struct tile_list *tile_list)
{
    int count;
    struct dirent **dirList;
    char source[NAME_MAX_SZ * 2];
   
    fprintf(stderr, YELLOW"loading tiles from %s...\n"NORMAL, directory);    

    /* escaneia tiles em diretorio apontado */
    int n = scandir(directory, &dirList, NULL, alphasort);
    count = n;
    if( n == -1 )
        kill("could not create directory list", 2);


    /* salva todos os tiles encontrados no arquivo */
    while(n--)
    {
        if(dirList[n]->d_type == DT_REG)
        {
            /* aloca memoria para tile */
            /* preenche tile com dados da imagem */
            /* salva tile em lista */
            fprintf(stderr, "\rloaded [%4d/%4d] of possible tiles",count-n, count);
            
            struct image *tile = malloc(sizeof(struct image));
            tile->name = malloc(sizeof(char) * NAME_MAX_SZ);

            sprintf(source, "%s/%s", directory, dirList[n]->d_name);
            strcpy(tile->name, dirList[n]->d_name);

            create_tile(tile, source);    

            insert_tile(tile_list, tile);
        }
        /* libera memoria */
        free(dirList[n]);
    }   free(dirList);
    
    fprintf(stderr, "\n");
    fprintf(stderr, "tile size is %dx%d\n", tile_list->ini->width, tile_list->ini->height);
}

void insert_tile(struct tile_list *p, struct image *n){

    /* se lista vazia preence pointer */
    if( p->list_size == 0 ){
        p->ini = n;
        p->end = n;
    }
    /* senao insere no inicio */
    else
    {
       n->prox = p->ini;
       p->ini->anter = n;
       p->ini = n;
    }
    p->list_size++;
}

void create_list(struct tile_list *l)
{
    /* inicializa lista */
    l->list_size = 0;
    l->ini = NULL;
    l->end = NULL;
}

void create_tile(struct image *tile, char* source)
{
    FILE* file;
    /* remove comentarios do arquivo original */
    file = fopen(source, "r+");
 
    if( file == NULL )
        kill("could not open specified file", 2);

    /* salva dados do arquivo em tile */
    read_image(file, tile);

    /* calcula rgb medio do tile */
    tile->avg = avg_tile(tile, 0, 0, tile->height, tile->width);

    tile->anter = NULL;
    tile->prox = NULL;
    fclose(file);
}

struct image* search_tile(struct tile_list *tiles, struct pixel avg)
{
    int i;
    struct image *aux, *tile = NULL;
    float diff, min;
    min = 1000;

    aux = tiles->ini;

    /* percorre a lista inteira */
    /* salva e retorna tile com menor diferenca */
    for(i = 0; i < tiles->list_size ; i++)
    {
        diff = rgb_distance(avg, aux->avg);
        if( diff < min)
        {
            tile = aux;
            min = diff;
        }

        aux = aux->prox;
    }
    return tile;
}

void place_tile(struct image *pict, struct image *tile, int comeco_h, int comeco_w)
{
    int i, j;
    
    /* copia dados de tile em mosaico 
    */
    for(i = 0; i < tile->height; i++)
        for(j = 0; j < tile->width; j++)
            if(i + comeco_h < pict->height && j + comeco_w < pict->width)
            {
                pict->map[i + comeco_h][j + comeco_w] = tile->map[i][j];
            }

    /* dummy test, just blur everything 
    for(i = comeco_h; i < comeco_h + tile->height; i++)
        for(j = comeco_w; j < comeco_w + tile->width; j++)
            if(i < pict->height && j < pict->width)
                pict->map[i][j] = tile->avg;  
    */          
}

struct pixel avg_tile(struct image *pict, int comeco_h, int comeco_w, int height, int width)
{
    int i, j;
    long long red, green, blue, sum;
    red = 0;
    green = 0;
    blue = 0;
    sum = 0;

    /* calcula rgb medio (squared) de um block */
    for(i = comeco_h; i < comeco_h + height; i++){
        for(j = comeco_w; j < comeco_w + width; j++){
            if(i < pict->height && j < pict->width)
            {
                red   += (pict->map[i][j].r) * (pict->map[i][j].r);
                green += (pict->map[i][j].g) * (pict->map[i][j].g);
                blue  += (pict->map[i][j].b) * (pict->map[i][j].b);
                sum++;
            }
        }
    }

    struct pixel pix;
    pix.r = sqrt( red   / sum );
    pix.g = sqrt( green / sum );
    pix.b = sqrt( blue  / sum );
    
    return pix;
}

void read_image(FILE *file, struct  image *pict)
{
    int i, j;
    char* p6 = "P6";
    char* p3 = "P3";

    remove_comentario(file);
    rewind(file);

    
    /* le o header da imagem */
    pict->type = malloc(sizeof(char) * 3);
    fscanf(file, "%s ", pict->type);

    /* verifica o formato */
    if(strcmp(pict->type, p3) == 0 || strcmp(pict->type, p6) == 0)
        fscanf(file, "%d %d %d", &pict->width, &pict->height, &pict->max_value);
    else
        kill("file format not supported", 2);

    /* aloca espaco para receber dados */
    pict->map = malloc(pict->height * sizeof(struct pixel) );
    for(i = 0; i < pict->height; i++)
        pict->map[i] = malloc( sizeof(struct pixel) * pict->width );

    
    /* le dados da imagem P3 ou P6 */
    if( strcmp(pict->type, "P3") == 0 )
    {
        for(i = 0; i < pict->height; i++){
            for(j = 0; j < pict->width; j++){ 
                fscanf(file, "%d ", &pict->map[i][j].r);
                fscanf(file, "%d ", &pict->map[i][j].g);
                fscanf(file, "%d ", &pict->map[i][j].b);
            }
        }
    }
    else if( strcmp(pict->type, "P6") == 0)
    {
        unsigned char value;

        fread(&value, sizeof(char), 1, file);

        for(i = 0; i < pict->height; i++){
            for(j = 0; j < pict->width; j++){ 
                fread (&value, sizeof(char), 1, file);
                pict->map[i][j].r = value;
                
                fread (&value, sizeof(char), 1, file);
                pict->map[i][j].g = value;
                
                fread (&value, sizeof(char), 1, file);
                pict->map[i][j].b = value;
            }
        }
    }
}

float rgb_distance(struct pixel x, struct pixel y)
{
    int dist;
    /* diferenca entre dois pixeis normal 
    dist = sqrt( pow(y.r - x.r, 2) + pow(y.g - x.b, 2) + pow(y.b - x.b, 2) );
    */

    /* diferenca redmean */
    int mr, dr, dg, db;
    mr = (x.r + y.r) / 2;
    dr = x.r - y.r;
    dg = x.g - y.g;
    db = x.b - y.b;
    dist = sqrt(
        (2 + (mr)/256 ) * pow(dr, 2) + 4*pow(dg, 2) 
        + 
        (2 + (255-mr)/256 ) * pow(db, 2) 
    );
    return dist;
}

void drop_image(struct image *img)
{
    /* libera dados alocados para imagem */
    int i;
    for(i = 0; i < img->height; i++)
    {
        free(img->map[i]);
    } 
    
    free(img->name);
    free(img->type);
    free(img->map);
}

void drop_tiles(struct tile_list *p)
{
    struct image *aux;
    int count = 0;

    fprintf(stderr, YELLOW"dropping tile list..\n"NORMAL);

    if( p == NULL )
        kill("a lista de tiles nao existe e nao pode ser removida", 2);
    
    /* libera memoria na lista */
    if( p->list_size == 0 ) return;
    else{
        aux = p->ini;
        while( aux->prox != NULL ){
            aux = aux->prox;
            drop_image(aux->anter);
            free(aux->anter);
            
            count++;
            fprintf(stderr, "\rliberou [%4d/%4d] tiles da lista  ", count - p->list_size, count);
            p->list_size--;
        } 
        
        drop_image(aux);
        free(aux);    
        count++;
        p->ini = p->end = NULL;
        p->list_size--;
        fprintf(stderr, "\rliberou [%4d/%4d] tiles da lista  \n", count - p->list_size, count);
    }
    fclose(p->inp_file);
    fclose(p->out_file);
}

void flux(int num, char const *args[], char *dir, char *inp, char *out)
{
    int i;
    int params = 0;

    /* salva caminhos default */
    strcpy(inp, DEFAULT_INP);
    strcpy(out, DEFAULT_OUT);
    strcpy(dir, DEFAULT_DIR); 

    /* se numero de parametros incoerente */
    if( num % 2 == 0 && num != 1 )
    {
        if(num >= 2 && strcmp(args[1], "-h") != 0) 
            kill("numero invalido de argumentos", 1);
        help();
        exit(0);
    } 
    /* percorre argumentos e salva redirecionamentos */
    else
    {
        for( i = 1; i < num ; i += 2){
            if(strcmp(args[i], "-i") == 0 && i+1 < num ){
                strcpy(inp, args[i+1]);
                params++;
            }

            else if(strcmp(args[i], "-o") == 0 && i+1 < num){
                strcpy(out, args[i+1]);
                params++;
            }
            
            else if(strcmp(args[i], "-p") == 0 && i+1 < num){
                strcpy(dir, args[i+1]);
                params++;
            }
        }
        if( params != (num - 1) / 2)
            kill("wrong usage of parameters", 1);
    }
}
