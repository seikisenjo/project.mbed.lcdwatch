# Prototype (Embedded device - Handheld LCD watch)

# Embedded device - Handheld LCD watch powered by LPC1768

# Fully functional watch with ability to:
  - adjust time whilst running
  - set alarm and notify user on time
  - light up LCD screen based on user input
    
# System Overview

  Initialize device (LCD, GPIO, Interrupt)
                          →
             Display current time on LCD screen
                          →
               seconds++ every 1000ms on timer interrupt 
                          →
  Several other supported interrupt to handle input event (button press to adjust/set time, light up LCD, set alarm etc)
