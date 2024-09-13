#ifndef WINFILELIST_H
#define WINFILELIST_H

#include "Templates/SmartPtr.h"
#include "ListView.h"
#include "Active.h"
#include "FolderWatcher.h"
#include "FindFile.h"
#include "SyncQueue.h"
#include <tchar.h>
#include <cassert>
#include "stdstring"
#include <vector>
#include <algorithm>

namespace Win
{

class FileListData
{
  public: // Class
    class Compare
    {
      public:
        bool operator()( FileListData const& a, FileListData const& b ) const
        { if ( a.IsFolder() && !b.IsFolder() ) return true;
          if ( !a.IsFolder() && b.IsFolder() ) return false;
          return ::CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, a.GetDisplayName().c_str(), -1, b.GetDisplayName().c_str(), -1) < 2;
        }
    };

  public:
    FileListData( void )
    : _fileName(0),
      _attr(0),
      _size(0.0),
      _imgIdx(0)
    {}

    FileListData( TCHAR const path[], unsigned long attr, double size, TCHAR const displayName[], int imgIdx )
    : _path(path),
      _attr(attr),
      _size(size),
      _displayName(displayName),
      _imgIdx(imgIdx)
    { _fileName = _path.rfind(TEXT('\\'));
      _fileName = (_fileName == stdstring::npos ? 0 : _fileName + 1);
    }

    stdstring GetPath( void ) const { return _path; }
    stdstring GetFileName( void ) const { return _path.substr(_fileName); }
    unsigned long GetAttr( void ) const { return _attr; }
    bool IsFolder( void ) const { return (_attr&FILE_ATTRIBUTE_DIRECTORY) != 0; }
    double GetSize( void ) const { return _size; }
    stdstring GetDisplayName( void ) const { return _displayName; }
    int GetIconIndex( void ) const { return _imgIdx; }

  protected:
    stdstring     _path;
    int           _fileName;
    unsigned long _attr;
    double        _size;

    stdstring     _displayName;
    int           _imgIdx;
};

template <class T>
class SyncVector
{
  public: // Class
    class DataLock : public Win::TransferLock
    {
      public:
        DataLock( void )
        : _data(0)
        {}

        DataLock( Win::Mutex& mutex, T& t )
        : Win::TransferLock(mutex),
          _data(&t)
        {}

        T* operator->( void ) { return _data; }
        T const* operator->( void ) const { return _data; }
        T& operator*( void ) { return *_data; }
        T const& operator*( void ) const { return *_data; }

        operator T*( void ) { return _data; }
        operator T const*( void ) const { return _data; }

      private:
        T* _data;
    };

    class ConstDataLock : public Win::TransferLock
    {
      public:
        ConstDataLock( void )
        : _data(0)
        {}

        ConstDataLock( Win::Mutex& mutex, T const& t )
        : Win::TransferLock(mutex),
          _data(&t)
        {}

        T const* operator->( void ) const { return _data; }
        T const& operator*( void ) const { return *_data; }
        operator T const*( void ) const { return _data; }

      private:
        T const* _data;
    };

  public:
    SyncVector( void )
    {}

    void push_back( T const& t )
    { Win::Lock lock(_mutex);
      _data.push_back(t);
    }

    void erase( int idx )
    { Win::Lock lock(_mutex);
      _data.erase(_data.begin() + idx);
    }

    void clear( void )
    { Win::Lock lock(_mutex);
      _data.clear();
    }

    int size( void ) const
    { Win::Lock lock(_mutex);
      return _data.size();
    }

    DataLock operator[]( int idx )
    { Win::Lock lock(_mutex);
      assert(idx < _data.size());
      return DataLock(_mutex, _data[idx]);
    }

    ConstDataLock operator[]( int idx ) const
    { Win::Lock lock(_mutex);
      assert(idx < _data.size());
      return ConstDataLock(_mutex, _data[idx]);
    }

    void reserve( int nEntries )
    { Win::Lock lock(_mutex);
      _data.reserve(nEntries);
    }

    template <class _Pr>
    void sort( _Pr _p )
    { Win::Lock lock(_mutex);
      std::sort(_data.begin(), _data.end(), _p);
    }

  private:
    mutable Win::Mutex _mutex;
    std::vector<T>     _data;
};

template <class _Data = FileListData>
class FileList : public Win::ListView
{
  public: // Typedefs
    typedef SyncVector<_Data>::DataLock DataLock;
    typedef SyncVector<_Data>::ConstDataLock ConstDataLock;

