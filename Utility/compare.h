#ifndef PHI__define_guard__Utility__Comparer_h
#define PHI__define_guard__Utility__Comparer_h

#include "../define.h"

namespace phi {

/*
The comparison system makes the following assumptions, this also means the
system only can process types which satisfy all of them.
	1. There is one and only one statement satisfies the relationship
	   between x and y:
			a. x < y
			b. x = y
			c. x > y
	2. If x = y and y = z, then x = z.
	3. If x < y and y < z, then x < z.

Comparer is an object which is used to compare two object. We define some kind of
Comparer, LessThanComparer, GreaterThanComparer, LessEqualThanComparer,
GreaterEqualThanComparer, HalfComparer and FullComparer.

#///////////////////////////////////////////////////////////////////////////////

        LessThanComparer
   LessEqualThanComparer
     GreaterThanComparer
GreaterEqualThanComparer
           EqualComparer
        NotEqualComparer

- lt_cmper, le_cmper, gt_cmper, ge_cmper, eq_cmper, ne_cmper (half_comparers),
  objects satisfying LessThanComparer, LessEqualThanComparer,
  GreaterThanComparer, GreaterEqualThanComparer, EqualComparer, NotEqualComparer
  respectively
- x, object of type X
- y, object of type Y

The following expressions must valid and have thier specified effects.

+-------------------+-------------+-----------------------+
| expression        | return type | operational semantics |
+-------------------+-------------+-----------------------+
| lt_cmper.lt(x, y) | bool        | return x < y          |
| lt_cmper.lt(y, x) | bool        | return y < x          |
+-------------------+-------------+-----------------------+
| gt_cmper.lt(x, y) | bool        | return x > y          |
| gt_cmper.lt(y, x) | bool        | return y > x          |
+-------------------+-------------+-----------------------+
| le_cmper.lt(x, y) | bool        | return x <= y         |
| le_cmper.lt(y, x) | bool        | return y <= x         |
+-------------------+-------------+-----------------------+
| ge_cmper.lt(x, y) | bool        | return x >= y         |
| ge_cmper.lt(y, x) | bool        | return y >= x         |
+-------------------+-------------+-----------------------+
| eq_cmper.lt(x, y) | bool        | return x == y         |
| eq_cmper.lt(y, x) | bool        | return y == x         |
+-------------------+-------------+-----------------------+
| ne_cmper.lt(x, y) | bool        | return x != y         |
| ne_cmper.lt(y, x) | bool        | return y != x         |
+-------------------+-------------+-----------------------+

#///////////////////////////////////////////////////////////////////////////////

FullComparer

- f_cmper, object satisfying FullComparer
- x, object of type X
- y, object of type Y

The following expressions must valid and have thier specified effects.

+------------------+-------------+-----------------------+
| expression       | return type | operational semantics |
+------------------+-------------+-----------------------+
| f_cmper.lt(x, y) | bool        | return x < y          |
| f_cmper.lt(y, x) | bool        | return y < x          |
| f_cmper.gt(x, y) | bool        | return x > y          |
| f_cmper.gt(y, x) | bool        | return y > x          |
| f_cmper.le(x, y) | bool        | return x <= y         |
| f_cmper.le(y, x) | bool        | return y <= x         |
| f_cmper.ge(x, y) | bool        | return x >= y         |
| f_cmper.ge(y, x) | bool        | return y >= x         |
| f_cmper.eq(x, y) | bool        | return x == y         |
| f_cmper.eq(y, x) | bool        | return y == x         |
| f_cmper.ne(x, y) | bool        | return x != y         |
| f_cmper.ne(y, x) | bool        | return y != x         |
| f_cmper(x, y)    | int         | return x <=> y        |
|                  |             | -1 : x < y            |
|                  |             |  0 : x == y           |
|                  |             |  1 : x > y            |
| f_cmper(y, x)    | int         | return y <=> x        |
|                  |             | -1 : y < x            |
|                  |             |  0 : y == x           |
|                  |             |  1 : y > x            |
+------------------+-------------+-----------------------+

We can see that full_comparers' expressions includes half_comparers', this
means full_comparers can be used as half_comparers.

The following classes structs and functions provides default behaviours
which will be used when user do not provide specific behaviour, and some
conveneint templates to help user construct standar HalfComparer or
FullComparer with thier LessThanComparer.

#///////////////////////////////////////////////////////////////////////////////

Why we design this comparison system ?

The most of algorithm and data structure use LessThanComparers in STL. It is
a powerful policy which uses them to implement other comparison.
But in some cases we need to confirm the relationship between x and y (x < y,
x = y or x >y) (e.g., binary search, searching in binary tree). There may exist
more efficient algorithm to get the result instead of twice
"less than comparison" ( if twice "less than comparison" is the most efficient
way, yuo can use ConstructFullComparerWithLessThanComparer to quickly construct
a "full comparer"). This makes the posiblity to users to improve the code with
provide a more integrated comparer, FullComparer. This is an example.

To compare two strings, you may write

	struct StringLessThanComparer {
		bool lt(const std::string& x, const std::string& y) {
			if (x.size() < y.size()) { return true; }
			if (x.size() > y.size()) { return false; }

			for (size_t i(0); i != x.size(); ++i) {
				if (x[i] < y[i]) { return true; }
				if (x[i] > y[i]) { return false; }
			}

			return false;
		}
	};

	StringLessThanComparer str_lt_cmper;

If x = "012345678a" and y = "0123456789", this LessThanComparar will expense
much time to judge str_lt_cmper.lt(x, y) is false and str.lt(y, x) is true.
Then confirm x > y. But when we use FullComparer
	
	struct StringFullComparer {
		int operator()(const std::string& x, const std::string& y) {
			if (x.size() < y.size()) { return -1; }
			if (x.size() > y.size()) { return 1; }

			for (size_t i(0); i != x.size(); ++i) {
				if (x[i] < y[i]) { return -1; }
				if (x[i] > y[i]) { return 1; }
			}

			return 0;
		}
	};

	StringFullComparer str_full_cmper;

We will get result from str_full_cmper(x, y) with once function calling. Then
compare the result with -1, 0, 1, confirm x > y quickly.

#///////////////////////////////////////////////////////////////////////////////

Why LessThanComparer has expressions, lt_cmper.lt(x, y) or lt_cmper.lt(y, x)
instead of lt_cmper(x, y) or lt_cmper(y, x) ?

We know the common instruction of functors is implementing operator(), then we
can treat them as ordinary functions.

Think the following case:
	Function F, which needs a FullComparer as argument, calls another function G
	which needs a LessThanComparer as argument.

If LessThanComparer expression is designed as lt_cmper(x, y) or lt_cmper(y, x),
we will need an adapter like this

	template<typename FullComparer> struct FullToLessThanComparer {
		FullComparer full_cmper;

		FullToLessThanComparer(const FullComparer& full_cmper):
			full_cmper(full_cmper) {}

		template<typename X, typename Y> bool operator()(X& x, Y& y) {
			return this->full_cmper.lt(x, y);
		}
	};

Adapter just forwards lt_cmper(x, y) to full_cmper.lt(x, y). And this may
interfere with efficiency. To avoid this, FullComparer expressions are
completely includes other HalfComparers. This means FullComparer can be used as
HalfComparers directly, easily, without any forwarding. Just throw them into
function.
*/

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename UnImplementedFullComparer>
struct AutoImplementFullComparer: public UnImplementedFullComparer {
	template<typename... Args>
	AutoImplementFullComparer(Args&&... args):
		UnImplementedFullComparer(Forward<Args>(args)...) {}

