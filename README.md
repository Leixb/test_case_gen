Random test case generator
==========================

This program can generate simple test case files from a set of rules. It is a work in progress and
the functionality is very limited.


Usage
=====

To install simply build by running `make` from the project tree. Once the program is build
you can run it as any executable.

Constant definitions
--------------------

Constants are variables with a name that are given a random value, the syntax for them is:

```
{ name : a b }
```

Where `name` is the name of the variable and `a` and `b` are numbers (or previously defined
variables) that determine the values the variable can take. Once run the generator will
assign a value between `a` and `b` inclusive to the variable by name `name` and will output
its contents.

### Hidden variables

If the name of the variable is preceded by an underscore (`_`) the generator will assign a value
to it but it will not output it.

String
------

Strings are characters delimited by `"` that are printed by the generator.

Intervals
---------

Intervals are formed by a number of repetitions and an interval:

```
( repetitions : a b )
```

Repeaters
---------

Repeaters are formed by a number of repetitions and the contents to repeat:

```
[ repetitions :
    contents
]
```

`contents` can be either a `constant`, `string`, `interval` or a `repetition` and can be
glued by a comma.

Examples
========

Generate `x` matrices of size `n` by `m`. x must be a number between 2 and 5,
n and m must be between 1 and 20 and their contents must be between -10000 and
10000:

```
{ x : 2 5 }
"
"
[ x :
    { n : 1 20 },
    { m : 1 20 },
    "
",
    [ n :
        ( m : -10000 10000 )
    ]
]
```




