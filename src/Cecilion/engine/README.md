#Cecilion Engine folder
This is the public folder for Cecilion engine. Everything inside this directory can be included by a client application.
Just make a normal include to the header you want to use.

Public access to the core can be achieved by creating header files somewhere inside the engine folder and include some
other header file from the core directory, 
this way the engine will have access to all the core stuff while the client application can use 
the header files we define in this directory.