	template<typename X, typename Y> bool eq(X& x, Y& y) {
		return this->operator()(x, y) == 0;
	}
	template<typename X, typename Y> bool eq(X& x, Y& y) const {
		return this->operator()(x, y) == 0;
	}

	template<typename X, typename Y> bool ne(X& x, Y& y) {
		return this->operator()(x, y) != 0;
	}
	template<typename X, typename Y> bool ne(X& x, Y& y) const {
		return this->operator()(x, y) != 0;
	}

	template<typename X, typename Y> bool lt(X& x, Y& y) {
		return this->operator()(x, y) == -1;
	}
	template<typename X, typename Y> bool lt(X& x, Y& y) const {
		return this->operator()(x, y) == -1;
	}

	template<typename X, typename Y> bool gt(X& x, Y& y) {
		return this->operator()(x, y) == 1;
	}
	template<typename X, typename Y> bool gt(X& x, Y& y) const {
		return this->operator()(x, y) == 1;
	}

	template<typename X, typename Y> bool le(X& x, Y& y) {
		return this->operator()(x, y) != 1;
	}
	template<typename X, typename Y> bool le(X& x, Y& y) const {
		return this->operator()(x, y) != 1;
	}

	template<typename X, typename Y> bool ge(X& x, Y& y) {
		return this->operator()(x, y) != -1;
	}
	template<typename X, typename Y> bool ge(X& x, Y& y) const {
		return this->operator()(x, y) != -1;
	}
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename LessThanComparer>
struct ConstructFullComparerWithLessThanComparer_ {
	LessThanComparer lt_cmper;

	template<typename... LessThanComparerConstructArgs>
	ConstructFullComparerWithLessThanComparer_(
		LessThanComparerConstructArgs&&... lt_cmper_construct_args):
		lt_cmper(Forward<LessThanComparerConstructArgs>(
			lt_cmper_construct_args)...) {}

	template<typename X, typename Y> int operator()(X& x, Y& y) {
		if (this->lt_cmper(x, y)) { return -1; }
		if (this->lt_cmper(y, x)) { return 1; }
		return 0;
	}
	template<typename X, typename Y> int operator()(X& x, Y& y) const {
		if (this->lt_cmper(x, y)) { return -1; }
		if (this->lt_cmper(y, x)) { return 1; }
		return 0;
	}
};

template<typename LessThanComparer>
using ConstructFullComparerWithLessThanComparer = AutoImplementFullComparer<
	ConstructFullComparerWithLessThanComparer_<LessThanComparer>>;

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename FullComparerFunctionPtr>
struct ConstructFullComparerWithFullComparerFunctionPtr_ {
	FullComparerFunctionPtr full_cmper_func_ptr;

