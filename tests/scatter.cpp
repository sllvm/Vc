/*  This file is part of the Vc library.

    Copyright (C) 2009 Matthias Kretz <kretz@kde.org>

    Vc is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    Vc is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Vc.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <Vc/Vc>
#include "unittest.h"
#include <iostream>
#include <cstring>

using namespace Vc;

template<typename Vec> void scatterArray()
{
    typedef typename Vec::IndexType It;
    const int count = 31999;
    typename Vec::EntryType array[count], out[count];
    for (int i = 0; i < count; ++i) {
        array[i] = i - 100;
    }
    typename It::Mask mask;
    for (It i(IndexesFromZero); !(mask = (i < count)).isEmpty(); i += Vec::Size) {
        typename Vec::Mask castedMask(mask);
        Vec a(array, i, castedMask);
        a.scatter(out, i, castedMask);
    }
    for (int i = 0; i < count; ++i) {
        COMPARE(array[i], out[i]);
    }
    COMPARE(0, std::memcmp(array, out, count * sizeof(typename Vec::EntryType)));
}

template<typename T> struct Struct
{
    T a;
    char x;
    T b;
    short y;
    T c;
    char z;
};

template<typename Vec> void scatterStruct()
{
    typedef typename Vec::IndexType It;
    typedef Struct<typename Vec::EntryType> S;
    const int count = 3999;
    S array[count], out[count];
    memset(array, 0, count * sizeof(S));
    memset(out, 0, count * sizeof(S));
    for (int i = 0; i < count; ++i) {
        array[i].a = i;
        array[i].b = i + 1;
        array[i].c = i + 2;
    }
    typename It::Mask mask;
    for (It i(IndexesFromZero); !(mask = (i < count)).isEmpty(); i += Vec::Size) {
        typename Vec::Mask castedMask(mask);
        Vec a(array, &S::a, i, castedMask);
        Vec b(array, &S::b, i, castedMask);
        Vec c(array, &S::c, i, castedMask);
        a.scatter(out, &S::a, i, castedMask);
        b.scatter(out, &S::b, i, castedMask);
        c.scatter(out, &S::c, i, castedMask);
    }
    VERIFY(0 == memcmp(array, out, count * sizeof(S)));
}

int main()
{
    runTest(scatterArray<int_v>);
    runTest(scatterArray<uint_v>);
    runTest(scatterArray<float_v>);
    runTest(scatterArray<double_v>);
    runTest(scatterArray<sfloat_v>);
    runTest(scatterArray<short_v>);
    runTest(scatterArray<ushort_v>);
    runTest(scatterStruct<int_v>);
    runTest(scatterStruct<uint_v>);
    runTest(scatterStruct<float_v>);
    runTest(scatterStruct<double_v>);
    runTest(scatterStruct<sfloat_v>);
    runTest(scatterStruct<short_v>);
    runTest(scatterStruct<ushort_v>);
    return 0;
}
