
# JNI

This project is C++ JNI wrapper which tries to make JNI bearable
in the sense of easy to write but also easy to write correctly.

**It is not done yet, nor is it close to a usuable state.**

Currently only embedding (JNI_CreateJavaVm) has been tested so that is
the usecase which definitely works right now. The code should work on
all major platforms but I haven't tested on other platforms than
Windows (VS2015) yet.

```cpp
#include <jni/jni.hpp> // include-all header do not use
                       // in production code
#include <iostream>

using jni::java::lang::Class;

int main(int argc, char *argv[]) {
  jni::embedded_vm vm;

  auto cls = Class::forName("java/lang/String");
  auto str = cls.newInstance();
  std::cout << "Class: " << cls << std::endl; // calls Object.toString()
  std::cout << "Empty string: " << str << std::endl;

  return 0;
}
```




