#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
using namespace std;
#endif /* __PROGTEST__ */

class CFile
{
  public:
                             CFile                         ( void );
                             CFile                         ( const CFile & src );
                             ~CFile                        ( void );
    CFile &                  operator =                    ( CFile src );
    bool                     seek                          ( uint32_t          offset );
    uint32_t                 read                          ( uint8_t         * dst,
                                                             uint32_t          bytes );
    uint32_t                 write                         ( const uint8_t   * src,
                                                             uint32_t          bytes );
    void                     truncate                      ( void );
    uint32_t                 fileSize                      ( void ) const;
    void                     addVersion                    ( void );
    bool                     undoVersion                   ( void );
  private:
    size_t pos;
    size_t size;
    size_t h_pos;
    size_t h_size;
    uint8_t * data;
    CFile * history;
};

CFile::CFile ( void )
: pos ( 0 ), size ( 0 ), h_pos ( 0 ), h_size ( 0 ), data ( nullptr ), history ( nullptr )
{
}

CFile::CFile ( const CFile & src )
: pos ( src.pos ),
  size ( src.size ),
  h_pos ( src.h_pos ),
  h_size ( src.h_size ),
  data ( size ? new uint8_t [ size ] : nullptr),
  history( h_size ? new CFile [ h_size ] : nullptr ) 
{
  for ( size_t i = 0; i < size; i++ )
    data[i] = src.data[i];
  for ( size_t i = 0; i < h_size; ++i )
    history[i] = src.history[i];
}

CFile::~CFile ( void )
{
  delete [] data;
  delete [] history;
}

CFile & CFile::operator = ( CFile src )
{
  std::swap( pos, src.pos );
  std::swap( size, src.size );
  std::swap( data, src.data );
  std::swap( h_pos, src.h_pos);
  std::swap( h_size, src.h_size);
  std::swap( history, src.history);
  return *this;
}

bool CFile::seek ( uint32_t offset )
{
  if ( offset < 0 || offset > size )
    return false;
  pos = offset;
  return true;
}

uint32_t CFile::read ( uint8_t * dst, uint32_t bytes )
{
  uint32_t idx = 0;
  while ( pos < size && bytes > 0 )
  {
    dst [ idx++ ] = data [ pos++ ];
    --bytes;
  }
  return idx;
}

uint32_t CFile::write ( const uint8_t * src, uint32_t bytes )
{
  //alloc bigger file
  if ( pos + bytes > size )
  {
    uint8_t * tmp = new uint8_t [ pos + bytes ];
    if ( size != 0 )
      for ( size_t i = 0; i < size; ++i )
        tmp[i] = data[i];
    delete [] data;
    data = tmp;
    size = pos + bytes;
  }
  for ( size_t i = 0; i < bytes; ++i )
    data[pos + i] = src[i];
  pos += bytes;
  return bytes;  
}

void CFile::truncate ( void )
{
  size = pos;
}
uint32_t CFile::fileSize ( void ) const
{
  return size;
}

void CFile::addVersion ( void )
{
  //realloc
  if ( h_pos >= h_size )
  {
    CFile * tmp = new CFile[ h_size * 2 + 10 ];
    if ( h_size != 0 )
    {
      for ( size_t i = 0; i < h_pos; ++i )
        tmp[i] = history[i];
    }
    delete [] history;
    history = tmp;
    h_size = h_size * 2 + 10;  
  }
  history[ h_pos++ ] = *this;
}

bool CFile::undoVersion ( void )
{
  if ( h_pos <= 0 )
    return false;
  *this = history[ --h_pos ];
  return true;
}

#ifndef __PROGTEST__
bool               writeTest                               ( CFile           & x,
                                                             const initializer_list<uint8_t> & data,
                                                             uint32_t          wrLen )
{
  return x . write ( data . begin (), data . size () ) == wrLen;
}

bool               readTest                                ( CFile           & x,
                                                             const initializer_list<uint8_t> & data,
                                                             uint32_t          rdLen )
{
  uint8_t  tmp[100];
  uint32_t idx = 0;

  if ( x . read ( tmp, rdLen ) != data . size ())
    return false;
  for ( auto v : data )
    if ( tmp[idx++] != v )
      return false;
  return true;
}

int main ( void )
{
  CFile f0;

  assert ( writeTest ( f0, { 10, 20, 30 }, 3 ) );
  assert ( f0 . fileSize () == 3 );
  assert ( writeTest ( f0, { 60, 70, 80 }, 3 ) );
  assert ( f0 . fileSize () == 6 );
  assert ( f0 . seek ( 2 ));
  assert ( writeTest ( f0, { 5, 4 }, 2 ) );
  assert ( f0 . fileSize () == 6 );
  assert ( f0 . seek ( 1 ));
  assert ( readTest ( f0, { 20, 5, 4, 70, 80 }, 7 ));
  assert ( f0 . seek ( 3 ));
  f0 . addVersion();
  assert ( f0 . seek ( 6 ));
  assert ( writeTest ( f0, { 100, 101, 102, 103 }, 4 ) );
  f0 . addVersion();
  assert ( f0 . seek ( 5 ));
  CFile f1 ( f0 );
  f0 . truncate ();
  assert ( f0 . seek ( 0 ));
  assert ( readTest ( f0, { 10, 20, 5, 4, 70 }, 20 ));
  assert ( f0 . undoVersion () );
  assert ( f0 . seek ( 0 ));
  assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80, 100, 101, 102, 103 }, 20 ));
  assert ( f0 . undoVersion () );
  assert ( f0 . seek ( 0 ));
  assert ( readTest ( f0, { 10, 20, 5, 4, 70, 80 }, 20 ));
  assert ( !f0 . seek ( 100 ));
  assert ( writeTest ( f1, { 200, 210, 220 }, 3 ) );
  assert ( f1 . seek ( 0 ));
  assert ( readTest ( f1, { 10, 20, 5, 4, 70, 200, 210, 220, 102, 103 }, 20 ));
  assert ( f1 . undoVersion () );
  assert ( f1 . undoVersion () );
  assert ( readTest ( f1, { 4, 70, 80 }, 20 ));
  assert ( !f1 . undoVersion () );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
