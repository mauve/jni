
#include <jni/errors.hpp>

namespace jni {

class error_category : public boost::system::error_category {
public:
  const char *name() const noexcept override { return "jni"; }

  std::string message(int error) const override {
    error_codes errc = static_cast<error_codes>(error);

    switch (errc) {
    case error_codes::success:
      return "success";
    case error_codes::unknown:
      return "unknown error";
    case error_codes::thread_detached:
      return "thread detached";
    case error_codes::invalid_version:
      return "invalid version";
    case error_codes::out_of_memory:
      return "out of memory";
    case error_codes::vm_already_created:
      return "vm already created";
    case error_codes::invalid_argument:
      return "invalid argument";
    }

    return "unknown error code: " + std::to_string(error);
  }
};

boost::system::error_category &get_error_category() {
  thread_local error_category category;
  return category;
}

boost::system::error_code make_error_code(error_codes errc) {
  return{ static_cast<int>(errc), get_error_category() };
}

} // namespace jni
