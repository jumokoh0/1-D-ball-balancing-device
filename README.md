# 1-D-ball-balancing-device
The code can be viewed in the 'source' file.

-------------------------

+ Device
-The distance of the ball is measured with an ultrasonic sensor and the distance is adjusted with a servo motor.
-The size of the styrofoam ball is small, so it is used as a large ball made using a 3D printer
![fullshot](https://github.com/jumokoh0/1-D-ball-balancing-device/blob/master/image/fullshot.jpg?raw=true)

-----------------------------------------

+ Programming
-Low pass filter and average filter are used due to low precision of ultrasonic sensor
-As the number of samples increases, a delay occurs, so there is some compromise.
-The distance is controlled through PID control, and P gain, I gain, and D gain are experimentally obtained.

---------------------------------------

+ Operation video
<https://youtu.be/EFqq0nyB4UI>