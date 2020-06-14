# C++ Experiments

## Zipping with std::transform
- Task: accumulate entries of array into same array
- Tested solutions: for loop vs `std::transform`
- Results in debug mode
```
Performing tests on std::vector
Transform took 36ms
For loop took 18ms

Performing tests on std::array
Transform took 8ms
For loop took 31ms

Performing tests on std::unique_ptr<std::array>
Transform took 8ms
For loop took 160ms
```

- Results in release mode
```
Performing tests on std::vector
Transform took 0ms
For loop took 2ms

Performing tests on std::array
Transform took 2ms
For loop took 1ms

Performing tests on std::unique_ptr<std::array>
Transform took 3ms
For loop took 0ms
```

Release mode actually reverts performance relations from debug mode! TODO: find why it is like that. How is STL linked and compiled?

## Containers
In folder `containers`, I'm currently playing with reimplementing STL containers, currently only `std::forward_list`.

## Dojos/Katas for interview preparation
Folder `dojos` contains dojos done for interview preparation (from books, not leetcode). Code style is not nice, writing that code without IDE as far as possible.

## Notes
Timings done on a MacBook Air 2012 sporting Ubuntu 18.04.

## TODOs
- check for different array sizes
- Test debug vs release
