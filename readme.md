# Introduce the rules
The border is 8*8 units larges, initia snakes is 2 units in length.

There are two players: player 1 is born in [7, 1], player 2 is born in [1, 7]; grid index is [0, 1, 2, .... 7] * [0, 1, 2, .... 7].

* player 1 is initially moving to right.
* player 2 is initially moving to left.

Both snakes have a skill -- poo:
* sanke can eat a poo, the result is it's minimise 1 unit of length (when the length is less or equal than 1 then it dies)
* each snake can only poo 1 once, the poo will exsit on the map a while
* a snake can poo another time, only it's poo disappeared or be eaten
* a snake can eat it's own poo, the result is minimise it's own length
## Logic of dead
1. Heads on the borders
2. Heads on another snakes (both dies when two snakes head-on-head)
