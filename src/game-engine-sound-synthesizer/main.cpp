/*
    Sound Synthesizer

    - For learning purposes. Based on https://www.youtube.com/watch?v=tgamhuQnOkM.
    - https://github.com/OneLoneCoder/synth/tree/master

    - g++ -o main.exe main.cpp olcNoiseMaker.h  -l Winmm
    - this helped compiling: https://github.com/OneLoneCoder/synth/issues/14
    - not working properly yet. Outputs a sound and then segmentation fault
    - have to learn more sbout sound waves, sin waves, and synth in general
*/

// have to include winmm.lib!

#include <iostream>

#include "olcNoiseMaker.h"

std::atomic<double> frequencyOutput = 0.0;
//double frequencyOutput = 0.0;
double octaveBaseFrequency = 110.0; // A2
double _12thRootOf2 = pow(2.0, 1.0 / 12.0);

double makeNoise(double dTime);


int main()
{
    std::wcout << "Synthesizer" << std::endl;

    // Get all sound hardware
    std::vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

    // Display findings
    for (auto d : devices)
    {
        std::wcout << "Found output device:" << d << std::endl;
        break; // testing finding only one
    }

    // Display a keyboard
	std::wcout << std::endl <<
		"|   |   |   |   |   | |   |   |   |   | |   | |   |   |   |" << std::endl <<
		"|   | S |   |   | F | | G |   |   | J | | K | | L |   |   |" << std::endl <<
		"|   |___|   |   |___| |___|   |   |___| |___| |___|   |   |__" << std::endl <<
		"|     |     |     |     |     |     |     |     |     |     |" << std::endl <<
		"|  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |" << std::endl <<
		"|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|" << std::endl << std::endl;


    // Create sound machine
    olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

    // Link noise function with sound machine
    sound.SetUserFunction(makeNoise);

   /* while(1)
    {
        // Add a keyboard
        std::wcout << "while" << std::endl;
        if(GetAsyncKeyState('A') & 0x8000)
        {
            frequencyOutput = octaveBaseFrequency * pow(_12thRootOf2, 1);
        }
        else
        {
            frequencyOutput = 0.0;
        }
        Sleep(500);
    }
    std::wcout << "after while" << std::endl;
    */

    int currentKey = -1;
    bool keyPressed = false;
    while (1)
    {
        keyPressed = false;
        for (int k = 0; k < 16; k++)
        {
            if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k])) & 0x8000)
            {
                if (currentKey != k)
                {
                    frequencyOutput = octaveBaseFrequency * pow(_12thRootOf2, k);
                    wcout << "\rNote On : " << sound.GetTime() << "s " << frequencyOutput << "Hz";
                    currentKey = k;
                }
                keyPressed = true;
            }
        }

        if (!keyPressed)
        {
            if (currentKey != -1)
            {
                std::wcout << "\rNote Off: " << sound.GetTime() << "s                        ";
                currentKey = -1;
            }
            frequencyOutput = 0.0;
        }
    }

    return 0;
}

/*double makeNoise(double dTime)
{
    // triangle wave
    double output = 1.0 * sin(220.0 * 2 * 3.14159 * dTime); // sin wave
    
    if(output > 0.0)
    {
        return 0.7;
    }
    else
    {
        return -0.7;
    }
}*/

double makeNoise(double dTime)
{
   // std::wcout << "make noise" << std::endl;
    Sleep(1);
    // return sine wave - 440 is frequancy - changes in this function will change sine wave form
    double output = sin(frequencyOutput * 2.0 * 3.14159 * dTime);
    return 0.5 * output; // sin wave
}

/*

Note On : 0.0115646s 146.832HzSegmentation fault
--------------------------------------------------------
vivi_@LAPTOP-8BV53D7K MINGW64 /c/github/cpp-projects/sound-synthesizer
$ ./main.exe
Synthesizer
Found output device:Alto-falantes (Realtek High Def

|   |   |   |   |   | |   |   |   |   | |   | |   |   |   |
|   | S |   |   | F | | G |   |   | J | | K | | L |   |   |
|   |___|   |   |___| |___|   |   |___| |___| |___|   |   |__
|     |     |     |     |     |     |     |     |     |     |
|  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |
|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|

Note Off: 0.0114966s                        Segmentation fault
----------------------------------------------------
Note On : 0.01161s 110HzSegmentation fault

IT KEEPS GIVING SEG FAULT AT TIME 0.011s
*/