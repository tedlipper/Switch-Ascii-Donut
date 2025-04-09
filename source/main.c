// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

// Include sdl2 headers
#include <SDL.h>
#include <SDL_mixer.h>

#include <unistd.h>
// for the donut
#include <math.h>

#define DEL 0x7F

int f;

// Rotation function replacing the R macro
void rotate(int mul, int shift, int *x, int *y) {
    f = *x;
    *x -= (mul * *y) >> shift;
    *y += (mul * f) >> shift;
    f = (3145728 - (*x) * (*x) - (*y) * (*y)) >> 11;
    *x = *x * f >> 10;
    *y = *y * f >> 10;
}

void UpdatedDonutDecider(int cA, int sA, int cB, int sB, int z[7200], char b[7200]);


//char gradient[19] = {' ', DEL, '.', '-', ':', ';', '!', '+', 's', 'T', 'Y', '$', '%', 'S', '8', 'A', '0', '#', '\0'};
char gradient[18] = {DEL, '.', '-', ':', ';', '!', '+', 's', 'T', 'Y', '$', '%', 'S', '8', 'A', '0', '#', '\0'};

//pacman has a 28 X 36 playfield

const int
    xAxis=79,                  // 160 for fullscreen
    yAxis=44,                   // 45 for fullscreen
    xScale=20,      //wider
    yScale=20,      //taller
    xLocation=40,//hihger further right
    yLocation=22,//higher further down
    totalChars=xAxis*yAxis;
const char 
    backgroundChar = ' ';

    float A = 0, B = 0;                 // angles of rotation

// Main program entrypoint
int main(int argc, char* argv[])
{
    // This example uses a text console, as a simple way to output text to the screen.
    // If you want to write a software-rendered graphics application,
    //   take a look at the graphics/simplegfx example, which uses the libnx Framebuffer API instead.
    // If on the other hand you want to write an OpenGL based application,
    //   take a look at the graphics/opengl set of examples, which uses EGL instead.
    consoleInit(NULL);

    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);

///////////////////////////////////////////music code
    Result rc = romfsInit();
    if (R_FAILED(rc))
        printf("romfsInit: %08X\n", rc);

    else
        printf("Press A button to play the sound!\n");

    // Start SDL with audio support
    SDL_Init(SDL_INIT_AUDIO);

    // Load support for the MP3 format
    Mix_Init(MIX_INIT_MP3);

    // open 44.1KHz, signed 16bit, system byte order,
    //  stereo audio, using 4096 byte chunks
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

    // Load sound file to use
    // Sound from https://freesound.org/people/jens.enk/sounds/434610/
    Mix_Music *audio = Mix_LoadMUS("romfs:/Donut_Theme.mp3");
///////////////musiccode

//usleep(1500000);

    // Other initialization goes here. As a demonstration, we print hello world.
  //  printf("Hello Emulator\n");

//*
    //donut code
 
    int z[totalChars];
    char b[totalChars];
    char outputBuffer[totalChars + yAxis]; // +yAxis for newlines
    int sA=1024,cA=0,sB=1024,cB=0,f;
    
//    */

Mix_PlayMusic(audio, -1); //Play the audio file

    // Main loop
    while (appletMainLoop())
    {

        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);

        // padGetButtonsDown returns the set of buttons that have been
        // newly pressed in this frame compared to the previous one
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus)
            break; // break in order to return to hbmenu

/*

        UpdatedDonutDecider(cA, sA, cB, sB, z, b);

        printf("\x1b[H");                   //actually moves the cursor to the top left
        
        for (int k = 0; k < yAxis; k++) 
        {
            for (int l = 0; l < xAxis; l++) 
            {
                putchar(b[(l + (k * xAxis))]);
            }
            printf("\n"); // Append newline after each row
        }
        rotate(8, 7, &cA, &sA);
        rotate(7, 8, &cB, &sB);
//*/


        // Update the donut and render the frame
        UpdatedDonutDecider(cA, sA, cB, sB, z, b);

        // Prepare the output buffer
        int idx = 0;
        for (int k = 0; k < yAxis; k++) {
            for (int l = 0; l < xAxis; l++) {
                outputBuffer[idx++] = b[(l + (k * xAxis))];  // Add character to buffer
            }
            outputBuffer[idx++] = '\n';  // Add newline after each row
        }
        outputBuffer[idx] = '\0';  // Null-terminate the string

        // Print the entire frame at once
        printf("\x1b[H");  // Move the cursor to the top left
        printf("%s", outputBuffer);  // Print the entire buffered output

        // Apply rotation for next frame
        rotate(8, 7, &cA, &sA);
        rotate(7, 8, &cB, &sB);
        usleep(30000);



        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }


    // Free the loaded sound
    Mix_FreeMusic(audio);

    // Shuts down SDL subsystems
    SDL_Quit();

    // Deinitialize and clean up resources used by the console (important!)
    romfsExit();


    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}


void UpdatedDonutDecider(int cA, int sA, int cB, int sB, int z[7200], char b[7200])
{
    for (int iterator=0; iterator < totalChars; iterator++)
    {
        b[iterator]=backgroundChar;
        z[iterator]=127;
    }

    int sj=0, cj=1024;
    for (int j = 0; j < 90; j++) 
    {
        int si = 0, ci = 1024;  // sine and cosine of angle i
        for (int i = 0; i < 324; i++) 
        {
            int R1 = 1, R2 = 2048, K2 = 5120*1024;
            int x0 = R1*cj + R2,
                x1 = ci*x0 >> 10,
                x2 = cA*sj >> 10,
                x3 = si*x0 >> 10,
                x4 = R1*x2 - (sA*x3 >> 10),
                x5 = sA*sj >> 10,
                x6 = K2 + R1*1024*x5 + cA*x3,
                x7 = cj*si >> 10,
                x = xLocation + xScale*(cB*x1 - sB*x4)/x6, //x pos and scale
                y = yLocation + yScale*(cB*x4 + sB*x1)/x6, // y pos and scale
                N = (-cA*x7 - cB*((-sA*x7>>10) + x2) - ci*(cj*sB >> 10) >> 10) - x5 >> 7;

            int o = x + xAxis * y; 
            int8_t zz = (x6-K2)>>15;
            if (yAxis > y && y > 0 && x > 0 && xAxis > x && zz < z[o]) 
            {
                z[o] = zz;
                b[o] = gradient[N > 0 ? N : 0];  //,?=m@
//                b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
            }
            rotate(5, 8, &ci, &si);  // Rotate i
        }

        rotate(9, 7, &cj, &sj);  // Rotate j
    }
}
