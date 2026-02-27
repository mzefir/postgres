# HelloWorld

## Declaration

[<<-- back to index](../typesIndex.md)

```c++
#include "HelloWorld.hpp"
```

Implements interface [HelloWorldIf](Interface.HelloWorldIf.md). Used to retrieve hello message for template project.

``` C++
// Namespace: Zef::Template
class HelloWorld : public HelloWorldIf {
public:
  HelloWorld() = default;
  virtual ~HelloWorld() = default;

  virtual std::string GetHelloWorld() const override;
};
```

## Methods

### `#!c++ HelloWorld() = default;`

Default constructor

| Parameter | - |
|:---|:---|
| N / A |  |

| Return value | - |
|:---|:---|
| N / A |  |

---

### `#!c++ virtual ~HelloWorld() = default;`

Destructor

| Parameter | - |
|:---|:---|
| N / A |  |

| Return value | - |
|:---|:---|
| N / A |  |

---

### `#!c++ virtual std::string GetHelloWorld() const override;`

Returns hello message

| Parameter | - |
|:---|:---|
| N / A |  |

| Return value | - |
|:---|:---|
| `#!c++ std::string` | Hello message |

---

## Example

```c++
#include <iostream>

#include "HelloWorld.hpp"

int main() {
  auto *world = new Zef::Template::HelloWorld();
  auto msg = world->GetHelloWorld();
  std::cout << msg << std::endl;
  delete world;
  return 0;
}
```

---

[<<-- back to index](../typesIndex.md)
