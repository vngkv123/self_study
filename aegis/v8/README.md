# Google V8 JavaScript Engine Analysis
**Handle**

```
template <typename T>
class Handle final : public HandleBase {
 public:
  V8_INLINE explicit Handle(T** location = nullptr)
      : HandleBase(reinterpret_cast<Object**>(location)) {
    // Type check:
    static_assert(std::is_convertible<T*, Object*>::value,
                  "static type violation");
  }
 ```
 
 - final -> can't be inherited class
