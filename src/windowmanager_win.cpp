#include "windowmanager.h"

#ifdef Q_WS_WIN
QRect WindowManager::getWindowRect(WId windowId)
{
    RECT rect;
    if (GetWindowRect(windowId, &rect))
    {
        return QRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    }
    else
    {
        return QRect();
    }
}

QRect WindowManager::getClientRect(WId windowId)
{
    RECT rect;
    if (GetClientRect(windowId, &rect))
    {
        return QRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    }
    else
    {
        return QRect();
    }
}

/*!
    Gets the caption of the specified window. If the window has no caption, an empty string is returned.
 */
QString WindowManager::getWindowText(WId windowId)
{
    QString str;
    int windowTextLength = GetWindowTextLengthW(windowId);
    if (windowTextLength > 0)
    {
        // We need an extra character for the null terminator so we add 1
        LPWSTR buffer = (LPWSTR)malloc(sizeof(WCHAR) * (windowTextLength + 1));
        if (buffer)
        {
            if (GetWindowTextW(windowId, buffer, windowTextLength + 1))
            {
                str = QString::fromWCharArray(buffer);
            }

            free(buffer);
        }
    }

    return str;
}

WId WindowManager::rootWindow()
{
    return GetDesktopWindow();
}

/*!
    Returns the window at the specified point, starting the search at the specified window.
    The desktop window is used if none is specified, and only visible and non-iconic windows
    are searched, and windows whose handles return true by the passed filter function.
 */
WId WindowManager::windowFromPoint(const QPoint &p, WId parent, bool (*filterFunction)(WId handle))
{
    // If we're passed a null handle, this means something probably went wrong, so just return the root window
    if (!parent)
    {
        return WindowManager::rootWindow();
    }

    // Create a list of window IDs
    QList<WId> *windowIds = new QList<WId>();

    // Here's where we get a little messy. We need to pass our filter function to the enumeration
    // callback, but the callback takes just one parameter... so we'll use a QPair object to pack
    // our window list and filter function pointer.
    QPair<void*, void*> *pair = new QPair<void*, void*>();
    pair->first = windowIds;
    pair->second = (void*)filterFunction;

    // Enumerate the child windows of this process, passing it the address of our window ID list
    // so that the enumeration callback can insert each of the child windows' handles into it, as
    // well as the filter function (since they are both encapsulated in our QPair)
    EnumChildWindows(parent, &processWindow, (LPARAM)pair);

    // If this window actually had any child windows...
    if (windowIds->length() > 0)
    {
        // ...then go through them and...
        for (int i = 0; i < windowIds->length(); i++)
        {
            WId winId = windowIds->at(i);

            // ...check if each child's rect contains our point; if so, check its children as well
            if (getWindowRect(winId).contains(p))
            {
                return windowFromPoint(p, winId, filterFunction);
            }
        }

        // If not, just return the window itself
        return parent;
    }
    else
    {
        // If it's got no child windows, the window itself is our match
        return parent;
    }
}

/*!
    The callback for EnumChildWindows; adds the window to the list
    if it's visible, non-iconic and matches our filter function.
 */
BOOL CALLBACK WindowManager::processWindow(WId windowId, LPARAM lParam)
{
    // lParam contains the pointer to our QPair, so let's cast that back
    QPair<void*, void*> *pair = (QPair<void*, void*>*)lParam;

    // The first object in the QPair was our list of window handles...
    QList<WId> *list = (QList<WId>*)pair->first;

    // ...and the second parameter was our filter function
    bool (*filter)(WId) = (bool (*)(WId))pair->second;

    // Apply our conditions and add the handle to the list
    if (IsWindowVisible(windowId) && !IsIconic(windowId) && filter(windowId))
    {
        list->append(windowId);
    }

    // Return true to continue iterating
    return true;
}
#endif
