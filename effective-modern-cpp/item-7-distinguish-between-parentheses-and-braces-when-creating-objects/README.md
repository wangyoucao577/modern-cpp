# Item 7. Distinguish between () and {} when creating objects

## Test Codes

- most_vexing_parse.cc
```bash
clang++ -std=c++14 most_vexing_parse.cc
most_vexing_parse.cc:22:25: warning: parentheses were disambiguated as a function declaration [-Wvexing-parse]
  TimeKeeper time_keeper(Timer());
                        ^~~~~~~~~
most_vexing_parse.cc:22:26: note: add a pair of parentheses to declare a variable
  TimeKeeper time_keeper(Timer());
                         ^
                         (      )
most_vexing_parse.cc:23:21: error: member reference base type 'TimeKeeper (Timer (*)())' is not a structure or union
  return time_keeper.get_time();
         ~~~~~~~~~~~^~~~~~~~~
1 warning and 1 error generated.
```

## Notes 



## References
- [wiki - Most vexing parse](https://en.wikipedia.org/wiki/Most_vexing_parse)
