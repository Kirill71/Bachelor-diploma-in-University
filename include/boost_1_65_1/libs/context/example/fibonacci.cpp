
//          Copyright Oliver Kowalke 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>
#include <memory>

#include <boost/context/continuation.hpp>

namespace ctx = boost::context;

int main() {
    int a;
    ctx::continuation c=ctx::callcc(
        [&a](ctx::continuation && c){
            a=0;
            int b=1;
            for(;;){
                c=c.resume();
                int next=a+b;
                a=b;
                b=next;
            }
            return std::move( c);
        });
    for ( int j = 0; j < 10; ++j) {
        std::cout << a << " ";
        c=c.resume();
    }
    std::cout << std::endl;
    std::cout << "main: done" << std::endl;
}