  public: // Class
    class DataHolder : public Win::Lock
    {
      public:
        DataHolder( FileList const& list )
        : Win::Lock(list._mutex)
        {}
    };

  public: // Methods
    explicit FileList( HWND hwnd = 0, bool enlistFolders = true )
    : Win::ListView(hwnd),
      _totalSize(0.0),
#pragma warning(disable: 4355) // 'this' used before initialized
      _dataFiller(*this, enlistFolders)
#pragma warning(default: 4355)
    { InitList(); 
    }

    FileList( HWND dlg, int id, bool enlistFolders = true )
    : Win::ListView(dlg, id),
      _totalSize(0.0),
#pragma warning(disable: 4355) // 'this' used before initialized
      _dataFiller(*this, enlistFolders)
#pragma warning(default: 4355)
    { InitList(); 
    }

    void Init( HWND hwnd )
    { ListView::Init(hwnd);
      InitList();
    }

    void SetBkColor( COLORREF color )
    { _small.SetBkColor(color);
      _large.SetBkColor(color);
      ListView::SetBkColor(color);
    }

    void EnlistFolders( bool enlist )
    { _dataFiller.EnlistFolders(enlist);
    }

    void ListSubFolders( bool list )
    { _dataFiller.ListSubFolders(list);
    }

    void SetFilter( std::vector<stdstring> const& filter )
    { _dataFiller.SetFilter(filter);
    }

    void ListFolder( TCHAR const folder[], bool listSubFolders = false )
    { _dataFiller.ListFolder(folder, listSubFolders);
    }

    void RefreshFileList( void )
    { _dataFiller.ListFolder(_folder.c_str());
    }

    void SortFileList( void )
    { _dataFiller.SortFileList();
    }

    int GetDataCount( void ) const
    { return _data.size();
    }

    DataLock GetDataLock( int idx )
    { return _data[idx];
    }

    ConstDataLock GetConstDataLock( int idx ) const
    { return _data[idx];
    }

    double GetTotalSize( void ) const
    { return _totalSize;
    }

  private: // Class
    class DataFiller : public Win::ActiveObject, public Win::FolderWatcher::NotifySink
    {
      public:
        DataFiller( FileList<_Data>& list, bool enlistFolders )
        : _list(list),
#pragma warning(disable: 4355) // 'this' used before initialized
          _watcher(0, this),
#pragma warning(default: 4355)
          _abort(false),
          _enlistFolders(enlistFolders),
          _listSubFolders(false),
          _data(list._data)
        {}

        ~DataFiller( void )
        {}

        void EnlistFolders( bool enlist )
        { _abort = true;
          PostCommand( SPtr<Command>(new GetFileListCmd(this, _list._folder.c_str(), _filter, enlist, _listSubFolders)) );
        }

        void ListSubFolders( bool list )
        { _abort = true;
          PostCommand( SPtr<Command>(new GetFileListCmd(this, _list._folder.c_str(), _filter, _enlistFolders, list)) );
        }

        void SetFilter( std::vector<stdstring> const& filter )
        { _abort = true;
          PostCommand( SPtr<Command>(new GetFileListCmd(this, _list._folder.c_str(), filter, _enlistFolders, _listSubFolders)) );
        }

        void ListFolder( TCHAR const folder[], bool listSubFolders = false )
        { _abort = true;
          PostCommand( SPtr<Command>(new GetFileListCmd(this, folder, _filter, _enlistFolders, listSubFolders)) );
        }

        void UpdateFileList( void )
        { PostCommand( SPtr<Command>(new UpdateFileListCmd(this)) );
        }

        void SortFileList( void )
        { PostCommand( SPtr<Command>(new SortFileListCmd(this)) );
        }

      protected: // Methods
        virtual void InitThread( void ) 
        { ::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_IDLE);
        }

        virtual void Run( void )
        { for ( ;; )
          { try
            { _event.Wait();
              if ( _isDying )
              { return;
              }
              Win::Lock lock(_list._mutex);
              _abort = false;
              while ( !_queue.IsEmpty() && !_abort )
              { SPtr<Command> cmd = _queue.PopHead();
                cmd->Perform();
              }
            }
            catch ( ... )
            {
            }
          }
        }

        virtual void FlushThread( void )
        { _abort = true;
          _event.Set();
        }

        virtual void FolderChanged( int id )
        { UpdateFileList();
        }

      private: // Methods
        bool Enlist( TCHAR const fileName[] )
        { if ( _filter.size() == 0 )
          { return true;
          }
          TCHAR const* ext = ::_tcsrchr(fileName, TEXT('.'));
          if ( ext == 0 )
          { return false;
          }
          for ( int i = 0; i < _filter.size(); ++i )
          { if ( ::_tcsicmp(ext, _filter[i].c_str()) == 0 )
            { return true;
            }
          }
          return false;
        }

