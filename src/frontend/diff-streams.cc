#include "player.hh"
#include "diff_generator.hh"
#include "display.hh"
#include <cstdio>

using namespace std;

int main( int argc, char * argv[] )
{
  if ( argc != 3 ) {
    cerr << "Usage: " << argv[ 0 ] << " SOURCE TARGET" << endl;
    return EXIT_FAILURE;
  }

  GenericPlayer<DiffGenerator> source_player( argv[ 1 ] );
  GenericPlayer<DiffGenerator> target_player( argv[ 2 ] );

  VideoDisplay display { source_player.example_raster() };

  while ( !source_player.eof() && !target_player.eof() ) {
    source_player.advance();
    RasterHandle target_raster = target_player.advance();

    vector< uint8_t > diff = target_player - source_player;

    fprintf( stderr, "Src Frame %u, ", source_player.frame_no() );
    fprintf( stderr, "Frame %u, original length: %lu bytes. Diff length: %lu bytes.\n",
	     target_player.frame_no(), target_player.original_size(), diff.size() );

    GenericPlayer<DiffGenerator> diff_player( source_player );

    RasterHandle diff_raster = diff_player.reconstruct_diff( diff );

    display.draw( diff_raster );

    if ( diff_player != target_player ) {
      cout << "Sad times...\n";
    }
  }
    
  return EXIT_SUCCESS;
}
