# Electronics Project
These were made during one of my AUT classes called electronic project, where the power supply and piano were drawn in Altium Designer and assembled by hand.

## Power Supply
<img src="power_supply.jpg" width="250" height="250">

The PSU is a half-wave bridge dual rail power supply which gave me the required voltage outputs of ±12 volts. This supplied a regulator which gave an additional 5-volt output which served the purpose of driving the load of the piano.

## Piano

<img src="digital_and_analog_piano.jpg" width="250" height="250">

### Analogue Piano
The analogue piano circuit uses a button in series with a particular value of resistor to change the time taken for the capacitor to charge and discharge. Each time the capacitor becomes full charged the 555-timer resets, and when completely discharged it becomes set. Values for the desired frequency were calculated using the formula:

 $ f = (1.44 / (Rn+2(R9)) C3) $ 

where Rn is Resistor 1 to 8 depending on the button pushed.
The advantage of this circuit is that the 555-timer is cheaper than a microcontroller, the calculations are simple and easy to implement. The disadvantage is that the design requires a good understanding of analogue electronics and the 555-timer.

### Digital Piano

The digital piano uses a single button with connected to a microcontroller (uC) which has an internal pull-up resistor. The uC polls when the input goes low due to a button press. When this goes low, it outputs to the base of a BJT which controls the current to the speaker. By using a timer for fast PWM which has a 50% duty cycle on the uC, a range of piano notes were able to be set. A button debounce was also implemented using code, which checks for a button press, then introduces a delay to allow any initial noise to stabilize before executing the intended code.

The advantage of this circuit is that there are few components, and it is easy to replicate once the code has been written. The disadvantage is that it requires a lot of upfront work, code to be written and a deep understand of microcontrollers and their datasheets.
