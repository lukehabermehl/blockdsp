//
//  autil_uiattrs.h
//  libblockdsp
//
//  Created by Luke on 8/29/16.
//  Copyright © 2016 Luke Habermehl. All rights reserved.
//

#ifndef autil_uiattrs_h
#define autil_uiattrs_h

/** APUUIAttribute is a typdef'd integral type that stores a description of a UI control */
typedef unsigned long APUUIAttribute;

const APUUIAttribute APU_UI_TYPE_SLIDER = 1ul << 4;
const APUUIAttribute APU_UI_TYPE_SWITCH = 1ul << 5;

const APUUIAttribute APU_UI_ORIENTATION_VERTICAL = 1ul;
const APUUIAttribute APU_UI_ORIENTATION_HORIZONTAL = 1ul << 1;


#endif /* autil_uiattrs_h */
