### TDS
## Top Down Shooter. 

Run and shoot and survive. This is a game is for fun and practice of the cpp language. This is a barebones version that I consider playable. 

# Build
Building should be straight forward. Everything is included in the Makefile. 
* Linux:

Run ```make``` 
This will just run g++ *.cpp (+ all necessary dependencies. You might have a few dependencies that will need to be installed first eg: -lX11 -lGL -lpthread -lpng)
* Windows:

Run ```make w```
This will make object files and link them (Window's cpmpile time was slower than linux hence why I did this. But there aren't any dependencies that'll be needing to installed).  
