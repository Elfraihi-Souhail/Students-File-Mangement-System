#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define sleep_ms(ms) Sleep(ms)

void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hOut, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, mode);
}

void enableRawMode() {}
void disableRawMode() {}

#else
#include <termios.h>
#include <unistd.h>
#define sleep_ms(ms) usleep((ms) * 1000)

struct termios orig_term;

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_term);
    struct termios raw = orig_term;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
}

void enableANSI() {}
#endif

// Menu tools
#define ANSI_COLOR_RESET   "\033[0m"
#define ANSI_COLOR_BLACK   "\033[30m"
#define ANSI_COLOR_WHITE   "\033[37m"
#define ANSI_COLOR_BG_WHITE "\033[47m"
#define ANSI_COLOR_BG_BLACK "\033[40m"

enum { KEY_UP = 1000, KEY_DOWN, KEY_ENTER };

void highlightLine(const char *str)
{
    printf(ANSI_COLOR_BG_WHITE ANSI_COLOR_BLACK "%s" ANSI_COLOR_RESET "\n", str);
}

void printESI()
{
    printf("\033[2J\033[H");
    printf("\n\t        \033[0;107m                  \033[0m          \033[0;107m                  \033[0m     \033[0;104m         \033[0m \n");
    sleep_ms(150);
    printf("\t      \033[0;107m                      \033[0m      \033[0;107m                      \033[0m   \033[0;104m         \033[0m  \n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m \n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;104m         \033[0m \n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m             \n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m       \033[0m  \n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m                      \033[0;107m       \033[0m       L'ECOLE  NATIONALE\n");
    sleep_ms(150);
    printf("\t     \033[0;107m                        \033[0m    \033[0;107m                        \033[0m    \033[0;107m       \033[0m  \n");
    sleep_ms(150);
    printf("\t     \033[0;107m                        \033[0m    \033[0;107m                        \033[0m    \033[0;107m       \033[0m       SUPERIEURE\n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m                                        \033[0;107m      \033[0m    \033[0;107m       \033[0m  \n");
    sleep_ms(150);
    printf("\t     \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m      \033[0m            \033[0;107m      \033[0m    \033[0;107m       \033[0m       D'INFORMATIQUE\n");
    sleep_ms(150);
    printf("\t      \033[0;107m      \033[0m          \033[0;107m      \033[0m      \033[0;107m      \033[0m          \033[0;107m      \033[0m     \033[0;107m       \033[0m  \n");
    sleep_ms(150);
    printf("\t      \033[0;107m                      \033[0m      \033[0;107m                      \033[0m     \033[0;107m       \033[0m  \n");
    sleep_ms(150);
    printf("\t        \033[0;107m                  \033[0m          \033[0;107m                  \033[0m       \033[0;107m       \033[0m  \n\n\n");
    sleep_ms(220);
    printf("\t\t\t                            \033[91mPlease                                 \n");
    printf("\t\t\t                  Click any key to continue...\033[0m               ");
    getchar();

    printf("\033[2J\033[H");
}

void Welcome()
{
    printf("\033[2J\033[H");
    printf("\t\t\t\033[96m                People's Democratic Republic of Algeria           \033[0m\n");
    printf("\t\t\t\033[96m         Ministry of Higher Education and Scientific Research\033[0m\n");
    printf("\t\t\t      \033[96m _______________________________________________________\n");
    printf("\t\t\t      \033[96m|        \033[92m_______\033[0m    \033[91m _______\033[0m    \033[97m _\033[0m                      \033[96m|\n");
    printf("\t\t\t      \033[96m|       \033[92m|  _____|\033[0m   \033[91m|  _____|\033[0m   \033[97m| |\033[0m  \033[92mHigher National\033[0m    \033[96m|\n");
    printf("\t\t\t      \033[96m|       \033[92m| |_____\033[0m    \033[91m| |_____\033[0m    \033[97m| |\033[0m                     \033[96m|\n");
    printf("\t\t\t      \033[96m|       \033[92m|  _____|\033[0m   \033[91m|_____  |\033[0m   \033[97m| |\033[0m  \033[91mSchool of\033[0m          \033[96m|\n");
    printf("\t\t\t      \033[96m|       \033[92m| |_____\033[0m    \033[91m _____| |\033[0m   \033[97m| |\033[0m                     \033[96m|\n");
    printf("\t\t\t      \033[96m|       \033[92m|_______|\033[0m   \033[91m|_______|\033[0m   \033[97m|_|\033[0m  \033[97mComputer Science\033[0m   \033[96m|\n");
    printf("\t\t\t      \033[96m|_______________________________________________________|\033[0m\n\n");

    printf("\t\t\t\t\033[96m     CPI - Second Year - Scholar Year: 2025/2026 \n");
    printf("\t\t\t\033[96m ___________________________________________________________________\n");
    printf("\t\t\t\033[96m|                                                                   |\n");
    printf("\t\t\t\033[96m|             REALISED By :  \033[97mELFRAIHI SOUHAIL \033[96m                      |\n");
    printf("\t\t\t\033[96m|                                    &                              |\n");
    printf("\t\t\t\033[96m|                             \033[97mKHITER HAMZA\033[96m                          |\n");
    printf("\t\t\t\033[96m|                                                                   |\n");
    printf("\t\t\t\033[96m|                                                                   |\n");
    printf("\t\t\t\033[96m|               SECTION :  \033[97mA\033[96m          GROUPE: \033[97m01\033[96m                    |\n");
    printf("\t\t\t\033[96m|                                                                   |\n");
    printf("\t\t\t\033[96m|                   TP2 :  \033[97mFSDS\033[96m                                     |\n");
    printf("\t\t\t\033[96m|___________________________________________________________________|\n\n\n");

    printf("\t\t\t                            \033[91mPlease                                 \n");
    printf("\t\t\t                  Click any key to continue...\033[0m               ");
    getchar();

    printf("\033[2J\033[H");
}

