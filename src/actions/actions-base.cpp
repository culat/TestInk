// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Gio::Actions tied to the application and independent of GUI.
 *
 * Copyright (C) 2018 Tavmjong Bah
 *
 * The contents of this file may be used under the GNU General Public License Version 2 or later.
 *
 */

#include <iostream>

#include <giomm.h>  // Not <gtkmm.h>! To eventually allow a headless version!
#include <glibmm/i18n.h>

#include "actions-base.h"
#include "actions-helper.h"

#include "actions/actions-extra-data.h"
#include "inkscape-application.h"

#include "inkscape.h"             // Inkscape::Application
#include "inkscape-version-info.h"// Inkscape version
#include "path-prefix.h"          // Extension directory
#include "selection.h"            // Selection
#include "object/sp-root.h"       // query_all()
#include "file.h"                 // dpi convert method
#include "io/resource.h"

void
print_inkscape_version()
{
    std::cout << Inkscape::inkscape_version() << std::endl;
}

void
print_debug_info()
{
    std::cout << Inkscape::debug_info() << std::endl;
}

void
print_system_data_directory()
{
    std::cout << Glib::build_filename(get_inkscape_datadir(), "inkscape") << std::endl;
}

void
print_user_data_directory()
{
    std::cout << Inkscape::IO::Resource::profile_path("") << std::endl;
}

// Helper function for query_x(), query_y(), query_width(), and query_height().
void
query_dimension(InkscapeApplication* app, bool extent, Geom::Dim2 const axis)
{
    SPDocument* document = nullptr;
    Inkscape::Selection* selection = nullptr;
    if (!get_document_and_selection(app, &document, &selection)) {
        return;
    }

    if (selection->isEmpty()) {
        selection->add(document->getRoot());
    }

    bool first = true;
    auto items = selection->items();
    for (auto item : items) {
        if (!first) {
            std::cout << ",";
        }
        first = false;
        Geom::OptRect area = item->documentVisualBounds();
        if (area) {
            if (extent) {
                std::cout << area->dimensions()[axis];
            } else {
                std::cout << area->min()[axis];
            }
        } else {
            std::cout << "0";
        }
    }
    std::cout << std::endl;
}

void
query_x(InkscapeApplication* app)
{
    query_dimension(app, false, Geom::X);
}

void
query_y(InkscapeApplication* app)
{
    query_dimension(app, false, Geom::Y);
}

void
query_width(InkscapeApplication* app)
{
    query_dimension(app, true, Geom::X);
}

void
query_height(InkscapeApplication* app)
{
    query_dimension(app, true, Geom::Y);
}

// Helper for query_all()
void
query_all_recurse (SPObject *o)
{
    SPItem *item = dynamic_cast<SPItem*>(o);
    if (item && item->getId()) {
        Geom::OptRect area = item->documentVisualBounds();
        if (area) {
            // clang-format off
            std::cout << item->getId()               << ","
                      << area->min()[Geom::X]        << ","
                      << area->min()[Geom::Y]        << ","
                      << area->dimensions()[Geom::X] << ","
                      << area->dimensions()[Geom::Y] << std::endl;
            // clang-format on
        }

        for (auto& child: o->children) {
            query_all_recurse (&child);
        }
    }
}

void
query_all(InkscapeApplication* app)
{
    SPDocument* doc = app->get_active_document();
    if (!doc) {
        std::cerr << "query_all: no document!" << std::endl;
        return;
    }

    SPObject *o = doc->getRoot();
    if (o) {
        query_all_recurse(o);
    }
}

void
pdf_page(int page)
{
    INKSCAPE.set_pdf_page(page);
}

void
convert_dpi_method(Glib::ustring method)
{
    if (method == "none") {
        sp_file_convert_dpi_method_commandline = FILE_DPI_UNCHANGED;
    } else if (method == "scale-viewbox") {
        sp_file_convert_dpi_method_commandline = FILE_DPI_VIEWBOX_SCALED;
    } else if (method == "scale-document") {
        sp_file_convert_dpi_method_commandline = FILE_DPI_DOCUMENT_SCALED;
    } else {
        std::cerr << "dpi_convert_method: invalid option" << std::endl;
    }
}

void
no_convert_baseline()
{
    sp_no_convert_text_baseline_spacing = true;
}

void
vacuum_defs(InkscapeApplication* app)
{
    SPDocument* document = nullptr;
    Inkscape::Selection* selection = nullptr;
    if (!get_document_and_selection(app, &document, &selection)) {
        return;
    }
    document->vacuumDocument();
}

