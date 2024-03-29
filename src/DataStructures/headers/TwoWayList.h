//
//  Copyright 2012 Alin Dobra and Christopher Jermaine
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#ifndef _TWO_WAY_LIST_H
#define _TWO_WAY_LIST_H

// Includes the basic TwoWayList as well as Keyify and Swapify
// (the other basic templates were removed from this file for clarity; their
// files are included below in order to make everything backwards compatible)

/** Changelog

		June 5, 2009; Alin; Added iterator interface on top of the map. The iterator
		interface of the underlying TwoWayList

		June 10, 2009; Alin; Made the copy constructor and opertor= private. Subtle
		bugs appear if the default constructors generated by the compiler are used.
		It is also against the Swapping paradigm.

		June 18, 2009; Alin; Simplified the Swapify and Keyify
		templates. Now they work only if the argument has the == and =
		operator defined. This is not a limitation since all basic types have that
*/

#include <iostream>
#include "Swap.h"
#include "Config.h"

// Helper macros for Frequently Done Actions
// Macro to streamline scan of TwoWayLists
// list "list" is scanned and element is local variable that is
// instantiated as each element of the list
// Usage Scenario:
//   FOREACH_TWL(val, myList){
//     do something with val
//   }END_FOREACH
//
//
#ifdef _HAS_AUTO
#define FOREACH_TWL(el, list) \
    for((list).MoveToStart(); !(list).AtEnd(); (list).Advance()) { \
    auto & el = (list).Current();
#else // _HAS_AUTO
#define FOREACH_TWL(el, list)																		\
	for((list).MoveToStart();	!(list).AtEnd(); (list).Advance()) {	\
	typeof((list).Current())& el = (list).Current();
#endif // _HAS_AUTO

#ifndef END_FOREACH
#define END_FOREACH }
#endif

// This is a template for a doubly linked list
// Type requires only the function swap ()...
template <class Type>
class TwoWayList {

public:
	// type definitions to make our life easier
	typedef Type element;

	// do a deep copy of the param that is sent in
	void copy (TwoWayList &me);

	// basic constructor function
	TwoWayList ();

	// deconstructor function
	virtual ~TwoWayList ();

	// swap operator
	void swap (TwoWayList & List);

	// deep copy (needs CopyFrom on Type)
	void CopyFrom(TwoWayList & List);

	// remove all the content
	void Clear(void);

	// add to current pointer position
	void Insert (Type & Item);

	// add at the end of the list
	void Append (Type & Item);

	// look at the current item
	Type &Current ();

	// remove from current position
	void Remove (Type & Item);

	// move the current pointer position backward through the list
	void Retreat ();

	// move the current pointer position forward through the list
	void Advance ();

	// operations to check the size of both sides
	int LeftLength ();
	int RightLength ();
	int Length();

	// check if empty
	bool IsEmpty();

	// operations to consult state
	bool AtStart ();
	bool AtEnd ();

	// operations to swap the left and right sides of two lists
	void SwapLefts (TwoWayList & List);
	void SwapRights (TwoWayList & List);

	// operations to move the the start of end of a list
	void MoveToStart ();
	void MoveToFinish ();

	// removes the contents of one list from another
	void Subtract (TwoWayList & takeMeOut);

	// fuction to merge two lists (steals the content from the argument
	void SuckUp(TwoWayList& suckMe);

private:

	struct Node {
		// data
		Type data;
		Node *next;
		Node *previous;

		// constructor
		Node () : next (0), previous (0) {}
		virtual ~Node () {}
	};

	struct Header {
		// data
		Node * first;
		Node * last;
		Node * current;
		int leftSize;
		int rightSize;
	};

	// the list itself is pointed to by this pointer
	Header *list;

	// making the default constructor and = operator private so  the list
	// can only be propagated by swap. Otherwise subtle bugs can appear
	TwoWayList(TwoWayList&);
	TwoWayList operator = (TwoWayList&);
};

// The following templates are used to quickly and easily create a
// class that wraps around a simple type (such as an int) and can be
// put into a TwoWayList or an InefficientMap.  The "Swapify" template
// attaches only a swap operation (so a simple type can be used with
// a TwoWayList)... the "Keyify" template attaches both a swap and an
// IsEqual (so a simple type can be used with either the TwoWayList or
// the InefficientMap).  For example, the following is valid code, which
// uses the Keyify template to attach a swap and IsEqual to an int:
//
// void IntSwap (int &a, int &b) {
//         int temp;
//         temp = a;
//         a = b;
//         b = temp;
// }
//
// int IntCheck (int &a, int &b) {
//         return a == b;
// }
//
// int main () {
//
// 	    typedef Keyify <int, IntSwap, IntCheck> keyifiedInt;
//          InefficientMap <keyifiedInt, keyifiedInt> foo;
//          keyifiedInt bar1, bar2;
//
//          bar1 = 12;
//          bar2 = 43;
//          foo.Insert (bar1, bar2);
//          ...
//
// SPECIAL NOTE: ONLY USE THESE TEMPLATES WITH SIMPLE TYPES (ints, doubles, etc.).
// These templates maky use of the = operation, so they are only safe with such types.
// If the type is anything more complicated, then the thing to do is to create a proper
// class containing the swap and IsEqual operations!
//
template <class Type>
class Swapify {
private:
	Type data;

public:
	void swap (Swapify &withMe) {
		SWAP(data, withMe.data);
	}

	void copy (Swapify &fromMe) {
		data = fromMe.data;
	}

	Swapify (const Type castFromMe) {
		data = castFromMe;
	}

	Type& GetData(void){
		return data;
	}

	operator Type() {
		return data;
	}

	Swapify () {}
	virtual ~Swapify () {}
};

template <class Type>
class Keyify {
private:
	Type data;

public:
	void swap (Keyify &withMe) {
		SWAP(data, withMe.data);
	}


	Keyify (const Type castFromMe) {
		data = castFromMe;
	}

	void copy (Keyify &fromMe) {
		data = fromMe.data;
	}

	operator Type() {
		return data;
	}

	int IsEqual(Keyify &checkMe) {
		return data == checkMe.data;
	}

	bool LessThan(Keyify& withMe){
		return data<withMe.data;
	}

	Keyify () {}
	virtual ~Keyify () {}
};

#endif
