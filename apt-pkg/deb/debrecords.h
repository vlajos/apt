// -*- mode: cpp; mode: fold -*-
// Description								/*{{{*/
// $Id: debrecords.h,v 1.8 2001/03/13 06:51:46 jgg Exp $
/* ######################################################################
   
   Debian Package Records - Parser for debian package records
   
   This provides display-type parsing for the Packages file. This is 
   different than the the list parser which provides cache generation
   services. There should be no overlap between these two.
   
   ##################################################################### */
									/*}}}*/
#ifndef PKGLIB_DEBRECORDS_H
#define PKGLIB_DEBRECORDS_H

#include <apt-pkg/pkgrecords.h>
#include <apt-pkg/tagfile.h>
#include <apt-pkg/fileutl.h>
#include <apt-pkg/pkgcache.h>

#include <string>

#ifndef APT_8_CLEANER_HEADERS
#include <apt-pkg/indexfile.h>
#endif

class APT_HIDDEN debRecordParserBase : public pkgRecords::Parser
{
   void *d;
 protected:
   pkgTagSection Section;

 public:
   // These refer to the archive file for the Version
   virtual std::string FileName();
   virtual std::string SourcePkg();
   virtual std::string SourceVer();

   virtual HashStringList Hashes() const;

   // These are some general stats about the package
   virtual std::string Maintainer();
   virtual std::string ShortDesc(std::string const &lang);
   virtual std::string LongDesc(std::string const &lang);
   virtual std::string Name();
   virtual std::string Homepage();

   // An arbitrary custom field
   virtual std::string RecordField(const char *fieldName);

   virtual void GetRec(const char *&Start,const char *&Stop);

   debRecordParserBase();
   virtual ~debRecordParserBase();
};

class APT_HIDDEN debRecordParser : public debRecordParserBase
{
   void *d;
 protected:
   FileFd File;
   pkgTagFile Tags;

   virtual bool Jump(pkgCache::VerFileIterator const &Ver);
   virtual bool Jump(pkgCache::DescFileIterator const &Desc);

 public:
   debRecordParser(std::string FileName,pkgCache &Cache);
   virtual ~debRecordParser();
};

// custom record parser that reads deb files directly
class APT_HIDDEN debDebFileRecordParser : public debRecordParserBase
{
   void *d;
   std::string debFileName;
   std::string controlContent;

   APT_HIDDEN bool LoadContent();
 protected:
   // single file files, so no jumping whatsoever
   bool Jump(pkgCache::VerFileIterator const &);
   bool Jump(pkgCache::DescFileIterator const &);

 public:
   virtual std::string FileName();

   debDebFileRecordParser(std::string FileName);
   virtual ~debDebFileRecordParser();
};

#endif
