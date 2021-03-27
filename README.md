# 2020-COMP3016-Coursework2

[View tech pitch](https://youtu.be/-pBKL0F48SU)

## Project

### Planned Outcome
A low poly terrain generator that allows the user to select an audio file to act as a seed. The user should be able to move around to experience the environment generated and adjust the colours of the environment.

![End Goal](/DocumentationImages/EndGoal.png?raw=true)

### Actual Outcome
An audio visualizer that reads in .csv files and outputs points (3D coordinates) rendered with OpenGL in clip space. The .csv files that are read in contain time, frequency and amplitude data abstracted from audio files.

![Screenshot](/DocumentationImages/screenshot1.png?raw=true)

## Demonstation Video


## How to make the executable
If you are viewing this project on GitHub you will not find a build of the project. If you wish to obtain an exe you will have to follow these steps. 
1. Download this repository & unzip the folder.
2. Open the solution (Coursework2.sln) in visual studio.
3. In the Solution Explorer right click the solution.
4. Select Build Solution.
5. You can now close visual studio.
6. In the project folder you will find another folder called media. Copy this.
7. Navigate to the build by entering the x64 folder and then the Debug folder.
8. Paste the media folder here.
9. You now have an executable project folder. You can move Debug anywhere you like and rename it.

## How to use
Once you have your executable project folder you can run the project by clicking Coursework2.exe.

### Visualizer
In the top right corner you have the visualizer. This is where you can view the audio data as points. When you load in you will be able to example data displayed in full. If you reach the end of the song text will display indicating this.
### Settings
The settings are located in the bottom left. This is where your main interaction will take place.
#### Songs
Here you have 3 buttons. When you click each one the program will load the data for the corresponding song. Due to the volume of data being loaded this may take some time so please be patient. You will know that the data has loaded when it appears in the visualizer.
#### Colours
Here you have 4 options: white, green, blue and red. By selecting each option you will be able to change the colour of the visualizer. You don't need to wait until the end of the song, the changes are instant.
### Name
This displays the name of the song being displayed. It is located underneath the visualizer.
### Play
When you load data it will originally be paused. Clicking play will scroll through the data.
### Pause
If you want to have a look at the data at a certain point you can click pause to stop scrolling through the data.
### Restart
Once you reach the end of the song you can go back to the begining by clicking restart. You can also click at any point in the song. The data will always start in the visualizer paused. 

## How it works
My program relies on .csv files that contain audio data taken from audio files. An audio file contains a lot of data but to be able to access that data you must first decode it. As I want to find the individual frequencies and amplitudes of each sample after decoding the audio I had to run the data through a fourier transform. As I struggled to this in C++ I created a python script so that I could focus on processing the obtained data.

![Sound decoding](/DocumentationImages/SoundDecoding.png?raw=true)

We have multiple songs that we can choose from. When a song is picked it will load the .csv data into the program. As we will be rendering in clip space we requires the values to be between -1 and 1 so we have to normalise our data using the equation below. This is done seperately for each value of frequency, amplitude and time as each group requires their own min and max value. Only normalised data of the current song is stored.

```
X' = (X - Xmin) / (Xmax - Xmin)
```

Once everything has been normalised we need to put that data into a vector as coordinates. We will be plotting time on the x axis, amplitude on the y axis and frequency on the z axis. These coordinates are loaded into the buffer then rendered as GL_Points using vertex and fragment shaders.

To allow user interaction we have buttons. These are multiple triangles rendered into clip space. Input is processed using glfw. To determine which button has been clicked the program takes the current mouse position. The mouse position outputs as the pixel position.

## Bugs
When playing the data at the very end the values do not move towards the left to leave space on the right. Instead the gap between each time increases resulting in an expanding appearance. This can be easily seen by clicking play on the test data.

## Where did my idea come from
A lot of people order personalised soundwave prints to hang on their walls, engrave into rings or make soundwave keyrings. I thought that I could take this facination with soundwaves into the virtual world and create a completely personal enviroment for people to enjoy.

At the start of the project I was supplied with a C++ & OpenGL project that displayed a 3D cube. This code contained everything needed to load and use shaders & textures. As I wanted to fully understand how shaders worked I deleted everything that was provided to me. All my shaders are written at the top of the of Project.cpp as a result.

Besides drawText(), the libraries and the assets used I everything was coded from scratch.

## Credit
### Music
* Song 1 - [slow motion from bensound](https://www.bensound.com/royalty-free-music/track/slow-motion)
* Song 2 - [energy from bensound](https://www.bensound.com/royalty-free-music/track/energy)
* Song 3 - [dubstep from bensound](https://www.bensound.com/royalty-free-music/track/dubstep)

### Code 
*  drawText() - [Ramazan Bellek](https://www.youtube.com/watch?v=elE__Nouv54)

### Libraries
* freeglut
* glfw