        double GetData( TCHAR const folder[] )
        { double totalSize = 0.0;
          for ( ShellFolderContent files(folder); !files.AtEnd() && !_abort; files.FindNext() )
          { if ( ::_tcsicmp(files.GetFileName(), TEXT(".")) != 0 && (_listSubFolders ? ::_tcsicmp(files.GetFileName(), TEXT("..")) != 0 : true) )
            { if ( files.IsFolder() )
              { if ( _listSubFolders )
                { totalSize += GetData(files.GetPath());
                }
                else if ( _enlistFolders )
                { _data.push_back(_Data(files.GetPath(), files.GetAttr(), files.GetSize(), files.GetDisplayName(), files.GetIconIndex()));
                  totalSize += files.GetSize();
                }
              }
              else if ( Enlist(files.GetFileName()) )
              { _data.push_back(_Data(files.GetPath(), files.GetAttr(), files.GetSize(), files.GetDisplayName(), files.GetIconIndex()));
                totalSize += files.GetSize();
              }
            }
          }
          return totalSize;
        }

        void UpdateData( TCHAR const folder[], std::vector<bool>& found, std::vector<_Data>& toAdd )
        { for ( ShellFolderContent files(folder); !files.AtEnd(); files.FindNext() )
          { if ( ::_tcsicmp(files.GetFileName(), TEXT(".")) != 0 && (_listSubFolders ? ::_tcsicmp(files.GetFileName(), TEXT("..")) != 0 : true) )
            { int idx = -1;
              if ( files.IsFolder() )
              { if ( _listSubFolders )
                { UpdateData(files.GetPath(), found, toAdd);
                  continue;
                }
                else if ( _enlistFolders )
                { idx = Find(files.GetPath(), files.GetFileName());
                  if ( idx >= 0 )
                  { found[idx] = true;
                  }
                  else
                  { toAdd.push_back(_Data(files.GetPath(), files.GetAttr(), files.GetSize(), files.GetDisplayName(), files.GetIconIndex())); 
                  }
                }
              }
              else if ( Enlist(files.GetFileName()) )
              { idx = Find(files.GetPath(), files.GetFileName());
                if ( idx >= 0 )
                { found[idx] = true;
                }
                else
                { toAdd.push_back(_Data(files.GetPath(), files.GetAttr(), files.GetSize(), files.GetDisplayName(), files.GetIconIndex())); 
                }
              }
            }
          }
        }

        int Find( TCHAR const path[], TCHAR const fileName[] ) const
        { for ( int i = 0; i < _data.size(); ++i )
          { if ( ::_tcsicmp(fileName, _data[i]->GetFileName().c_str()) == 0 )
            { if ( ::_tcsicmp(path, _data[i]->GetPath().c_str()) == 0 )
              { return i;
              }
            }
          }
          return -1;
        }

        void _GetFileList( stdstring const& folder, std::vector<stdstring> const& filter, bool enlistFolders, bool listSubFolders )
        {
          _watcher.WatchFolder(0);
          _list.PostMessage(LVM_DELETEALLITEMS);
          _data.clear();
          _list._totalSize = 0.0;
          _list._folder = folder;
          _filter = filter;
          _enlistFolders = enlistFolders;
          _listSubFolders = listSubFolders;
          if ( _list._folder == stdstring() )
          { return;
          }
          _list._totalSize = GetData(_list._folder.c_str());
          if ( !_abort )
          { _data.sort(_Data::Compare());
            _list.PostMessage(LVM_SETITEMCOUNT, _data.size());
            _list.PostMessage(LVM_ENSUREVISIBLE, 0, FALSE);
            _list.PostProcess(0, _data.size() - 1, _abort);
            _watcher.WatchFolder(_list._folder.c_str(), _listSubFolders, FILE_NOTIFY_CHANGE_FILE_NAME|(_enlistFolders ? FILE_NOTIFY_CHANGE_DIR_NAME : 0));
          }
        }

