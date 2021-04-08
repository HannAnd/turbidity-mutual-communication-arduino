Test code for mutual communication between a singular Raspberry Pi (acting as primary controller)
and one or more Arduino Nano Everys.
The Arduino is connected to an analog turbidy sensor (dfrobot, Gravity series) and a 4-channel
active low relay (SainSmart).
The Raspberry Pi sends commands to the Arduino to take turbidity readings at regular intervals
and commands the Arduino to open/close relay channels based on the reading it receives.
The Raspberry Pi code is calibrated to recognize the turbidity levels of plain water, a weak instant
coffee solution, a strong instant coffee solution or clear air to turn on channel 1, channel 2,
channel 3 or channel 4, respectively (with all channels turning off if the reading is not within the
calibrated values).
The Arduino sends turbidity readings when requested (an average of five readings) and opens/closes
relay channels based on the commands of the Pi.


Known problems:
-The Arduino will not open/close relay channels in response to the first turbidity reading, only the
readings after the first.
-The turbidity readings lag after moving the sensor: the first reading after moving the sensor will
still be reflective of the previous detected substance, though it has usually corrected by the second.
Increasing or decreasing the delay between readings doesn't affect the lag.

Features in progress:
-Ability to send more than one type of variable from the Arduino to the Raspberry Pi (such as a string
to communicate that the relay channel has been flipped).
-Raspberry Pi logging the turbidity readings in a csv file, with the ability to add new entries to the
file each time the code is initiated rather than overwriting the existing file.
