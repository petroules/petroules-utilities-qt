#include "jumplist.h"
#include "jumplists_win32.h"

class JumpList::Private
{
#ifdef Q_WS_WIN
public:
    IShellLinkW* createShellLink(const Destination &destination) const;
    IShellItem* createShellItem(const Destination &destination) const;

    ICustomDestinationList* destinationList;
    IObjectArray* removedDestinations;
    UINT maximumShownDestinations;
#endif
};

JumpList::JumpList()
    : d(new Private())
{
#ifdef Q_WS_WIN
    // Create the custom jump list object
    CoCreateInstance(CLSID_DestinationList, NULL, CLSCTX_INPROC_SERVER, IID_ICustomDestinationList, reinterpret_cast<void**>(&this->d->destinationList));

    // Create a list to hold items removed by the user
    this->d->destinationList->BeginList(&this->d->maximumShownDestinations, IID_IObjectArray, reinterpret_cast<void**>(&this->d->removedDestinations));
#endif
}

JumpList::~JumpList()
{
#ifdef Q_WS_WIN
    if (this->d->removedDestinations)
    {
        this->d->removedDestinations->Release();
    }

    if (this->d->destinationList)
    {
        this->d->destinationList->Release();
    }
#endif

    delete this->d;
}

void JumpList::appendFrequentCategory()
{
#ifdef Q_WS_WIN
    this->d->destinationList->AppendKnownCategory(KDC_FREQUENT);
#endif
}

void JumpList::appendRecentCategory()
{
#ifdef Q_WS_WIN
    this->d->destinationList->AppendKnownCategory(KDC_RECENT);
#endif
}

void JumpList::appendCustomCategory(const QString &name, const QList<Destination> &destinations)
{
#ifdef Q_WS_WIN
    IObjectCollection* obj_collection;
    CoCreateInstance(CLSID_EnumerableObjectCollection, NULL, CLSCTX_INPROC, IID_IObjectCollection, reinterpret_cast<void**>(&obj_collection));

    // TODO: make sure that the file path is valid and the file extension is registered to this application
    // otherwise it will crash the application
    foreach (const Destination &destination, destinations)
    {
        obj_collection->AddObject(this->d->createShellItem(destination));
    }

    IObjectArray* object_array;
    obj_collection->QueryInterface(IID_IObjectArray, reinterpret_cast<void**>(&object_array));
    this->d->destinationList->AppendCategory(name.toStdWString().c_str(), object_array);

    object_array->Release();
    obj_collection->Release();
#else
    Q_UNUSED(name);
    Q_UNUSED(destinations);
#endif
}

void JumpList::appendTasks(const QList<Destination> &destinations)
{
#ifdef Q_WS_WIN
    // Create the collection of destinations to add
    IObjectCollection* obj_collection;
    CoCreateInstance(CLSID_EnumerableObjectCollection, NULL, CLSCTX_INPROC, IID_IObjectCollection, reinterpret_cast<void**>(&obj_collection));

    // Add the destinations to the collection
    foreach (const Destination &destination, destinations)
    {
        obj_collection->AddObject(this->d->createShellLink(destination));
    }

    // Add the tasks to the destination list
    IObjectArray* object_array;
    obj_collection->QueryInterface(IID_IObjectArray, reinterpret_cast<void**>(&object_array));
    this->d->destinationList->AddUserTasks(object_array);

    object_array->Release();
    obj_collection->Release();
#else
    Q_UNUSED(destinations);
#endif
}

QList<Destination> JumpList::frequentDestinations() const
{
    QList<Destination> list;
    return list;
}

QList<Destination> JumpList::recentDestinations() const
{
    QList<Destination> list;
    return list;
}

/*!
    Returns the maximum number of recent items shown on the jump list.
 */
int JumpList::maximumShownDestinations() const
{
#ifdef Q_WS_WIN
    return this->d->maximumShownDestinations;
#else
    return 0;
#endif
}

void JumpList::removeAll()
{

}

void JumpList::abort()
{
#ifdef Q_WS_WIN
    this->d->destinationList->AbortList();
#endif
}

void JumpList::save()
{
#ifdef Q_WS_WIN
    this->d->destinationList->CommitList();
#endif
}

void JumpList::reportFileUsage(const QString &fileName)
{
#ifdef Q_WS_WIN
    QFileInfo file(fileName);
    if (file.exists())
    {
        SHAddToRecentDocs(0x00000003, fileName.toStdWString().c_str());
    }
#else
    Q_UNUSED(fileName);
#endif
}

void JumpList::reportLinkUsage(const JumpListLink &link)
{
#ifdef Q_WS_WIN

#else
    Q_UNUSED(link);
#endif
}

#ifdef Q_WS_WIN
IShellLinkW* JumpList::Private::createShellLink(const Destination &destination) const
{
    if (!destination.isLink() && !destination.isSeparator())
    {
        return NULL;
    }

    IShellLinkW* shellLink = NULL;
    HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, reinterpret_cast<void**>(&shellLink));
    if (SUCCEEDED(hr))
    {
        const JumpListLink &link = destination.link();
        if (destination.isLink())
        {
            shellLink->SetPath(link.path.toStdWString().c_str());
            shellLink->SetArguments(link.arguments.toStdWString().c_str());
            shellLink->SetIconLocation(link.iconPath.toStdWString().c_str(), link.iconIndex);
            shellLink->SetDescription(link.toolTip.toStdWString().c_str());
            shellLink->SetWorkingDirectory(link.workingDirectory.toStdWString().c_str());
        }

        IPropertyStore* propertyStore = NULL;
        hr = shellLink->QueryInterface(IID_IPropertyStore, reinterpret_cast<void**>(&propertyStore));
        if (SUCCEEDED(hr))
        {
            PROPVARIANT pv;

            if (destination.isSeparator())
            {
                hr = InitPropVariantFromBoolean(TRUE, &pv);
                if (SUCCEEDED(hr))
                {
                    hr = propertyStore->SetValue(PKEY_AppUserModel_IsDestListSeparator, pv);
                }
            }
            else
            {
                hr = InitPropVariantFromString(link.title.toStdWString().c_str(), &pv);
                if (SUCCEEDED(hr))
                {
                    hr = propertyStore->SetValue(PKEY_Title, pv);
                }
            }

            // Save the changes we made to the property store
            propertyStore->Commit();
            propertyStore->Release();

            PropVariantClear(&pv);
        }
    }

    return shellLink;
}

IShellItem* JumpList::Private::createShellItem(const Destination &destination) const
{
    if (!destination.isFile())
    {
        return NULL;
    }

    IShellItem *shellItem = NULL;

    HMODULE shell = LoadLibrary(L"shell32.dll");
    if (shell)
    {
        t_SHCreateItemFromParsingName SHCreateItemFromParsingName = reinterpret_cast<t_SHCreateItemFromParsingName>(GetProcAddress(shell, "SHCreateItemFromParsingName"));
        if (SHCreateItemFromParsingName != NULL)
        {
            SHCreateItemFromParsingName(destination.file().path.toStdWString().c_str(), NULL, IID_IShellItem, reinterpret_cast<void**>(&shellItem));
        }

        FreeLibrary(shell);
    }

    return shellItem;
}
#endif
