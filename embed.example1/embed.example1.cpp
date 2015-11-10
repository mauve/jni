
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
