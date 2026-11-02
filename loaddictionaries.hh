/* This file is (c) 2008-2012 Konstantin Isakov <ikm@goldendict.org>
 * Part of GoldenDict. Licensed under GPLv3 or later, see the LICENSE file */

#ifndef __LOADDICTIONARIES_HH_INCLUDED__
#define __LOADDICTIONARIES_HH_INCLUDED__

#include "initializing.hh"
#include "config.hh"
#include "dictionary.hh"

#include <QThread>
#include <QNetworkAccessManager>
#include <QSet>

/// Use loadDictionaries() function below -- this is a helper thread class
class LoadDictionaries: public QThread, public Dictionary::Initializing
{
  Q_OBJECT

  QStringList nameFilters;
  QSet< QString > visitedPaths;
  std::string indexDirectory;
  Config::Paths const & paths;
  Config::SoundDirs const & soundDirs;
  Config::Hunspell const & hunspell;
  Config::Transliteration const & transliteration;
  std::vector< sptr< Dictionary::Class > > dictionaries;
  std::string exceptionText;
  int maxPictureWidth;
  unsigned int maxHeadwordSize;
  unsigned int maxHeadwordToExpand;

public:

  LoadDictionaries( Config::Class const & cfg );
  ~LoadDictionaries();

  virtual void run();

  std::vector< sptr< Dictionary::Class > > const & getDictionaries() const
  { return dictionaries; }

  /// Empty string means to exception occurred
  std::string const & getExceptionText() const
  { return exceptionText; }

signals:

  void indexingDictionarySignal( QString const & dictionaryName );

public:

  virtual void indexingDictionary( std::string const & dictionaryName ) throw();

private:

  void handlePath( Config::Path const & );
};

/// Loads all dictionaries mentioned in the configuration passed, into the
/// supplied array. When necessary, a window would pop up describing the process.
/// If showInitially is passed as true, the window will always popup.
/// If doDeferredInit is true (default), doDeferredInit() is done on all
/// dictionaries at the end.
/// startedLoader may point to a loader already started with the same cfg. Its
/// owner must keep it alive until this call returns. It will not be restarted.
void loadDictionaries( QWidget * parent, bool showInitially,
                       Config::Class const & cfg,
                       std::vector< sptr< Dictionary::Class > > &,
                       QNetworkAccessManager & dictNetMgr,
                       bool doDeferredInit = true,
                       LoadDictionaries * startedLoader = 0 );

/// Runs deferredInit() on all the given dictionaries. Useful when
/// loadDictionaries() was previously called with doDeferredInit = false.
void doDeferredInit( std::vector< sptr< Dictionary::Class > > & );
#endif

