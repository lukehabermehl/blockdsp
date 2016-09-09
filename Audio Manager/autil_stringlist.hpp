//
//  autil_stringlist.hpp
//  libblockdsp
//
//  Created by Luke on 9/8/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef autil_stringlist_hpp
#define autil_stringlist_hpp

#include <memory>

class APUStringList
{
public:
    APUStringList();
    APUStringList(APUStringList& rhs);

    void append(const char *str);
    size_t size();
    const char *getString(size_t index);
    void setString(size_t index, const char *str);

    APUStringList copy();

    APUStringList& operator=(APUStringList const &rhs);

private:
    class Pimpl;
    std::shared_ptr<Pimpl> _pimpl;
};

#endif /* autil_stringlist_hpp */
