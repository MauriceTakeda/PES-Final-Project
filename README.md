# PES-Final-Project


## Acceleration Detector

### Summary
The purpose of this project is to build a linear acceleration detector using the FRDM-KL25z board. This acceleration detector works by changing the color of the RGB LED based on whether or not the board's linear acceleration is greater than a set target acceleration value. The project incorporates a UART command processor so that the user can input commands in order to interact with the program as it runs on the board. The user has the ability to set the RGB LED color that will be lit up upon reaching the target linear acceleration. The user also has the ability to set the target acceleration value in m/s^2, and print the measured acceleration values every 1 second. The color of the RGB LED should be white when the measured acceleration value is less than the target.

### Usage
After opening up a serial terminal (baud rate = 38400, data size = 8, parity = none, stop bits = 2) and flashing the FRDM-KL25z with project code, place the board flat on a surface. Move the board while keeping it flat. If the board is rotated as it is moving, the acceleration due to gravity will negatively affect the acceleration measurements. Upon reaching an acceleration greater than or equal to the target value, the RGB LED should change colors.

### Command Info
| Command | Arguments | Description | Example |
| --- | --- | --- | --- |
| color | r g b | Set target color with rgb values from 0-255 | color 250 30 30 |
| acceleration | target acceleration | Set target acceleration in m/s^2 | acceleration 1.8 |
| print | none | Print acceleration values every 1 second. Press any key to stop printing | print |

### Default Configuration
| Field | Value |
| --- | --- |
| target color | r=0, g=255, b=0 |
| target acceleration | 10.0 m/s^2 |


## Testing

### Test Outline
| Name | Type | Description |
| --- | --- | --- |
| cbfifo test | automatic | Test functionality of circular buffer API |
| cmd processor test | manual | Test both valid and invalid commands in UART command processor |
| system test | manual | Test the entire system functionality including RGB LED functionality and accelerometer measurements |

### Test Results

#### cbfifo test
This was a test done in software in order to ensure proper functionality of circular buffer API. The contents of the test are contained in the cbfifo.c file, and the test is run after peripherals are initialized in the main loop witin PES_Final_Project.c. The result of the test is that it was passed successfully.

#### cmd processor test
This test was done manually by typing in various commands (both valid and invalid) and ensuring the command processor responded as expected. The commands that were entered as well as the response can be viewed in the images below. Based on these images, the command processor test passed successfully.

![Manual_Command_Tests](https://user-images.githubusercontent.com/54907470/206053897-0a8c347b-6c12-4693-8e3d-e5a4a56fc0de.PNG)

![Manual_Command_Tests_2](https://user-images.githubusercontent.com/54907470/206054328-efc637b8-524f-44c9-80fa-148d5933baec.PNG)

#### system test
This test was done manually and tested the system functionality as a whole. The test can be viewed in the system.mov video, and shows the functionality of the RGB LED changing from white to the set target color value (r=30, g=30, b=250) upon reaching the target acceleration value (2.0 m/s^2). The video also shows the acceleration measurement prints every 1 second when the print command is active. Once any key is pressed the prints should stop, as they do in the video. The results of this test were also a success.
