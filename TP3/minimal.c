#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAXCIRCLE 20.0

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 400;
static unsigned int WINDOW_HEIGHT = 400;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

static float X = 1;
static float Y = 1;

/*Structures*/

typedef enum {
    BLUE,
    BLACK,
    MAGENTA,
    RED,
    YELLOW,
    WHITE,
    GREEN,
    CYAN,
    GREY50
} Couleurs;

static const float v_couleurs[][3] = {
    /*BLUE =*/ {0.f, 0.f, 255.f},
    /*BLACK =*/ {0.f, 0.f, 0.f},
    /*MAGENTA =*/ {255.f, 0.f, 255.f},
    /*RED =*/ {255.f, 0.f, 0.f},
    /*YELLOW =*/ {255.f, 255.f, 0.f},
    /*WHITE =*/ {255.f, 255.f, 255.f},
    /*GREEN =*/ {0.f, 255.f, 0.f},
    /*CYAN =*/ {0.f, 255.f, 255.f},
    /*GREY50 =*/ {127.5, 127.5, 127.5}
};

/* Fonctions */
void onResize() {
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-X, X, -Y, Y);
}

float ortho2D(float x, float w) {
    return (-X + 2. * X * x / w);
}

void drawSquare(float x, float y, float longueur, const float color[3], int ligne) {
    GLenum type = ligne ? GL_LINE_LOOP : GL_QUADS;
    glBegin(type);
      glColor3ub(color[0], color[1], color[2]);
      glVertex2f(ortho2D(x - longueur / 2.f, WINDOW_WIDTH), -(ortho2D(y - longueur / 2.f, WINDOW_HEIGHT)));
      glVertex2f(ortho2D(x + longueur / 2.f, WINDOW_WIDTH), -(ortho2D(y - longueur / 2.f, WINDOW_HEIGHT)));
      glVertex2f(ortho2D(x + longueur / 2.f, WINDOW_WIDTH), -(ortho2D(y + longueur / 2.f, WINDOW_HEIGHT)));
      glVertex2f(ortho2D(x - longueur / 2.f, WINDOW_WIDTH), -(ortho2D(y + longueur / 2.f, WINDOW_HEIGHT)));
    glEnd();
}

void drawLandmark(float x, float y) {
    glBegin(GL_LINES);
    /*Axe X*/
      glColor3ub(255, 0, 0);
      glVertex2f(ortho2D(0, 1), -(ortho2D(y, WINDOW_HEIGHT)));
      glVertex2f(ortho2D(1, 1), -(ortho2D(y, WINDOW_HEIGHT)));

    /*Axe Y*/
      glColor3ub(0, 255, 0);
      glVertex2f(ortho2D(x, WINDOW_WIDTH), -(ortho2D(0, 1)));
      glVertex2f(ortho2D(x, WINDOW_WIDTH), -(ortho2D(1, 1)));
    glEnd();
}

void drawCircle(float x, float y, float rayon, const float color[3], int ligne) {
    int i;
    GLenum primitiveType = ligne ? GL_LINE_LOOP : GL_POLYGON;
    glBegin(primitiveType);
      glColor3ub(color[0], color[1], color[2]);
      for(i = 0; i <= MAXCIRCLE; ++i) {
        glVertex2f(ortho2D(x + rayon * cos(((2.0 * M_PI) / MAXCIRCLE) * i), WINDOW_WIDTH), -(ortho2D(y + rayon * sin(((2.0 * M_PI) / MAXCIRCLE) * i), WINDOW_HEIGHT)));
      }
    glEnd();
}

void drawRoundedSquare(float x, float y, float rayon, float longueur, const float color[3], int ligne) {
    float r = ((rayon > 100) ? 100 : rayon) / 100.f * longueur;
    float dc = (longueur / 2.f - r / 2.f);
    drawSquare(x + r, y, longueur - r, color, ligne);
    drawSquare(x, y + r, longueur - r, color, ligne);
    drawSquare(x - r, y, longueur - r, color, ligne);
    drawSquare(x, y - r, longueur - r, color, ligne);
    drawCircle(x - dc, y - dc, r, color, ligne);
    drawCircle(x + dc, y - dc, r, color, ligne);
    drawCircle(x - dc, y + dc, r, color, ligne);
    drawCircle(x + dc, y + dc, r, color, ligne);
}

