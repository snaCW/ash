# ASH Namespace

Ash namespace contains different containers. Some of these containers are available at `std` namespace, so make sure use them in your projects.

## Hierarchy

```Directory
namespace ash
├── namespace matrix
│   ├── class position
│   ├── class matrix<T>
│   └── class sparse_matrix<T>
├── class vector<T>
└── class linked_list<T>
```

## Usage

If we are at the main directory, to access each class, you must `#include` it first like below:

- `#include "vector.h"`
- `#include "matrix/matrix.h"`

Later when compiling using terminal, make sure you use the proper address:

```Bash
g++ matrix/position.h main.cpp
```

## Need more information?

You can easily click on each class name below and go to the dedicated markdown file.

- matrix::
  - matrix
  - position
  - sparse_matrix
- linked_list
- [vector](help/vector.md)
