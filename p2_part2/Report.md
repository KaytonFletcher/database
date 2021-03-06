# P2 - Part 2: SortedDB

Group Members: Kayton Fletcher

The bulk of the implementation for project 2 part 2 is within `SortedDB.cc` and `SortedDB.h`. 

All of the functions specified by the DBFile interface have been implemented for the SortedDB, as specified in the project 2 specification.

`DBFile.cc` now holds a pointer to an `InternalDBFile`, which can be a HeapDB or SortedDB. 

For this project I overhauled test.h to remove many of the bad practices and annoyances it had. To do this I used `inline`, so this project *must be compiled with c++17*.

I also greatly restructured the file layout into directories to organize the unwieldy number of files in the project. I did this before reading the mention in the announcement to not do this. This makefile should ensure this still works with a script, as it compiles the `test.out` executable. 

For SortedDB, a Bigq instance is used to keep a running order of records being added to the database, being merged with the existing records in the file when a read operation occurs such as GetNext or MoveFirst.

The CNF and sort order provided to the SortedDB determine whether a linear or binary search will be used when getting the next record with a CNF.

## Assumptions
It is assumed that both `bison` and `flex` are installed to compile the parser needed. Additionally, gtest must be installed as a library that can be linked on the system, as I only specify `-lgtest` within the `Makefile` and will not be providing the program with my submission.

The needed files `catalog`, `Lexer.l` and `Parser.y` have been included in the zip file.

One should ensure `dbfile_dir`, `tpch_dir`, and `catalog_path` are all set to the desired location. These are now static variables within the `test.h` file.

Finally, many adjustments were made to the starter code provided, and as such I am expecting the files I have provided to be used when compiling my DB classes.

## Compiling The Programs
To compile `test.out`, the program that runs test.cc as provided, run the following command
``` 
make test.out
```
To compile `gtests.out`, the program that runs the unit tests I wrote, run the command
```
make gtests.out
```

No `main.cc` was submitted with this project, other than the one in the `gtests` folder.

## Running The Programs
Running the programs is very straight forward. Treat each as an executable.

`./test.out` and `./gtests.out`.
Both `gtests.out` and `test.out` require user input. For `gtests.out`, it will require a CNF to be provided for nation.bin. The location of the nation.bin heap database will also need to be updated for the test case to pass and the CNF provided must be "(n_regionkey = 3)".

The unit tests created with gtest are all located in `gtests/main.cc`. The gtests created are simple and test SortedDB.

## test.out Customer Output
![Import File](../database/p2_part2/screenshots/2021-03-02-221828_619x859_scrot.png)

![Import File](../database/p2_part2/screenshots/2021-03-02-221856_796x997_scrot.png)

![Import File](../database/p2_part2/screenshots/2021-03-02-221913_775x992_scrot.png)


## Gtest Output
![Import File](../database/p2_part2/screenshots/2021-03-02-225448_687x768_scrot.png)