	ConstructFullComparerWithFullComparerFunctionPtr_(
		FullComparerFunctionPtr full_cmper_func_ptr):
		full_cmper_func_ptr(full_cmper_func_ptr) {}

	template<typename X, typename Y> int operator()(X& x, Y& y) {
		return this->full_cmper_func_ptr(x, y);
	}
	template<typename X, typename Y> int operator()(X& x, Y& y) const {
		return this->full_cmper_func_ptr(x, y);
	}
};

template<typename FullComparerFunctionPtr>
using ConstructFullComparerWithFullComparerFunctionPtr =
	AutoImplementFullComparer<ConstructFullComparerWithFullComparerFunctionPtr_<
		FullComparerFunctionPtr>>;

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

struct DefaultEqualComparer {
	template<typename X, typename Y> bool eq(X& x, Y& y) const {
		return x == y;
	}
};

struct DefaultNotEqualComparer {
	template<typename X, typename Y> bool ne(X& x, Y& y) const {
		return x != y;
	}
};

struct DefaultLessThanComparer {
	template<typename X, typename Y> bool lt(X& x, Y& y) const { return x < y; }
};

struct DefaultGreaterThanComparer {
	template<typename X, typename Y> bool gt(X& x, Y& y) const { return x > y; }
};

struct DefaultLessEqualThanComparer {
	template<typename X, typename Y> bool le(X& x, Y& y) const {
		return x <= y;
	}
};

struct DefaultGreaterEqualThanComparer {
	template<typename X, typename Y> bool ge(X& x, Y& y) const {
		return x >= y;
	}
};

struct DefaultThreeWayComparer {
	template<typename X, typename Y> int tw(X& x, Y& y) const {
		if (x < y) { return -1; }
		if (y < x) { return 1; }
		return 0;
	}
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

struct DefaultHalfComparer {
	template<typename X, typename Y> bool lt(X& x, Y& y) const { return x < y; }

	template<typename X, typename Y> bool gt(X& x, Y& y) const { return x > y; }

	template<typename X, typename Y> bool le(X& x, Y& y) const {
		return x <= y;
	}

	template<typename X, typename Y> bool ge(X& x, Y& y) const {
		return x >= y;
	}
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

struct DefaultFullComparer {
	template<typename X, typename Y> int operator()(X& x, Y& y) const {
		if (x < y) { return -1; }
		if (y < x) { return 1; }
		return 0;
	}

	template<typename X, typename Y> bool eq(X& x, Y& y) const {
		return x == y;
	}

	template<typename X, typename Y> bool ne(X& x, Y& y) const {
		return x != y;
	}

	template<typename X, typename Y> bool lt(X& x, Y& y) const { return x < y; }

	template<typename X, typename Y> bool gt(X& x, Y& y) const { return x > y; }

	template<typename X, typename Y> bool le(X& x, Y& y) const {
		return x <= y;
	}

	template<typename X, typename Y> bool ge(X& x, Y& y) const {
		return x >= y;
	}
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename FullComparer> struct ReverseFullComparer_ {
	FullComparer full_cmper;

	template<typename... FullComparerConstructArgs>
	ReverseFullComparer_(
		FullComparerConstructArgs&&... full_cmper_construct_args):
		full_cmper(
			Forward<FullComparerConstructArgs>(full_cmper_construct_args)...) {}

	template<typename X, typename Y>
	int operator()(const X& x, const Y& y) const {
		return -this->full_cmper(x, y);
	}
};

template<typename FullComparer>
using ReverseFullComparer =
	AutoImplementFullComparer<ReverseFullComparer_<FullComparer>>;

}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Comparer> struct ReverseComparer: public Comparer {
	const Comparer& cmper;

	ReverseComparer(const Comparer& cmper): cmper(cmper) {}

	template<typename X, typename Y> int operator()(X& x, Y& y) const {
		return -this->cmper.operator()(x, y);
	}

	template<typename X, typename Y> bool eq(X& x, Y& y) const {
		return this->cmper.eq(x, y);
	}

	template<typename X, typename Y> bool ne(X& x, Y& y) const {
		return this->cmper.ne(x, y);
	}

	template<typename X, typename Y> bool lt(X& x, Y& y) const {
		return this->cmper.gt(x, y);
	}

	template<typename X, typename Y> bool gt(X& x, Y& y) const {
		return this->cmper.lt(x, y);
	}

	template<typename X, typename Y> bool le(X& x, Y& y) const {
		return this->cmper.ge(x, y);
	}

	template<typename X, typename Y> bool ge(X& x, Y& y) const {
		return this->cmper.le(x, y);
	}
};

#endif