#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


//---------ESTRUTURAS--------//
typedef struct Obstaculo{
	float x, y;
	float largura;
	float altura;
} Obstaculo;

typedef struct Tanque{
	float x, y;
	float vel;
	float angle;
	float mov_x;
	float mov_y;
	int shield;
	int shield_HP;
	int id_player;
	int id_tanque;
	int esq;
	int dir;
	int forward;
	int back;
	int tiro_cd_max;
	ALLEGRO_BITMAP *sprite;
} Tanque;

typedef struct Projetil{
	float x, y;
	float angle;
	int hit;
	int viajando;
	int id;
	int cooldown;
} Projetil;

//-----VARIÁVEIS ALLEGRO-----//
//Display
ALLEGRO_DISPLAY *display = NULL;

//Queue de eventos
ALLEGRO_EVENT_QUEUE *event_queue = NULL;

//Timer
ALLEGRO_TIMER *timer = NULL;

//Bitmaps
ALLEGRO_BITMAP *background_mapa1  = NULL;
ALLEGRO_BITMAP *background_mapa2  = NULL;
ALLEGRO_BITMAP *background_mapa3  = NULL;
ALLEGRO_BITMAP *obstaculos1       = NULL;
ALLEGRO_BITMAP *obstaculos2       = NULL;
ALLEGRO_BITMAP *obstaculos3       = NULL;
ALLEGRO_BITMAP *background_menu   = NULL;
ALLEGRO_BITMAP *sprite_escudo1    = NULL;
ALLEGRO_BITMAP *sprite_escudo2    = NULL;
ALLEGRO_BITMAP *sprite_escudohurt = NULL;
ALLEGRO_BITMAP *sprite_tiros      = NULL;
ALLEGRO_BITMAP *heart             = NULL;
ALLEGRO_BITMAP *energy            = NULL;
ALLEGRO_BITMAP *score_block1      = NULL;
ALLEGRO_BITMAP *score_block2      = NULL;
ALLEGRO_BITMAP *dark_overlay      = NULL;
ALLEGRO_BITMAP *pickscreen        = NULL;
ALLEGRO_BITMAP *pickscreen1       = NULL;
ALLEGRO_BITMAP *pickscreen2       = NULL;
ALLEGRO_BITMAP *pickscreen3       = NULL;
ALLEGRO_BITMAP *pausado           = NULL;
ALLEGRO_BITMAP *loadingscreen     = NULL;
ALLEGRO_BITMAP *musicscreen       = NULL;
ALLEGRO_BITMAP *musicscreen1      = NULL;
ALLEGRO_BITMAP *musicscreen2      = NULL;
ALLEGRO_BITMAP *musicscreen3      = NULL;

//Audio
ALLEGRO_SAMPLE *goldenvengeance = NULL;
ALLEGRO_SAMPLE *breadysteadygo 	= NULL;
ALLEGRO_SAMPLE *jawbreaker 		= NULL;
ALLEGRO_SAMPLE *spaceboyfriend  = NULL;
ALLEGRO_SAMPLE *ending			= NULL;
ALLEGRO_SAMPLE *lazer 			= NULL;
ALLEGRO_SAMPLE *click			= NULL;
ALLEGRO_SAMPLE *hit 			= NULL;
ALLEGRO_SAMPLE_INSTANCE *musica = NULL;

//Fonte(s)
ALLEGRO_FONT *fonte      = NULL;
ALLEGRO_FONT *fontemaior = NULL;
ALLEGRO_FONT *omorifont  = NULL;

//Mouse State
ALLEGRO_MOUSE_STATE mouseState;

//-----VARIÁVEIS GLOBAIS-----//
//Constantes
const float FPS = 60;
const float sprite_scale = 0.25;
const float angulo_mov = M_PI/120;
const float volume = 0.2;
const float vol_pause = 0.06;
const int SCREEN_W = 960;
const int SCREEN_H = 720;
const int SPRITE_W = 256;
const int SPRITE_H = 256;
const int animation_speed = 10;
const int projectile_speed = 12;

//Variáveis de controle
float shield_angle = 0;
float sen_x = 0;
float sen_y = 0;
float quina_sen_x = 0;
float quina_sen_y = 0;
float frame_time = 0;
int shield_cd1 = 0; int shield_cd2 = 0;
int numvitorias1 = 0;
int numvitorias2 = 0;
int render_trigger = 1;
int game_open = 1;
int id_mapa = 1;
int id_musica = 1;
int vencedor = 0;
int main_menu = 1;
int tank_selection1 = 0;
int tank_selection2 = 0;
int map_selection = 0;
int musicselection = 0;
int play_once = 0;
int play_once2 = 0;
int play_once4 = 0;
int play_once5 = 0;
bool finish = false;
bool paused = false;

//Objetos
Tanque player1;
Tanque player2;
Projetil tiro1;
Projetil tiro2;
Obstaculo bloco1;
Obstaculo bloco2;
Obstaculo bloco3;
Obstaculo bloco4;
Obstaculo bloco5;
Obstaculo bloco6;
Obstaculo bloco7;
Obstaculo bloco8;
Obstaculo bloco9;

FILE *historico = NULL;

//-------FUNÇÕES GERAIS------//
//Inicializações e etc
void initAllegro();
void initEventQueue();
void initTanks();
void loadMemory();
void loadObstacles();
void destroy();
void initMusicas();

//Renderizações
void desenhaTudo();
void desenhaCenario();
void desenhaTanques();
void desenhaObstaculos();
void desenhaTiro(Projetil *tiro);
void desenhaBarras();
void desenhaMenu(ALLEGRO_MOUSE_STATE mouse, int id);

//Seções interativas
void menu();
void jogo();
void telaFinal();
void events();
void ingameEvents();
void escolheTanque(Tanque *player);

//Gameplay
void moveTanque(Tanque *player);
void criaTiro(Projetil *tiro);
void atira();
void cooldowns();
void checaVida();

//Colisões
void colideTudo();
void colideTela(Tanque *player);
bool colisaoCirculos(float x1, float x2, float y1, float y2, int r1, int r2, int isplayer);
bool colisaoCirculoRetangulo(Obstaculo *bloco, float player_x, float player_y, int raio);
void colisaoLados(Obstaculo *bloco, Tanque *player, int raio);
bool colisaoQuinas(int quina_x, int quina_y, Tanque *player, int raio);
void colideObstaculos(Obstaculo *bloco, Tanque *player1, Tanque *player2);
void colideQuinas(int quina_x, int quina_y, Tanque *player, int raio);
void colidePlayers();
void colideTiro(Tanque *player, Projetil *tiro);


