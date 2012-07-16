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
// for Emacs -*- c++ -*-

private:
	// The type of this attribute
	AttributeTypeEnum type;
	// category of this attribute
	TypeCategoryEnum cat;

	// Size, for strings and LOBs
	int size;

	// Constructor with type, category and size.
	AttributeType(AttributeTypeEnum t, TypeCategoryEnum c, int s):
		type(t), cat(c), size(s) { }

	// Constructor with only type and category.
	AttributeType(AttributeTypeEnum t, TypeCategoryEnum c):
		type(t), cat(c), size(-1) { }

	// function to get the name of the string
	const char* getAttName(void);