void printloading()
{
    printf("\033[2J\033[H");
    sleep_ms(200);
    printf("\033[0m\n\n\n\n\t\t\t\t\t\t       \033[34m\033[1mLoading.\033[0m\n\n");
    sleep_ms(200);
    printf("\033[2J\033[H");
    printf("\033[0m\n\n\n\n\t\t\t\t\t\t       \033[34m\033[1mLoading..\033[0m\n\n");
    sleep_ms(200);
    printf("\033[2J\033[H");
    printf("\033[0m\n\n\n\n\t\t\t\t\t\t       \033[34m\033[1mLoading...\033[0m\n\n");
    sleep_ms(200);
    printf("\033[2J\033[H");
    printf("\033[0m\n\n\n\n\t\t\t\t\t\t       \033[34m\033[1mDONE\033[0m\n\n");
    sleep_ms(200);
    printf("\033[2J\033[H");
}

void printMenu(int arrowpos)
{
    printf("\033[2J\033[H");
    printf("\033[0m\n\t\t\t\t\t     \033[34m\033[1mTREE TRAVERSAL TECHNIQUES \033[0m\n\n\n");
    int maxopt = 5;
    char options[][150] = {"Create Trees", "Check BST", "Traversals", "Check Traversals", "Exit"};
    for (int i = 0; i < maxopt; i++)
    {
        if (arrowpos == i + 1)
        {
            printf("\t --->     ");
            highlightLine(options[i]);
            printf("\n");
        }
        else
        {
            printf("\t       %s\n\n", options[i]);
        }
    }
    printf("\x1b[0m\n                \033[32m\033[1mUSAGE NOTES:\033[0m\n");
    printf("\x1b[0m\n         \033[32m- Use the arrow keys to navigate through the menu.\x1b[0m");
    printf("\x1b[0m\n         \033[32m- Press Enter to select an option.\x1b[0m\n\n");
}

int readKey() {
#ifdef _WIN32
    int ch = _getch();
    if (ch == 224) { // Arrow key prefix on Windows
        ch = _getch();
        if (ch == 72) return KEY_UP;
        if (ch == 80) return KEY_DOWN;
    }
    if (ch == 13) return KEY_ENTER;
    return ch;
#else
    int ch = getchar();
    if (ch == 27) { // ESC sequence
        if (getchar() == '[') {
            ch = getchar();
            if (ch == 'A') return KEY_UP;
            if (ch == 'B') return KEY_DOWN;
        }
    }
    if (ch == '\n') return KEY_ENTER;
    return ch;
#endif
}

int Menu() {
    int i = 1; 
    int maxopt = 5; 
    
    enableRawMode();
    
    while (1) { 
        printMenu(i); 
        int key = readKey();
        
        if (key == KEY_UP) { 
            i = (i == 1) ? maxopt : i - 1;
        } 
        else if (key == KEY_DOWN) { 
            i = (i == maxopt) ? 1 : i + 1;
        } 
        else if (key == KEY_ENTER) { 
            break;
        }
    } 
    
    disableRawMode();
    
    return i; // Return the selected option
}

int main(){
    enableANSI();
    
    // Uncomment these if you want to show them
    // Welcome();
    // printESI();
     printloading();
    
   int choice = Menu();
    
    printf("\033[2J\033[H");
    printf("You selected option %d\n", choice);
    
    return 0;
}