int main(){
	initAllegro();
	initEventQueue();
	loadMemory();

	menu();
	initTanks();

	jogo();
	
	destroy();
	return 0;
}


//-------CORPO DAS FUNÇÕES-------//
//Inicializações e etc
void initAllegro(){
	//------------------------------ROTINAS DE INICIALIZAÇÃO------------------------------//
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		exit(-1);
	}
	
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		exit(-1);
	}
	
	//installs e addons
	al_init_primitives_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_install_mouse();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_init_acodec_addon();
	
	al_set_window_title(display, "Project Conflict");
}

void initEventQueue(){
	//cria a fila de eventos
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	al_start_timer(timer);
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());
}

void initTanks(){
	player1.id_player = 1; tiro1.id = 1;
	player1.angle = 0;
	player1.x = (float)SPRITE_W*sprite_scale;
	player1.y = (float)SCREEN_H - (SPRITE_H*sprite_scale);
	player1.esq = 0; player1.dir = 0; player1.forward = 0; player1.back = 0;
	escolheTanque(&player1);
	player1.shield_HP = player1.shield;
	
	player2.id_player = 2; tiro2.id = 2;
	player2.angle = M_PI/2;
	player2.x = (float)SCREEN_W - (SPRITE_W*sprite_scale);
	player2.y = (float)SPRITE_H*sprite_scale;
	player2.esq = 0; player2.dir = 0; player2.forward = 0; player2.back = 0;
	escolheTanque(&player2);
	player2.shield_HP = player2.shield;
}

