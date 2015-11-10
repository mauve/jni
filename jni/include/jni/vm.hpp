
#pragma once

#include <jni/config.hpp>
#include <jni/api_version.hpp>
#include <jni/raw/types.hpp>

#include <memory>
#include <string>
#include <vector>

#include <boost/system/error_code.hpp>

namespace jni {

class environment;

class JNI_EXPORT vm {
public:
  static vm &current();

  virtual ~vm() = 0;

  virtual boost::system::error_code destroy() = 0;

  virtual boost::system::error_code
  attach_current_thread(const std::string &thread_name, api_version version,
                        raw::object_ref group, void **env) = 0;

  virtual boost::system::error_code
  attach_current_thread_as_daemon(const std::string &thread_name,
                                  api_version version, raw::object_ref group,
                                  void **env) = 0;

  virtual boost::system::error_code detach_current_thread() = 0;

  virtual boost::system::error_code get_current_env(api_version version,
                                                    void **env) = 0;

  struct init_args {
    api_version version;
    std::vector<std::pair<std::string, void *>> options;
    bool ignore_unrecognized;
  };

  static init_args get_default_init_args();

  static boost::system::error_code
  create_vm(const init_args &args = get_default_init_args());

  // used by jni::init::on_load to register the vm
  static void set_vm(void *);
};

class JNI_EXPORT embedded_vm {
public:
  embedded_vm();
  embedded_vm(embedded_vm &&other);
  ~embedded_vm();

  embedded_vm &operator=(embedded_vm &&other);

  void destroy();

  embedded_vm(const embedded_vm &) = delete;
  embedded_vm &operator=(const embedded_vm &) = delete;

private:
  bool _destroyed;
};

} // namespace jni
