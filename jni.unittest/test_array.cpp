
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <jni/array.hpp>

using jni_pod_types =
    boost::mpl::list<bool, std::uint8_t, std::uint16_t, std::int16_t,
                     std::int32_t, std::int64_t, float, double>;
using jni_pod_types_integers =
    boost::mpl::list<std::uint8_t, std::uint16_t, std::int16_t, std::int32_t,
                     std::int64_t>;
using jni_pod_types_floats = boost::mpl::list<float, double>;

BOOST_AUTO_TEST_SUITE(array_tests)

BOOST_AUTO_TEST_CASE_TEMPLATE(create_from_size, T, jni_pod_types) {
  jni::array<T> array{10};

  BOOST_TEST(array.size() == 10);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(create_from_buffer, T, jni_pod_types) {
  T buffer[10] = {T(0)};

  jni::array<T> array{buffer, buffer + 10};
  BOOST_TEST(array.size() == 10);
  // TODO: compare values
}

BOOST_AUTO_TEST_CASE_TEMPLATE(create_and_move, T, jni_pod_types) {
  jni::array<T> original{10};

  auto target{std::move(original)};

  BOOST_TEST(original.size() == 0);
  BOOST_TEST(target.size() == 10);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(create_and_move_assign, T, jni_pod_types) {
  jni::array<T> original{10};
  jni::array<T> target{5};

  BOOST_TEST(original.size() == 10);
  BOOST_TEST(target.size() == 5);

  target = std::move(original);

  BOOST_TEST(original.size() == 0);
  BOOST_TEST(target.size() == 10);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(load, T, jni_pod_types_integers) {
  jni::array<T> array{5};

  array.load();
  BOOST_TEST(array.loaded());
  BOOST_TEST(array.size() == 5);

  for (const auto &item : array) {
    BOOST_TEST(item == T{0});
  }
}

BOOST_AUTO_TEST_CASE(load_bool) {
  jni::array<bool> array{5};

  array.load();
  BOOST_TEST(array.loaded());
  BOOST_TEST(array.size() == 5);

  for (const auto &item : array) {
    BOOST_TEST(!item);
  }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(load_float, T, jni_pod_types_floats) {
  jni::array<T> array{5};

  array.load();
  BOOST_TEST(array.loaded());
  BOOST_TEST(array.size() == 5);

  for (const auto &item : array) {
    BOOST_CHECK_CLOSE(item, T{0.0}, 0.0001);
  }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(load_and_commit_load, T, jni_pod_types_integers) {
  jni::array<T> array{5};

  array.load();
  BOOST_TEST(array.loaded());

  for (auto &item : array) {
    item = T{5};
  }

  array.commit();
  BOOST_TEST(!array.loaded());

  array.load();
  BOOST_TEST(array.loaded());
  BOOST_TEST(array.size() == 5);

  for (const auto &item : array) {
    BOOST_TEST(item == T{5});
  }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(load_and_commit_load_floats, T,
                              jni_pod_types_floats) {
  jni::array<T> array{5};

  array.load();
  BOOST_TEST(array.loaded());

  for (auto &item : array) {
    item = T{5};
  }

  array.commit();
  BOOST_TEST(!array.loaded());

  array.load();
  BOOST_TEST(array.loaded());
  BOOST_TEST(array.size() == 5);

  for (const auto &item : array) {
    BOOST_CHECK_CLOSE(item, T{5}, 0.0001);
  }
}

BOOST_AUTO_TEST_CASE(load_and_commit_load_bool) {
  jni::array<bool> array{5};

  array.load();
  BOOST_TEST(array.loaded());

  for (auto &item : array) {
    item = true;
  }

  array.commit();
  BOOST_TEST(!array.loaded());

  array.load();
  BOOST_TEST(array.loaded());
  BOOST_TEST(array.size() == 5);

  for (const auto &item : array) {
    BOOST_TEST(item);
  }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(load_and_abort_load, T, jni_pod_types_integers) {
  jni::array<T> array{5};

  array.load();
  BOOST_TEST(array.loaded());

  for (auto &item : array) {
    item = T{5};
  }

  array.abort();
  BOOST_TEST(!array.loaded());

  array.load();
  BOOST_TEST(array.loaded());
  BOOST_TEST(array.size() == 5);

  for (const auto &item : array) {
    BOOST_TEST(item != T{5});
  }
}

BOOST_AUTO_TEST_CASE_TEMPLATE(load_and_abort_load_floats, T,
                              jni_pod_types_floats) {
  jni::array<T> array{5};

  array.load();
  BOOST_TEST(array.loaded());

  for (auto &item : array) {
    item = T{5};
  }

  array.abort();
  BOOST_TEST(!array.loaded());

  array.load();
  BOOST_TEST(array.loaded());
  BOOST_TEST(array.size() == 5);

  for (const auto &item : array) {
    BOOST_CHECK_CLOSE(item, T{0}, 0.0001);
  }
}

BOOST_AUTO_TEST_CASE(load_and_abort_load_bool) {
  jni::array<bool> array{5};

  array.load();
  BOOST_TEST(array.loaded());

  for (auto &item : array) {
    item = true;
  }

  array.abort();
  BOOST_TEST(!array.loaded());

  array.load();
  BOOST_TEST(array.loaded());
  BOOST_TEST(array.size() == 5);

  for (const auto &item : array) {
    BOOST_TEST(!item);
  }
}

BOOST_AUTO_TEST_SUITE_END()