        void _UpdateFileList( void )
        {
          if ( _list._folder == stdstring() )
          { return;
          }
          std::vector<bool> found(_data.size(), false);
          std::vector<_Data> toAdd;
          UpdateData(_list._folder.c_str(), found, toAdd);
          if ( _abort )
          { return;
          }
          // Remove items that were not found
          std::vector<int> toRemove;
          int i;
          for ( i = 0; i < found.size(); ++i )
          { if ( !found[i] )
            { toRemove.push_back(i);
            }
          }
          for ( i = 0; i < toRemove.size(); ++i )
          { int newIdx = toRemove[i] - i;
            _list._totalSize -= _data[newIdx]->GetSize();
            _data.erase(newIdx);
            _list.PostMessage(LVM_DELETEITEM, newIdx);
          }
          int firstNew = _data.size();
          if ( toAdd.size() > 0 )
          { _data.reserve(_data.size() + toAdd.size());
            for ( i = 0; i < toAdd.size(); ++i )
            { _data.push_back(toAdd[i]);
            }
            for ( i = firstNew; i < _data.size(); ++i )
            { _list._totalSize += _data[i]->GetSize();
            }
            _list.PostMessage(LVM_SETITEMCOUNT, _data.size(), LVSICF_NOINVALIDATEALL|LVSICF_NOSCROLL);
          }
          _list.PostProcess(firstNew, _data.size() - 1, _abort);
        }

        void _SortFileList( void )
        { _data.sort(_Data::Compare());
          _list.PostMessage(LVM_REDRAWITEMS, 0, _data.size() - 1);
        }

      private: // Class
        class Command
        { public: Command( DataFiller* f ) : _f(f) {}
                  virtual void Perform( void ) = 0;
          protected: DataFiller* _f;
        };

        class GetFileListCmd : public Command
        { public: 
            GetFileListCmd( DataFiller* f, TCHAR const folder[], std::vector<stdstring> const& filter, bool enlistFolders, bool listSubFolders )
            : Command(f),
              _folder(folder),
              _filter(filter),
              _enlistFolders(enlistFolders),
              _listSubFolders(listSubFolders)
            {}
            
            virtual void Perform( void )
            { _f->_GetFileList(_folder, _filter, _enlistFolders, _listSubFolders);
            }
          private: 
            stdstring              _folder;
            std::vector<stdstring> _filter;
            bool                   _enlistFolders;
            bool                   _listSubFolders;
        };

        class UpdateFileListCmd : public Command
        { public: UpdateFileListCmd( DataFiller* f ) : Command(f) {}
                  virtual void Perform( void ) { _f->_UpdateFileList(); }
        };

        class SortFileListCmd : public Command
        { public: SortFileListCmd( DataFiller* f ) : Command(f) {}
                  virtual void Perform( void ) { _f->_SortFileList(); }
        };

      private: // Methods
        void PostCommand( SPtr<Command>& cmd )
        { _queue.PushBack(cmd);
          _event.Set();
        }

      private: // Data members
        FileList<_Data>&           _list;
        Win::FolderWatcher         _watcher;
        
        Win::Event                 _event;
        SyncQueue< SPtr<Command> > _queue;
        volatile bool              _abort;
                
        bool                       _enlistFolders;
        bool                       _listSubFolders;
        std::vector<stdstring>     _filter;
        SyncVector<_Data>&         _data;

      // Friends
      friend class GetFileListCmd;
      friend class UpdateFileListCmd;
      friend class SortFileListCmd;
    };

  protected: // Methods
    virtual void InitList( void )
    { if ( _hwnd != 0 )
      { // Set image lists
        SetSmallImageList(_small);
        SetLargeImageList(_large);
      }
    }

    virtual void PostProcess( int firstIdx, int lastIdx, volatile bool const& abort ) {}

  protected:
    SystemSmallImageList      _small;
    SystemLargeImageList      _large;

    stdstring                 _folder;
    double                    _totalSize;

    mutable Win::Mutex        _mutex;
    SyncVector<_Data>         _data;

    DataFiller                _dataFiller;

  friend class DataFiller;
  friend class DataHolder;
};

template < class _Data = FileListData, class _List = FileList<_Data> >
class FileListNotifyHandler : public ListViewNotifyHandler
{
  public:
    FileListNotifyHandler( _List& list )
    : _list(list)
    {}

    // Overriden methods - must be called from overriden methods !!
    virtual bool OnGetDispInfo( HWND hwnd, LVITEM& item )
    { if ( item.iItem < _list.GetDataCount() )
      { _List::ConstDataLock data = _list.GetConstDataLock(item.iItem);
        ::_tcsncpy(item.pszText, data->GetDisplayName().c_str(), item.cchTextMax);
        item.iImage = data->GetIconIndex();
        item.mask |= LVIF_TEXT|LVIF_IMAGE;
      }
      return true;
    }


  protected:
    _List& _list;
};

} // namespace Win

#endif // WINFILELIST_H