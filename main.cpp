#include <GL/glut.h>
#include <iostream>
#include <string>

// Tamanho da janela
const int width = 800;
const int height = 600;

// Posições e dimensões das barras
float barWidth = 20.0f;
float barHeight = 100.0f;
float bar1Y = height / 2 - barHeight / 2;
float bar2Y = height / 2 - barHeight / 2;

// Posição e velocidade da bola
float ballX = width / 2;
float ballY = height / 2;
float ballSize = 20.0f;

// Velocidades
float barSpeed = 10.0f;
float ballXSpeed = 5.0f;
float ballYSpeed = 5.0f;

// Placar
int score1 = 0;
int score2 = 0;

// Estado das teclas
bool bar1Up = false, bar1Down = false;
bool bar2Up = false, bar2Down = false;

// Condições de vitória e dificuldade
const int maxPoints = 5; // Pontuação máxima para vencer
int difficultyLevel = 2; // Nível de dificuldade (1: Fácil, 2: Médio, 3: Difícil)

// Estado do jogo
enum GameState { MENU, PLAYING, GAME_OVER };
GameState currentState = MENU;
std::string victoryMessage = "";

// Função para ajustar a dificuldade
void setDifficulty(int level)
{
    difficultyLevel = level;
    switch (level)
    {
    case 1: // Fácil
        ballXSpeed = 3.0f;
        ballYSpeed = 3.0f;
        barHeight = 175.0f;
        break;
    case 2: // Médio
        ballXSpeed = 4.0f;
        ballYSpeed = 4.0f;
        barHeight = 125.0f;
        break;
    case 3: // Difícil
        ballXSpeed = 6.0f;
        ballYSpeed = 6.0f;
        barHeight = 100.0f;
        break;
    default:
        ballXSpeed = 4.0f;
        ballYSpeed = 4.0f;
        barHeight = 125.0f;
        break;
    }
}

// Função que verifica se algum jogador venceu
void checkVictory()
{
    if (score1 >= maxPoints)
    {
        victoryMessage = "Jogador 1 venceu!";
        currentState = GAME_OVER;
    }
    else if (score2 >= maxPoints)
    {
        victoryMessage = "Jogador 2 venceu!";
        currentState = GAME_OVER;
    }
}

// Função que desenha o texto na tela
void drawText(float x, float y, std::string text)
{
    glRasterPos2f(x, y);
    for (char c : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (currentState == MENU)
    {
        // Desenha o menu
        drawText(width / 2 - 100, height / 2 + 50, "Bem-vindo ao Pong!");
        drawText(width / 2 - 100, height / 2, "Pressione 1 para Facil");
        drawText(width / 2 - 100, height / 2 - 30, "Pressione 2 para Medio");
        drawText(width / 2 - 100, height / 2 - 60, "Pressione 3 para Dificil");
    }
    else if (currentState == PLAYING)
    {
        // Desenha o placar
        drawText(width / 2 - 50, height - 50, std::to_string(score1) + " - " + std::to_string(score2));

        // Desenha as barras
        glRectf(10, bar1Y, 10 + barWidth, bar1Y + barHeight);
        glRectf(width - 30, bar2Y, width - 30 + barWidth, bar2Y + barHeight);

        // Desenha a bola
        glRectf(ballX, ballY, ballX + ballSize, ballY + ballSize);
    }
    else if (currentState == GAME_OVER)
    {
        // Desenha a mensagem de vitória
        drawText(width / 2 - 100, height / 2, victoryMessage);
        drawText(width / 2 - 100, height / 2 - 50, "Pressione Enter para voltar ao menu");
    }

    glutSwapBuffers();
}

void update(int value)
{
    if (currentState == PLAYING)
    {
        // Movimenta as barras
        if (bar1Up && bar1Y + barHeight < height)
            bar1Y += barSpeed;
        if (bar1Down && bar1Y > 0)
            bar1Y -= barSpeed;
        if (bar2Up && bar2Y + barHeight < height)
            bar2Y += barSpeed;
        if (bar2Down && bar2Y > 0)
            bar2Y -= barSpeed;

        // Movimenta a bola
        ballX += ballXSpeed;
        ballY += ballYSpeed;

        // Colisões com as bordas superiores e inferiores
        if (ballY <= 0 || ballY + ballSize >= height)
            ballYSpeed = -ballYSpeed;

        // Colisões com as barras
        if ((ballX <= 30 && ballY + ballSize >= bar1Y && ballY <= bar1Y + barHeight) ||
            (ballX + ballSize >= width - 30 && ballY + ballSize >= bar2Y && ballY <= bar2Y + barHeight))
        {
            ballXSpeed = -ballXSpeed;
        }

        // Pontuação
        if (ballX <= 0)
        {
            score2++;
            ballX = width / 2;
            ballY = height / 2;
        }
        else if (ballX + ballSize >= width)
        {
            score1++;
            ballX = width / 2;
            ballY = height / 2;
        }

        // Verifica se algum jogador venceu
        checkVictory();
    }
    glutTimerFunc(16, update, 0); // Aproximadamente 60 FPS

    glutPostRedisplay();
}

void handleKeysDown(unsigned char key, int x, int y)
{
    if (currentState == PLAYING)
    {
        if (key == 'w')
            bar1Up = true;
        if (key == 's')
            bar1Down = true;
        if (key == 'o')
            bar2Up = true;
        if (key == 'l')
            bar2Down = true;
    }
    else if (currentState == MENU)
    {
        if (key == '1')
        {
            setDifficulty(1);
            currentState = PLAYING;
        }
        else if (key == '2')
        {
            setDifficulty(2);
            currentState = PLAYING;
        }
        else if (key == '3')
        {
            setDifficulty(3);
            currentState = PLAYING;
        }
    }
    else if (currentState == GAME_OVER && key == 13) // Enter para voltar ao menu
    {
        currentState = MENU;
        score1 = 0;
        score2 = 0;
        ballX = width / 2;
        ballY = height / 2;
    }
}

void handleKeysUp(unsigned char key, int x, int y)
{
    if (currentState == PLAYING)
    {
        if (key == 'w')
            bar1Up = false;
        if (key == 's')
            bar1Down = false;
        if (key == 'o')
            bar2Up = false;
        if (key == 'l')
            bar2Down = false;
    }
}

// Função para evitar o redimensionamento da janela
void reshape(int w, int h)
{
    glutReshapeWindow(width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);
    glutPostRedisplay();
}

void Init(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("TP3 - Jogo Pong");
}

int main(int argc, char **argv)
{
    Init(argc, argv);

    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    glutKeyboardFunc(handleKeysDown);
    glutKeyboardUpFunc(handleKeysUp);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