void loadMemory(){
	//Bitmaps
	background_mapa1 =   al_load_bitmap("Backgrounds\\conflict_map_1.png");
	background_mapa2 =   al_load_bitmap("Backgrounds\\conflict_map_2.png");
	background_mapa3 =   al_load_bitmap("Backgrounds\\conflict_map_3.png");
	background_menu =    al_load_bitmap("Assets\\tela_inicial.png");
	obstaculos1 =        al_load_bitmap("Backgrounds\\conflict_objects_1.png");
	obstaculos2 =        al_load_bitmap("Backgrounds\\conflict_objects_2.png");
	obstaculos3 =        al_load_bitmap("Backgrounds\\conflict_objects_3.png");
	sprite_escudo1 =     al_load_bitmap("Sprites\\Shield.png");
	sprite_escudo2 =     al_load_bitmap("Sprites\\Shield2.png");
	sprite_escudohurt =  al_load_bitmap("Sprites\\ShieldHurt.png");
	sprite_tiros =       al_load_bitmap("Sprites\\Plasma.png");
	heart =              al_load_bitmap("Assets\\heart_sprite.png");
	energy =             al_load_bitmap("Assets\\raio_sprite.png");
	score_block1 =       al_load_bitmap("Assets\\text_box1.png");
	score_block2 =       al_load_bitmap("Assets\\text_box2.png");
	dark_overlay =       al_load_bitmap("Assets\\dark_overlay.png");
	pickscreen =         al_load_bitmap("Assets\\selection_empty.png");
	pickscreen1 =        al_load_bitmap("Assets\\selection_1.png");
	pickscreen2 =        al_load_bitmap("Assets\\selection_2.png");
	pickscreen3 =        al_load_bitmap("Assets\\selection_3.png");
	pausado =            al_load_bitmap("Assets\\pausado.png");
	loadingscreen =      al_load_bitmap("Assets\\loadingscreen.png");
	musicscreen =        al_load_bitmap("Assets\\musicselectionempty.png");
	musicscreen1 =       al_load_bitmap("Assets\\musicselection1.png");
	musicscreen2 =       al_load_bitmap("Assets\\musicselection2.png");
	musicscreen3 =       al_load_bitmap("Assets\\musicselection3.png");

	fprintf(stderr, "bitmaps carregados\n");

	al_draw_bitmap(loadingscreen, 0, 0, 0);
	al_flip_display();

	//Musicas
	goldenvengeance = al_load_sample("Soundtrack\\GOLDENVENGEANCE.ogg");
	breadysteadygo  = al_load_sample("Soundtrack\\BREADY_STEADY_GO.ogg");
	jawbreaker      = al_load_sample("Soundtrack\\JAWBREAKER.ogg");
	spaceboyfriend  = al_load_sample("Soundtrack\\SPACE_BOYFRIENDS_TAPE.ogg");
	ending			= al_load_sample("Soundtrack\\HOW_TRAGIC.ogg");

	//Efeitos Sonoros
	lazer = al_load_sample("SFX\\lazer_gun.ogg");
	click = al_load_sample("SFX\\click.ogg");
	hit   = al_load_sample("SFX\\hit.ogg");

	al_reserve_samples(30);
	al_play_sample(spaceboyfriend, volume, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
	fprintf(stderr, "musicas carregadas\n");

	//Fontes
	fonte = al_load_ttf_font("Fonts\\mindustryfont.ttf",20,0);
	fontemaior = al_load_ttf_font("Fonts\\mindustryfont.ttf",40,0);
	omorifont = al_load_ttf_font("Fonts\\OMORI_GAME2.ttf", 64, 0);
	fprintf(stderr, "fontes carregadas\n");

	//Histórico
	historico = fopen("historico.dat", "r");
	fscanf(historico, "%i%i", &numvitorias1, &numvitorias2);
	fclose(historico);
}

void loadObstacles(){

	switch(id_mapa){
		case 1: //Mapa 1
		bloco1.x = 152; bloco1.y = 360; bloco1.largura =  64; bloco1.altura = 480;
		bloco2.x = 808; bloco2.y = 360; bloco2.largura =  64; bloco2.altura = 480;
		bloco3.x = 260; bloco3.y = 360; bloco3.largura = 152; bloco3.altura =  64;
		bloco4.x = 700; bloco4.y = 360; bloco4.largura = 152; bloco4.altura =  64;
		bloco5.x = 480; bloco5.y = 568; bloco5.largura = 368; bloco5.altura =  64;
		bloco6.x = 480; bloco6.y = 152; bloco6.largura = 368; bloco6.altura =  64;
		bloco7.x = 480; bloco7.y = 360; bloco7.largura =  64; bloco7.altura = 128;
		bloco8.x =-200; bloco8.y =-200; bloco8.largura =   0; bloco8.altura =   0;
		bloco9.x =-200; bloco9.y =-200; bloco9.largura =   0; bloco9.altura =   0;
		break;

		case 2: //Mapa 2
		bloco1.x = 152; bloco1.y = 256; bloco1.largura =  64; bloco1.altura = 272;
		bloco2.x = 808; bloco2.y = 464; bloco2.largura =  64; bloco2.altura = 272;
		bloco3.x = 260; bloco3.y = 360; bloco3.largura = 152; bloco3.altura =  64;
		bloco4.x = 700; bloco4.y = 360; bloco4.largura = 152; bloco4.altura =  64;
		bloco5.x = 480; bloco5.y = 568; bloco5.largura = 368; bloco5.altura =  64;
		bloco6.x = 480; bloco6.y = 152; bloco6.largura = 368; bloco6.altura =  64;
		bloco7.x = 480; bloco7.y = 360; bloco7.largura =  64; bloco7.altura = 128;
		bloco8.x =  92; bloco8.y = 568; bloco8.largura = 184; bloco8.altura =  64;
		bloco9.x = 868; bloco9.y = 152; bloco9.largura = 184; bloco9.altura =  64;
		break;

		case 3: //Mapa 3
		bloco1.x =  92; bloco1.y = 152; bloco1.largura = 184; bloco1.altura =  64;
		bloco2.x = 868; bloco2.y = 568; bloco2.largura = 184; bloco2.altura =  64;
		bloco3.x = 328; bloco3.y =  92; bloco3.largura =  64; bloco3.altura = 184;
		bloco4.x = 632; bloco4.y = 628; bloco4.largura =  64; bloco4.altura = 184;
		bloco5.x = 152; bloco5.y = 464; bloco5.largura =  64; bloco5.altura = 272;
		bloco6.x = 808; bloco6.y = 256; bloco6.largura =  64; bloco6.altura = 272;
		bloco7.x = 260; bloco7.y = 568; bloco7.largura = 152; bloco7.altura =  64;
		bloco8.x = 700; bloco8.y = 152; bloco8.largura = 152; bloco8.altura =  64;
		bloco9.x = 480; bloco9.y = 360; bloco9.largura = 368; bloco9.altura =  64;
		break;
	}
}

void destroy(){
	al_destroy_bitmap(background_mapa1);
	al_destroy_bitmap(background_mapa2);
	al_destroy_bitmap(background_mapa3);
	al_destroy_bitmap(obstaculos1);
	al_destroy_bitmap(obstaculos2);
	al_destroy_bitmap(obstaculos3);
	al_destroy_bitmap(background_menu);
	al_destroy_bitmap(sprite_escudo1);
	al_destroy_bitmap(sprite_escudo2);
	al_destroy_bitmap(sprite_escudohurt);
	al_destroy_bitmap(sprite_tiros);
	al_destroy_bitmap(player1.sprite);
	al_destroy_bitmap(player2.sprite);
	al_destroy_bitmap(heart);
	al_destroy_bitmap(energy);
	al_destroy_bitmap(score_block1);
	al_destroy_bitmap(score_block2);
	al_destroy_bitmap(dark_overlay);
	al_destroy_bitmap(pickscreen);
	al_destroy_bitmap(pickscreen1);
	al_destroy_bitmap(pickscreen2);
	al_destroy_bitmap(pickscreen3);
	al_destroy_bitmap(pausado);
	al_destroy_bitmap(loadingscreen);
	al_destroy_bitmap(musicscreen);
	al_destroy_bitmap(musicscreen1);
	al_destroy_bitmap(musicscreen2);
	al_destroy_bitmap(musicscreen3);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_font(fonte);
	al_destroy_font(fontemaior);
	al_destroy_font(omorifont);
	al_destroy_sample(goldenvengeance);
	al_destroy_sample(breadysteadygo);
	al_destroy_sample(jawbreaker);
	al_destroy_sample(spaceboyfriend);
	al_destroy_sample(ending);
}

void initMusicas(){
	switch(id_musica){
		case 1:
			musica = al_create_sample_instance(jawbreaker);
			al_attach_sample_instance_to_mixer(musica, al_get_default_mixer());
			al_set_sample_instance_gain(musica, volume*2);
			al_set_sample_instance_playing(musica, true);
			break;

		case 2:
			musica = al_create_sample_instance(goldenvengeance);
			al_attach_sample_instance_to_mixer(musica, al_get_default_mixer());
			al_set_sample_instance_gain(musica, volume*2);
			al_set_sample_instance_playing(musica, true);
			break;

		case 3:
			musica = al_create_sample_instance(breadysteadygo);
			al_attach_sample_instance_to_mixer(musica, al_get_default_mixer());
			al_set_sample_instance_gain(musica, volume*2);
			al_set_sample_instance_playing(musica, true);
			break;
	}
}

//Renderizações
void desenhaTudo(){
	desenhaCenario();
	atira();
	desenhaObstaculos();
	desenhaTanques();
	desenhaBarras();
	render_trigger = 0;
}

void desenhaCenario(){
	al_clear_to_color(al_map_rgb(0, 0, 0));
	
	switch(id_mapa){
		case 1: //mapa1
			al_draw_bitmap(background_mapa1, 0, 0, 0);
			break;
		case 2: //mapa2
			al_draw_bitmap(background_mapa2, 0, 0, 0);
			break;
		case 3: //mapa3
			al_draw_bitmap(background_mapa3, 0, 0, 0);
			break;
		default:
			al_clear_to_color(al_map_rgb(255, 0, 255));
	}
}

void desenhaTanques(){
	//player 1
	al_draw_scaled_rotated_bitmap(player1.sprite, SPRITE_W/2, SPRITE_H/2, player1.x, player1.y, sprite_scale, sprite_scale, player1.angle, 0);

	//player 1 shield shenanigangs
	if(!tiro2.hit && !shield_cd1){ al_draw_scaled_rotated_bitmap(sprite_escudo1, 144, 144, player1.x, player1.y, 1/3.6, 1/3.6, shield_angle, 0); if(!play_once4){ play_once4 = 1; } } 
	else if(!shield_cd1) { shield_cd1 = 10; }
	if(shield_cd1){ al_draw_scaled_rotated_bitmap(sprite_escudohurt, 144, 144, player1.x, player1.y, 1/3.6, 1/3.6, shield_angle, 0); 
													  if(play_once4){ al_play_sample(hit, volume, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, NULL); play_once4 = 0; }}

	//player 2
	al_draw_scaled_rotated_bitmap(player2.sprite, SPRITE_W/2, SPRITE_H/2, player2.x, player2.y, sprite_scale, sprite_scale, player2.angle, 0);

	//player 2 shield shenanigangs
	if(!tiro1.hit && !shield_cd2){ al_draw_scaled_rotated_bitmap(sprite_escudo2, 144, 144, player2.x, player2.y, 1/3.6, 1/3.6, -shield_angle, 0); if(!play_once5){ play_once5 = 1; } } 
	else if(!shield_cd2) { shield_cd2 = 10; }
	if(shield_cd2){ al_draw_scaled_rotated_bitmap(sprite_escudohurt, 144, 144, player2.x, player2.y, 1/3.6, 1/3.6, -shield_angle, 0); 
													  if(play_once5){ al_play_sample(hit, volume, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, NULL); play_once5 = 0; }}
}

void desenhaObstaculos(){
	switch(id_mapa){
		case 1: //mapa1
			al_draw_bitmap(obstaculos1, 0, 0, 0);
			break;
		case 2: //mapa2
			al_draw_bitmap(obstaculos2, 0, 0, 0);
			break;
		case 3: //mapa3
			al_draw_bitmap(obstaculos3, 0, 0, 0);
			break;
	}
}

void desenhaTiro(Projetil *tiro){
	al_draw_scaled_rotated_bitmap(sprite_tiros, 64, 64, tiro->x, tiro->y, sprite_scale, sprite_scale, tiro->angle + M_PI/2, 0);
	if(tiro->x > SCREEN_W || tiro->x < 0 || tiro->y > SCREEN_H || tiro->y < 0){ tiro->viajando = 0; }
	if(colisaoCirculoRetangulo(&bloco1, tiro->x, tiro->y, 1)
	|| colisaoCirculoRetangulo(&bloco2, tiro->x, tiro->y, 1)
	|| colisaoCirculoRetangulo(&bloco3, tiro->x, tiro->y, 1)
	|| colisaoCirculoRetangulo(&bloco4, tiro->x, tiro->y, 1)
	|| colisaoCirculoRetangulo(&bloco5, tiro->x, tiro->y, 1)
	|| colisaoCirculoRetangulo(&bloco6, tiro->x, tiro->y, 1)
	|| colisaoCirculoRetangulo(&bloco7, tiro->x, tiro->y, 1)
	|| colisaoCirculoRetangulo(&bloco8, tiro->x, tiro->y, 1)
	|| colisaoCirculoRetangulo(&bloco9, tiro->x, tiro->y, 1)){ tiro->viajando = 0; }
}

void desenhaBarras(){
	float shield_percentage1 = (60 * player1.shield_HP)/player1.shield;
	float shield_percentage2 = (60 * player2.shield_HP)/player2.shield;

	float cooldown_percentage1 = (60 * tiro1.cooldown)/player1.tiro_cd_max;
	float cooldown_percentage2 = (60 * tiro2.cooldown)/player2.tiro_cd_max;

	al_draw_filled_rectangle(player1.x - 30, player1.y + 45, player1.x + 30 - cooldown_percentage1, player1.y + 55, al_map_rgb(0, 128, 255));
	al_draw_filled_rectangle(player2.x - 30, player2.y + 45, player2.x + 30 - cooldown_percentage2, player2.y + 55, al_map_rgb(128, 255, 0));

	al_draw_bitmap(energy, player1.x - 45, player1.y + 35, 0);
	al_draw_bitmap(energy, player2.x - 45, player2.y + 35, 0);

	if(shield_percentage1 >= 0)	{ 
		al_draw_filled_rectangle(player1.x - 30, player1.y - 45, player1.x - 30 + shield_percentage1, player1.y - 55, al_map_rgb(240, 20, 20));
	}
	if(shield_percentage2 >= 0)	{ 
		al_draw_filled_rectangle(player2.x - 30, player2.y - 45, player2.x - 30 + shield_percentage2, player2.y - 55, al_map_rgb(240, 20, 20));
	}

	al_draw_scaled_bitmap(heart, 0, 0, 30, 30, player1.x - 42, player1.y - 62, 24, 24, 0);
	al_draw_scaled_bitmap(heart, 0, 0, 30, 30, player2.x - 42, player2.y - 62, 24, 24, 0);

	ALLEGRO_COLOR cor = al_map_rgb(0, 0, 0);

	switch(player1.shield_HP){
		default: al_draw_text(fonte, cor, player1.x - 39, player1.y - 60, 0, "0"); break;
		case 1: al_draw_text(fonte, cor, player1.x - 35, player1.y - 60, 0, "1"); break;
		case 2: al_draw_text(fonte, cor, player1.x - 37, player1.y - 60, 0, "2"); break;
		case 3: al_draw_text(fonte, cor, player1.x - 37, player1.y - 60, 0, "3"); break;
		case 4: al_draw_text(fonte, cor, player1.x - 37, player1.y - 60, 0, "4"); break;
		case 5: al_draw_text(fonte, cor, player1.x - 37, player1.y - 60, 0, "5"); break;
		case 6: al_draw_text(fonte, cor, player1.x - 37, player1.y - 60, 0, "6"); break;
		case 7: al_draw_text(fonte, cor, player1.x - 37, player1.y - 60, 0, "7"); break;
		case 8: al_draw_text(fonte, cor, player1.x - 37, player1.y - 60, 0, "8"); break;
		case 9: al_draw_text(fonte, cor, player1.x - 37, player1.y - 60, 0, "9"); break;
		case 10: al_draw_text(fonte, cor, player1.x - 42, player1.y - 60, 0, "10"); break;
	}
	switch(player2.shield_HP){
		default: al_draw_text(fonte, cor, player2.x - 39, player2.y - 60, 0, "0"); break;
		case 1: al_draw_text(fonte, cor, player2.x - 35, player2.y - 60, 0, "1"); break;
		case 2: al_draw_text(fonte, cor, player2.x - 37, player2.y - 60, 0, "2"); break;
		case 3: al_draw_text(fonte, cor, player2.x - 37, player2.y - 60, 0, "3"); break;
		case 4: al_draw_text(fonte, cor, player2.x - 37, player2.y - 60, 0, "4"); break;
		case 5: al_draw_text(fonte, cor, player2.x - 37, player2.y - 60, 0, "5"); break;
		case 6: al_draw_text(fonte, cor, player2.x - 37, player2.y - 60, 0, "6"); break;
		case 7: al_draw_text(fonte, cor, player2.x - 37, player2.y - 60, 0, "7"); break;
		case 8: al_draw_text(fonte, cor, player2.x - 37, player2.y - 60, 0, "8"); break;
		case 9: al_draw_text(fonte, cor, player2.x - 37, player2.y - 60, 0, "9"); break;
		case 10: al_draw_text(fonte, cor, player2.x - 42, player2.y - 60, 0, "10"); break;
	}
}

void desenhaMenu(ALLEGRO_MOUSE_STATE mouse, int id){
	if((mouseState.x >  18 &&  mouseState.x < 322 && mouseState.y > 187 && mouseState.y < 489)){ al_draw_bitmap(pickscreen1, 0, 0, 0); } else
	if((mouseState.x > 329 &&  mouseState.x < 631 && mouseState.y > 187 && mouseState.y < 489)){ al_draw_bitmap(pickscreen2, 0, 0, 0); } else
	if((mouseState.x > 638 &&  mouseState.x < 940 && mouseState.y > 187 && mouseState.y < 489)){ al_draw_bitmap(pickscreen3, 0, 0, 0); } else
	{
		al_draw_bitmap(pickscreen, 0, 0, 0);
	}
	if(id == 1 || id == 2){ al_draw_textf(omorifont, al_map_rgb(246, 196, 31), 10, -10, 0, "PLAYER %i", id); }
	else if(id == 3){
		if((mouseState.x >  18 &&  mouseState.x < 322 && mouseState.y > 336 && mouseState.y < 639)){ al_draw_bitmap(pickscreen1, 0, 0, 0); } else
		if((mouseState.x > 329 &&  mouseState.x < 631 && mouseState.y > 336 && mouseState.y < 639)){ al_draw_bitmap(pickscreen2, 0, 0, 0); } else
		if((mouseState.x > 638 &&  mouseState.x < 940 && mouseState.y > 336 && mouseState.y < 639)){ al_draw_bitmap(pickscreen3, 0, 0, 0); } else
		{
			al_draw_bitmap(pickscreen, 0, 0, 0);
		}
	}
	else if(id == 4){
		if((mouseState.x >  65 &&  mouseState.x < 590 && mouseState.y > 285 && mouseState.y < 324)){ if(play_once){ al_play_sample(jawbreaker, volume, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_LOOP, NULL); play_once = 0; }
																									 al_draw_bitmap(musicscreen1, 0, 0, 0); } else
		if((mouseState.x > 365 &&  mouseState.x < 890 && mouseState.y > 455 && mouseState.y < 493)){ if(play_once){ al_play_sample(goldenvengeance, volume, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_LOOP, NULL); play_once = 0; }
																									 al_draw_bitmap(musicscreen2, 0, 0, 0); } else
		if((mouseState.x >  65 &&  mouseState.x < 590 && mouseState.y > 617 && mouseState.y < 655)){ if(play_once){ al_play_sample(breadysteadygo, volume, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_LOOP, NULL); play_once = 0; }
																									 al_draw_bitmap(musicscreen3, 0, 0, 0); } else
		{
			al_stop_samples();
			al_draw_bitmap(musicscreen, 0, 0, 0);
			play_once = 1;
		}
		al_draw_textf(omorifont, al_map_rgb(246, 196, 31), 10, -10, 0, "ATUAL: %i", id_musica);
	}
}

//Seções interativas
void menu(){
	for(;main_menu;){
		if(!game_open){ break; }
		al_draw_bitmap(background_menu, 0, 0, 0);

		if(musicselection){
			al_get_mouse_state (&mouseState);
			desenhaMenu(mouseState, 4);
			if((mouseState.x >  65 &&  mouseState.x < 590 && mouseState.y > 285 && mouseState.y < 324) && (mouseState.buttons & 1)){ 
				id_musica = 1; 
				if(play_once2){ 
					al_play_sample(click, 0.5, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, NULL); 
					play_once2 = 0;
				}
			}
			if((mouseState.x > 365 &&  mouseState.x < 890 && mouseState.y > 455 && mouseState.y < 493) && (mouseState.buttons & 1)){ 
				id_musica = 2; 
				if(play_once2){ 
					al_play_sample(click, 0.5, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, NULL); 
					play_once2 = 0;
				}
			}
			if((mouseState.x >  65 &&  mouseState.x < 590 && mouseState.y > 617 && mouseState.y < 655) && (mouseState.buttons & 1)){ 
				id_musica = 3; 
				if(play_once2){ 
					al_play_sample(click, 0.5, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, NULL); 
					play_once2 = 0;
				}
			}
		}
		events();
		al_flip_display();
	}

	for(tank_selection1 = 1; tank_selection1;){
		if(!game_open){ break; }
		events();
		al_get_mouse_state (&mouseState);
		desenhaMenu(mouseState, 1);
		if((mouseState.x >  18 &&  mouseState.x < 322 && mouseState.y > 187 && mouseState.y < 489) && (mouseState.buttons & 1)){ player1.id_tanque = 3; }
		if((mouseState.x > 329 &&  mouseState.x < 631 && mouseState.y > 187 && mouseState.y < 489) && (mouseState.buttons & 1)){ player1.id_tanque = 1; }
		if((mouseState.x > 638 &&  mouseState.x < 940 && mouseState.y > 187 && mouseState.y < 489) && (mouseState.buttons & 1)){ player1.id_tanque = 2; }
		al_flip_display();
	}

	for(tank_selection2 = 1; tank_selection2;){
		if(!game_open){ break; }
		events();
		al_get_mouse_state (&mouseState);
		desenhaMenu(mouseState, 2);
		if((mouseState.x >  18 &&  mouseState.x < 322 && mouseState.y > 187 && mouseState.y < 489) && (mouseState.buttons & 1)){ player2.id_tanque = 3; }
		if((mouseState.x > 329 &&  mouseState.x < 631 && mouseState.y > 187 && mouseState.y < 489) && (mouseState.buttons & 1)){ player2.id_tanque = 1; }
		if((mouseState.x > 638 &&  mouseState.x < 940 && mouseState.y > 187 && mouseState.y < 489) && (mouseState.buttons & 1)){ player2.id_tanque = 2; }
		al_flip_display();
	}

	if(!main_menu && !tank_selection1 && !tank_selection2){
		pickscreen  = al_load_bitmap("Assets\\map_selection_empty.png");
		pickscreen1 = al_load_bitmap("Assets\\map_selection_1.png");
		pickscreen2 = al_load_bitmap("Assets\\map_selection_2.png");
		pickscreen3 = al_load_bitmap("Assets\\map_selection_3.png");
	}

	for(map_selection = 1; map_selection;){
		if(!game_open){ break; }
		events();
		al_get_mouse_state (&mouseState);
		desenhaMenu(mouseState, 3);
		if((mouseState.x >  18 &&  mouseState.x < 322 && mouseState.y > 336 && mouseState.y < 639) && (mouseState.buttons & 1)){ id_mapa = 1; }
		if((mouseState.x > 329 &&  mouseState.x < 631 && mouseState.y > 336 && mouseState.y < 639) && (mouseState.buttons & 1)){ id_mapa = 2; }
		if((mouseState.x > 638 &&  mouseState.x < 940 && mouseState.y > 336 && mouseState.y < 639) && (mouseState.buttons & 1)){ id_mapa = 3; }
		al_flip_display();
	}
}

void jogo(){
	al_rest(0.5);
	loadObstacles();
	al_stop_samples();
	initMusicas();
	int play_once3 = 1;

	for(;game_open;){
		events();
		if(render_trigger && !paused && !finish){
			desenhaTudo();
			al_flip_display();
			checaVida();
		}
		if(paused){
			desenhaTudo();
			al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba_f(0, 0, 0, 0.4));
			al_draw_bitmap(pausado, 0, 0, 0);
			al_flip_display();
		}
		if(finish){
			if(play_once3){ al_rest(0.5); al_stop_sample_instance(musica); al_play_sample(ending, volume, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_LOOP, NULL); play_once3 = 0; }
			telaFinal();
		}
	}
	historico = fopen("historico.dat", "w");
	if(vencedor == 1){ numvitorias1++; }
	if(vencedor == 2){ numvitorias2++; }
	fprintf(historico, "%i %i", numvitorias1, numvitorias2);
	fclose(historico);
}

