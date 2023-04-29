# AT89S52-Frequency-generator

This project aims to design and implement a system that can generate a square 
wave with different frequencies up to 9999 Hz using Atmel's AT89S52 microcontroller. 

The system consists of four main components: a hex keypad, a quad seven segment 
display, a push button, and a LED on pin P3.7. The following steps describe how 
the system works:

1. The user enters the desired frequency value (in Hz) via the hex keypad, which is 
connected to port P1 of the microcontroller. The hex keypad has 16 keys and is 
scanned by the software to determine which key is pressed.

2. The entered frequency value is displayed on the quad seven segment display, 
which is connected to port P2 of the microcontroller. The quad seven segment 
display has four common anode digits that can show numbers from 0 to 9. The 
segments of each digit are controlled by port P2, while the common anodes are 
controlled by pins P0.0 to P0.3 through 2N3906 transistors. The display is 
refreshed using timer 0 interrupt. Each time a key is pressed, the displayed value is 
shifted to the left until the push button is pressed , keeping the last entered four 
digits only.

3. After entering the frequency value, the user presses the push button, which is 
connected to pin INT0 of the microcontroller. This triggers an external interrupt 
that loads timer 1 with the appropriate values and enables square wave generation 
on pin P3.7 of the microcontroller. The square wave has a frequency equal to the 
entered value and a duty cycle of 50%.

4. If the user wants to enter a new frequency value, they can do so while the signal 
is being generated normally ,after the value is entered the user presses the push 
button again, which will resume square wave generation with the new entered 
frequency , this step can be repeated for as many times as desired.
