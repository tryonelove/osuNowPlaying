#define PATH_MAX 4096
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void getSongFronTitle(unsigned char** window_name){
    int i;
    for(i=0; i<strlen(*window_name)-8; i++){
        (*window_name)[i] = (*window_name)[i+8];
    }
    (*window_name)[i] = '\0';
}

int main(int argc, char *argv[]){
    Display *display;
    Window focus = 0;
    int revert;
    char path[PATH_MAX];
    char old_window_title[255];
    XTextProperty current_window_title;
    FILE* now_playing;

    current_window_title.value = NULL;

    if(argc < 2) {
        getcwd(path, sizeof(path));
    } else {
        strcpy(path, argv[1]);
    }

    strcat(path, "/now_playing.txt");
    printf("Using the following now_playing.txt location: %s\n", path);
    display = XOpenDisplay(NULL);

    // Wait until osu! window is active
    // WARNING: FOR SOME REASONG IT CAN'T FETCH SOME WINDOW NAMES
    // SO THE BEST WAY IS TO OPEN OSU RIGHT AFTER PROGRAM LAUNCH
    while(current_window_title.value == NULL ||strstr(current_window_title.value, "osu!") == NULL){
        if(!XGetInputFocus(display, &focus, &revert)) continue;
        if(!XGetWMName(display, focus, &current_window_title)){
            strcpy(old_window_title, "");
            continue;
        }    
        if(current_window_title.value == NULL) continue;
    }
    
    printf("Found osu! window.\n");

    while(1){
        XGetWMName(display, focus, &current_window_title);
        if(current_window_title.value == NULL || strstr(current_window_title.value, "osu!") == NULL){
            continue;
        }
        if(strcmp(current_window_title.value, "osu!") != 0){
            getSongFronTitle(&current_window_title.value);
        }

        // // If playing beatmap then remove unneeded 'osu! - ' part 

        // Update the file only if new window title
        if(strcmp(current_window_title.value, old_window_title) != 0){
            printf("Set current_window_name to: %s\n", current_window_title.value);
            now_playing = fopen(path, "w+");

            if(strcmp(current_window_title.value, "osu!") == 0){
                fputs("Selecting a song...", now_playing);
            } else {
                fputs(current_window_title.value, now_playing);
            }
            int length = strlen(current_window_title.value);
            strncpy(old_window_title, current_window_title.value, length+1);
            old_window_title[length]='\0';
            XFree(current_window_title.value);

            fclose(now_playing);
        }
        usleep(100000);
    }
    return 0;
}