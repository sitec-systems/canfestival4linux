This is a fork of the CanFestival-3 project http://dev.automforge.net/CanFestival-3

Latest work done:

- I needed the stack to be more dynamic, i wanted to be able to dynamically build the OD and the CO_Data struct without any global declaration, so i have made few changes. (this is not a dirty hack it is even cleaner i think)

- solving array of string or domain issue (search for "Array of strings issue" in the mailing list)

- solving bugs on sdo block transfer and dcf management

- stm32F0/F1/F4 basic support

Any feedback, comments, tests results (on other platforms than Linux would be great) are welcome.

You can contact me at : 
fbeaulier < a t > ingelibre < d o t > fr