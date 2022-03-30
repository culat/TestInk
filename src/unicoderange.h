// SPDX-License-Identifier: GPL-2.0-or-later
/** @file
 * TODO: insert short description here
 *//*
 * Authors: see git history
 *
 * Copyright (C) 2014 Authors
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */
#include <glibmm/ustring.h>
#include <vector>

// A type which can hold any UTF-32 or UCS-4 character code.
typedef unsigned int gunichar;

struct Urange{
	char* start;
	char* end;
};

class UnicodeRange{
public:
UnicodeRange(const char* val);
int add_range(char* val);
bool contains(char unicode);
Glib::ustring attribute_string();
gunichar sample_glyph();

private:
std::vector<Urange> range;
std::vector<gunichar> unichars;
};