void telaFinal(){
	desenhaTudo();
	al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba_f(0, 0, 0, 0.4));
	if(vencedor == 1){ al_draw_bitmap(score_block1, 190, 70, 0); al_draw_textf(fontemaior, al_map_rgb(0, 0, 0), 610, 517, ALLEGRO_ALIGN_CENTRE, "%i" , numvitorias1 + 1); }
	if(vencedor == 2){ al_draw_bitmap(score_block2, 190, 70, 0); al_draw_textf(fontemaior, al_map_rgb(0, 0, 0), 610, 517, ALLEGRO_ALIGN_CENTRE, "%i" , numvitorias2 + 1); }
	al_flip_display();
}

void events(){
	ALLEGRO_EVENT evento;

	al_wait_for_event(event_queue, &evento);

	if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
		game_open = 0;
	}
	
	if(!paused && !finish && !main_menu){
		ingameEvents(evento);
	}

	if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
		if(tank_selection1){ tank_selection1 = 0; al_play_sample(click, 0.5, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, NULL); }
		else if(tank_selection2){ tank_selection2 = 0; al_play_sample(click, 0.5, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, NULL); }
		else if(map_selection){ map_selection = 0; al_play_sample(click, 0.5, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, NULL); }
	}

	if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
		play_once2 = 1;
	}

	if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
		switch(evento.keyboard.keycode){
			case ALLEGRO_KEY_ESCAPE:
				if(!finish && !main_menu && !tank_selection1 && !tank_selection2 && !map_selection){
					if(paused){ paused = 0; al_set_sample_instance_gain(musica, volume*4); }
					else{ paused = 1; al_set_sample_instance_gain(musica, volume*2); }
				}
				if(!finish && main_menu && !tank_selection1 && !tank_selection2 && !map_selection){
					if(musicselection){ 
						al_stop_samples(); 
						al_play_sample(spaceboyfriend, volume, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_LOOP, NULL); 
						musicselection = 0; 
					}else{ 
						al_stop_samples(); musicselection = 1; 
					}
				}
				break;
			case ALLEGRO_KEY_ENTER:
				if(finish){ game_open = 0; }
				if(main_menu && !musicselection){ main_menu = 0; }
				break;
		}
	}
}

