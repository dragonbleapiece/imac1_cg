#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 400;
static unsigned int WINDOW_HEIGHT = 400;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/*Structures*/

typedef struct Point {
    float x, y; /*Position 2D du point*/
    unsigned char r, g, b; /*Couleur du point*/
    struct Point* next;
} Point, *PointList;

typedef struct Primitive {
    GLenum primitiveType;
    PointList points;
    struct Primitive* next;
} Primitive, *PrimitiveList;

typedef enum {
    BLUE,
    BLACK,
    MAGENTA,
    RED,
    YELLOW,
    WHITE,
    GREEN,
    CYAN
} Couleurs;

static const int v_couleurs[][3] = {
    /*BLUE =*/ {0, 0, 255},
    /*BLACK =*/ {0, 0, 0},
    /*MAGENTA =*/ {255, 0, 255},
    /*RED =*/ {255, 0, 0},
    /*YELLOW =*/ {255, 255, 0},
    /*WHITE =*/ {255, 255, 255},
    /*GREEN =*/ {0, 255, 0},
    /*CYAN =*/ {0, 255, 255}
};

static int pal = 0;

/* Fonctions */
void onResize() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1., 1., -1., 1.);
}

Point *allocPoint(float x, float y, unsigned char r, unsigned char g, unsigned char b) {
    Point *temp;
    temp = (Point *) malloc(sizeof(*temp));
    if(temp != NULL) {
        temp->x = x;
        temp->y = y;
        temp->r = r;
        temp->g = g;
        temp->b = b;
        temp->next = NULL;
    }

    return temp;
}

void addPointToList(Point *point, PointList *list) {
    point->next = *list;
    *list = point;
}

void drawPoints(PointList list) {
    Point *point = list;
    while(point != NULL) {
        glColor3ub(point->r, point->g, point->b);
        glVertex2f(-1 + 2. * point->x / WINDOW_WIDTH, -(-1 + 2. * point->y / WINDOW_HEIGHT));
        point = point->next;
    }
}

void deletePoints(PointList *list) {
    Point *point = *list;
    Point *temp = point;
    while(point != NULL) {
        point = point->next;
        free(temp);
        temp = point;
    }
}

Primitive *allocPrimitive(GLenum primitiveType) {
    Primitive *temp;
    temp = (Primitive *) malloc(sizeof(*temp));

    if(temp != NULL) {
        temp->primitiveType = primitiveType;
        temp->points = NULL;
        temp->next = NULL;
    }

    return temp;
}

void addPrimitive(Primitive *primitive, PrimitiveList *list) {
    primitive->next = *list;
    *list = primitive;
}

void drawPrimitives(PrimitiveList list) {
    Primitive *primitive = list;
    glBegin(primitive->primitiveType);
    while(primitive != NULL) {
        drawPoints(primitive->points);
        primitive = primitive->next;
    }
    glEnd();
}

void deletePrimitive(PrimitiveList *list) {
    Primitive *primitive = *list;
    Primitive *temp = primitive;
    while(primitive != NULL) {
        primitive = primitive->next;
        deletePoints(&(temp->points));
        free(temp);
        temp = primitive;
    }
}

void palette() {
    if(!pal) {
        int i;
        int size = (float)sizeof(v_couleurs) / (float)sizeof(*v_couleurs);
        GLfloat l = 2.0 / size;
        for(i = 0; i < size; ++i) {
            glColor3ub(v_couleurs[i][0], v_couleurs[i][1], v_couleurs[i][2]);
            glBegin(GL_QUADS);
                glVertex2f(-1 + l * i, -1);
                glVertex2f(-1 + l * (i + 1), -1);
                glVertex2f(-1 + l * (i + 1), 1);
                glVertex2f(-1 + l * i, 1);
            glEnd();
        }
        SDL_GL_SwapBuffers();
        pal = !pal;
    }
}

int main(int argc, char** argv) {

    /* Initialisation de la SDL */
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /*SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);*/

    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Padawan OpenGL", NULL);

    /* Boucle d'affichage */
    int loop = 1;
    float r = 0;
    float g = 0;
    float b = 0;
    int x = 0;
    int y = 0;
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(r, g, b, 1);
    SDL_GL_SwapBuffers();
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        /* Placer ici le code de dessin */


        /* Echange du front et du back buffer : mise à jour de la fenêtre */
        /*SDL_GL_SwapBuffers();*/

        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e)) {

            /* L'utilisateur ferme la fenêtre : */
            if(e.type == SDL_QUIT) {
                loop = 0;
                break;
            }

            /* Quelques exemples de traitement d'evenements : */
            switch(e.type) {

                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    x = e.button.x;
                    y = e.button.y;
                    break;

                case SDL_MOUSEMOTION:
                    r = e.button.x % 255 / 255.0;
                    g = e.button.y % 255 / 255.0;
                    b = (e.button.x + e.button.y) % 255 / 255.0;
                    break;

                /* Touche clavier */
                case SDL_KEYDOWN:
                    printf("touche pressée (code = %d)\n", e.key.keysym.sym);
                    switch(e.key.keysym.sym) {
                        case(SDLK_q):
                            loop = 0;
                            break;
                        case(SDLK_SPACE):
                            if(e.key.state == SDL_PRESSED) {
                                palette();
                            }
                            break;
                        default:
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    printf("touche relachee (code = %d)\n", e.key.keysym.sym);
                    switch(e.key.keysym.sym) {
                        case(SDLK_SPACE):
                            if(e.key.state == SDL_RELEASED) {
                                SDL_GL_SwapBuffers();
                                pal = !pal;
                            }
                            break;
                        default:
                            break;
                    }
                    break;

                case SDL_VIDEORESIZE:
                    WINDOW_HEIGHT = e.resize.h;
                    WINDOW_WIDTH = e.resize.w;
                    onResize();
                    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE);
                    break;

                default:
                    break;
            }
        }

        /* Calcul du temps écoulé */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;

        /* Si trop peu de temps s'est écoulé, on met en pause le programme */
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }
    }

    /* Liberation des ressources associées à la SDL */
    SDL_Quit();

    return EXIT_SUCCESS;
}
