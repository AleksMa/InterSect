#ifndef INTERSECT_EMPTYSURFACE_H
#define INTERSECT_EMPTYSURFACE_H


#include "AbstractSurface.h"

class EmptySurface : public AbstractSurface {
public:
    vector<Point> make_mash() override;

    float max_z() override;

    float min_z() override;
};


#endif //INTERSECT_EMPTYSURFACE_H