void ingameEvents(ALLEGRO_EVENT evento){
	
	float time_for_update = 1/animation_speed;
	
	if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
		switch(evento.keyboard.keycode){
			case ALLEGRO_KEY_A:
				player1.esq = 1;
				break;
			case ALLEGRO_KEY_D:
				player1.dir = 1;
				break;
			case ALLEGRO_KEY_W:
				player1.forward = 1;
				break;
			case ALLEGRO_KEY_S:
				player1.back = 1;
				break;
			case ALLEGRO_KEY_LEFT:
				player2.esq = 1;
				break;
			case ALLEGRO_KEY_RIGHT:
				player2.dir = 1;
				break;
			case ALLEGRO_KEY_UP:
				player2.forward = 1;
				break;
			case ALLEGRO_KEY_DOWN:
				player2.back = 1;
				break;
			case ALLEGRO_KEY_SPACE:
				if(!tiro1.viajando && !tiro1.cooldown){ criaTiro(&tiro1); tiro1.cooldown = player1.tiro_cd_max; al_play_sample(lazer, 0.3, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, NULL); }
				break;
			case ALLEGRO_KEY_ENTER:
				if(!tiro2.viajando && !tiro2.cooldown){ criaTiro(&tiro2); tiro2.cooldown = player2.tiro_cd_max; al_play_sample(lazer, 0.3, ALLEGRO_AUDIO_PAN_NONE, 1, ALLEGRO_PLAYMODE_ONCE, NULL); }
				break;
		}
	}
	
	if(evento.type == ALLEGRO_EVENT_KEY_UP){
		switch(evento.keyboard.keycode){
			case ALLEGRO_KEY_A:
				player1.esq = 0;
				break;
			case ALLEGRO_KEY_D:
				player1.dir = 0;
				break;
			case ALLEGRO_KEY_W:
				player1.forward = 0;
				break;
			case ALLEGRO_KEY_S:
				player1.back = 0;
				break;
			case ALLEGRO_KEY_LEFT:
				player2.esq = 0;
				break;
			case ALLEGRO_KEY_RIGHT:
				player2.dir = 0;
				break;
			case ALLEGRO_KEY_UP:
				player2.forward = 0;
				break;
			case ALLEGRO_KEY_DOWN:
				player2.back = 0;
				break;
		}
	}
	
	if (evento.type == ALLEGRO_EVENT_TIMER){
		moveTanque(&player1);
		moveTanque(&player2);
		colideTudo();
		cooldowns();

		if(frame_time > time_for_update){
			render_trigger = 1;
			frame_time = 0;
		}
		frame_time += 0.01;
		shield_angle += angulo_mov/3;
	}
}

