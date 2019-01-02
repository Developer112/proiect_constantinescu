#include "spcwar.h"

int a=1;
char ch;
ssize_t read_size;
#define BUFFER_SIZE 1024
#define N 20
#define CN 100
#define UG 1
int v[N+1][N+1];
char buffer[BUFFER_SIZE];

void interpreteaza_matrice() {
    for(int i=1;i<=N;i++) {
        for(int j=1;j<=N;j++) {
            if(v[i][j] == 1) {
                printf(" * ");
            } else if(v[i][j] == 2) {
                printf(" # ");
            } else if(v[i][j] == 3) {
                printf(" ! ");
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }
}

void generate(void) {

    for(int i=1;i<=2;i++) {
        for(int j=1;j<=N;j++) {
            v[i][j] = 1;
        }
    }

    v[N-1][N/2] = 2;
    v[N][N/2] = 2;
    v[N][N/2 - 1] = 2;
    v[N][N/2 + 1] = 2;
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

bool allZero() {
    for(int i=N-2;i>=1;i--) {
        for(int j=1;j<=N;j++) {
            if(v[i][j] == 1) {
                return false;
            }
        }
    }

    return true;
}

void urca_gloante() {
    for(int i=2;i<=N-2;i++) {
        for(int j=1;j<=N;j++) {
            if(v[i][j] == 3) {
                if(v[i-1][j] == 1) {
                    v[i][j] = 0;
                    v[i-1][j] = 0;
                } else {
                    v[i-1][j] = v[i][j];
                    v[i][j] = 0;
                }
            }
        }
    }
}

void trage() {
    int poz=0;
    for(int j=1;j<=N;j++) {
        if(v[N-1][j] == 2) {
            poz = j;
            break;
        }
    }
    switch(v[N-2][poz]) {
        case 1:
            v[N-2][poz] = 0;
            break;
        case 0:
            v[N-2][poz] = 3;
            break;
        default :
            break;
    }
}

void move_left() {
    for(int i=N;i>=N-1;i--) {
        int x = v[i][1];
        for(int j=2;j<=N;j++) {
            v[i][j-1] = v[i][j];
        }
        v[i][N] = x;
    }
}

void move_right() {
    for(int i=N;i>=N-1;i--) {
        int x = v[i][N];
        for(int j = N-1;j>=1;j--) {
            v[i][j+1] = v[i][j];
        }
        v[i][1] = x;
    }
}

void coboara_nave(_Bool *game) {

    for(int j=1;j<=N;j++) {
        if(v[N-2][j] == 1) {
            *game = true;
            return;
        }
    }

    for(int i=N-3;i>=1;i--) {
        for(int j=1;j<=N;j++) {
            v[i+1][j] = v[i][j];
        }
    }
}

bool kbhit()
{
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
}

void run_game(_Bool *game){
    if(kbhit()) {
        system("stty raw");
        read_size = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        system("stty cooked");
        if (read_size > 0) {
            ch = buffer[0];
            switch (ch) {
                case 'a':
                    move_left();
                    system("clear");
                    interpreteaza_matrice();
                    break;
                case 'd':
                    move_right();
                    system("clear");
                    interpreteaza_matrice();
                    break;
                case 'e':
                    printf("Exited! \n");
                    *game = true;
                    break;
                case ' ':
                    trage();
                    system("clear");
                    interpreteaza_matrice();
                    break;
                default:
                    system("clear");
                    interpreteaza_matrice();
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
    interpreteaza_matrice();

    do {
        if(a%UG == 0) {
            urca_gloante();
            system("clear");
            interpreteaza_matrice();
        }

        if(a%CN == 0) {
            if(allZero() == false) {
                coboara_nave(&game_over);
                system("clear");
                interpreteaza_matrice();
            } else {
                system("clear");
                printf("You win!\n");
                return 0;
            }
        }

        enable_raw_mode();
        run_game(&game_over);
        disable_raw_mode();
        tcflush(0 , TCIFLUSH);
        fflush(stdout);
        usleep(100000);
        a++;
    } while(!game_over);

    return 0;
}