std::vector<std::vector<Glib::ustring>> raw_data_base =
{
    // clang-format off
    {"app.inkscape-version",          N_("Inkscape Version"),        "Base",       N_("Print Inkscape version and exit")                   },
    {"app.debug-info",                N_("Debug Info"),              "Base",       N_("Print debugging information and exit")              },
    {"app.system-data-directory",     N_("System Directory"),        "Base",       N_("Print system data directory and exit")              },
    {"app.user-data-directory",       N_("User Directory"),          "Base",       N_("Print user data directory and exit")                },
    {"app.action-list",               N_("List Actions"),            "Base",       N_("Print a list of actions and exit")                  },
    {"app.vacuum-defs",               N_("Clean up Document"),       "Base",       N_("Remove unused definitions (gradients, etc.)")       },
    {"app.quit",                      N_("Quit"),                    "Base",       N_("Quit Inkscape, check for data loss")                },
    {"app.quit-immediate",            N_("Quit Immediately"),        "Base",       N_("Immediately quit Inkscape, no check for data loss") },

    {"app.open-page",                 N_("Import Page Number"),      "Import",     N_("Select PDF page number to import")                  },
    {"app.convert-dpi-method",        N_("Import DPI Method"),       "Import",     N_("Set DPI conversion method for legacy Inkscape files")},
    {"app.no-convert-baseline",       N_("No Import Baseline Conversion"), "Import", N_("Do not convert text baselines in legacy Inkscape files")},

    {"app.query-x",                   N_("Query X"),                 "Query",      N_("Query 'x' value(s) of selected objects")            },
    {"app.query-y",                   N_("Query Y"),                 "Query",      N_("Query 'y' value(s) of selected objects")            },
    {"app.query-width",               N_("Query Width"),             "Query",      N_("Query 'width' value(s) of object(s)")               },
    {"app.query-height",              N_("Query Height"),            "Query",      N_("Query 'height' value(s) of object(s)")              },
    {"app.query-all",                 N_("Query All"),               "Query",      N_("Query 'x', 'y', 'width', and 'height'")             }
    // clang-format on
};

void
add_actions_base(InkscapeApplication* app)
{
    auto *gapp = app->gio_app();

    // Note: "radio" actions are just an easy way to set type without using templating.
    // clang-format off
    gapp->add_action(               "inkscape-version",                                    sigc::ptr_fun(&print_inkscape_version)                 );
    gapp->add_action(               "debug-info",                                          sigc::ptr_fun(&print_debug_info)                       );
    gapp->add_action(               "system-data-directory",                               sigc::ptr_fun(&print_system_data_directory)            );
    gapp->add_action(               "user-data-directory",                                 sigc::ptr_fun(&print_user_data_directory)              );
    gapp->add_action(               "action-list",        sigc::mem_fun(app, &InkscapeApplication::print_action_list)                             );
    gapp->add_action(               "vacuum-defs",        sigc::bind<InkscapeApplication*>(sigc::ptr_fun(&vacuum_defs),               app)        );
    gapp->add_action(               "quit",               sigc::mem_fun(app, &InkscapeApplication::on_quit)                                       );
    gapp->add_action(               "quit-immediate",     sigc::mem_fun(app, &InkscapeApplication::on_quit_immediate)                             );

    gapp->add_action_radio_integer( "open-page",                                           sigc::ptr_fun(&pdf_page),                             0);
    gapp->add_action_radio_string(  "convert-dpi-method",                                  sigc::ptr_fun(&convert_dpi_method),              "none");
    gapp->add_action(               "no-convert-baseline",                                 sigc::ptr_fun(&no_convert_baseline)                    );


    gapp->add_action(               "query-x",            sigc::bind<InkscapeApplication*>(sigc::ptr_fun(&query_x),                   app)        );
    gapp->add_action(               "query-y",            sigc::bind<InkscapeApplication*>(sigc::ptr_fun(&query_y),                   app)        );
    gapp->add_action(               "query-width",        sigc::bind<InkscapeApplication*>(sigc::ptr_fun(&query_width),               app)        );
    gapp->add_action(               "query-height",       sigc::bind<InkscapeApplication*>(sigc::ptr_fun(&query_height),              app)        );
    gapp->add_action(               "query-all",          sigc::bind<InkscapeApplication*>(sigc::ptr_fun(&query_all),                 app)        );
    // clang-format on

    app->get_action_extra_data().add_data(raw_data_base);
}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4 :
