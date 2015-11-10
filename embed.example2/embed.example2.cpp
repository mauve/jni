
#include <jni/jni.hpp> // include-all header do not use
                       // in production code
#include <iostream>
#include <iomanip>

using jni::java::lang::Class;

int main(int argc, char *argv[]) {
  jni::embedded_vm vm;

  if (argc < 2) {
    std::cerr << "usage: .\\embed.example2 class-name" << std::endl;
    return 1;
  }

  auto class_name = argv[1];

  try {
    auto cls = Class::forName(class_name);
    std::cout << std::boolalpha << "Class: " << cls
              << "\n" // calls Object.toString()
              << "  name: " << cls.getName() << "\n"
              << "  simpleName: " << cls.getSimpleName() << "\n"
              << "  isEnum: " << cls.isEnum() << "\n"
              << "  isAnnotation: " << cls.isAnnotation() << "\n"
              << "  isArray: " << cls.isArray() << "\n"
              << "  isInterface: " << cls.isInterface() << "\n"
              << "  isLocalClass: " << cls.isLocalClass() << "\n"
              << "  isPrimitive: " << cls.isPrimitive() << "\n"
              << "  isSynthetic: " << cls.isSynthetic() << "\n";
  } catch (const jni::java::lang::Throwable &exception) {
    std::cerr << "Error: " << exception << std::endl;
    return 1;
  }

  return 0;
}
