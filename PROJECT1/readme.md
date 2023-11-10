## Compiling and Running
Compilation is easy; simply run `make` and then run `./p1c`. Things work as intended on our end!
## Difficulties
Had some trouble coming up with the implementation of the structure of our data, so the way we store and access our processes is a bit obtuse and redundant, and it uses an ugly magic number of 99,999 max processes. Comments were left throughout the code detailing this problem, but essentially, rather than simply traversing the tree structure we were building, we kept a couple very large arrays to store the pointers to the process structs. If we had more time, we would have removed this redundancy and just traversed the tree from its root to find the family relationships.
While the output does contain all the information asked, it doesn't format exactly the way the example on the assignment sheet was notated. With some more work parsing the stat files, we could've created the exact match, but as it was only required to be "similar to" the sample, we figured it was fine.
Mostly everything else seemed to go without a hitch!
