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
For loop took 98ms (160ms when looking up size() during each iteration)
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

Release mode inverts performance relations from debug mode! Relative differences stay consistent for varying array/vector sizes. Templates also don't matter, thanks to template instantiation. TODO: find why it is like that. How is STL linked and compiled?

### Performance considerations

First source [DrDobbs](https://www.drdobbs.com/stl-algorithms-vs-hand-written-loops/184401446).
- (minor) Checking against `.end()` in each iteration of a hand-written for-loop could be bad. But `end()` is probably well optimized and inlined, so should be no issue. Release mode seems to optimize that away.
- Library implementers use knowledge about containers to iterate faster (e.g. memory layout, internal data structure)
- Non-trivial algorithms are super sophisticated. Not counting this one; we're doing trivial stuff here

Summing up, this source does not explain enough what is going on.

What does debug vs release: Seems to be similar in [gcc](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html) and [clang](https://clang.llvm.org/docs/CommandGuide/clang.html). CMake passes `-O3 -DNDEBUG` to the compiler, so highest standard-conform optimization. Which kinds of optimizations are done in a for loop? Which ones for `std::transform`?

#### For-loop optimizations

#### std::transform optimizations

Alternative: you could look at the generated assembly code! use godbolt.org

## Containers
In folder `containers`, I'm currently playing with reimplementing STL containers, currently only `std::forward_list`.

## Dojos/Katas for interview preparation
Folder `dojos` contains dojos done for interview preparation (from books, not leetcode). Code style is not nice, writing that code without IDE as far as possible.

## Notes
Timings done on a MacBook Air 2012 sporting Ubuntu 18.04.

## TODOs
- Test debug vs release
- Compare assembly result of range-based for-loop vs `std::for_each` (expecting to be same)
