# HelloWorldIf

## Declaration

[<<-- back to index](../typesIndex.md)

```c++
#include "HelloWorldIf.hpp
```

```c++
// Namespace: Zef::Template
class HelloWorldIf {
public:
  HelloWorldIf() = default;
  virtual ~HelloWorldIf() = default;

  virtual std::string GetHelloWorld() const = 0;
};
```

Dummy interface used in template project, desrcibe returning hello message

| Method | Purpose |
|:---|:---|
| virtual ~HelloWorldIf() | Default constructor |
| virtual ~HelloWorldIf() | Default virtual destructor |
| virtual std::string GetHelloWorld() const | Returns hello message |

---

[<<-- back to index](../typesIndex.md)
