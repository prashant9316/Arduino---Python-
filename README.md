# Arduino---Python-
This Project interfaces Arduino Uno using python to detect knocks and unlock gate if it detects desired faces using LBPH algorithm  
We are going to interface the Arduino using the python serial library.  
I have modified the pyduino library and some more functions usefull for this project.  
You can directly import the class Arduino from this library and create a class as shown below:  
from pyduino import Arduino  
arduino = Arduino('COM11',9600)  
COM11 is the Port and 9600 is the baud rate   
