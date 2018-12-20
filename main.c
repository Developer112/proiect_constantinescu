#include "spcwar.h"

int v[11][11];
char ch;
bool *game;
ssize_t read_size;
#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];

void generate(void) {

    for(int i=1;i<=2;i++) {
        for(int j=1;j<=10;j++) {
            v[i][j] = 1;
        }
    }

    v[9][5] = 2;
    v[10][5] = 2;
    v[10][4] = 2;
    v[10][6] = 2;
}

void enable_raw_mode()
{
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
}

void disable_raw_mode()
{
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

bool kbhit()
{
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
}

void move_spaceship(_Bool *game){
    if(kbhit()) {
        system("stty raw");
        read_size = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        system("stty cooked");
        if (read_size > 0) {
            ch = buffer[0];
            switch (ch) {
                case 'a':
                    printf("Pressed a! \n");
                    break;
                case 'd':
                    printf("Pressed d! \n");
                    break;
                case 'e':
                    printf("Exited! \n");
                    *game = true;
                    break;
            }
        }
    }
}

int main(void) {

    bool game_over = false;


    printf("Press S to start the game! \n");
    system("stty raw");
    scanf("%c" , &ch);
    system("stty cooked");
    if(ch != 's') {
        printf("Press S to start the game! \n");
    }

    while(ch != 's') {
        system("stty raw");
        scanf("%c" , &ch);
        system("stty cooked");
        if(ch != 's') {
            printf("Press S to start the game! \n");
        }
    }

    printf("The game starts in 3... \n");
    sleep(1);
    printf("2...\n");
    sleep(1);
    printf("1...\n");
    sleep(1);
    system("clear");

    generate();

    do {
        enable_raw_mode();
        move_spaceship(&game_over);
        disable_raw_mode();
        tcflush(0 , TCIFLUSH);
        printf(".");
        fflush(stdout);
        usleep(10 * 1000);

    } while(!game_over);

    return 0;
}