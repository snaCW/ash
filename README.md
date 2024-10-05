# vector\<T\> Class

This class should only be used for **learning purposes**. Please use `std::vector<T>` for your projects.

| Class | Header Guard |
| - | - |
| `std::vector<T>` | `_VECTOR_` |
| `vector<T>` | `VECTOR_H` |

## How to use `std::vector<T>`

At the top of your c++ file, add this line:

``` Cpp
#include <vector>
```

Now you can use the `std::vector<T>` with this pattern:

``` Cpp
std::vector<type> name;
```

## How to use `vector<T>`

Clone the repository. Since any files with `main.cpp` name is ignored by Git, you can freely add a `cpp` file to the project. Or rather, you can copy the `vector.h` file to your own project.

Include the header file:

``` Cpp
#include "vector.h"
```

> A lot of methods are identical between `std::vector<T>` and `vector<T>`, so make sure you don't include them at the same time.

The rest is pretty similar to `std::vector<T>`.
