#include <gtest/gtest.h>
#include "vol2com/utils/boundedvalue.h"
#include "signalspy.hpp"

using namespace vol2com;

TEST(BoundedValue, Getters)
{
  constexpr BoundedValue::value_type min = 0;
  constexpr BoundedValue::value_type max = 359;
  constexpr BoundedValue::value_type val = 10;

  BoundedValue v{ min, max, val, BoundedValue::EOverflowBehavior::Clamp };
  SignalSpy spy(&v, &BoundedValue::valueChanged);

  EXPECT_EQ(v.min(),   min);
  EXPECT_EQ(v.max(),   max);
  EXPECT_EQ(v.value(), val);
  EXPECT_EQ(v.OverflowBehavior(), BoundedValue::EOverflowBehavior::Clamp);

  v.setValue(val);
  ASSERT_EQ(spy.size(), 0u) << "Signal emitted unless value is not changed (val->val)";
}

TEST(BoundedValue, Increments)
{
  constexpr BoundedValue::value_type min = 0;
  constexpr BoundedValue::value_type max = 359;
  constexpr BoundedValue::value_type val = 10;

  BoundedValue v{ min, max, val, BoundedValue::EOverflowBehavior::Clamp };
  SignalSpy spy(&v, &BoundedValue::valueChanged);

  // Preincrement
  auto next = v.value() + 1;
  ++v;
  EXPECT_EQ(v.value(), next);
  ASSERT_EQ(spy.size(), 1u);
  EXPECT_EQ(std::get<0>(spy[0]), next);

  // Postincrement
  v++;
  next += 1;
  EXPECT_EQ(v.value(), next);
  ASSERT_EQ(spy.size(), 2u);
  EXPECT_EQ(std::get<0>(spy[1]), next);

  --v;
  next -= 1;
  EXPECT_EQ(v.value(), next);
  ASSERT_EQ(spy.size(), 3u);
  EXPECT_EQ(std::get<0>(spy[2]), next);

  v--;
  next -= 1;
  EXPECT_EQ(v.value(), next);
  ASSERT_EQ(spy.size(), 4u);
  EXPECT_EQ(std::get<0>(spy[3]), next);
}

TEST(BoundedValue, Clamp)
{
  constexpr BoundedValue::value_type min = 0;
  constexpr BoundedValue::value_type max = 359;
  constexpr BoundedValue::value_type val = 10;

  BoundedValue v{ min, max, val, BoundedValue::EOverflowBehavior::Clamp };
  SignalSpy spy(&v, &BoundedValue::valueChanged);

  v.setValue(-10);
  EXPECT_EQ(v.value(), min) << "New value is out MIN bound";
  ASSERT_EQ(spy.size(), 1u);
  EXPECT_EQ(std::get<0>(spy[0]), min) << "Emitted value should be equal to MIN";

  v.setValue(min);
  EXPECT_EQ(v.value(), min);
  ASSERT_EQ(spy.size(), 1u) << "Signal emitted unless value is not changed (min->min)";

  v += -1;
  v -=  1;
  EXPECT_EQ(v.value(), min) << "The value should not have changed";
  EXPECT_EQ(spy.size(), 1u) << "Signal emitted unless value is not changed (min->min)";

  v.setValue(400);
  EXPECT_EQ(v.value(), max) << "New value is out MAX bound";
  ASSERT_EQ(spy.size(), 2u);
  EXPECT_EQ(std::get<0>(spy[1]), max) << "Emitted value should be equal to MAX";

  v +=  1;
  v -= -1;
  EXPECT_EQ(v.value(), max) << "The value should not have changed";
  EXPECT_EQ(spy.size(), 2u) << "Signal emitted unless value is not changed (max->max)";
}

TEST(BoundedValue, Overflow)
{
  using vt = BoundedValue::value_type;

  constexpr vt min = -10;
  constexpr vt max = 10;
  constexpr vt val = 0;
  constexpr vt cir = max * 2 + 1;  //number of sufficient to overflow with return to the same position

  static constexpr uint8_t check = static_cast<uint8_t>(10)
                                 - static_cast<uint8_t>(20);

  BoundedValue v{ min, max, max, BoundedValue::EOverflowBehavior::Overflow };
  SignalSpy spy(&v, &BoundedValue::valueChanged);

  ASSERT_EQ(v.value(), max) << "Starting value is wrong";

  v += 1;
  ASSERT_EQ(v.value(), min) << "Overflow failed (max->min)";
  ASSERT_EQ(spy.size(), 1u);
  EXPECT_EQ(std::get<0>(spy[0]), min) << "Emitted value should be equal to MIN";

  v -= 1;
  ASSERT_EQ(v.value(), max) << "Overflow failed (min->max)";
  ASSERT_EQ(spy.size(), 2u);
  EXPECT_EQ(std::get<0>(spy[1]), max) << "Emitted value should be equal to MAX";

  v += cir;
  ASSERT_EQ(v.value(), max) << "Should return to previous value (max)";
  ASSERT_EQ(spy.size(), 2u) << "Signal emitted unless value is not changed (max->max)";

  v -= cir;
  ASSERT_EQ(v.value(), max) << "Should return to previous value (max)";
  ASSERT_EQ(spy.size(), 2u) << "Signal emitted unless value is not changed (max->max)";

  v.setValue(max);
  ASSERT_EQ(v.value(), max);
  ASSERT_EQ(spy.size(), 2u) << "Signal emitted unless value is not changed (max->max)";

  v.setValue(0);
  ASSERT_EQ(v.value(), 0);
  ASSERT_EQ(spy.size(), 3u);
  EXPECT_EQ(std::get<0>(spy[2]), 0) << "Emitted value should be equal to 0";

  v.setValue(min);
  ASSERT_EQ(v.value(), min);
  ASSERT_EQ(spy.size(), 4u);
  EXPECT_EQ(std::get<0>(spy[3]), min) << "Emitted value should be equal to MIN";
}