void escolheTanque(Tanque *player){
	if(player->id_tanque == 1){
		player->vel = 1.5;
		player->shield = 5;
		player->tiro_cd_max = 60;
		switch(player->id_player){
			case 1:
				player->sprite = al_load_bitmap("Sprites\\Tanques\\Color1\\Tank1_001.png");
				break;
			case 2:
				player->sprite = al_load_bitmap("Sprites\\Tanques\\Color2\\Tank1_001.png");
				break;
			case 3:
				player->sprite = al_load_bitmap("Sprites\\Tanques\\Color3\\Tank1_001.png");
				break;
		}
	}
	
	if(player->id_tanque == 2){
		player->vel = 1.2;
		player->shield = 8;
		player->tiro_cd_max = 80;
		switch(player->id_player){
			case 1:
				player->sprite = al_load_bitmap("Sprites\\Tanques\\Color1\\Tank2_001.png");
				break;
			case 2:
				player->sprite = al_load_bitmap("Sprites\\Tanques\\Color2\\Tank2_001.png");
				break;
			case 3:
				player->sprite = al_load_bitmap("Sprites\\Tanques\\Color3\\Tank2_001.png");
				break;
		}
	}
	
	if(player->id_tanque == 3){
		player->vel = 1.8;
		player->shield = 3;
		player->tiro_cd_max = 30;
		switch(player->id_player){
			case 1:
				player->sprite = al_load_bitmap("Sprites\\Tanques\\Color1\\Tank3_001.png");
				break;
			case 2:
				player->sprite = al_load_bitmap("Sprites\\Tanques\\Color2\\Tank3_001.png");
				break;
			case 3:
				player->sprite = al_load_bitmap("Sprites\\Tanques\\Color3\\Tank3_001.png");
				break;
		}
	}
}

