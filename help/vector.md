# ash::vector\<T\> Class

This class should only be used for **learning purposes**. Please use `std::vector<T>` for your projects.

| Class | Header Guard |
| - | - |
| `std::vector<T>` | `_VECTOR_` |
| `ash::vector<T>` | `ASH_VECTOR_H_INCLUDED` |

## How to use `ash::vector<T>`

Clone the repository. Since any files with `main.cpp` name is ignored by Git, you can freely add a `cpp` file to the project. Or rather, you can copy the `vector.h` file to your own project.

Include the header file:

``` Cpp
#include "vector.h"
```

> A lot of methods are identical between `std::vector<T>` and `ash::vector<T>`, so make sure you don't include them at the same time.

**Usage example:**

``` Cpp
ash::vector<int> v;
```

> You can also use `using namespace ash;` in your `main.cpp` file to omit the `ash` from `ash::vector<int>`.
