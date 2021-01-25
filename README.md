# test-malloc-automatically




This project aims to develop a tool for testing malloc packages, which can help students in the memory lab(specifically for SNU students) and provide an experience of using an automated testing tool(KLEE). 

In many cases, testing aims to maximize code coverage or path coverage, and maximizing path coverage is desirable in theory. Since students' implementations tend to have little paths in a function, maximizing code coverage could be sufficient in this project. 

This tool tries  generates random inputs and measures code coverage, and it subsequently tries to create inputs that cover the unexecuted lines using symbolic execution. 
