/*
 * Time Suite - Clock, stopwatch, and countdown timer.
 * Windows console; uses conio.h for key input.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

typedef enum{
    CLOCK, STOPWATCH, TIMER, EXIT, INVALID
}Choice;

Choice showMenu(void);
void runClock(void);
void runStopWatch(void);
void runTimer(void);

int main(void){
    Choice selection;

    printf("****WELCOME TO THE TIME SUITE****\n");
    do{
        selection = showMenu();
        switch(selection){
            case CLOCK:
                runClock();
                break;
            case STOPWATCH:
                runStopWatch();
                break;
            case TIMER:
                runTimer();
                break;
            case EXIT:
                printf("\033[H\033[J");
                printf("Thank you for visiting THE TIME SUITE =D\n");
                break;
            default:
                printf("\033[H\033[J");
                printf("You did not select one of the choices. Please try again!\n");
                break;

        }

    }while(selection != EXIT);

    return 0;
    }

/* Prompt for CLOCK, STOPWATCH, TIMER, or EXIT; return matching enum or INVALID. */
Choice showMenu(void){
    char choice[50] = "";

    printf("Type in what you would like to access:\n");
    printf("- CLOCK\n");
    printf("- STOPWATCH\n");
    printf("- TIMER\n");
    printf("- EXIT\n");
    printf("Enter here: ");
    fgets(choice, sizeof(choice), stdin);
    for(int i = 0; choice[i] != '\0'; i++){
        if(choice[i] == '\n'){
            choice[i] = '\0';
            break;
        }
        else{
            choice[i] = toupper((unsigned char)choice[i]);  /* safe for all char values */
    }
    }
    if(strcmp(choice, "CLOCK") == 0){
        return CLOCK;
    }
    else if(strcmp(choice, "TIMER") == 0){
        return TIMER;
    }
    else if(strcmp(choice, "STOPWATCH") == 0){
        return STOPWATCH;
    }
    else if(strcmp(choice, "EXIT") == 0){
        return EXIT;
    }
    else{
        return INVALID;
    }
}

