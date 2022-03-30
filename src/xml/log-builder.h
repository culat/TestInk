// SPDX-License-Identifier: GPL-2.0-or-later
/** @file
 * @brief Object building an event log
 *//*
 * Authors: see git history
 *
 * Copyright (C) 2018 Authors
 * Copyright 2005 MenTaLguY <mental@rydia.net>
 * 
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#ifndef SEEN_INKSCAPE_XML_LOG_BUILDER_H
#define SEEN_INKSCAPE_XML_LOG_BUILDER_H

#include "inkgc/gc-managed.h"
#include "xml/node-observer.h"

namespace Inkscape {
namespace XML {

class Event;
class Node;

/**
 * @brief Event log builder
 *
 * This object records all events sent to it via the public methods in an internal event log.
 * Calling detach() then returns the built log. Calling discard() will clear all the events
 * recorded so far.
 */
class LogBuilder {
public:
    LogBuilder() : _log(nullptr) {}
    ~LogBuilder() { discard(); }

    /** @name Manipulate the recorded event log
     * @{ */
    /**
     * @brief Clear the internal log
     */
    void discard();
    /**
     * @brief Get the internal event log
     * @return The recorded event chain
     */
    Event *detach();
    /*@}*/

    /** @name Record events in the log
     * @{ */
    void addChild(Node &node, Node &child, Node *prev);

    void removeChild(Node &node, Node &child, Node *prev);

    void setChildOrder(Node &node, Node &child,
                       Node *old_prev, Node *new_prev);

    void setContent(Node &node,
                    Util::ptr_shared old_content,
                    Util::ptr_shared new_content);

    void setAttribute(Node &node, GQuark name,
                      Util::ptr_shared old_value,
                      Util::ptr_shared new_value);

    void setElementName(Node& node, GQuark old_name, GQuark new_name);
    /*@}*/

private:
    Event *_log;
};

}
}

#endif
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
