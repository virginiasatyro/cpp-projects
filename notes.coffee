explicação sobre como rodar o programa no windows e programando no linux

https://seanthegeek.net/234/graphical-linux-applications-bash-ubuntu-windows/

como compilar no linux 

https://github.com/OneLoneCoder/olcPixelGameEngine/wiki/Compiling-on-Linux

g++ -o olcExampleProgram olcExampleProgram.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

run no linux

./olcGame...


g++ -o main main.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

-------------
XLaunch

É necessário configurar toda vez que for entrar, usar configurações padrão;


creating a token to do github push

https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token

https://github.com/Joseph21-6147/From-ConsoleGameEngine-to-PixelGameEngine/blob/main/OneLoneCoder_Matrix.PGE.cpp

###############################################################################################################

SPLINES - Programming & Using Splines - Part#1

DATA: 10/01/2024

    - Splines, usadas em desenhos de engenharia, são réguas flexíveis, de madeira ou plástico, 
    que podem ser curvadas de forma a passar por um dado conjunto de pontos (xi,fi) chamados nós.
    Um spline é uma curva definida matematicamente por dois ou mais pontos de controle. 
    Os pontos de controle que ficam na curva são chamados de nós.

    - based on, for learning purposes: https://www.youtube.com/watch?v=9_aJGUTePYo

    - https://pt.wikipedia.org/wiki/Spline

    - use multiple control points and the path is garanteed to go on all points - CATMULL-ROM

    - https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline (Catmull–Rom spline)
    
    - run:  g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

    - TODO: 
        - play with the splines to generate the path of a speceship;
        - automatically generate the looped spline;
        - calculate the gradient so the spaceship will know where to point the nose;
        - interesting for charecter AI;

###############################################################################################################

Programming Mazes

DATA: 05/11/2023

    - Based on: https://www.youtube.com/watch?v=Y37-gB83HKE - for learning purposes

    - https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/SmallerProjects/OneLoneCoder_Mazes.cpp

    -  g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

    - TODO:
        - what is flood fill algorithm?
        - write on paper the math and logic
        - use this algorthm in a game


###############################################################################################################

Code-It-Yourself! Sound Synthesizer #2 - Oscillators & Envelopes

DATA: 12/10/2023

    - https://www.youtube.com/watch?v=OSCzKOqtgcA&t=26s
    - TO DO <<<<<<<<<<<<<<<<<<<<<<<<<<<----------------------------->
    - Como essa é a continuação do outro projeto, não apareceream conceitos tão novos;
    - Melhor esperar e voltar para esse projeto com mais calma, depois de conseguir rodar o som propriamente;
    

###############################################################################################################

Code-It-Yourself Synthesizer Sneak Peak (Moonlight Sonata 3rd)

DATA: 12/10/2023

    - https://www.youtube.com/watch?v=Wj76dEyt__U
    - Projeto incrível!
    - Teclas de piano subindo o  prompt ao som de Moonlight Sonata 3rd
    - Mas não tem a explicação do projeto nesse vídeo

###############################################################################################################

Code-It-Yourself! Sound Synthesizer #1 - Basic Noises

DATA: 12/10/2023

    - tem grande potencial para se tornar um projeto muito massa!

    - For learning purposes. Based on https://www.youtube.com/watch?v=tgamhuQnOkM.
    - https://github.com/OneLoneCoder/synth/tree/master

    - COMPILE: g++ -o main.exe main.cpp olcNoiseMaker.h  -l Winmm
    - this helped compiling: https://github.com/OneLoneCoder/synth/issues/14
    - not working properly yet. Outputs a sound and then segmentation fault
    - have to learn more sbout sound waves, sin waves, and synth in general

###############################################################################################################


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FIREWORKS

VIDEO 
https://www.youtube.com/watch?v=OmTD5IoqHJU

TO DO: 
- particles should be affected by gravity

==============================================================================================================

    interesting but not quiet related

    https://github.com/rougier/CPP-Crash-Course

    https://www.shadertoy.com/

    https://www.shadertoy.com/view/DtjSWh

    https://github.com/rougier/URFIST-git-course

    https://github.com/rougier/ML-Recipes

###############################################################################################################

UNIVERSE 

https://www.youtube.com/watch?v=ZZY9YE7rZJw&list=PLrOv9FMX8xJEEQFU_eAvYTsyudrewymNl&index=29

procedural generation - control of randomness

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

PROJECTS ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