//Gameplay
void moveTanque(Tanque *player){
	if(player->dir){
		player->angle += angulo_mov;
	}
	if(player->esq){
		player->angle -= angulo_mov;
	}
	if(player->forward){
		player->mov_x = (player->vel*sin(player->angle + M_PI/2)); 
		player->mov_y = -(player->vel*cos(player->angle + M_PI/2));
		player->x += player->mov_x;
		player->y += player->mov_y; 
	}
	if(player->back){
		player->mov_x = -(player->vel*sin(player->angle + M_PI/2)); 
		player->mov_y = (player->vel*cos(player->angle + M_PI/2));
		player->x += player->mov_x;
		player->y += player->mov_y; 
	}
}

void criaTiro(Projetil *tiro){
	tiro->viajando = 1;
	switch(tiro->id){
		case 1:
			tiro->x = player1.x;
			tiro->y = player1.y;
			tiro->angle = player1.angle;
			break;
		case 2:
			tiro->x = player2.x;
			tiro->y = player2.y;
			tiro->angle = player2.angle;
			break;
		}
}

void atira(){
	if(tiro1.viajando && !tiro1.hit){
		if(!paused && !finish){
			tiro1.x += projectile_speed*sin(tiro1.angle + M_PI/2);
			tiro1.y -= projectile_speed*cos(tiro1.angle + M_PI/2);
		}
		desenhaTiro(&tiro1);
	} else { criaTiro(&tiro1); tiro1.viajando = 0; }
	if(tiro2.viajando && !tiro2.hit){
		if(!paused && !finish){
			tiro2.x += projectile_speed*sin(tiro2.angle + M_PI/2);
			tiro2.y -= projectile_speed*cos(tiro2.angle + M_PI/2);
		}
		desenhaTiro(&tiro2);
	} else { criaTiro(&tiro2); tiro2.viajando = 0; }
}

void cooldowns(){
	if (shield_cd1){ shield_cd1--; }
	if (shield_cd2){ shield_cd2--; }
	if (tiro1.cooldown){ tiro1.cooldown--; }
	if (tiro2.cooldown){ tiro2.cooldown--; }
}

void checaVida(){
	if(!player1.shield_HP){ vencedor = 2; finish = true; }
	if(!player2.shield_HP){ vencedor = 1; finish = true; }
}

//Colisões
void colideTudo(){
	colideObstaculos(&bloco1, &player1, &player2);
	colideObstaculos(&bloco2, &player1, &player2);
	colideObstaculos(&bloco3, &player1, &player2);
	colideObstaculos(&bloco4, &player1, &player2);
	colideObstaculos(&bloco5, &player1, &player2);
	colideObstaculos(&bloco6, &player1, &player2);
	colideObstaculos(&bloco7, &player1, &player2);
	colideObstaculos(&bloco8, &player1, &player2);
	colideObstaculos(&bloco9, &player1, &player2);

	colidePlayers();
	colideTela(&player1);
	colideTela(&player2);
	colideTiro(&player1, &tiro2);
	colideTiro(&player2, &tiro1);
}

