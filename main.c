#define PATH_MAX 4096
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>



int main(int argc, char *argv[]){
    Display *display;
    Window focus = 0;
    int revert;

    char* current_window_name = "";
    char* old_window_name = "";
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
    while(current_window_name == "" ||strstr(current_window_name, "osu!") == NULL){
        if(!XGetInputFocus(display, &focus, &revert)) continue;
        if(!XFetchName(display, focus, &current_window_name)){
            current_window_name=""; 
            continue;
        }    
        if(current_window_name == NULL) continue;
    }
    
    printf("Found osu! window.\n");

    while(1){
        XFetchName(display, focus, &current_window_name);
        if(current_window_name == NULL || strstr(current_window_name, "osu!") == NULL){
            continue;
        }
        if(strcmp(current_window_name, old_window_name) != 0){
            old_window_name = current_window_name;
            printf("Set current_window_name to: %s\n", current_window_name);
            now_playing = fopen(path, "w+");
            if(strcmp(current_window_name, "osu!") == 0){
                fputs("Selecting a song...", now_playing);
            } else {
                fputs(current_window_name, now_playing);
            }
            fclose(now_playing);
        }
    }
    return 0;
}