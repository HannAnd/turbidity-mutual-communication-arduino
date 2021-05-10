Test code for mutual communication between a singular Raspberry Pi (acting as primary controller)
and one or more Arduino Nano Every.
The Arduino is connected to an analog turbidity sensor (dfrobot, Gravity series) and a 4-channel
active low relay (SainSmart).
The Raspberry Pi sends commands to the Arduino to take turbidity readings at regular intervals
and commands the Arduino to open/close relay channels based on the reading it receives.
The Raspberry Pi code is calibrated to recognize the turbidity levels of plain water, a weak instant
coffee solution, a strong instant coffee solution or clear air to turn on channel 1, channel 2,
channel 3 or channel 4, respectively (with all channels turning off if the reading is not within the
calibrated values).
The Arduino sends turbidity readings when requested (an average of five readings) and opens/closes
relay channels based on the commands of the Pi.

Features in progress:
-Ability to send more than one type of variable from the Arduino to the Raspberry Pi (such as a string
to communicate that the relay channel has been flipped).
