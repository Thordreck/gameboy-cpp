
#include "doctest.h"

import utilities;

TEST_CASE("utilities.Fixed size queue reports empty properly")
{
	utils::fixed_size_queue<int, 10> queue;

	CHECK(queue.is_empty());
	CHECK_EQ(queue.count(), 0);
}

TEST_CASE("utilities.Fixed size queue reports full properly")
{
	utils::fixed_size_queue<int, 3> queue;
	queue.try_push(0);
	queue.try_push(1);
	queue.try_push(2);

	CHECK(queue.is_full());
	CHECK_EQ(queue.count(), 3);
}

TEST_CASE("utilities.Fixed size queue pushes individual elements successfully until full")
{
	utils::fixed_size_queue<int, 3> queue;

	CHECK(queue.try_push(0));
	CHECK(queue.try_push(1));
	CHECK(queue.try_push(2));
	CHECK_FALSE(queue.try_push(3));
	CHECK_FALSE(queue.try_push(4));
}

TEST_CASE("utilities.Fixed size queue pops elements successfully until empty")
{
	utils::fixed_size_queue<int, 3> queue;

	queue.try_push(0);
	queue.try_push(1);
	queue.try_push(2);
	queue.try_push(3);

	CHECK(queue.try_pop().transform([] (auto e) { return e == 0; }).value_or(false));
	CHECK(queue.try_pop().transform([] (auto e) { return e == 1; }).value_or(false));
	CHECK(queue.try_pop().transform([] (auto e) { return e == 2; }).value_or(false));
	CHECK_FALSE(queue.try_pop().has_value());
}

TEST_CASE("utilities.Fixed size queue pushes element range successfully")
{
	utils::fixed_size_queue<int, 3> queue;

	auto range = { 0, 1, 2 };
	queue.try_push(range);

	CHECK(queue.try_pop().transform([] (auto e) { return e == 0; }).value_or(false));
	CHECK(queue.try_pop().transform([] (auto e) { return e == 1; }).value_or(false));
	CHECK(queue.try_pop().transform([] (auto e) { return e == 2; }).value_or(false));
	CHECK_FALSE(queue.try_pop().has_value());
}

TEST_CASE("utilities.Fixed size fails to push range if there is not enough free space")
{
	utils::fixed_size_queue<int, 3> queue;
	queue.try_push(0);

	auto range = { 1, 2, 3 };
	CHECK_FALSE(queue.try_push(range));

	CHECK(queue.try_pop().transform([] (auto e) { return e == 0; }).value_or(false));
	CHECK_FALSE(queue.try_pop().has_value());
}
