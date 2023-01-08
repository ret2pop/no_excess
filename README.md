# NoExcess
This is basically a scheme-like language that is aimed to
be scheme with no additional fluff. The programming language
is "done" now in the sense that it is turing complete and
there are some features like strings, ints, floats, booleans, and
some built-in functions to make the language usable, though
I plan to add support for macros and other stuff. This
is a functional programming language so functions are first-class.

I wrote this programming language within just six days, and I wrote
the bulk of it (~1400 LoC) within the first two days.

## TODO
Each todo here represents one task that I have not done.
Note that all the tasks that I finished are bigger than
the ones that I have not, and that is simply because I
made this checklist after I did everything in the first
checklist.

Estimate: 95% done.

### DONE
- [X] Write the lexer (easy)
- [X] Write the parser (medium-hard)
- [X] Write hash table (medium)
- [X] Write the visitor (hard)
- [X] Write a few print functions (trivial)
- [X] Write the print function for lists (trivial)
- [X] Implement a primitive include system (easy-medium)

### NOT DONE
- [ ] Finish type conversion builtins (easy)
- [ ] Finish read and write builtins (easy)
- [ ] Make appealing frontend (easy)
- [ ] Implement macros (medium)

### Wishlist
- [ ] Maybe a garbage collector?
- [ ] Write a standard library for math and other utilities

## BUILDING
To build and install this program on unix-likes, run:
`make && sudo make install`

## SYNTAX
Programming in this language, you may find some differences from scheme. Here are the basics:
1. You can define anonymous functions with `lambda`. All functions are first class and also self evaluating expressions.
2. Each anonymous function can contain exactly one expression.
3. Each function may have an arbitrary amount of parameters (at least, as many as possible before memory problems occur).
4. String, integer, float, boolean, function, list, and symbol types are supported.
5. You may bind a symbol to an expression in order to create a global variable with `bind`.
6. There are several primitive functions from which you may build more complicated functions.
7. Each final evaluated value will be printed.

You may find actual code examples in `doc/`. Also, there will be more functionality than
the above, but I have not implemented it yet. Some future ones are:

8. The only side effects are printing expressions with the `print` builtin and writing to files with the `write` builtin.
9. You may define macros.
10. Let's all have fun and play together!

Wait a second, that last one isn't one of the rules...

## CONTRIBUTING
If you send diffs to me that improve this code, chances are I will accept the diffs and
try to merge them in.

## LICENSE
See `./LICENSE`.
