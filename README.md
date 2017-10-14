# Reversi
Reversi board game running in terminal using ncurses.

Game info: https://en.wikipedia.org/wiki/Reversi

## Features
- Local play
- Play vs AI (Using Simple AI, or minimax algorithm)
- Play over network (TCP connection)


Menu:

![Menu screen](https://i.imgur.com/7iSeFUy.png)

Ingame:

![Game screenshot](https://i.imgur.com/Itj8kxg.png)

AI vs AI over network connection:

![AI vs AI over network](images/ai_over_tcp.gif)

## Build
### Requirements:
C++ 14 compatible compiler, unix system with ncurses

~~~~
make build
./Reversi

~~~~