void drawFirstArm(float x, float y, const float color[3], int ligne) {

        GLenum type = ligne ? GL_LINE_LOOP : GL_POLYGON;
        drawCircle(x - 30, y, 20, color, ligne);
        drawCircle(x + 30, y, 10, color, ligne);

        glBegin(type);
            glColor3ub(color[0], color[1], color[2]);
            glVertex2d(ortho2D(x - 30, WINDOW_WIDTH), - ( ortho2D(y + 20, WINDOW_HEIGHT)));
            glVertex2d(ortho2D(x + 30, WINDOW_WIDTH), - ( ortho2D(y + 10, WINDOW_HEIGHT)));
            glVertex2d(ortho2D(x + 30, WINDOW_WIDTH), - ( ortho2D(y - 10, WINDOW_HEIGHT)));
            glVertex2d(ortho2D(x - 30, WINDOW_WIDTH), - ( ortho2D(y - 20, WINDOW_HEIGHT)));
        glEnd();
}

void drawSecondArm(float x, float y, const float color[3], int ligne) {

        GLenum type = ligne ? GL_LINE_LOOP : GL_POLYGON;
        drawRoundedSquare(x - 22.5, y, 20, 10, color, ligne);
        drawRoundedSquare(x + 22.5, y, 20, 10, color, ligne);

        glBegin(type);
            glColor3ub(color[0], color[1], color[2]);
            glVertex2d(ortho2D(x - 23, WINDOW_WIDTH), - ( ortho2D(y + 3, WINDOW_HEIGHT)));
            glVertex2d(ortho2D(x + 23, WINDOW_WIDTH), - ( ortho2D(y + 3, WINDOW_HEIGHT)));
            glVertex2d(ortho2D(x + 23, WINDOW_WIDTH), - ( ortho2D(y - 3, WINDOW_HEIGHT)));
            glVertex2d(ortho2D(x - 23, WINDOW_WIDTH), - ( ortho2D(y - 3, WINDOW_HEIGHT)));
        glEnd();
}

void drawThirdArm(float x, float y, const float color[3], int ligne) {

        GLenum type = ligne ? GL_LINE_LOOP : GL_POLYGON;
        drawRoundedSquare(x - 20, y, 10, 6, color, ligne);
        drawCircle(x + 20, y, 8, color, ligne);

        glBegin(type);
            glColor3ub(color[0], color[1], color[2]);
            glVertex2d(ortho2D(x - 18, WINDOW_WIDTH), - ( ortho2D(y + 2, WINDOW_HEIGHT)));
            glVertex2d(ortho2D(x + 18, WINDOW_WIDTH), - ( ortho2D(y + 2, WINDOW_HEIGHT)));
            glVertex2d(ortho2D(x + 18, WINDOW_WIDTH), - ( ortho2D(y - 2, WINDOW_HEIGHT)));
            glVertex2d(ortho2D(x - 18, WINDOW_WIDTH), - ( ortho2D(y - 2, WINDOW_HEIGHT)));
        glEnd();
}


GLuint createFirstArmID(float x, float y, const float color[3]) {
    GLuint id = glGenLists(1);

    glNewList(id, GL_COMPILE);
        drawFirstArm(x, y, color, 0);
        drawFirstArm(x, y, v_couleurs[BLACK], 1);
    glEndList();

    return id;

}

GLuint createSecondArmID(float x, float y, const float color[3]) {
    GLuint id = glGenLists(1);

    glNewList(id, GL_COMPILE);
        drawSecondArm(x + 52.5, y, color, 0);
        drawSecondArm(x + 52.5, y, v_couleurs[BLACK], 1);
    glEndList();

    return id;

}

