# Handwritten Digits Guesser Toy
This is the code for a thing I made that guesses numerical digits the user draws. It uses a neural network trained with MNIST's dataset, through my deep learning framework. All graphics are done with SFML.

To build (on windows), type:
```Bash
g++ -I src/include -c main.cpp
g++ main.o -o Handwritten-Digits-Guesser-Toy -L src/lib -l sfml-graphics -l sfml-window -l sfml-system
```
To use, click on the grid to start drawing. You can draw anything you like, but the neural network will only recognize numberical digits. Press _i_ on your keyboard to invert the board and _c_ to clear it.


![image](https://user-images.githubusercontent.com/70760644/141691740-ee356b13-3adf-4e71-8841-3958b4ba3ad2.png)
![image](https://user-images.githubusercontent.com/70760644/141691810-8c9e0bc3-1797-4483-bf0f-77f7e7dc928c.png)

