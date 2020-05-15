#define PATH_MAX 4096
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>

char* getSongFronTitle(char* window_name){
    int i;
    char* beatmap_title = window_name;
    for(i=0; i<strlen(window_name)-8; i++){
        beatmap_title[i] = window_name[i+8];
    }
    beatmap_title[i] = '\0';
    return beatmap_title;
}

int main(int argc, char *argv[]){
    Display *display;
    Window focus = 0;
    int revert;

    char* current_window_title = "";
    char* old_window_title = "";
    char path[PATH_MAX];

    FILE* now_playing;

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
    while(current_window_title == "" ||strstr(current_window_title, "osu!") == NULL){
        if(!XGetInputFocus(display, &focus, &revert)) continue;
        if(!XFetchName(display, focus, &current_window_title)){
            current_window_title=""; 
            continue;
        }    
        if(current_window_title == NULL) continue;
    }
    
    printf("Found osu! window.\n");

    while(1){
        XFetchName(display, focus, &current_window_title);
        if(current_window_title == NULL || strstr(current_window_title, "osu!") == NULL){
            continue;
        }

        // If playing beatmap then remove unneeded 'osu! - ' part 
        if(strcmp(current_window_title, "osu!") != 0){
            current_window_title = getSongFronTitle(current_window_title);
        }

        // Update the file only if new window title
        if(strcmp(current_window_title, old_window_title) != 0){
            printf("Set current_window_name to: %s\n", current_window_title);
            now_playing = fopen(path, "w+");
            if(strcmp(current_window_title, "osu!") == 0){
                fputs("Selecting a song...", now_playing);
            } else {
                fputs(current_window_title, now_playing);
            }
            old_window_title = current_window_title;
            fclose(now_playing);
        }
    }
    return 0;
}