/* Live digital clock; B returns to menu. */
void runClock(void){
    printf("\033[H\033[J");
    time_t rawtime = 0;
    struct tm *pTime = NULL;
    char key = '\0';

    printf("****DIGITAL CLOCK****\n");
    printf("B = BACK TO MENU\n\n");
    while(key != 'B'){
        if(_kbhit()){
            key = _getch();
            key = toupper((unsigned char)key);
        }
        time(&rawtime);
        pTime = localtime(&rawtime);
        printf("\r%02d:%02d:%02d", pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
        fflush(stdout);
        Sleep(50);
        }
        printf("\033[H\033[J");
}

/* Stopwatch with laps; S start, R pause/resume, Space lap/reset, B back. */
void runStopWatch(void){
    printf("\033[H\033[J");
    int lapCount = 0;
    long long start_ms = 0;
    long long now_ms = 0;
    long long elapsed_ms = 0;
    long long stored_ms = 0;
    
    bool isRunning = false;
    bool beenRunning = false;
    char key = '\0';

    printf("****STOPWATCH****\n");
    printf("S =  START | R = STOP/RESUME | SPACE = LAP/RESET | B = BACK TO MENU\n");
    printf("00:00:00.000");

    while(key != 'B'){
        if(_kbhit()){
            key = _getch();
            key = toupper((unsigned char)key);

            switch(key){
                case 'S':
                    if(!isRunning && !beenRunning){
                        start_ms = GetTickCount64();
                        stored_ms = 0;
                        isRunning = true;
                        beenRunning = true;
                    }
                    break;
                case ' ':
                    if(isRunning){
                        lapCount += 1;
                        printf("\rLap %d: ", lapCount);

                        now_ms = GetTickCount64();
                        elapsed_ms = stored_ms + (now_ms - start_ms);

                        int ms = elapsed_ms % 1000;

                        long long elapsed_seconds = elapsed_ms / 1000;
                        int seconds = elapsed_seconds % 60;

                        long long elapsed_minutes = elapsed_seconds / 60;
                        int minutes = elapsed_minutes % 60;

                        int hours = elapsed_minutes / 60;

                        printf("%02d:%02d:%02d.%03d\n", hours, minutes, seconds, ms);
                        fflush(stdout);
                    }
                    else{
                        lapCount = 0;
                        start_ms = 0;
                        now_ms = 0;
                        elapsed_ms = 0;
                        stored_ms = 0;
                        beenRunning = false;
                        isRunning = false;

                        printf("\033[H\033[J");
                        printf("****STOPWATCH****\n");
                        printf("S =  START | R = STOP/RESUME | SPACE = LAP/RESET | B = BACK TO MENU\n");
                        printf("00:00:00.000");
                    }
                    break;
                case 'R':
                    if(beenRunning){
                        if(isRunning){
                        stored_ms = elapsed_ms;
                        isRunning = false;
                    }
                    else{
                       start_ms = GetTickCount64();
                       isRunning = true;
                    }
                }
                    break;
                case 'B':
                    break;
                default:
                    break;
            }
        }
            if(isRunning){
                now_ms = GetTickCount64();
                elapsed_ms = stored_ms + (now_ms - start_ms);
            }
                int ms = elapsed_ms % 1000;

                long long elapsed_seconds = elapsed_ms / 1000;
                int seconds = elapsed_seconds % 60;

                long long elapsed_minutes = elapsed_seconds / 60;
                int minutes = elapsed_minutes % 60;

                int hours = elapsed_minutes / 60;

                printf("\r%02d:%02d:%02d.%03d", hours, minutes, seconds, ms); 
                fflush(stdout);
            Sleep(50);
        }
    printf("\033[H\033[J");
}

/*
Countdown timer. Accepts SS, MM:SS, or HH:MM:SS
After entering time: S start, R pause/resume, Space cancel, Escape menu
 */
void runTimer(void){
    char input[50] = "";
    long total_seconds = 0;
    long hours = 0;
    long minutes = 0; 
    long seconds = 0;
    int key = 0;
    bool running = false;
    bool paused = false;
    long remaining_seconds = 0;
    long long pause_start_ms = 0;

    while(1){
        printf("\033[H\033[J");
        printf("****TIMER****\n");
        printf("Enter time: SS, MM:SS, or HH:MM:SS\n");
        printf("Note: Typing in 90 will set the timer to 1:30\n");
        printf("Press ESC to go back to menu\n");
        printf("Enter here: ");
        fflush(stdout);
        input[0] = '\0';
        int len = 0;
        /* Read line character-by-character so Escape can return to menu */
        while(len < (int)sizeof(input) - 1){
            int c = _getch();
            if(c == 0 || c == 0xE0){  /* extended key; consume scan code */
                (void)_getch();
                continue;
            }
            if(c == 27){  /* Escape */
                printf("\033[H\033[J");
                return;
            }
            if(c == '\r' || c == '\n'){
                input[len] = '\0';
                break;
            }
            if(c == '\b' && len > 0){
                len--;
                printf("\b \b");
                continue;
            }
            if(c >= 32 && c < 127){
                input[len++] = (char)c;
                input[len] = '\0';
                putchar(c);
            }
        }
        if(len == 0 && input[0] == '\0'){
            continue;
        }
        printf("\n");

        /* Parse HH:MM:SS, MM:SS, or seconds-only */
        hours = 0;
        minutes = 0;
        seconds = 0;
        total_seconds = 0;
        if(sscanf(input, "%ld:%ld:%ld", &hours, &minutes, &seconds) == 3){
            total_seconds = hours * 3600 + minutes * 60 + seconds;
        }
        else if(sscanf(input, "%ld:%ld", &minutes, &seconds) == 2){
            total_seconds = minutes * 60 + seconds;
        }
        else if(sscanf(input, "%ld", &seconds) == 1){
            total_seconds = seconds;
        }

        if(total_seconds <= 0){
            printf("Invalid input. Please try again.\n");
            Sleep(400);
            continue;
        }

        hours = total_seconds / 3600;
        minutes = (total_seconds % 3600) / 60;
        seconds = total_seconds % 60;

        printf("\033[H\033[J");
        printf("****TIMER****\n");
        printf(" S = START | R = STOP/RESUME | SPACE = CANCEL | ESCAPE = BACK TO MENU\n\n");
        if(hours > 0){
            printf("  Time: %ld:%02ld:%02ld\n", hours, minutes, seconds);
        }
        else{
            printf("  Time: %ld:%02ld\n", minutes, seconds);
        }

        /* Wait for S (start), Space (cancel), or Escape (menu) */
        while(1){
            if(_kbhit()){
                key = _getch();
                if(key == 0 || key == 0xE0){
                    (void)_getch();
                    key = 0;
                    continue;
                }
                key = toupper((unsigned char)key);
                if(key == 27){
                    printf("\033[H\033[J");
                    return;
                }
                if(key == ' '){
                    break;
                }
                if(key == 'S' && !running){
                    running = true;
                    remaining_seconds = total_seconds;
                    paused = false;
                    pause_start_ms = 0;

                    break;
                }
            }
            Sleep(50);
        }

        if(key == ' '){
            continue;
        }

        /* Update the "Time:" line in place (cursor up one, overwrite with \r) */
        printf("\033[1A\r");
        long long last_tick = GetTickCount64();
        while(running && remaining_seconds > 0){
            if(_kbhit()){
                key = _getch();
                if(key == 0 || key == 0xE0){
                    (void)_getch();
                    continue;
                }
                key = toupper((unsigned char)key);
                if(key == 27){
                    running = false;
                    printf("\033[H\033[J");
                    return;
                }
                if(key == ' '){
                    running = false;
                    break;
                }
                if(key == 'R'){
                    if(paused){
                        /* Resume: adjust last_tick forward by the pause duration */
                        long long pause_duration = GetTickCount64() - pause_start_ms;
                        last_tick += pause_duration;
                        paused = false;
                    }
                    else{
                        /* Pause: record when pause started */
                        pause_start_ms = GetTickCount64();
                        paused = true;
                    }
                }
            }

            /* Decrement remaining time every second, excluding paused time. */
            if(!paused){
                long long now = GetTickCount64();
                long long elapsed = now - last_tick;
                if(elapsed >= 1000){
                    int ticks = (int)(elapsed / 1000);
                    remaining_seconds -= ticks;
                    if(remaining_seconds < 0){
                        remaining_seconds = 0;
                    }
                    last_tick += (long long)ticks * 1000;
                }
            }

            if(remaining_seconds <= 0){
                break;
            }

            hours = remaining_seconds / 3600;
            minutes = (remaining_seconds % 3600) / 60;
            seconds = remaining_seconds % 60;
            if(hours > 0){
                printf("\r  Time: %ld:%02ld:%02ld  ", hours, minutes, seconds);
            }
            else{
                printf("\r  Time: %ld:%02ld  ", minutes, seconds);
            }
            Sleep(50);
        }

        if(remaining_seconds <= 0 && running){
            printf("\r  Time: 0:00  \n");
            printf("  Timer complete!\n");
            printf("  Press any key to continue...\n");
            fflush(stdout);
            _getch();
        }
        running = false;
    }
    printf("\033[H\033[J");
}
    

