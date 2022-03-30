// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef SEEN_NR_FILTER_TILE_H
#define SEEN_NR_FILTER_TILE_H

/*
 * feTile filter primitive renderer
 *
 * Authors:
 *   Felipe Corrêa da Silva Sanches <juca@members.fsf.org>
 *
 * Copyright (C) 2007 authors
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#include "display/nr-filter-primitive.h"

namespace Inkscape {
namespace Filters {

class FilterSlot;

class FilterTile : public FilterPrimitive {
public:
    FilterTile();
    static FilterPrimitive *create();
    ~FilterTile() override;

    void render_cairo(FilterSlot &slot) override;
    void area_enlarge(Geom::IntRect &area, Geom::Affine const &trans) override;
    double complexity(Geom::Affine const &ctm) override;

    Glib::ustring name() override { return Glib::ustring("Tile"); }
};

} /* namespace Filters */
} /* namespace Inkscape */

#endif /* __NR_FILTER_TILE_H__ */
/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:fileencoding=utf-8:textwidth=99 :