void colideTela(Tanque *player){
	// X
	if(player->x - player->vel - 40 <= 0){ player->x = 40 + player->vel; }
	if(player->x + player->vel + 40 >= SCREEN_W){ player->x = SCREEN_W - 40 - player->vel; }
	// Y
	if(player->y - player->vel - 40 <= 0){ player->y = 40 + player->vel; }
	if(player->y + player->vel + 40 >= SCREEN_H){ player->y = SCREEN_H - 40 - player->vel; }
}

bool colisaoCirculoRetangulo(Obstaculo *bloco, float player_x, float player_y, int raio){
	float distancia_x = abs(player_x - bloco->x);
	float distancia_y = abs(player_y - bloco->y);

	if(distancia_x > (bloco->largura/2 + raio)){ return 0; }
	if(distancia_y > (bloco->altura/2 + raio)){ return 0; }

	if(distancia_x <= (bloco->largura/2) || distancia_y <= (bloco->altura/2)){ return 1; }


	return 0;
}

void colisaoLados(Obstaculo *bloco, Tanque *player, int raio){
	float dist_pbx_esquerda = abs(player->x - (bloco->x - bloco->largura/2));
	float dist_pbx_direita 	= abs(player->x - (bloco->x + bloco->largura/2));
	float dist_pby_topo 	= abs(player->y - (bloco->y - bloco->altura/2));
	float dist_pby_baixo 	= abs(player->y - (bloco->y + bloco->altura/2));

	if((player->x > bloco->x - bloco->largura/2 - raio) && (player->x < bloco->x + bloco->largura/2 + raio)){
		if((player->y > bloco->y - bloco->altura/2) && (player->y < bloco->y + bloco->altura/2)){
			if(dist_pbx_esquerda < dist_pbx_direita){ player->x = bloco->x - bloco->largura/2 - raio; }
			if(dist_pbx_esquerda > dist_pbx_direita){ player->x = bloco->x + bloco->largura/2 + raio; }
		}
	}
	if((player->y > bloco->y - bloco->altura/2 - raio) && (player->y < bloco->y + bloco->altura/2 + raio)){ 
		if((player->x > bloco->x - bloco->largura/2) && (player->x < bloco->x + bloco->largura/2)){
			if(dist_pby_topo < dist_pby_baixo){ player->y = bloco->y - bloco->altura/2 - raio; }
			if(dist_pby_topo > dist_pby_baixo){ player->y = bloco->y + bloco->altura/2 + raio; }
		}
	}
}

bool colisaoQuinas(int quina_x, int quina_y, Tanque *player, int raio){
	float dist_x = player->x - quina_x;
	float dist_y = player->y - quina_y;
	float dist = hypot(dist_x, dist_y);
	quina_sen_x = dist_x/dist;
	quina_sen_y = dist_y/dist;

	return dist <= raio;
}

bool colisaoCirculos(float x1, float x2, float y1, float y2, int r1, int r2, int isplayer){
	float dist_x = x1 - x2;
   	float dist_y = y1 - y2;
   	float dist = hypot(dist_x, dist_y);
   	if(isplayer){ sen_x = dist_x/dist;
   				  sen_y = dist_y/dist; }

   	return dist <= r1 + r2;
}

void colideObstaculos(Obstaculo *bloco, Tanque *player1, Tanque *player2){
	//lados player 1
	colisaoLados(bloco, player1, 40);
	//quinas player 1
	if(colisaoQuinas(bloco->x - bloco->largura/2, bloco->y - bloco->altura/2, player1, 40)){ colideQuinas(bloco->x - bloco->largura/2, bloco->y - bloco->altura/2, player1, 40); }
	if(colisaoQuinas(bloco->x + bloco->largura/2, bloco->y - bloco->altura/2, player1, 40)){ colideQuinas(bloco->x + bloco->largura/2, bloco->y - bloco->altura/2, player1, 40); }
	if(colisaoQuinas(bloco->x - bloco->largura/2, bloco->y + bloco->altura/2, player1, 40)){ colideQuinas(bloco->x - bloco->largura/2, bloco->y + bloco->altura/2, player1, 40); }
	if(colisaoQuinas(bloco->x + bloco->largura/2, bloco->y + bloco->altura/2, player1, 40)){ colideQuinas(bloco->x + bloco->largura/2, bloco->y + bloco->altura/2, player1, 40); }

	//lados player 2
	colisaoLados(bloco, player2, 40);
	//quinas player 2
	if(colisaoQuinas(bloco->x - bloco->largura/2, bloco->y - bloco->altura/2, player2, 40)){ colideQuinas(bloco->x - bloco->largura/2, bloco->y - bloco->altura/2, player2, 40); }
	if(colisaoQuinas(bloco->x + bloco->largura/2, bloco->y - bloco->altura/2, player2, 40)){ colideQuinas(bloco->x + bloco->largura/2, bloco->y - bloco->altura/2, player2, 40); }
	if(colisaoQuinas(bloco->x - bloco->largura/2, bloco->y + bloco->altura/2, player2, 40)){ colideQuinas(bloco->x - bloco->largura/2, bloco->y + bloco->altura/2, player2, 40); }
	if(colisaoQuinas(bloco->x + bloco->largura/2, bloco->y + bloco->altura/2, player2, 40)){ colideQuinas(bloco->x + bloco->largura/2, bloco->y + bloco->altura/2, player2, 40); }
}

void colideQuinas(int quina_x, int quina_y, Tanque *player, int raio){
	player->x = quina_x + (raio) * quina_sen_x;
	player->y = quina_y + (raio) * quina_sen_y;
}

void colidePlayers(){
	if(colisaoCirculos(player1.x, player2.x, player1.y, player2.y, 40, 40, 1)){
		float temp_1x = player1.x;
		float temp_2x = player2.x;
		float temp_1y = player1.y;
		float temp_2y = player2.y;
		player1.x = temp_2x + 80 * sen_x;
		player1.y = temp_2y + 80 * sen_y;
		player2.x = temp_1x - 80 * sen_x;
		player2.y = temp_1y - 80 * sen_y;
	}
}

void colideTiro(Tanque *player, Projetil *tiro){
	if(colisaoCirculos(player->x, tiro->x, player->y, tiro->y, 40, 10, 0) && tiro->viajando){ tiro->hit = 1; player->shield_HP--; }
	else { tiro->hit = 0; }
}