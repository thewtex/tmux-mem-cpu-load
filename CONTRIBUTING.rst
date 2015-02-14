============
Contributing
============

Want to improve the quality of tmux-mem-cpu-load code? Great! Here's a quick
guide:

1. Fork, then clone the repo:

    git clone git@github.com:your-username/tmux-mem-cpu-load

2. Make your change. Add tests for your change.
3. See if it compiles and runs like it should.
4. Run tests to check if you didn't break anything:

    make test

Push to your fork and `submit a pull request`_.

At this point you're waiting on us. We'll review your changes as soon as we can.
Before merging your changes we may request you to make some changes or
corrections.

Style guidelines
----------------

You'll need to follow the subsequent rules in order to get your code merged:

* Use Allman_ style for block braces.
* No space before `(`
* Add space after each `(` and before each `)`
* Use braces single line statements
* Don't use mixed case naming style, use underscores instead.
  Bad example:

    int myAwesomeVariable = 0;
    doSomething( myAwesomeVariable );

  Good example:

    int my_awesome_variable = 0;
    do_something( my_awesome_variable );

* Don't vertically align tokens on consecutive lines.
* If you break up an argument list, align the line to opening brace
* Use 2 space indentation (no tabs)
* Use spaces around operators, except for unary operators, such as `!`.
* Add LICENSE header in new files you create.
* Put vim modeline as the first line of file header
* Use the lower-case for CMake commands
* Do not add trailing whitespace
  You can use this bash script to strip unnecessary whitespaces:
  http://git.io/z_GA3A

.. _`submit a pull request`: https://github.com/thewtex/tmux-mem-cpu-load/compare/
.. _Allman: http://en.wikipedia.org/wiki/Indent_style#Allman_style
