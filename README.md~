# test-malloc-automatically




This project aims to develop a tool for testing malloc packages, which can help students in the memory lab(specifically for SNU students) and provide an experience of using an automated testing tool(KLEE). 

In many cases, testing aims to maximize code coverage or path coverage, and maximizing path coverage is desirable in theory. Since students' implementations tend to have little paths in a function, maximizing code coverage could be sufficient in this project. 

This tool tries  generates random inputs and measures code coverage, and it subsequently tries to create inputs that cover the unexecuted lines using symbolic execution. 






### About Random Testing (During my TA works)
Random testing of 60 students' code showed an average of 75\% code coverage. At this time, I tested the size of the random testing from 100 to 10,000, but there was no big difference, indicating that a small testing set was sufficient for this simple malloc implementation. Since only malloc and free were tested, the uncovering parts were usually for realloc, calloc, and error handling parts. During the evaluation process, students with compile errors or infinite loops were excluded. 

Although random testing were not instantly useful for catching bugs in the code that students completed after trial and error, it is expected to be helpful if these random testing tools are used during development.