GLuint createThirdArmID(float x, float y, const float color[3]) {
    GLuint id = glGenLists(1);

    glNewList(id, GL_COMPILE);
        drawThirdArm(x + 95, y, color, 0);
        drawThirdArm(x + 95, y, v_couleurs[BLACK], 1);
    glEndList();

    return id;

}

float rand_a_b(float a, float b) {
    return (rand() / (float)RAND_MAX) * (b - a) + a;
}


void drawArm(float x, float y, float *alpha, float *beta, float *gamma, GLuint idFirst, GLuint idSecond, GLuint idThird) {



        glPushMatrix();

            glTranslatef(ortho2D(x - 30, WINDOW_WIDTH), ortho2D(y, WINDOW_HEIGHT), 0.0);
            glRotatef(*alpha, 0.0, 0.0, 1.0);
           glTranslatef(-ortho2D(x - 30, WINDOW_WIDTH), -ortho2D(y, WINDOW_HEIGHT), 0.0);
            glCallList(idFirst);
            glPushMatrix();

               glTranslatef(ortho2D(x - 22.5 + 52.5, WINDOW_WIDTH), ortho2D(y, WINDOW_HEIGHT), 0.0);
                glRotatef(*beta, 0.0, 0.0, 1.0);
                glTranslatef(-ortho2D(x - 22.5 + 52.5, WINDOW_WIDTH), -ortho2D(y, WINDOW_HEIGHT), 0.0);
                glCallList(idSecond);
                glPushMatrix();

                    glTranslatef(ortho2D(x - 20 + 95, WINDOW_WIDTH), ortho2D(y, WINDOW_HEIGHT), 0.0);
                    glRotatef(*gamma, 0.0, 0.0, 1.0);
                    glTranslatef(-ortho2D(x - 20 + 95, WINDOW_WIDTH), -ortho2D(y, WINDOW_HEIGHT), 0.0);
                    glCallList(idThird);
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();

        *alpha += rand_a_b(0, 2);
        *beta += rand_a_b(0, 2);
        *gamma += rand_a_b(0, 2);

}

int main(int argc, char** argv) {

    time(NULL);

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
    float r = 255.f;
    float g = 255.f;
    float b = 255.f;
    float x = 0.f;
    float y = 0.f;
    float alpha = 45.f;
    float beta = -10.f;
    float gamma = 35.f;
    float theta = 40.f;
    float epsilon = 30.f;
    const float centerX = WINDOW_WIDTH / 2.f;
    const float centerY = WINDOW_HEIGHT / 2.f;
    int nCouleurs = sizeof(v_couleurs) / sizeof(*v_couleurs);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 60 / 255.f, 120 / 255.f, 1);
    SDL_GL_SwapBuffers();

    GLuint idFirst = createFirstArmID(centerX, centerY, v_couleurs[GREY50]),
        idSecond = createSecondArmID(centerX, centerY, v_couleurs[GREY50]),
        idThird = createThirdArmID(centerX, centerY, v_couleurs[GREY50]);

    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        /*Transformation*/
        glMatrixMode(GL_MODELVIEW);

        glLoadIdentity();
        glScalef(2, 2, 1);

        /* Placer ici le code de dessin */

            glClear(GL_COLOR_BUFFER_BIT);

            drawLandmark(centerX, centerY);

            drawArm(centerX, centerY, &alpha, &beta, &gamma, idFirst, idSecond, idThird);


            SDL_GL_SwapBuffers();

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
                    break;



                /* Touche clavier */
                case SDL_KEYDOWN:
                    printf("touche pressée (code = %d)\n", e.key.keysym.sym);
                    switch(e.key.keysym.sym) {
                        case(SDLK_q):
                            loop = 0;
                            break;
                        default:
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    printf("touche relachee (code = %d)\n", e.key.keysym.sym);
                    break;

                case SDL_VIDEORESIZE:
                    WINDOW_HEIGHT = e.resize.h;
                    WINDOW_WIDTH = e.resize.w;
                    X = WINDOW_WIDTH / 2.f;
                    Y = WINDOW_HEIGHT / 2.f;
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
