#include <GL/glut.h>
#include <iostream>

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
const int maxPoints = 5;  // Pontuação máxima para vencer
int difficultyLevel = 2;  // Nível de dificuldade (1: Fácil, 2: Médio, 3: Difícil)

// Função para ajustar a dificuldade
void setDifficulty(int level) {
    difficultyLevel = level;
    switch(level) {
        case 1: // Fácil
            ballXSpeed = 3.0f;
            ballYSpeed = 3.0f;
            barHeight = 150.0f;
            break;
        case 2: // Médio
            ballXSpeed = 5.0f;
            ballYSpeed = 5.0f;
            barHeight = 100.0f;
            break;
        case 3: // Difícil
            ballXSpeed = 7.0f;
            ballYSpeed = 7.0f;
            barHeight = 75.0f;
            break;
        default:
            ballXSpeed = 5.0f;
            ballYSpeed = 5.0f;
            barHeight = 100.0f;
            break;
    }
}

// Função que verifica se algum jogador venceu
void checkVictory() {
    if (score1 >= maxPoints) {
        std::cout << "Jogador 1 venceu!" << std::endl;
        exit(0); // Termina o jogo
    } else if (score2 >= maxPoints) {
        std::cout << "Jogador 2 venceu!" << std::endl;
        exit(0); // Termina o jogo
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha o placar
    glRasterPos2f(width / 2 - 50, height - 50);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + score1);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '-');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + score2);

    // Desenha as barras
    glRectf(10, bar1Y, 10 + barWidth, bar1Y + barHeight);
    glRectf(width - 30, bar2Y, width - 30 + barWidth, bar2Y + barHeight);

    // Desenha a bola
    glRectf(ballX, ballY, ballX + ballSize, ballY + ballSize);

    glutSwapBuffers();
}

void update(int value) {
    // Movimenta as barras
    if (bar1Up && bar1Y + barHeight < height) bar1Y += barSpeed;
    if (bar1Down && bar1Y > 0) bar1Y -= barSpeed;
    if (bar2Up && bar2Y + barHeight < height) bar2Y += barSpeed;
    if (bar2Down && bar2Y > 0) bar2Y -= barSpeed;

    // Movimenta a bola
    ballX += ballXSpeed;
    ballY += ballYSpeed;

    // Colisões com as bordas superiores e inferiores
    if (ballY <= 0 || ballY + ballSize >= height) ballYSpeed = -ballYSpeed;

    // Colisões com as barras
    if ((ballX <= 30 && ballY + ballSize >= bar1Y && ballY <= bar1Y + barHeight) ||
        (ballX + ballSize >= width - 30 && ballY + ballSize >= bar2Y && ballY <= bar2Y + barHeight)) {
        ballXSpeed = -ballXSpeed;
    }

    // Pontuação
    if (ballX <= 0) {
        score2++;
        ballX = width / 2;
        ballY = height / 2;
    } else if (ballX + ballSize >= width) {
        score1++;
        ballX = width / 2;
        ballY = height / 2;
    }

    // Verifica se algum jogador venceu
    checkVictory();
    glutTimerFunc(16, update, 0); // Aproximadamente 60 FPS
    
    glutPostRedisplay();
    
}

void handleKeysDown(unsigned char key, int x, int y) {
    if (key == 'w') bar1Up = true;
    if (key == 's') bar1Down = true;
    if (key == 'o') bar2Up = true;
    if (key == 'l') bar2Down = true;
}

void handleKeysUp(unsigned char key, int x, int y) {
    if (key == 'w') bar1Up = false;
    if (key == 's') bar1Down = false;
    if (key == 'o') bar2Up = false;
    if (key == 'l') bar2Down = false;
}

// Função para evitar o redimensionamento da janela
void reshape(int w, int h) {
    glutReshapeWindow(width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);
    glutPostRedisplay();
}

void Init(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("TP3 - Jogo Pong");
}

int main(int argc, char** argv) {
    Init(argc, argv);

    // Ajusta o nível de dificuldade (1: Fácil, 2: Médio, 3: Difícil)
    setDifficulty(3);

    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    glutKeyboardFunc(handleKeysDown);
    glutKeyboardUpFunc(handleKeysUp);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
