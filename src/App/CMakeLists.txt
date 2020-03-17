add_executable(App src/Sandbox.cpp)

#link Cecilion library, thats all we have to to on this side of the src folder :)
target_link_libraries(App Cecilion)