// SPDX-License-Identifier: GPL-2.0-or-later
/** @file
 * @brief SVG color matrix filter effect
 *//*
 * Authors:
 *   Hugo Rodrigues <haa.rodrigues@gmail.com>
 *
 * Copyright (C) 2006 Hugo Rodrigues
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */
#ifndef SP_FECOLORMATRIX_H_SEEN
#define SP_FECOLORMATRIX_H_SEEN

#include <vector>
#include "sp-filter-primitive.h"
#include "display/nr-filter-colormatrix.h"

#define SP_FECOLORMATRIX(obj) (dynamic_cast<SPFeColorMatrix*>((SPObject*)obj))
#define SP_IS_FECOLORMATRIX(obj) (dynamic_cast<const SPFeColorMatrix*>((SPObject*)obj) != NULL)

class SPFeColorMatrix : public SPFilterPrimitive {
public:
    SPFeColorMatrix();
    ~SPFeColorMatrix() override;

    Inkscape::Filters::FilterColorMatrixType type = Inkscape::Filters::COLORMATRIX_MATRIX;
    double value = 0;
    std::vector<double> values;

private:
    bool value_set = false;

protected:
	void build(SPDocument* doc, Inkscape::XML::Node* repr) override;
	void release() override;

	void set(SPAttr key, const gchar* value) override;

	void update(SPCtx* ctx, unsigned int flags) override;

	Inkscape::XML::Node* write(Inkscape::XML::Document* doc, Inkscape::XML::Node* repr, guint flags) override;

	void build_renderer(Inkscape::Filters::Filter* filter) override;
};

#endif /* !SP_FECOLORMATRIX_H_SEEN */

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
