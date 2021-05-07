///////////////////////////////////////////////////////////////////////////////
// disks_test.cpp
//
// Unit tests for disks.hpp
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "rubrictest.hpp"
#include "disks.hpp"

int main() {

	Rubric rubric;

	const disk_state alt_one(1), alt_three(3);

	auto sorted_one(alt_one);
	sorted_one.swap(0);

	auto sorted_three(alt_three); // LD LD LD
	sorted_three.swap(0);         // DL LD LD
	sorted_three.swap(2);         // DL DL LD
	sorted_three.swap(1);         // DD LL LD
	sorted_three.swap(4);         // DD LL DL
	sorted_three.swap(3);         // DD LD LL
	sorted_three.swap(2);         // DD DL LL

	rubric.criterion("disk_state still works", 1,
		[&]() {
			TEST_EQUAL("total_count() for n=1", 2, alt_one.total_count());
			TEST_EQUAL("dark_count() for n=1", 1, alt_one.dark_count());
			TEST_EQUAL("light_count() for n=1", 1, alt_one.light_count());
			TEST_TRUE("is_index(0) for n=1", alt_one.is_index(0));
			TEST_TRUE("is_index(1) for n=1", alt_one.is_index(1));
			TEST_FALSE("is_index(2) for n=1", alt_one.is_index(2));
			TEST_EQUAL("get(0) for n=1", DISK_LIGHT, alt_one.get(0));
			TEST_EQUAL("get(1) for n=1", DISK_DARK, alt_one.get(1));

			TEST_EQUAL("total_count() for n=3", 6, alt_three.total_count());
			TEST_EQUAL("dark_count() for n=3", 3, alt_three.dark_count());
			TEST_EQUAL("light_count() for n=3", 3, alt_three.light_count());
			TEST_TRUE("is_index(0) for n=3", alt_three.is_index(0));
			TEST_TRUE("is_index(1) for n=3", alt_three.is_index(1));
			TEST_TRUE("is_index(2) for n=3", alt_three.is_index(2));
			TEST_TRUE("is_index(3) for n=3", alt_three.is_index(3));
			TEST_TRUE("is_index(4) for n=3", alt_three.is_index(4));
			TEST_TRUE("is_index(5) for n=3", alt_three.is_index(5));
			TEST_FALSE("is_index(6) for n=3", alt_three.is_index(6));
			TEST_EQUAL("get(0) for n=3", DISK_LIGHT, alt_three.get(0));
			TEST_EQUAL("get(1) for n=3", DISK_DARK, alt_three.get(1));
			TEST_EQUAL("get(2) for n=3", DISK_LIGHT, alt_three.get(2));
			TEST_EQUAL("get(3) for n=3", DISK_DARK, alt_three.get(3));
			TEST_EQUAL("get(4) for n=3", DISK_LIGHT, alt_three.get(4));
			TEST_EQUAL("get(5) for n=3", DISK_DARK, alt_three.get(5));

			TEST_EQUAL("get(0) after swap", DISK_DARK, sorted_one.get(0));
			TEST_EQUAL("get(1) after swap", DISK_LIGHT, sorted_one.get(1));

			TEST_EQUAL("get(0) after swaps", DISK_DARK, sorted_three.get(0));
			TEST_EQUAL("get(1) after swaps", DISK_DARK, sorted_three.get(1));
			TEST_EQUAL("get(2) after swaps", DISK_DARK, sorted_three.get(2));
			TEST_EQUAL("get(3) after swaps", DISK_LIGHT, sorted_three.get(3));
			TEST_EQUAL("get(4) after swaps", DISK_LIGHT, sorted_three.get(4));
			TEST_EQUAL("get(5) after swaps", DISK_LIGHT, sorted_three.get(5));
		});

	rubric.criterion("sorted_disks still works", 1,
		[&]() {
			auto temp = sorted_disks(alt_three, 13);
			TEST_EQUAL("sorted_disks::after", temp.after(), alt_three);
			TEST_EQUAL("sorted_disks::swap_count", 13, temp.swap_count());
		});

	rubric.criterion("disk_state::is_initialized", 1,
		[&]() {
			TEST_TRUE("is_initialized() for n=1", alt_one.is_initialized());
			TEST_TRUE("is_initialized() for n=1", alt_three.is_initialized());
			TEST_FALSE("is_initialized() after swap", sorted_one.is_initialized());
			TEST_FALSE("is_initialized() after swaps", sorted_three.is_initialized());
		});

	rubric.criterion("disk_state::is_sorted", 1,
		[&]() {
			TEST_FALSE("is_sorted() for n=1", alt_one.is_sorted());
			TEST_FALSE("is_sorted() for n=1", alt_three.is_sorted());
			TEST_TRUE("is_sorted() after swap", sorted_one.is_sorted());
			TEST_TRUE("is_sorted() after swaps", sorted_three.is_sorted());
		});

	rubric.criterion("alternate, n=3", 1,
		[&]() {
			auto output = sort_alternate(disk_state(3));
			TEST_TRUE("actually sorted", output.after().is_sorted());
			TEST_EQUAL("number of swaps must be 6", 6, output.swap_count());
		});

	rubric.criterion("alternate, n=4", 1,
		[&]() {
			auto output = sort_alternate(disk_state(4));
			TEST_TRUE("actually sorted", output.after().is_sorted());
			TEST_EQUAL("number of swaps must be 10", 10, output.swap_count());
		});

	rubric.criterion("alternate, other values", 1,
		[&]() {

			auto trial = [](unsigned n) {
				return sort_alternate(disk_state(n)).swap_count();
			};

			TEST_EQUAL("n=10 gives 55 swaps", 55, trial(10));
			TEST_EQUAL("n=20 gives 210 swaps", 210, trial(20));
			TEST_EQUAL("n=30 gives 465 swaps", 465, trial(30));
			TEST_EQUAL("n=40 gives 820 swaps", 820, trial(40));
			TEST_EQUAL("n=50 gives 1275 swaps", 1275, trial(50));
			TEST_EQUAL("n=60 gives 1830 swaps", 1830, trial(60));
			TEST_EQUAL("n=70 gives 2485 swaps", 2485, trial(70));
			TEST_EQUAL("n=80 gives 3240 swaps", 3240, trial(80));
			TEST_EQUAL("n=90 gives 4095 swaps", 4095, trial(90));
			TEST_EQUAL("n=100 gives 5050 swaps", 5050, trial(100));
		});

	rubric.criterion("lawnmower, n=3", 1,
		[&]() {
			auto output = sort_lawnmower(disk_state(3));
			TEST_TRUE("actually sorted", output.after().is_sorted());
			TEST_EQUAL("number of swaps must be 6", 6, output.swap_count());
		});

	rubric.criterion("lawnmower, n=4", 1,
		[&]() {
			auto output = sort_lawnmower(disk_state(4));
			TEST_TRUE("actually sorted", output.after().is_sorted());
			TEST_EQUAL("number of swaps must be 10", 10, output.swap_count());
		});

	rubric.criterion("lawnmower, other values", 1,
		[&]() {

			auto trial = [](unsigned n) {
				return sort_lawnmower(disk_state(n)).swap_count();
			};

			TEST_EQUAL("n=10 gives 55 swaps", 55, trial(10));
			TEST_EQUAL("n=20 gives 210 swaps", 210, trial(20));
			TEST_EQUAL("n=30 gives 465 swaps", 465, trial(30));
			TEST_EQUAL("n=40 gives 820 swaps", 820, trial(40));
			TEST_EQUAL("n=50 gives 1275 swaps", 1275, trial(50));
			TEST_EQUAL("n=60 gives 1830 swaps", 1830, trial(60));
			TEST_EQUAL("n=70 gives 2485 swaps", 2485, trial(70));
			TEST_EQUAL("n=80 gives 3240 swaps", 3240, trial(80));
			TEST_EQUAL("n=90 gives 4095 swaps", 4095, trial(90));
			TEST_EQUAL("n=100 gives 5050 swaps", 5050, trial(100));
		});

	return rubric.run();
}
